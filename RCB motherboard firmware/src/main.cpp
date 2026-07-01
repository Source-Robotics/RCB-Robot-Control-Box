#include "stm32f1xx_hal.h"
#include "iodefs.h"
#include "constants.h"
#include "adc_init.h"
#include "pwm_init.h"
#include "hw_init.h"
#include "qfplib-m3.h"
#include "utils.h"
#include "EEPROM.h"
#include "debounce.h"
#include "SerialPacketParser.h"
#include "communication.h"
#include "common.h"

// Define serial port
#define Serial Serialx
#define TIMING_DEBUG 1
HardwareSerial Serialx(RX_COM, TX_COM); // PA3, PA2 RX,TX

// Define timer for PWM
HardwareTimer *MyTim2;
ADC_HandleTypeDef hadc1;

// Define button debounce and press handler
ButtonHandler button(Button_state);

// Exponential Moving Average (EMA) filter coefficients
#define EMA_A_V1 0.15
#define EMA_A_V2 0.15
#define EMA_A_C 0.15

String command_ = "idle";
String status = "test";

// Button function
void handleButtonActions() {
  switch (button.getState()) {
    case PRESSED:
        Serial.println("Single Press detected");
        break;
    case DOUBLE_PRESS:
        Serial.println("Double Press detected");
        break;
    case TRIPLE_PRESS:
        Serial.println("Triple Press detected");
        break;
    case HELD_2S:
        Serial.println("Button Held for 2 seconds");
        break;
    case HELD_3S:
        Serial.println("Button Held for 3 seconds");
        break;
    default:
        break;
}
button.resetState(); // Reset state after handling
}



void setup () {

  // Init system
  HAL_Init();
  SystemClock_Config();
  TM_SystemClock_Config(CLOCK_PRESCALE); //15, Clock speed = 8*15

  // Init digital inputs and outputs
  Init_Digital_Inputs();
  Init_Digital_Outputs();
  GPIO_init();

  // Init ADC
  MX_ADC1_Init();
  HAL_ADCEx_Calibration_Start(&hadc1);

  // Begin serial
  Serial.begin(SERIAL_SPEED);

  // Begin PWM 50000 = 25Khz -> center aligned
  // PWM duty for 25khz goes from 0 - 5120 if CPU clock is 128 Mhz
  MyTim2 = pwm_high(PRECHARGE_PIN, PWM_FREQ * 2);
  MyTim2 = pwm_high(PWM_BRAKE, PWM_FREQ * 2);
  pwm_align(MyTim2);

  // EEPROM stuff
  digitalWriteFast(EEPROM_WP, LOW);
  Init_EEPROM();
  //Write_config();
  Set_Default_config();
  read_config();

  // Init timer ticker with callback function
  Ticker_init(TIM3, LOOP_FREQ, IT_callback);

  // Disable relay and precharge FET on startup instantly
  digitalWriteFast(RELAY_PIN, HIGH); // relay is OFF
  pwm_set(PRECHARGE_PIN, PWM_MIN, 13); // 0V, Precharge FET is off, current goes thru relay

  // Enable brake FET to discharge capacitors that got slightly charged because of the relay is in NC state on startup
  digitalWriteFast(EN_BRAKE_FET, HIGH); // // Set PC15 HIGH, Enable brake FET   //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  
  pwm_set(PWM_BRAKE, PWM_MAX-10, 13); // 3V3, BRAKE FET  PWM

  // Small delay
  delay(500);

  // DISABLE brake FET
  digitalWriteFast(EN_BRAKE_FET, LOW); // Set PC15 LOW, Disable brake FET  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  
  pwm_set(PWM_BRAKE, PWM_MIN + 10, 13); // 0V, BRAKE FET is OFF

  // Enable precharge FET and wait a bit for CAPS to charge
  digitalWriteFast(RELAY_PIN, HIGH); // relay is OFF
  pwm_set(PRECHARGE_PIN, PWM_MAX, 13); // 3V3, Precharge FET is on
  delay(300);

  // Disable precharge FET and turn on relay 
  pwm_set(PRECHARGE_PIN, PWM_MIN, 13); // 0V, Precharge FET is off
  digitalWriteFast(RELAY_PIN, LOW); // relay is ON

  


}

void loop () {

  #if (TIMING_DEBUG > 0)
  int c = micros();
  #endif

  // Global ticker for timing
  uint32_t ms = HAL_GetTick();
  static uint32_t last_time = 0;

  // Turn on raspberry pi
  digitalWriteFast(Pi_enable, HIGH); 


  int vbus1 = Get_voltage_mA(ADC_CHANNEL_5_READ_VBUS1());
  int vbus2 = Get_voltage_mA(ADC_CHANNEL_4_READ_VBUS2());
  int current = Get_current_mA(ADC_CHANNEL_3_READ_CURRENT());


  // Apply integer-based EMA filtering

  controller.filtered_VBUS1_mV = (int)qfp_fadd(qfp_fmul(EMA_A_V1, vbus1), qfp_fmul(qfp_fsub(1, EMA_A_V1), controller.filtered_VBUS1_mV));
  controller.filtered_VBUS2_mV = (int)qfp_fadd(qfp_fmul(EMA_A_V2, vbus2), qfp_fmul(qfp_fsub(1, EMA_A_V2), controller.filtered_VBUS2_mV));
  controller.filtered_Current_mA = (int)qfp_fadd(qfp_fmul(EMA_A_C, current), qfp_fmul(qfp_fsub(1, EMA_A_C), controller.filtered_Current_mA));

  // Brake FET logic
  if (controller.filtered_VBUS2_mV >25000){
    digitalWriteFast(EN_BRAKE_FET, HIGH); // // Set PC15 HIGH, Enable brake FET   //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  
    controller.brake_pwm_duty = 8100;
    pwm_set(PWM_BRAKE, controller.brake_pwm_duty, 13); // 3V3, BRAKE FET  PWM
    digitalWriteFast(LED1, HIGH);
  }

  if(controller.filtered_VBUS2_mV < 25000){
    digitalWriteFast(EN_BRAKE_FET, LOW); // // Set PC15 HIGH, Enable brake FET   //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  
    controller.brake_pwm_duty = 8;
    pwm_set(PWM_BRAKE, controller.brake_pwm_duty , 13); // 3V3, BRAKE FET  PWM
    digitalWriteFast(LED1, LOW);
  }
  ////////////////////////

  if ((ms - last_time) > 500) // run every x ms
  {

    /*
    int vbus1 = ADC_CHANNEL_5_READ_VBUS1();
    int vbus2 = ADC_CHANNEL_4_READ_VBUS2();
    int current = ADC_CHANNEL_3_READ_CURRENT();
    Serial.print("VBUS1: ");
    int vvbus1 = Get_voltage_mA(vbus1);
    int vvbus2 = Get_voltage_mA(vbus2);
    Serial.print(vvbus1);
    Serial.print(" VBUS2: ");
    Serial.print(vvbus2);
    Serial.print(" Current: ");
    Serial.println(Get_current_mA(current));
    Serial.print("exect time: ");
    Serial.println(controller.execution_time);
    */


    // Print filtered integer values


    // TODO Add error state 
    // TODO add power button state
    // Read relay state: 
    int relayState = !digitalRead(RELAY_PIN);
    
    float power_usage = (((controller.filtered_VBUS1_mV + controller.filtered_VBUS2_mV) / 2) * controller.filtered_Current_mA)/1000000;
    Serial.print("$");
    Serial.print(" ");
    Serial.print(controller.filtered_VBUS1_mV);
    Serial.print(" ");
    Serial.print(controller.filtered_VBUS2_mV);
    Serial.print(" ");
    Serial.print(controller.filtered_Current_mA);
    Serial.print(" ");
    Serial.print(power_usage);
    Serial.print(" ");
    Serial.print(relayState);
    Serial.print(" ");
    Serial.print(command_);
    Serial.print(" ");
    Serial.print(status);
    Serial.print(" ");
    Serial.print(controller.SERIAL_NUMBER);
    Serial.print(" ");
    Serial.print(controller.HARDWARE_VERSION);
    Serial.print(" ");
    Serial.print(controller.BATCH_DATE);
    Serial.print(" ");
    Serial.print(controller.SOFTWARE_VERSION);
    Serial.println(" ");

    /*
    Serial.print(controller.filtered_VBUS1_mV );         // first variable is program time in seconds. This can be plotted on an x-axis!
    Serial.print("\t");
    Serial.print(controller.filtered_VBUS2_mV);   // second variable is sin(t)
    Serial.print("\t");      // this last "\t" isn't required, but doesn't hurt
    Serial.println(vbus1); // third varible is cos(t). make sure to finish with a println
    */




    /*
    Serial.print("Terst ");
    Serial.print(Get_voltage_mA(1500));
    Serial.print("Filtered VBUS1: ");
    Serial.print(controller.filtered_VBUS1_mV);
    Serial.print("  Filtered VBUS2: ");
    Serial.print(controller.filtered_VBUS2_mV);
    Serial.print("  Filtered Current: ");
    Serial.println(controller.filtered_Current_mA);
    Serial.print("Exec time: ");
    Serial.println(controller.execution_time);
    */


  last_time = ms;
  }



  /*
  Check for VBUS error
  */
  check_vbus_error(ms);

  /*
  Perform actions based on button state (also debounce it)
  */
  button.update();
  handleButtonActions();

  /*
  UART CLI INTERFACE
  */
  UART_protocol(Serial);
  Cyclic_UART(Serial, ms);

  /*
  Debug variables
  */
  #if (TIMING_DEBUG > 0)
  int c2 = micros();
  controller.execution_time = c2 - c;
  #endif
}





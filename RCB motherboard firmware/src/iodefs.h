/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    iodefs.h
  * @brief   This file provides code for pin declarations
  * @author Petar Crnjak
  ******************************************************************************
  * @attention
  *
  * Copyright (c) Source robotics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IODEFS_H
#define IODEFS_H

#include <Arduino.h>
#include <stdio.h>

#define PRECHARGE_PIN PA_2 //Driver PRECHARGE PIN
#define PWM_BRAKE PA_1 //Driver PWM BRAKE

#define EN_BRAKE_FET PA_8 // Enable brake fet

#define ESTOP_SIGNAL2 PA_9
#define ESTOP_SIGNAL PA_10
#define ESTOP_MCU_onboard PA_11

#define Pi_enable PC_15

#define RELAY_PIN PB_2

#define LED1 PB_13

#define Supply_on_off PB_14

#define Button_state PB_15

#define EEPROM_SCL PB_10
#define EEPROM_SDA PB_11
#define EEPROM_WP PB_12

#define VBUS2 PA_4 // CHANNEL 4 VBUS_MES2
#define CURRENT_SENSE  PA_3 // CHANNEL 3 CURRENT_SENSE
#define VBUS1 PA_5 // CHANNEL 5  VBUS_MES1

#define TX_COM PB_6
#define RX_COM PB_7


#endif
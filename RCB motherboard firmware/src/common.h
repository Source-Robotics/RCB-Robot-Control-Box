
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    common.h
 * @brief   This file provides code where we declare global structures
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/


#ifndef COMMON_H
#define COMMON_H

#include "constants.h"
#include <stdint.h>

/// @brief  Structure for general controller variables
typedef struct
{

    volatile int SERIAL_NUMBER = 0;
    volatile int HARDWARE_VERSION = 0; // Version of the Spectral BLDC driver PCB
    volatile int BATCH_DATE = 0;       // Date the batch was produced
    volatile int SOFTWARE_VERSION = 0; // Software release

    volatile uint32_t interrupt_tick = 0; // Ticks of our interrupt routine

    volatile int Sense1_Raw; // RAW ADC measure of current 1
    volatile int Sense2_Raw; // RAW ADC measure of current 2
    volatile int VBUS_RAW;   // RAW ADC measure of VBUS
    volatile int TEMP_RAW;   // RAW ADC measure of temperature

    volatile int Current_mA;     // Current 1 in mA range
    volatile int VBUS1_mV;       // Vbus in mV range
    volatile int VBUS2_mV;       // Vbus in mV range

    volatile int filtered_VBUS1_mV = 0;
    volatile int filtered_VBUS2_mV = 0;
    volatile int filtered_Current_mA = 0;

    volatile int Max_temperature = 75; // Max temperature in degrees
    volatile int Min_temperature = -20; // Min temperature in degrees

    volatile int Max_Vbus = 29500; // Max vbus voltage in mV
    volatile int Min_Vbus = 9000;  // Min vbus voltage in mV

    volatile int execution_time = 0; // Execution time of our interrupt loops

    volatile bool Error = 0;             // General error

    // temporary test variables
    volatile float temp = 0;
    volatile float temp1 = 0;
    volatile float temp2 = 0;

    // Cyclic UART
    volatile int cyclic = 0;        // If 1 we Periodically send messages without request from host.
    volatile int cyclic_a = 0;      // If 1 we Periodically send messages without request from host.
    volatile int cyclic_period = 0; // We send messages every cyclic_period

    volatile int brake_pwm_duty = 0;

    volatile uint32_t vbus_high_start_time = 0;
    volatile bool vbus_error_flag = false;


} Measure;

extern Measure controller;


#endif

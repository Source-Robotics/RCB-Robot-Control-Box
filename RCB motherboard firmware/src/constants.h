/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    constants.h
 * @brief   This file provides code where we declare constants
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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VBUS_ERROR_TIME 4000 // Time in ms for VBUS error detection
#define MAX_DUTY 0.95f

#define MIN_DUTY 0.05f

#define ADC_RESOLUTION_ 12

#define PI 3.14159265359f

#define PI2 6.283185307f

#define PWM_FREQ 25000

#define LOOP_FREQ 5000 

#define LOOP_TIME 0.0002 

#define CLOCK_PRESCALE 14

#define SERIAL_SPEED 256000 // 256000 works, Tested with 1Mbit and works

#define SENSE_RESISTOR 0.0005f

#define ADC_MIDPOINT 1.65f

#define ADC_MIDPOINT_BIT 2048

#define CURRENT_AMP_GAIN 200

#define R1_1 180000

#define R2_1 10000

#define R1_2 180000

#define R2_2 10000

#define PWM_MAX 8191

#define PWM_MIN 0

/*
EEPROM CONSTANTS
*/

///// EEPROM variable address list
#define SERIAL_NUMBER_EEPROM 32
#define HARDWARE_VERSION_EEPROM 36
#define BATCH_DATA_EEPROM 40
#define SOFTWARE_VERSION_EEPROM 48

#endif
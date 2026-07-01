/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    debounce.h
  * @brief   This file provides code for all the function prototypes for debounce.cpp
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
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <Arduino.h>
#include <stdio.h>
#include "iodefs.h"
#include "stm32f1xx_hal.h"


#include <Arduino.h> // Required for PlatformIO

enum ButtonState {
    IDLE,
    PRESSED,
    DOUBLE_PRESS,
    TRIPLE_PRESS,
    HELD_2S,
    HELD_3S
};

class ButtonHandler {
    public:
        ButtonHandler(uint8_t pin, uint32_t debounce = 50, uint32_t multiPressTime = 300);
        
        void update(); 
        ButtonState getState();  
        void resetState();
    
    private:
        uint8_t buttonPin;
        uint32_t debounceTime;
        uint32_t multiPressTime; 
    
        bool lastButtonState;
        uint32_t buttonPressTime;
        uint32_t lastReleaseTime;
        uint8_t pressCount;  
    
        ButtonState buttonState;
    
        static constexpr uint32_t hold2s = 2000;
        static constexpr uint32_t hold3s = 3000;
    };
    
    #endif

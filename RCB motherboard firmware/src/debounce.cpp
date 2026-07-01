/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    debounce.cpp
  * @brief   This file provides code for initialization of PWM peripheral
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


#include "debounce.h"


ButtonHandler::ButtonHandler(uint8_t pin, uint32_t debounce, uint32_t multiPress) {
    buttonPin = pin;
    debounceTime = debounce;
    multiPressTime = multiPress;

    pinMode(buttonPin, INPUT_PULLDOWN); 
    
    lastButtonState = LOW;
    buttonState = IDLE;
    buttonPressTime = 0;
    lastReleaseTime = 0;
    pressCount = 0;
}

void ButtonHandler::update() {
    bool currentState = digitalRead(buttonPin);
    uint32_t now = HAL_GetTick();

    // Button Pressed
    if (currentState == HIGH && lastButtonState == LOW) {
        buttonPressTime = now;
    }

    // Button Released
    if (currentState == LOW && lastButtonState == HIGH) {
        uint32_t pressDuration = now - buttonPressTime;

        if (pressDuration >= debounceTime) {
            if (pressDuration < hold2s) {
                pressCount++;  
                lastReleaseTime = now;
            } 
            else if (pressDuration < hold3s) {
                buttonState = HELD_2S;
                pressCount = 0;
            } 
            else {
                buttonState = HELD_3S;
                pressCount = 0;
            }
        }
    }

    // Detect double/triple press
    if (pressCount > 0 && (now - lastReleaseTime) > multiPressTime) {
        if (pressCount == 1) {
            buttonState = PRESSED;
        } 
        else if (pressCount == 2) {
            buttonState = DOUBLE_PRESS;
        } 
        else if (pressCount >= 3) {
            buttonState = TRIPLE_PRESS;
        }
        pressCount = 0;  // Reset after detection
    }

    lastButtonState = currentState;
}

ButtonState ButtonHandler::getState() {
    return buttonState;
}

void ButtonHandler::resetState() {
    buttonState = IDLE;
}
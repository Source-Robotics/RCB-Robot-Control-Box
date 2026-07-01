/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    communication.cpp
 * @brief   This file provides code for our UART CLI
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

#include "communication.h"

SerialPacketParser parser;
char command[20];
char argument[20];

/// @todo Cyclic ?
/// @todo Cascade commands
/*
*/

/// @param Serialport Serial port we want to use
void UART_protocol(Stream &Serialport)
{
    /*
       CLI INTERFACE
      */
    while (Serialport.available())
    {
        char c = Serialport.read();

        if (parser.parse(c, command, argument))
        {

            

            // Print all currently set PID parameters for every control mode
            if (strcmp(command, "test1") == 0)
            {
                Serialport.print("test1 ");

            }


            // Set/Get CAN ID
            else if (strcmp(command, "test2") == 0)
            {

                if (strlen(argument) != 0)
                {
                    int temp_var = atoi(argument);
                    if (temp_var < 0 || temp_var > 15)
                    {
                        Serialport.println("test2");
                    }
                    else
                    {
                        __NOP();
                    }
                }

                Serialport.print("test2");
            }

            // Set/Get CAN ID
            else if (strcmp(command, "test3") == 0)
            {

                if (strlen(argument) != 0)
                {
                    int temp_var = atoi(argument);
                    if (temp_var < 0 || temp_var > 15)
                    {
                        Serialport.println("test3");
                    }
                    else
                    {
                        __NOP();
                    }
                }

                Serialport.print("test3");
            }

   
            else
            {
                Serialport.println("Unknown command");
            }

            parser.resetCommandAndArgument(command, argument);
            break;
        }
    }
}



/// @brief Contains code for cyclic uart functions that send data without host request
/// @param Serialport
/// @param ms
void Cyclic_UART(Stream &Serialport, uint32_t ms)
{

    if (controller.cyclic == 1)
    {
        if (controller.cyclic_period > 0)
        {
            static uint32_t last_time_ = 0;
            if ((ms - last_time_) >= controller.cyclic_period) // run every x ms
            {
                Serialport.print("$");
                Serialport.print(" ");


                last_time_ = ms;
            }
        }
    }
}

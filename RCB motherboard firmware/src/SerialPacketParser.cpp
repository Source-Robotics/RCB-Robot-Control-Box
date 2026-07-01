/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    SerialPacketParser.cpp
 * @brief   This file provides code for serial parser utility
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

#include "SerialPacketParser.h"

SerialPacketParser::SerialPacketParser()
{
  bufferIndex = 0;
  packetComplete = false;
}

bool SerialPacketParser::parse(char c, char *command, char *argument)
{

  if (c == '#')
  {
    bufferIndex = 0;
    packetComplete = false;
  }

  if (bufferIndex < sizeof(buffer) - 1)
  {
    buffer[bufferIndex++] = c;
    buffer[bufferIndex] = '\0'; // Null-terminate the buffer

    if (c == '\n')
    {
      packetComplete = true;
    }
  }

  if (packetComplete)
  {
    // Reset the buffer and flag after successfully processing a command
    bufferIndex = 0;
    packetComplete = false;

    // Extract the command and argument
    if (sscanf(buffer, "#%19s %19s", command, argument) == 2)
    {
      // Dispatch based on the parsed command
      if (strlen(command) + strlen(argument) <= 20)
      {

        // Dispatch based on the parsed command
        if (strcmp(command, "test1") == 0)
        {
          return true;
        }
        else if (strcmp(command, "test2") == 0)
        {
          return true;
        }
        else if (strcmp(command, "test3") == 0)
        {
          return true;
        }

        else
        {
          // Unknown command
          resetCommandAndArgument(command, argument);
          return false;
        }
      }
      else
      {
        // Combined length of command and argument exceeds 20 characters
        resetCommandAndArgument(command, argument);
        return false;
      }
    }

    else if (sscanf(buffer, "#%19s", command) == 1)
    {
      if (strlen(command) <= 20)
      {
        // Dispatch based on the parsed command
        if (strcmp(command, "test1") == 0)
        {
          return true;
        }
        else if (strcmp(command, "test2") == 0)
        {
          return true;
        }
        else if (strcmp(command, "test3") == 0)
        {
          return true;
        }

        else
        {
          // Unknown command
          resetCommandAndArgument(command, argument);
          return false;
        }
      }
      else
      {
        // Command length exceeds 20 characters
        resetCommandAndArgument(command, argument);
        return false;
      }
    }
    else
    {
      // Invalid command or argument
      resetCommandAndArgument(command, argument);
      return false;
    }
  }
  return false; // No valid packet yet
}

bool SerialPacketParser::handleIq(char *argument)
{
  // Your code to handle the "start" command
  // Use 'argument' for any additional data related to this command

  return true; // Successfully handled
}

void SerialPacketParser::resetCommandAndArgument(char *command, char *argument)
{
  // Reset the command and argument arrays
  memset(command, 0, sizeof(command));
  memset(argument, 0, sizeof(argument));
}
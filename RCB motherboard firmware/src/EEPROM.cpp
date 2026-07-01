/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    common.cpp
 * @brief   This file provides code for utilities for EEPROM handling
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

#include "EEPROM.h"

I2C_eeprom eeprom(DEVICEADDRESS, EEPROM);

/// @brief Write float value to pageadress location + 3.
/// @brief If page address is 0; data will be written to 0, 1,2 and 3
/// @param pageAddress page aaddress
/// @param data data we want to write
void writeFloat(unsigned int pageAddress, float data)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  memcpy(temp_byte, &data, 4);
  eeprom.writeByte(pageAddress, temp_byte[0]);
  eeprom.writeByte(pageAddress + 1, temp_byte[1]);
  eeprom.writeByte(pageAddress + 2, temp_byte[2]);
  eeprom.writeByte(pageAddress + 3, temp_byte[3]);
}

/// @brief Read float value from pageadress location + 3.
/// @brief If page address is 0; data will be read to 0, 1,2 and 3
/// @param pageAddress page aaddress
/// @return float value
float readFloat(unsigned int pageAddress)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  temp_byte[0] = eeprom.readByte(pageAddress);
  temp_byte[1] = eeprom.readByte(pageAddress + 1);
  temp_byte[2] = eeprom.readByte(pageAddress + 2);
  temp_byte[3] = eeprom.readByte(pageAddress + 3);
  float restoredFloat;
  memcpy(&restoredFloat, temp_byte, 4);
  return restoredFloat;
}

/// @brief Read int value from pageadress location + 3.
/// @brief If page address is 0; data will be read to 0, 1,2 and 3
/// @param pageAddress page aaddress
/// @return int value
int32_t readInt(unsigned int pageAddress)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  temp_byte[0] = eeprom.readByte(pageAddress);
  temp_byte[1] = eeprom.readByte(pageAddress + 1);
  temp_byte[2] = eeprom.readByte(pageAddress + 2);
  temp_byte[3] = eeprom.readByte(pageAddress + 3);
  int32_t restoredInt;
  memcpy(&restoredInt, temp_byte, 4);
  return restoredInt;
}

/// @brief Write int value to pageadress location + 3.
/// @brief If page address is 0; data will be written to 0, 1,2 and 3
/// @param pageAddress page aaddress
/// @param data data we want to write
void writeInt(unsigned int pageAddress, int32_t data)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  memcpy(temp_byte, &data, 4);
  eeprom.writeByte(pageAddress, temp_byte[0]);
  eeprom.writeByte(pageAddress + 1, temp_byte[1]);
  eeprom.writeByte(pageAddress + 2, temp_byte[2]);
  eeprom.writeByte(pageAddress + 3, temp_byte[3]);
}

/*
/// @brief Read byte value from pageadress location
/// @param pageAddress page aaddress
/// @return byte value
int8_t readInt_8t(unsigned int pageAddress)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  temp_byte[0] = eeprom.readByte(pageAddress);
  temp_byte[1] = eeprom.readByte(pageAddress + 1);
  temp_byte[2] = eeprom.readByte(pageAddress + 2);
  temp_byte[3] = eeprom.readByte(pageAddress + 3);
  int32_t restoredInt;
  memcpy(&restoredInt, temp_byte, 4);
  return restoredInt;
}

/// @brief Write int value to pageadress location
/// @param pageAddress page aaddress
/// @param data byte we want to write
void writeInt_8t(unsigned int pageAddress, int8_t data)
{
  uint8_t temp_byte[4] = {0, 0, 0, 0};
  memcpy(temp_byte, &data, 4);
  eeprom.writeByte(pageAddress, temp_byte[0]);
  eeprom.writeByte(pageAddress + 1, temp_byte[1]);
  eeprom.writeByte(pageAddress + 2, temp_byte[2]);
  eeprom.writeByte(pageAddress + 3, temp_byte[3]);
}
*/

/// @brief Init EEPROM memory
void Init_EEPROM()
{
  Wire.setSDA(EEPROM_SDA);
  Wire.setSCL(EEPROM_SCL);
  eeprom.begin();
  Wire.setClock(1000000);
}

/// @brief Read the config saved in the EEPROM
void read_config()
{

  controller.HARDWARE_VERSION = readInt(HARDWARE_VERSION_EEPROM);
  controller.BATCH_DATE = readInt(BATCH_DATA_EEPROM);
  controller.SOFTWARE_VERSION = readInt(SOFTWARE_VERSION_EEPROM);
  controller.SERIAL_NUMBER = readInt(SERIAL_NUMBER_EEPROM);

 
}

/// @brief Clean the config that is saved in the EEPROM
void Clean_config()
{
}

/// @brief Write latest data to the EEPROM
/// @details
void Write_config()
{

  writeInt(SERIAL_NUMBER_EEPROM,controller.SERIAL_NUMBER);
  writeInt(HARDWARE_VERSION_EEPROM, controller.HARDWARE_VERSION);
  writeInt(BATCH_DATA_EEPROM, controller.BATCH_DATE);
  writeInt(SOFTWARE_VERSION_EEPROM, controller.SOFTWARE_VERSION);
 
}

/// @brief  Reset to default config.  First write to EEPROM then read it back.
void Set_Default_config(){

  writeInt(SERIAL_NUMBER_EEPROM,123);
  writeInt(HARDWARE_VERSION_EEPROM, 111);
  writeInt(BATCH_DATA_EEPROM, 200);
  writeInt(SOFTWARE_VERSION_EEPROM, 122);

  read_config();

}

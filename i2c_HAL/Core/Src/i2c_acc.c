/*
 * i2c_uss.c
 *
 *  Created on: Apr 12, 2024
 *      Author: lars
 */

#include "i2c_acc.h"



static const uint16_t dev_id = 0x1C << 1;
static I2C_HandleTypeDef i2c_handle;

static HAL_StatusTypeDef command(uint8_t reg, uint8_t val);
static HAL_StatusTypeDef read_register(uint8_t reg, uint8_t *data, uint8_t size);

void configure_vma208(I2C_HandleTypeDef i2ch)
{
	i2c_handle = i2ch;

	// Set to 4G
	uint8_t data[2] = {0x0E, 0x01};
	command(0x0E, 0x01);

	// Deactivate
	data[0] = 0x2A;
	data[1] = 0x00;
	command(0x2A, 0);

	// Setup and Activate
	data[1] = 0b110<<3 | 1U;
	command(0x2A, data[1]);
}

void read_whoami(uint8_t *data)
{
	read_register(0x0D, data, 1);
}

void read_status(uint8_t *data)
{
	read_register(0x00, data, 1);
}

void read_x(uint8_t *data)
{
	read_register(0x01, data, 2);
}

void read_y(uint8_t *data)
{
	read_register(0x03, data, 2);
}

void read_z(uint8_t *data)
{
	read_register(0x05, data, 2);
}

void read_xyz(uint8_t *data){
	read_register(0x01, data, 6);
}

static HAL_StatusTypeDef command(uint8_t reg, uint8_t val)
{
	uint8_t data[] = {reg, val};
	return HAL_I2C_Master_Transmit(&i2c_handle, dev_id, data, 2, 100);
}

static HAL_StatusTypeDef read_register(uint8_t reg, uint8_t *data, uint8_t size)
{
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Read(&i2c_handle, dev_id, reg, 1, data, size, 100);

	return ret;
}
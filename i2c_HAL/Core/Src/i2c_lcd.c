/*
 * i2c_lcd.c
 *
 *  Created on: Apr 7, 2024
 *      Author: lars
 */

/** File includes **/
#include "stm32l1xx_hal.h"
#include <stdint.h>

#include "i2c_lcd.h"


#define DISPLAY_ON		(1U<<2)
#define SHOW_CURSOR 	(1U<<1)
#define SHOW_BLINKING	(1U<<0)

/** Struct **/
struct lcd_states {
	uint8_t functionModes;
	uint8_t displayControl;
	uint8_t entryMode;
};

struct lcd_params {
	I2C_HandleTypeDef i2c_handle;
	uint8_t rows;
	uint8_t cols;
};

/** Function prototypes **/
static HAL_StatusTypeDef command(uint8_t value);

/** Global Variables **/
static struct lcd_params i2cLCD;
static struct lcd_states states;
static const uint16_t dev_addr = 0x3E << 1;

HAL_StatusTypeDef configure_i2c_lcd(uint8_t rows, uint8_t cols, I2C_HandleTypeDef i2ch)
{
	i2cLCD.i2c_handle = i2ch;

	HAL_StatusTypeDef ret;

	// Wait for power stability (docs rec 15ms)
	HAL_Delay(15);

	/***
	*   Set function modes (001XNFXX)
	*   N: 0 = 1-Line mode, 1 = 2-Line mode
	*   F: 0 = 5*8 dots, 1 = 5*10 dots
	***/
	states.functionModes = 0b00101000;
	ret = command(states.functionModes);
	if(ret != HAL_OK)
	{
		return ret;
	}
	// Wait at least 39 us
	HAL_Delay(1);

	/***
	*	Set Display Control (00001DCB)
	*	D: 0 = Display Off, 1 = Display On
	*	C: 0 = Cursor Off, 1 = Cursor On
	*	B: 0 = Blink Off, 1 = Blink On
	***/
	states.displayControl = 0b00001111;
	ret = command(states.displayControl);
	if(ret != HAL_OK)
	{
		return ret;
	}
	// Wait at least 39 us
	HAL_Delay(2);

	/***
	* 	Display Clear (00000001)
	***/
	ret = command(0b00000001);
	if(ret != HAL_OK)
	{
		return ret;
	}
	// Wait at least 1.53 ms
	HAL_Delay(2);

	/***
	* 	Entry Mode Set (000001|I/D|S)
	* 	I/D: 0 = Decrement Mode, 1 = Increment Mode
	* 	S: Shift
	*/
	states.entryMode = 0b0000110;
	ret = command(states.entryMode);
	if(ret != HAL_OK)
	{
		return ret;
	}
	// Wait at least 1.53 ms
	HAL_Delay(2);

	return HAL_OK;
}

HAL_StatusTypeDef lcd_write(unsigned char value)
{
	unsigned char data[2] = {0x40, value};
	return HAL_I2C_Master_Transmit(&i2cLCD.i2c_handle, dev_addr, data, 2, 100);
}

HAL_StatusTypeDef lcd_writes(char *value, uint32_t len)
{
	uint32_t size = 0;
	while(len-1 > size)
	{
		unsigned char data[2] = {0x40, value[size]};
		HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&i2cLCD.i2c_handle, dev_addr, data, 2, 100);
		if(ret != HAL_OK)
		{
			return ret;
		}
		size++;
	}
	unsigned char data[2] = {0x40, value[size]};
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&i2cLCD.i2c_handle, dev_addr, data, 2, 100);

	return ret;
}

HAL_StatusTypeDef lcd_clear(void)
{
	HAL_StatusTypeDef ret =	command(0x01);
	HAL_Delay(2000);
	return ret;
}

HAL_StatusTypeDef ctrl_lcd_display(void)
{
	states.displayControl |= DISPLAY_ON;

	return command(states.displayControl);
}

HAL_StatusTypeDef ctrl_lcd_nodisplay(void)
{
	states.displayControl &= ~DISPLAY_ON;

	return command(states.displayControl);
}

HAL_StatusTypeDef ctrl_lcd_blink(void)
{
	states.displayControl |= SHOW_BLINKING;

	return command(states.displayControl);
}

HAL_StatusTypeDef ctrl_lcd_noblink(void)
{
	states.displayControl &= ~SHOW_BLINKING;

	return command(states.displayControl);
}

HAL_StatusTypeDef set_row_col(uint8_t row, uint8_t col)
{
	if(row)
	{
		col = col | 0xc0;
	} else {
		col = col |0x80;
	}
	return command(col);
}

static HAL_StatusTypeDef command(uint8_t value)
{
	unsigned char data[2] = {0x80, value};
	return HAL_I2C_Master_Transmit(&i2cLCD.i2c_handle, dev_addr, data, 2, 100);
}

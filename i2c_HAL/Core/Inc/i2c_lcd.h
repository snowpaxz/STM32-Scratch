/*
 * i2c_lcd.h
 *
 *  Created on: Apr 7, 2024
 *      Author: lars
 */

#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

HAL_StatusTypeDef configure_i2c_lcd(uint8_t rows, uint8_t cols, I2C_HandleTypeDef i2ch);
HAL_StatusTypeDef lcd_write(uint8_t value);
HAL_StatusTypeDef lcd_writes(char *value, uint32_t len);
HAL_StatusTypeDef lcd_clear(void);
HAL_StatusTypeDef ctrl_lcd_display(void);
HAL_StatusTypeDef ctrl_lcd_nodisplay(void);
HAL_StatusTypeDef ctrl_lcd_blink(void);
HAL_StatusTypeDef ctrl_lcd_noblink(void);
HAL_StatusTypeDef set_row_col(uint8_t row, uint8_t col);

#endif /* INC_I2C_LCD_H_ */
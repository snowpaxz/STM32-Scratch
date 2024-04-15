/*
 * i2c_uss.h
 *
 *  Created on: Apr 12, 2024
 *      Author: lars
 */

#ifndef INC_I2C_ACC_H_
#define INC_I2C_ACC_H_

#include <stdint.h>
#include "stm32l1xx_hal.h"

void configure_vma208(I2C_HandleTypeDef i2ch);
void read_whoami(uint8_t *data);
void read_status(uint8_t *data);
void read_x(uint8_t *data);
void read_y(uint8_t *data);
void read_z(uint8_t *data);
void read_xyz(uint8_t *data);


#endif /* INC_I2C_ACC_H_ */
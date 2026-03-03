 /*
 * i2c.h
 *
 *  Created on: Mar 3, 2026
 *      Author: maksym
 */

#include "stm32f407xx.h"

#ifndef I2C_H_
#define I2C_H_

//Configuration structure for I2Cx peripheral
typedef struct{
	uint32_t I2C_SLCSpeed;
	uint8_t I2C_DeviseAddress;
	uint8_t I2C_AckControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;


//Handle structure for I2C peripheral
typedef struct{
	I2C_REG_t *pI2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handle_t;


//@I2C_SLCSpeed
#define I2C_SCL_SPEED_SM 100000
#define I2C_SCL_SPEED_FM 400000
#define I2C_SCL_SPEED_2K 200000

//@I2C_AckControl
#define I2C_ACK_ENABLE  1
#define I2C_ACK_DISABLE 0

//@I2C_FMDutyCycle
#define I2C_FM_DUTY_2     0
#define I2C_FM_DUTY_16_9  1

//APIs supported by I2C

void I2C_Init(I2C_Handle_t *pI2C_Handle);
void I2C_DeInit(I2C_REG_t *pI2Cx);
void I2C_ClockControl(I2C_REG_t *pI2Cx, uint8_t en_di_mode);
void I2C_IRQ_InterruptConfig(uint8_t IRQ_Number, uint8_t en_di_mode);
void I2C_IRQ_PriorityConfig(uint8_t IRQ_Number, uint8_t IRQ_Priority);
void I2C_PeripheralControl(uint8_t IRQ_Number, uint8_t IRQ_Priority);
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t event);
uint8_t I2C_GetFlagStatus(I2C_REG_t *pI2Cx, uint32_t flag);

#endif /* I2C_H_ */

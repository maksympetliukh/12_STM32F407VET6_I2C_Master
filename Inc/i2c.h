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
	uint32_t I2C_SCLSpeed;
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
void I2C_PeripheralControl(I2C_REG_t *pI2Cx, uint8_t en_di_mode);
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t event);
uint8_t I2C_GetFlagStatus(I2C_REG_t *pI2Cx, uint32_t flag);
uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPLLOutputClock(void);
void I2C_Master_Transmit(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint8_t len, uint8_t SlaveAddr);
void I2C_GenerateStartCondition(I2C_REG_t *pI2Cx);
void I2C_GenerateStopCondition(I2C_REG_t *pI2Cx);
void I2C_ExecuteAddressPhase(I2C_REG_t *pI2Cx, uint8_t SlaveAddr);
void I2C_ClearADDRFlag(I2C_REG_t *pI2Cx);

/*
 * I2C Flags
 */
#define I2C_SB_FLAG          (1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG        (1 << I2C_SR1_ADDR)
#define I2C_BTF_FLAG         (1 << I2C_SR1_BTF)
#define I2C_ADD10_FLAG       (1 << I2C_SR1_ADD10)
#define I2C_STOPF_FLAG       (1 << I2C_SR1_STOPF)
#define I2C_RXNE_FLAG        (1 << I2C_SR1_RXNE)
#define I2C_TXE_FLAG         (1 << I2C_SR1_TXE)
#define I2C_BERR_FLAG        (1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG        (1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG          (1 << I2C_SR1_AF)
#define I2C_OVR_FLAG         (1 << I2C_SR1_OVR)
#define I2C_PECERR_FLAG      (1 << I2C_SR1_PECERR)
#define I2C_TIMEOUT_FLAG     (1 << I2C_SR1_TIMEOUT)
#define I2C_SMBALERT_FLAG    (1 << I2C_SR1_SMBALERT)

#endif /* I2C_H_ */

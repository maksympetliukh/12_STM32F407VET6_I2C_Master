/*
 * i2c.c
 *
 *  Created on: Mar 3, 2026
 *      Author: maksym
 */


#include "i2c.h"

/**************************************
 * @fn          I2C_Init
 *
 * @brief       This function initializes the I2C port according to the specified settings in the handle structure
 *
 * @param[in]   Pointer to the structure which contains I2C specified configuration settings
 *
 * @return      none
 */
void I2C_Init(I2C_Handle_t *pI2C_Handle){

}

/**************************************
 * @fn          I2C_DeInit
 *
 * @brief       This function disables I2C port
 *
 * @param[in]   Pointer to the structure which contains I2C peripheral registers
 *
 * @return      none
 */
void I2C_DeInit(I2C_REG_t *pI2Cx){
	if(pI2Cx == NULL){return;}

	if(pI2Cx == I2C1){
		I2C1_REG_RESET();
	}else if(pI2Cx == I2C2){
		I2C2_REG_RESET();
	}else if(pI2Cx == I2C3){
		I2C3_REG_RESET();
	}
}

/**************************************
 * @fn          I2C_ClockControl
 *
 * @brief       This function enables/disables a Peripheral Clock for the I2C peripheral
 *
 * @param[in]   Pointer to the structure which contains I2C peripheral registers
 * @param[in]   Enable/Disable mode variable
 *
 * @return      none
 */
void I2C_ClockControl(I2C_REG_t *pI2Cx, uint8_t en_di_mode){
	if(en_di_mode == ENABLE){
		if(pI2Cx == I2C1){
			RCC_I2C1_CLK_ENABLE();
		}else if(pI2Cx == I2C2){
			RCC_I2C2_CLK_ENABLE();
		}else if(pI2Cx == I2C3){
			RCC_I2C3_CLK_ENABLE();
		}
	}else if(en_di_mode == DISABLE){
		if(pI2Cx == I2C1){
			RCC_I2C1_CLK_DISABLE();
		}else if(pI2Cx == I2C2){
			RCC_I2C2_CLK_DISABLE();
		}else if(pI2Cx == I2C3){
			RCC_I2C3_CLK_DISABLE();
		}
	}
}

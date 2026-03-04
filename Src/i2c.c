/*
 * i2c.c
 *
 *  Created on: Mar 3, 2026
 *      Author: maksym
 */


#include "i2c.h"

//Unused function
uint32_t RCC_GetPLLOutputClock(void){
	return 0;
}

//Array which contains all division factors of the AHB prescaler
uint16_t AHB_Prescaler[8] = {2, 4, 8, 16, 32, 64, 128, 256, 512};

//Array which contains all division factors of the APB prescaler
uint16_t APB1_Prescaler[4] = {2, 4, 8, 16};

/**************************************
 * @fn          RCC_GetPCLK1Value
 *
 * @Brief       This function returns the value of frequency of the required peripheral clock
 *
 * return       Variable which contains the value of frequency of the peripheral clock
 */
uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, SystemClk;

	uint8_t clksrc, tmp, ahb_prescaler, apb1_prescaler;
	clksrc = ((RCC->CFGR >> 2) & 0x3); //Right shift by 2 to take bits [3:2] and mask other bits

	if(clksrc == 0){
		SystemClk = 16000000;
	}else if(clksrc == 1){
		SystemClk = 8000000;
	}else if(clksrc == 2){
		SystemClk = RCC_GetPLLOutputClock();
	}

	//AHB bus prescaling
	tmp = ((RCC->CFGR >> 4) & 0xF);

	if(tmp < 8){
		ahb_prescaler = 1;
	}else{
		ahb_prescaler = AHB_Prescaler[tmp - 8];
	}

	//APB prescaling
	tmp = ((RCC->CFGR >> 10) & 0x7);

	if(tmp < 4){
		apb1_prescaler = 1;
	}else{
		apb1_prescaler = APB1_Prescaler[tmp - 4];
	}

	pclk1 = (SystemClk / ahb_prescaler) / apb1_prescaler;

	return pclk1;
}

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
	uint32_t tempreg = 0;

	//Configure the ACK control bit
	tempreg |= pI2C_Handle->I2C_Config.I2C_AckControl << 10;
	pI2C_Handle->pI2Cx->CR1 = tempreg;

	//Configure the FREQ field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2C_Handle->pI2Cx->CR2 = (tempreg & 0x3F);

	//Program the device own address OAR1
	tempreg = 0;
	tempreg |= pI2C_Handle->I2C_Config.I2C_DeviseAddress << 1;
	tempreg |= (1 << 14);//according to the RM0090 14th bit must be set despite it is a reserved bit
	pI2C_Handle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2C_Handle->I2C_Config.I2C_SLCSpeed <= I2C_SCL_SPEED_SM){

		//In case of Standard mode
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2C_Handle->I2C_Config.I2C_SLCSpeed);
		tempreg |= (ccr_value & 0xFFF);

	}else if(pI2C_Handle->I2C_Config.I2C_SLCSpeed <= I2C_SCL_SPEED_FM){

		//In case of Fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2C_Handle->I2C_Config.I2C_FMDutyCycle << 14);

		if(pI2C_Handle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2C_Handle->I2C_Config.I2C_SLCSpeed);
		}else if(pI2C_Handle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_16_9){
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2C_Handle->I2C_Config.I2C_SLCSpeed);
		}
		tempreg |= (ccr_value & 0xFFF);
	}

	pI2C_Handle->pI2Cx->CCR = tempreg;

	//TRISE configuration
	tempreg = 0;
	if(pI2C_Handle->I2C_Config.I2C_SLCSpeed <= I2C_SCL_SPEED_SM){

		//standard mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
	}else{
		//fast mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000U) + 1;
	}

	pI2C_Handle->pI2Cx->TRISE = (tempreg & 0xFFF);
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

//Helper functions
void I2C_GenerateStartCondition(I2C_REG_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

uint8_t I2C_GetFlagStatus(I2C_REG_t *pI2Cx, uint32_t flag){
	if(pI2Cx->SR1 & flag){
		return RESET;
	}
	return SET;
}

void I2C_ExecuteAddressPhase(I2C_REG_t *pI2Cx, uint8_t SlaveAddr){
	SlaveAddr = SlaveAddr << 1; //make a space for r/w bit
	SlaveAddr &= ~(1); //clear the bit 0
	pI2Cx->DR = SlaveAddr;
}

void I2C_ClearADDRFlag(I2C_REG_t *pI2Cx){
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}

void I2C_GenerateStopCondition(I2C_REG_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

/*********************************************
 * @fn           I2C_Master_Transmit
 *
 * @brief        This function sends data from Master to slave via I2C peripheral
 *
 * @param[in]    Pointer to the structure which contains I2C specified configuration settings
 * @param[in]    Pointer to the TX buffer with data which we want to send
 * @param[in]    Length of the data
 * @param[in]    Address of the slave device
 *
 * @return       none
 */
void I2C_Master_Transmit(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint8_t len, uint8_t SlaveAddr){

	//Generate START condition
	I2C_GenerateStartCondition(pI2C_Handle->pI2Cx);

	//Confirm that start generation is completed by checking the SB flag in the SR1
	//Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2C_Handle->pI2Cx, I2C_SB_FLAG));

	//Send address of the slave with r/w bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhase(pI2C_Handle->pI2Cx, SlaveAddr);

	//Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2C_Handle->pI2Cx, I2C_ADDR_FLAG));

	//Clear the ADDR flag according to its software sequence
	//Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2C_Handle->pI2Cx);

	//Transmit the data until the length becomes 0
	while(len > 0){
		while(!I2C_GetFlagStatus(pI2C_Handle->pI2Cx, I2C_TXE_FLAG));
		pI2C_Handle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	//When len becomes 0, wait for TXE = 1 and BTF = 1 before generating the STOP condition
	//TXE = 1, BTF = 1 means that both SR1 and DR are empty and next transmission should begin
	//When BTF = 1 SCL will be stretched (pulled to low)
	while(!I2C_GetFlagStatus(pI2C_Handle->pI2Cx, I2C_TXE_FLAG));
	while(!I2C_GetFlagStatus(pI2C_Handle->pI2Cx, I2C_BTF_FLAG));

	//Generate the STOP condition and master need not to wait for the completion of stop condition
	//Generating STOP automatically clears the BTF
	I2C_GenerateStopCondition(pI2C_Handle->pI2Cx);
}

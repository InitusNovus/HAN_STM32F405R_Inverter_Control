/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 *
 */

#include "GAS_Can.h"
#include <stdio.h>

CAN_FilterTypeDef sFilterConfig;
CAN_RxHeaderTypeDef canRxHeader;
CAN_TxHeaderTypeDef canTxHeader;
uint8_t canRx0Data[8];
uint32_t TxMailBox;
uint32_t WhatIsThis;
stm32_msg1_t stm32_1;
stm32_msg2_t stm32_2;

uint32_t STM32_ID = 0x32F103A;
uint32_t STM32_ID2 = 0x32F103B;

/*-------------------------Function Prototypes--------------------------------*/
void GAS_Can_txSetting(void);
void GAS_Can_rxSetting(void);
void GAS_Can_init(void);
void GAS_Can_sendMessage(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);
void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);

void GAS_Can_txSetting(void)
{

	  //canTxHeader.StdId = (STM32_ID>>18)&0x7ff;
	  canTxHeader.ExtId = STM32_ID;
	  canTxHeader.IDE	= CAN_ID_EXT;
	  canTxHeader.RTR	= CAN_RTR_DATA;
	  canTxHeader.DLC	=	8;


}

void GAS_Can_rxSetting(void){

	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	 if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	  {
	    /* Filter configuration Error */
	    Error_Handler();
	  }
}

void GAS_Can_init(void)
{

	GAS_Can_txSetting();
	GAS_Can_rxSetting();


	HAL_CAN_Start(&hcan2);

	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}



//	if (HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK)
//
//
//	{
//
//
//	/* Reception Error */
//
//
//	Error_Handler();
//
//
//	}

}

void GAS_Can_sendMessage(void)
{

	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader, &stm32_1.TxData[0], &TxMailBox);
}

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	if(hcan->Instance == CAN1)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, stm32_2.RxData);
//
//	}
//}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, stm32_2.RxData);

	}
}

void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan)
{
	WhatIsThis=HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0);
		if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) != 0)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, stm32_2.RxData);
	}

}

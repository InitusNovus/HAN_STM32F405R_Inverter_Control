/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 *
 */

#include "GAS_Can.h"
#include "CanMessage.h"
#include <stdio.h>

#define GAS_CAN_SFBK 14U

uint8_t canRx0Data[8];

/*-------------------------Function Prototypes--------------------------------*/
static void GAS_Can_rxSetting(void);

/*----------------------Function Implementations------------------------------*/
static void GAS_Can_rxSetting(void)
{
    {//Filter bank 0 setting
        CAN_FilterTypeDef sFilterConfig;
        uint16_t StdId = 0;

        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.SlaveStartFilterBank = GAS_CAN_SFBK;

        InverterControl_msg_rx_1.can = &hcan1;
		InverterControl_msg_rx_2.can = &hcan1;
		
		sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
        InverterControl_msg_rx_1.fifoAssignment = sFilterConfig.FilterFIFOAssignment;
		InverterControl_msg_rx_2.fifoAssignment = sFilterConfig.FilterFIFOAssignment;

        sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
        sFilterConfig.FilterBank = 0;

		StdId = InverterControl_msg_rx_1.StdId; //The first 32bit ID list of filter bank 0
        sFilterConfig.FilterIdHigh = StdId << 5; // FIM: 0 for msg_rx_1
        sFilterConfig.FilterIdLow = 0x0000;

		StdId = InverterControl_msg_rx_2.StdId; //The second 32bit ID list of filter bank 0
        sFilterConfig.FilterMaskIdHigh = StdId << 5; //FIM: 1 for msg_rx_2
        sFilterConfig.FilterMaskIdLow = 0x0000;

        InverterControl_msg_rx_1.fim = 0;
		InverterControl_msg_rx_2.fim = 1;

        if (HAL_CAN_ConfigFilter(InverterControl_msg_rx_1.can, &sFilterConfig) != HAL_OK)
        {
            /* Filter configuration Error */
            Error_Handler();
        }
    }
}

void GAS_Can_init(void)
{
    GAS_Can_rxSetting();

    HAL_CAN_Start(&hcan1);

    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
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

void GAS_Can_sendMessage(GAS_Can_message_tx_t *msg)
{
    uint32_t TxMailBox;
    CAN_TxHeaderTypeDef canTxHeader;

    TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(msg->can);
    if (msg->idType == ext)
        canTxHeader.ExtId = msg->id;
    else
        canTxHeader.StdId = msg->id;
    canTxHeader.DLC = msg->dlc;
    canTxHeader.IDE = msg->ide;
    canTxHeader.RTR = msg->rtr;
    HAL_CAN_AddTxMessage(msg->can, &canTxHeader, msg->data, &TxMailBox);
}

// void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	if(hcan->Instance == CAN2)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, stm32_2.RxData);
//	}
// }
//
// void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan)
//{
//	if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) != 0)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, stm32_2.RxData);
//	}
// }

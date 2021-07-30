/*
 * CAN.h
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

//#include "stm32f4xx_hal.h"
#include "can.h"
//#include "GAS_Vadc.h"

#define GAS_CAN_SFBK 14U

typedef enum
{
	fifo0 = 0,
	fifo1,
}GAS_Can_rxfifo;

typedef struct
{
	CAN_HandleTypeDef *can;
	CAN_FilterTypeDef filter;
	CAN_RxHeaderTypeDef header;
	unit32_t data[2];
}GAS_Can_message_rx_t;

typedef struct
{
	CAN_HandleTypeDef *can;
	GAS_Can_rxfifo fifo;
	uint32_t address;
}GAS_Can_message_rx_config;

typedef struct
{
	CAN_HandleTypeDef *can;
	CAN_TxHeaderTypeDef header;
	uint32_t data[2];
}GAS_Can_message_tx_t;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage(void);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);

extern void GAS_Can_init_message_rx(GAS_Can_message_rx_t *msg);
extern void GAS_Can_init_message_tx(GAS_Can_message_tx_t *msg);
#endif /* SRC_GETANDSEND_H_ */

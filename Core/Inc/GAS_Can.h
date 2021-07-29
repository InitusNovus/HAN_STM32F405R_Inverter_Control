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

typedef union{
	uint8_t TxData[8];
	struct{
		unsigned int sensor0 : 12;
		unsigned int sensor1 : 12;
		unsigned int sensor2 : 12;
		unsigned int sensor3 : 12;
		unsigned int sensor4 : 12;
		unsigned int reserved: 4;

	}__attribute__((aligned(1),packed)) B;

}stm32_msg1_t;

typedef union{
	uint8_t RxData[8];
	struct{
		unsigned int sensor0 : 12;
		unsigned int sensor1 : 12;
		unsigned int sensor2 : 12;
		unsigned int sensor3 : 12;
		unsigned int sensor4 : 12;
		unsigned int reserved: 4;

	}__attribute__((aligned(1),packed)) B;

}stm32_msg2_t;

extern stm32_msg1_t stm32_1;
extern stm32_msg2_t stm32_2;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage(void);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */

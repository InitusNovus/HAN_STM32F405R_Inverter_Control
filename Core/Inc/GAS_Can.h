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


typedef enum
{
	std = 0,
	ext,
}GAS_Can_idType;

typedef struct
{
    CAN_HandleTypeDef *can;
    uint32_t id;
    GAS_Can_idType idType;
    uint32_t ide;
    uint32_t rtr;
    uint32_t dlc;
    uint8_t data[8];
}GAS_Can_message_tx_t;


extern void GAS_Can_init(void);
//extern void GAS_Can_sendMessage(void);
extern void GAS_Can_sendMessage(GAS_Can_message_tx_t *msg);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);



//static inline __attribute__((always_inline)) GAS_Can_



#endif /* SRC_GETANDSEND_H_ */

/*
 * CanMessage.h
 *
 *  Created on: Aug 7, 2021
 *      Author: InitusNovus
 *      TODO: rx msg2
 */

#ifndef INC_CANMESSAGE_H_
#define INC_CANMESSAGE_H_

/***Includes******************************************************************/
#include <stdbool.h>
#include "GAS_Can.h"
#include "InverterControl.h"

/***Enumerations**************************************************************/
typedef enum
{
	CanMsg_none = 0,
	CanMsg_updated,
	CanMsg_overridden,
}CanMsg_update;
typedef enum
{
	none = 0,
	write = 1,
}CanMsg_InverterControl_rx_1_control;

typedef enum
{
	inv1 = 0,
	inv2 = 1,
	inv3 = 2,
	inv4 = 3,
}CanMsg_InverterControl_rx_1_invSel;

/***Data Structures***********************************************************/
typedef union
{
	uint8_t 								U[2];

	struct {
	CanMsg_InverterControl_rx_1_control 	control		:1;
	CanMsg_InverterControl_rx_1_invSel		invSel		:2;
	uint8_t 								reserved	:5;

	bool									setBe1		:1;
	bool	 								rstBe1		:1;
	bool									setBe2		:1;
	bool	 								rstBe2		:1;
	uint8_t 								reserved1	:4;
	}__attribute__((aligned(1), packed)) 	B;
}CanMsg_InverterControl_rx_1_msg;

typedef struct
{
	CAN_HandleTypeDef 						*can;			//FIXME: When CAN filter bank calculation is made
	uint32_t								fifoAssignment;	//FIXME: When CAN filter bank calculation is made
	uint32_t								fim;
	/*.can, .fifoAssignment and .fim fields is filled by GAS_can*/
	uint16_t								StdId;		//Std address
	CAN_RxHeaderTypeDef 					canRxHeader;
	CanMsg_InverterControl_rx_1_msg			msg;
	CanMsg_update							updated;
}CanMsg_InverterControl_rx_1;

typedef union{
	uint8_t 								U[8];
	struct{
		uint16_t 							EFon;
		uint16_t 							BE1on;
		uint16_t 							BE2on;
		uint16_t 							Remain;
	}__attribute__((aligned(1), packed))	B;
}CanMsg_InverterControl_rx_2_msg;

typedef struct
{
	CAN_HandleTypeDef 						*can;			//FIXME: When CAN filter bank calculation is made
	uint32_t								fifoAssignment;	//FIXME: When CAN filter bank calculation is made
	uint32_t								fim;
	/*.can, .fifoAssignment and .fim fields is filled by GAS_can*/
	uint16_t								StdId;		//Std address
	CAN_RxHeaderTypeDef 					canRxHeader;
	CanMsg_InverterControl_rx_2_msg			msg;
	CanMsg_update							updated;
}CanMsg_InverterControl_rx_2;

/***Global Variables**********************************************************/
extern CanMsg_InverterControl_rx_1 InverterControl_msg_rx_1;
extern CanMsg_InverterControl_rx_2 InverterControl_msg_rx_2;

#endif /* INC_CANMESSAGE_H_ */

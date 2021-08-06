/*
 * CanMessage.h
 *
 *  Created on: Aug 7, 2021
 *      Author: InitusNovus
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
	none = 0,
	write = 1,
}CanMsg_InverterControl_rx_1_control;

typedef enum
{
	inv0 = 0,
	inv1,
	inv2,
	inv3,
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
}CanMsg_InverterControl_rx_1;

/***Global Variables**********************************************************/
extern CanMsg_InverterControl_rx_1 InverterControl_msg_rx_1;

#endif /* INC_CANMESSAGE_H_ */

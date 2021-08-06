/*
 * CanMessage.c
 *
 *  Created on: Aug 7, 2021
 *      Author: InitusNovus
 */

/***Includes******************************************************************/
#include "CanMessage.h"


CanMsg_InverterControl_rx_1 InverterControl_msg_rx_1 =
{
//		.can = &hcan1,
//		.fifoAssignment = CAN_RX_FIFO0,
		/*.can and .fifoAssignment field is filled by GAS_can*/
		.StdId = 0x100,
};

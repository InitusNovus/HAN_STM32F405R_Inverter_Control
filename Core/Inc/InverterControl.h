/*
 * InverterControl.h
 *
 *  Created on: Jul 30, 2021
 *      Author: InitusNovus
 */

#ifndef INC_INVERTERCONTROL_H_
#define INC_INVERTERCONTROL_H_

/***Includes******************************************************************/
#include <stdbool.h>
#include "gpio.h"

/***Defines*******************************************************************/
//#define INVCON_TEST

/***Function Prototypes*******************************************************/
extern void InverterControl_init(void);
//extern void InverterControl_Startup(void);
extern void InverterControl_Run_service(void);
extern void InverterControl_Run_1ms(void);
#ifdef INVCON_TEST
extern void InverterControl_test_1000ms(void);
#endif


/***Public Implicit Function Implementation***********************************/


#endif /* INC_INVERTERCONTROL_H_ */

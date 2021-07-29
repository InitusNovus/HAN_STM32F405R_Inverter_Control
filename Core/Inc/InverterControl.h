/*
 * InverterControl.h
 *
 *  Created on: Jul 30, 2021
 *      Author: InitusNovus
 */

#ifndef INC_INVERTERCONTROL_H_
#define INC_INVERTERCONTROL_H_

#include <stdbool.h>
#include "gpio.h"


#define INVCON_TEST


extern void InverterControl_Startup(void);

#ifdef INVCON_TEST
extern void InverterControl_test_1000ms(void);
#endif



#endif /* INC_INVERTERCONTROL_H_ */

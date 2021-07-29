/*
 * GAS_Scheduler.h
 *
 *  Created on: Oct 30, 2020
 *      Author: Suprhimp
 */

#ifndef INC_GAS_SCHEDULER_H_
#define INC_GAS_SCHEDULER_H_

#include <stdint.h>

typedef struct
{
	volatile uint8_t flag_1ms;
	volatile uint8_t flag_10ms;
	volatile uint8_t flag_100ms;
	volatile uint8_t flag_1000ms;
}Scheduler_task;

extern Scheduler_task gTask;

extern void GAS_Scheduler_init(void);
//extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
extern void GAS_Scheduler(void);
#endif /* INC_GAS_SCHEDULER_H_ */

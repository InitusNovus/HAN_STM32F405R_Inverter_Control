/*
 * InverterControl.c
 *
 *  Created on: Jul 30, 2021
 *      Author: InitusNovus
 */

#include "stm32f4xx_hal.h"
#include "inverterControl.h"
#include <stdint.h>

#define INVCON_INV1_BE1_GPIOX GPIOC
#define INVCON_INV1_BE1_GPIOP GPIO_PIN_11
#define INVCON_INV1_BE2_GPIOX GPIOC
#define INVCON_INV1_BE2_GPIOP GPIO_PIN_10
#define INVCON_INV1_BA3_GPIOX GPIOA
#define INVCON_INV1_BA3_GPIOP GPIO_PIN_15

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}InverterControl_Inverter_io;

typedef struct
{
	InverterControl_Inverter_io be1;
	InverterControl_Inverter_io be2;
	InverterControl_Inverter_io ba3;
} InverterControl_Inverter_t;

typedef struct
{
	InverterControl_Inverter_t Inv1;
	InverterControl_Inverter_t Inv2;
	InverterControl_Inverter_t Inv3;
	InverterControl_Inverter_t Inv4;
}InverterControl_t;

InverterControl_t InverterControl;

static void InverterControl_Startup_inv(InverterControl_Inverter_t* inv);
static void InverterControl_On_be1(InverterControl_Inverter_t* inv);
static void InverterControl_Off_be1(InverterControl_Inverter_t* inv);
static void InverterControl_On_be2(InverterControl_Inverter_t* inv);
static void InverterControl_Off_be2(InverterControl_Inverter_t* inv);
static bool InverterControl_Read_ba3(InverterControl_Inverter_t* inv);

void InverterControl_init(void)
{
	InverterControl.Inv1.be1.GPIOx = 	INVCON_INV1_BE1_GPIOX;
	InverterControl.Inv1.be1.GPIO_Pin = INVCON_INV1_BE1_GPIOP;
	InverterControl.Inv1.be2.GPIOx = 	INVCON_INV1_BE2_GPIOX;
	InverterControl.Inv1.be2.GPIO_Pin = INVCON_INV1_BE2_GPIOP;
	InverterControl.Inv1.ba3.GPIOx = 	INVCON_INV1_BA3_GPIOX;
	InverterControl.Inv1.ba3.GPIO_Pin = INVCON_INV1_BA3_GPIOP;
}

#ifdef INVCON_TEST
void InverterControl_test_1000ms(void)
{
	static int test = 0;
	if(test == 0)
	{
		InverterControl_On_be1(&InverterControl.Inv1);
//		InverterControl_Off_be2(&InverterControl.Inv1);
		test = 1;
	}
	else
	{
		InverterControl_Off_be1(&InverterControl.Inv1);
//		InverterControl_On_be2(&InverterControl.Inv1);
		test = 0;
	}

	uint32_t a;

	if(InverterControl_Read_ba3(&InverterControl.Inv1))
	{
		InverterControl_On_be2(&InverterControl.Inv1);
	}
	else
	{
		InverterControl_Off_be2(&InverterControl.Inv1);
	}
}
#endif

void InverterControl_Startup(void)
{

}

void InverterControl_Startup_inv(InverterControl_Inverter_t* inv)
{

}


static inline __attribute__((always_inline)) void InverterControl_On_be1(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be1.GPIOx,inv->be1.GPIO_Pin,GPIO_PIN_SET);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be1(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be1.GPIOx,inv->be1.GPIO_Pin,GPIO_PIN_RESET);
}

static inline __attribute__((always_inline)) void InverterControl_On_be2(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be2.GPIOx,inv->be2.GPIO_Pin,GPIO_PIN_SET);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be2(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be2.GPIOx,inv->be2.GPIO_Pin,GPIO_PIN_RESET);
}

static inline __attribute__((always_inline)) bool InverterControl_Read_ba3(InverterControl_Inverter_t* inv)
{
	return (bool)HAL_GPIO_ReadPin(inv->ba3.GPIOx,inv->ba3.GPIO_Pin);
}

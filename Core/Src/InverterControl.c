/*
 * InverterControl.c
 *
 *  Created on: Jul 30, 2021
 *      Author: InitusNovus
 */

/***Includes******************************************************************/
#include "stm32f4xx_hal.h"
#include "inverterControl.h"
#include "CanMessage.h"
#include <stdint.h>

/***Defines*******************************************************************/
#define INVCON_INV1_BE1_GPIOX GPIOC
#define INVCON_INV1_BE1_GPIOP GPIO_PIN_11
#define INVCON_INV1_BE2_GPIOX GPIOC
#define INVCON_INV1_BE2_GPIOP GPIO_PIN_10
#define INVCON_INV1_BA3_GPIOX GPIOA
#define INVCON_INV1_BA3_GPIOP GPIO_PIN_15

#define INVCON_INV2_BE1_GPIOX GPIOA
#define INVCON_INV2_BE1_GPIOP GPIO_PIN_10
#define INVCON_INV2_BE2_GPIOX GPIOA
#define INVCON_INV2_BE2_GPIOP GPIO_PIN_9
#define INVCON_INV2_BA3_GPIOX GPIOA
#define INVCON_INV2_BA3_GPIOP GPIO_PIN_8

#define INVCON_INV3_BE1_GPIOX GPIOC
#define INVCON_INV3_BE1_GPIOP GPIO_PIN_9
#define INVCON_INV3_BE2_GPIOX GPIOC
#define INVCON_INV3_BE2_GPIOP GPIO_PIN_8
#define INVCON_INV3_BA3_GPIOX GPIOC
#define INVCON_INV3_BA3_GPIOP GPIO_PIN_7

#define INVCON_INV4_BE1_GPIOX GPIOC
#define INVCON_INV4_BE1_GPIOP GPIO_PIN_6
#define INVCON_INV4_BE2_GPIOX GPIOB
#define INVCON_INV4_BE2_GPIOP GPIO_PIN_15
#define INVCON_INV4_BA3_GPIOX GPIOB
#define INVCON_INV4_BA3_GPIOP GPIO_PIN_14


/***Enumerations**************************************************************/

typedef enum
{
	InverterControl_io_none = 0,
	InverterControl_io_reset = 1,
	InverterControl_io_set = 2,
}InverterControl_Inverter_io_control_t;


/***Data Structures***********************************************************/

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	InverterControl_Inverter_io_control_t control;
	bool status;
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


/***Global Variable Declarations***/

InverterControl_t InverterControl;



/***Private Function Prototypes***********************************************/

static void InverterControl_Startup_inv(InverterControl_Inverter_t* inv);
static void InverterControl_Update_io(InverterControl_Inverter_t* inv);
static void InverterControl_On_be1(InverterControl_Inverter_t* inv);
static void InverterControl_Off_be1(InverterControl_Inverter_t* inv);
static void InverterControl_On_be2(InverterControl_Inverter_t* inv);
static void InverterControl_Off_be2(InverterControl_Inverter_t* inv);
static bool InverterControl_Read_ba3(InverterControl_Inverter_t* inv);


/***Public Function Implementations*******************************************/

void InverterControl_init(void)
{
	InverterControl.Inv1.be1.GPIOx = 	INVCON_INV1_BE1_GPIOX;
	InverterControl.Inv1.be1.GPIO_Pin = INVCON_INV1_BE1_GPIOP;
	InverterControl.Inv1.be1.control =	InverterControl_io_none;
	InverterControl.Inv1.be2.GPIOx = 	INVCON_INV1_BE2_GPIOX;
	InverterControl.Inv1.be2.GPIO_Pin = INVCON_INV1_BE2_GPIOP;
	InverterControl.Inv1.be2.control = 	InverterControl_io_none;
	InverterControl.Inv1.ba3.GPIOx = 	INVCON_INV1_BA3_GPIOX;
	InverterControl.Inv1.ba3.GPIO_Pin = INVCON_INV1_BA3_GPIOP;
	InverterControl.Inv1.ba3.control =	InverterControl_io_none;

	InverterControl.Inv2.be1.GPIOx = 	INVCON_INV2_BE1_GPIOX;
	InverterControl.Inv2.be1.GPIO_Pin = INVCON_INV2_BE1_GPIOP;
	InverterControl.Inv2.be1.control =	InverterControl_io_none;
	InverterControl.Inv2.be2.GPIOx = 	INVCON_INV2_BE2_GPIOX;
	InverterControl.Inv2.be2.GPIO_Pin = INVCON_INV2_BE2_GPIOP;
	InverterControl.Inv2.be2.control = 	InverterControl_io_none;
	InverterControl.Inv2.ba3.GPIOx = 	INVCON_INV2_BA3_GPIOX;
	InverterControl.Inv2.ba3.GPIO_Pin = INVCON_INV2_BA3_GPIOP;
	InverterControl.Inv2.ba3.control =	InverterControl_io_none;

	InverterControl.Inv3.be1.GPIOx = 	INVCON_INV3_BE1_GPIOX;
	InverterControl.Inv3.be1.GPIO_Pin = INVCON_INV3_BE1_GPIOP;
	InverterControl.Inv3.be1.control =	InverterControl_io_none;
	InverterControl.Inv3.be2.GPIOx = 	INVCON_INV3_BE2_GPIOX;
	InverterControl.Inv3.be2.GPIO_Pin = INVCON_INV3_BE2_GPIOP;
	InverterControl.Inv3.be2.control = 	InverterControl_io_none;
	InverterControl.Inv3.ba3.GPIOx = 	INVCON_INV3_BA3_GPIOX;
	InverterControl.Inv3.ba3.GPIO_Pin = INVCON_INV3_BA3_GPIOP;
	InverterControl.Inv3.ba3.control =	InverterControl_io_none;

	InverterControl.Inv4.be1.GPIOx = 	INVCON_INV4_BE1_GPIOX;
	InverterControl.Inv4.be1.GPIO_Pin = INVCON_INV4_BE1_GPIOP;
	InverterControl.Inv4.be1.control =	InverterControl_io_none;
	InverterControl.Inv4.be2.GPIOx = 	INVCON_INV4_BE2_GPIOX;
	InverterControl.Inv4.be2.GPIO_Pin = INVCON_INV4_BE2_GPIOP;
	InverterControl.Inv4.be2.control = 	InverterControl_io_none;
	InverterControl.Inv4.ba3.GPIOx = 	INVCON_INV4_BA3_GPIOX;
	InverterControl.Inv4.ba3.GPIO_Pin = INVCON_INV4_BA3_GPIOP;
	InverterControl.Inv4.ba3.control =	InverterControl_io_none;
}

void InverterControl_Run_service(void)
{
	InverterControl_Update_io(&InverterControl.Inv1);
	InverterControl_Update_io(&InverterControl.Inv2);
	InverterControl_Update_io(&InverterControl.Inv3);
	InverterControl_Update_io(&InverterControl.Inv4);
}

void InverterControl_Run_1ms(void)
{
	CAN_RxHeaderTypeDef canRxHeader;
	uint8_t rxDataBuf[8];
	while(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 0)
	{
		HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, rxDataBuf);
		if(canRxHeader.FilterMatchIndex == InverterControl_msg_rx_1.fim)
		{
			uint8_t dlc = canRxHeader.DLC;
			if(dlc > 0)
			{
				if(dlc > 8)
					dlc = 8;
				while(dlc--)
					InverterControl_msg_rx_1.msg.U[dlc] = rxDataBuf[dlc];
			}
		}
	}
}

#ifdef INVCON_TEST
void InverterControl_test_1000ms(void)
{
	static int test = 0;
	if(test == 0)
	{
		InverterControl_On_be1(&InverterControl.Inv1);
		InverterControl_Off_be2(&InverterControl.Inv1);
		InverterControl_On_be1(&InverterControl.Inv2);
		InverterControl_Off_be2(&InverterControl.Inv2);
		InverterControl_On_be1(&InverterControl.Inv3);
		InverterControl_Off_be2(&InverterControl.Inv3);
		InverterControl_On_be1(&InverterControl.Inv4);
		InverterControl_Off_be2(&InverterControl.Inv4);
		test = 1;
	}
	else
	{
		InverterControl_Off_be1(&InverterControl.Inv1);
		InverterControl_On_be2(&InverterControl.Inv1);
		InverterControl_Off_be1(&InverterControl.Inv2);
		InverterControl_On_be2(&InverterControl.Inv2);
		InverterControl_Off_be1(&InverterControl.Inv3);
		InverterControl_On_be2(&InverterControl.Inv3);
		InverterControl_Off_be1(&InverterControl.Inv4);
		InverterControl_On_be2(&InverterControl.Inv4);
		test = 0;
	}


//	if(InverterControl_Read_ba3(&InverterControl.Inv1))
//	{
//		InverterControl_On_be2(&InverterControl.Inv1);
//	}
//	else
//	{
//		InverterControl_Off_be2(&InverterControl.Inv1);
//	}
}
#endif

void InverterControl_Startup(void)
{
	InverterControl_Startup_inv(&InverterControl.Inv1);
	InverterControl_Startup_inv(&InverterControl.Inv2);
	InverterControl_Startup_inv(&InverterControl.Inv3);
	InverterControl_Startup_inv(&InverterControl.Inv4);
}


/***Private Function Implementations******************************************/

/*Startup routine for each converter.*/
static void InverterControl_Startup_inv(InverterControl_Inverter_t* inv)
{

}


/***Private Inline Function Implementations***********************************/

static inline __attribute__((always_inline)) void InverterControl_Update_io(InverterControl_Inverter_t* inv)
{
	if(inv->be1.control == InverterControl_io_reset)
	{
		InverterControl_Off_be1(inv);
	}
	else if(inv->be1.control == InverterControl_io_set)
	{
		InverterControl_On_be1(inv);
	}
	inv->be1.control = InverterControl_io_none;

	if(inv->be2.control == InverterControl_io_reset)
	{
		InverterControl_Off_be2(inv);
	}
	else if(inv->be2.control == InverterControl_io_set)
	{
		InverterControl_On_be2(inv);
	}
	inv->be2.control = InverterControl_io_none;

	InverterControl_Read_ba3(inv);
	inv->ba3.control = InverterControl_io_none;
}

static inline __attribute__((always_inline)) void InverterControl_On_be1(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be1.GPIOx,inv->be1.GPIO_Pin,GPIO_PIN_SET);
	inv->be1.status = (bool)HAL_GPIO_ReadPin(inv->be1.GPIOx,inv->be1.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be1(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be1.GPIOx,inv->be1.GPIO_Pin,GPIO_PIN_RESET);
	inv->be1.status = (bool)HAL_GPIO_ReadPin(inv->be1.GPIOx,inv->be1.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_On_be2(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be2.GPIOx,inv->be2.GPIO_Pin,GPIO_PIN_SET);
	inv->be2.status = (bool)HAL_GPIO_ReadPin(inv->be2.GPIOx,inv->be2.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be2(InverterControl_Inverter_t* inv)
{
	HAL_GPIO_WritePin(inv->be2.GPIOx,inv->be2.GPIO_Pin,GPIO_PIN_RESET);
	inv->be2.status = (bool)HAL_GPIO_ReadPin(inv->be2.GPIOx,inv->be2.GPIO_Pin);
}

static inline __attribute__((always_inline)) bool InverterControl_Read_ba3(InverterControl_Inverter_t* inv)
{
	inv->ba3.status = (bool)HAL_GPIO_ReadPin(inv->ba3.GPIOx,inv->ba3.GPIO_Pin);
	return inv->ba3.status;
}

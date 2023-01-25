/*
 * InverterControl.c
 *
 *  Created on: Jul 30, 2021
 *      Author: InitusNovus
 */

/***Includes******************************************************************/
#include "inverterControl.h"
#include "CanMessage.h"
#include "stm32f4xx_hal.h"
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

#define INVCON_HWRECONFIGURE

/***Enumerations**************************************************************/

typedef enum
{
    InverterControl_io_none = 0,
    InverterControl_io_reset = 1,
    InverterControl_io_set = 2,
} InverterControl_Inverter_io_control_t;

/***Data Structures***********************************************************/

typedef struct
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    InverterControl_Inverter_io_control_t control;
    bool status;
} InverterControl_Inverter_io;

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
} InverterControl_t;

/***Global Variable Declarations***/

InverterControl_t InverterControl;


volatile uint32_t testCountCanRxFifo = 0;
volatile uint32_t testCountCanRx2 = 0;

/***Private Function Prototypes***********************************************/

static void InverterControl_Startup_inv(InverterControl_Inverter_t *inv);
static void InverterControl_Update_con(InverterControl_Inverter_t *inv, InverterControl_Inverter_io_control_t be1,
                                       InverterControl_Inverter_io_control_t be2);
static void InverterControl_Update_io(InverterControl_Inverter_t *inv);
static void InverterControl_On_be1(InverterControl_Inverter_t *inv);
static void InverterControl_Off_be1(InverterControl_Inverter_t *inv);
static void InverterControl_On_be2(InverterControl_Inverter_t *inv);
static void InverterControl_Off_be2(InverterControl_Inverter_t *inv);
static bool InverterControl_Read_ba3(InverterControl_Inverter_t *inv);

/***Public Function Implementations*******************************************/

void InverterControl_init(void)
{
    InverterControl.Inv1.be1.GPIOx = INVCON_INV1_BE1_GPIOX;
    InverterControl.Inv1.be1.GPIO_Pin = INVCON_INV1_BE1_GPIOP;
    InverterControl.Inv1.be1.control = InverterControl_io_none;
    InverterControl.Inv1.be2.GPIOx = INVCON_INV1_BE2_GPIOX;
    InverterControl.Inv1.be2.GPIO_Pin = INVCON_INV1_BE2_GPIOP;
    InverterControl.Inv1.be2.control = InverterControl_io_none;
    InverterControl.Inv1.ba3.GPIOx = INVCON_INV1_BA3_GPIOX;
    InverterControl.Inv1.ba3.GPIO_Pin = INVCON_INV1_BA3_GPIOP;
    InverterControl.Inv1.ba3.control = InverterControl_io_none;

    InverterControl.Inv2.be1.GPIOx = INVCON_INV2_BE1_GPIOX;
    InverterControl.Inv2.be1.GPIO_Pin = INVCON_INV2_BE1_GPIOP;
    InverterControl.Inv2.be1.control = InverterControl_io_none;
    InverterControl.Inv2.be2.GPIOx = INVCON_INV2_BE2_GPIOX;
    InverterControl.Inv2.be2.GPIO_Pin = INVCON_INV2_BE2_GPIOP;
    InverterControl.Inv2.be2.control = InverterControl_io_none;
    InverterControl.Inv2.ba3.GPIOx = INVCON_INV2_BA3_GPIOX;
    InverterControl.Inv2.ba3.GPIO_Pin = INVCON_INV2_BA3_GPIOP;
    InverterControl.Inv2.ba3.control = InverterControl_io_none;

    InverterControl.Inv3.be1.GPIOx = INVCON_INV3_BE1_GPIOX;
    InverterControl.Inv3.be1.GPIO_Pin = INVCON_INV3_BE1_GPIOP;
    InverterControl.Inv3.be1.control = InverterControl_io_none;
    InverterControl.Inv3.be2.GPIOx = INVCON_INV3_BE2_GPIOX;
    InverterControl.Inv3.be2.GPIO_Pin = INVCON_INV3_BE2_GPIOP;
    InverterControl.Inv3.be2.control = InverterControl_io_none;
    InverterControl.Inv3.ba3.GPIOx = INVCON_INV3_BA3_GPIOX;
    InverterControl.Inv3.ba3.GPIO_Pin = INVCON_INV3_BA3_GPIOP;
    InverterControl.Inv3.ba3.control = InverterControl_io_none;

    InverterControl.Inv4.be1.GPIOx = INVCON_INV4_BE1_GPIOX;
    InverterControl.Inv4.be1.GPIO_Pin = INVCON_INV4_BE1_GPIOP;
    InverterControl.Inv4.be1.control = InverterControl_io_none;
    InverterControl.Inv4.be2.GPIOx = INVCON_INV4_BE2_GPIOX;
    InverterControl.Inv4.be2.GPIO_Pin = INVCON_INV4_BE2_GPIOP;
    InverterControl.Inv4.be2.control = InverterControl_io_none;
    InverterControl.Inv4.ba3.GPIOx = INVCON_INV4_BA3_GPIOX;
    InverterControl.Inv4.ba3.GPIO_Pin = INVCON_INV4_BA3_GPIOP;
    InverterControl.Inv4.ba3.control = InverterControl_io_none;
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
    /*CAN RX routine*/
    CAN_RxHeaderTypeDef canRxHeader;
    uint8_t rxDataBuf[8];

    /*InverterControl message CAN1_RX*/
    while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 0)
    {
    	testCountCanRxFifo ++;	//Test
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, rxDataBuf);
        if (canRxHeader.FilterMatchIndex == InverterControl_msg_rx_1.fim)
        {
            uint8_t dlc = canRxHeader.DLC;
            if (dlc > 0)
            {
                if (dlc > 8)
                    dlc = 8;
                while (dlc)
                {
                	InverterControl_msg_rx_1.msg.U[dlc-1] = rxDataBuf[dlc-1];
                	dlc--;
                }
            }
            if (InverterControl_msg_rx_1.updated == CanMsg_updated ||
                InverterControl_msg_rx_1.updated == CanMsg_overridden)
            {
                InverterControl_msg_rx_1.updated = CanMsg_overridden;
            }
            else
            {
                InverterControl_msg_rx_1.updated = CanMsg_updated;
            }
        }
        else if (canRxHeader.FilterMatchIndex == InverterControl_msg_rx_2.fim)
        {
        	testCountCanRx2++;
            uint8_t dlc = canRxHeader.DLC;
            if (dlc > 0)
            {
                if (dlc > 8)
                    dlc = 8;
                while (dlc)
                {
                	InverterControl_msg_rx_2.msg.U[dlc-1] = rxDataBuf[dlc-1];
                	dlc--;
                }
            }
            if (InverterControl_msg_rx_2.updated == CanMsg_updated ||
                InverterControl_msg_rx_2.updated == CanMsg_overridden)
            {
                InverterControl_msg_rx_2.updated = CanMsg_overridden;
            }
            else
            {
                InverterControl_msg_rx_2.updated = CanMsg_updated;
            }
        }
    }

#ifdef INVCON_HWRECONFIGURE
    /* Inv1 BE1 -> BE1
     * Inv2 BE1 -> BE2
     * Inv3 BE1 -> EF
     * The others are not used
     */
    if (InverterControl_msg_rx_2.updated != CanMsg_none)
    {
        CanMsg_InverterControl_rx_2_msg msg = InverterControl_msg_rx_2.msg;
        if (msg.B.BE1on != false)
        {
            InverterControl_Update_con(&InverterControl.Inv1, InverterControl_io_set, InverterControl_io_none);
        }
        else
        {
            InverterControl_Update_con(&InverterControl.Inv1, InverterControl_io_reset, InverterControl_io_none);
        }
        if (msg.B.BE2on != false)
        {
            InverterControl_Update_con(&InverterControl.Inv2, InverterControl_io_set, InverterControl_io_none);
        }
        else
        {
            InverterControl_Update_con(&InverterControl.Inv2, InverterControl_io_reset, InverterControl_io_none);
        }
        if (msg.B.EFon != false)
        {
            InverterControl_Update_con(&InverterControl.Inv3, InverterControl_io_set, InverterControl_io_none);
        }
        else
        {
            InverterControl_Update_con(&InverterControl.Inv3, InverterControl_io_reset, InverterControl_io_none);
        }

        if (InverterControl_msg_rx_2.updated == CanMsg_overridden)
        {
            // TODO: overridden message routine
        }

        /*Reset the flag*/
        InverterControl_msg_rx_2.updated = CanMsg_none;
        // TODO: tx msg: status ack
    }
#else
    if (InverterControl_msg_rx_1.updated != CanMsg_none)
    {
        CanMsg_InverterControl_rx_1_msg msg = InverterControl_msg_rx_1.msg;
        if (msg.B.control == true)
        {
            InverterControl_Inverter_t *inv;
            InverterControl_Inverter_io_control_t be1;
            InverterControl_Inverter_io_control_t be2;
            if (msg.B.rstBe1 == true)
                be1 = InverterControl_io_reset;
            else if (msg.B.setBe1 == true)
                be1 = InverterControl_io_set;
            if (msg.B.rstBe2 == true)
                be2 = InverterControl_io_reset;
            else if (msg.B.setBe2 == true)
                be2 = InverterControl_io_set;

            switch ((uint8_t)msg.B.invSel)
            {
            case inv1:
                inv = &InverterControl.Inv1;
                break;
            case inv2:
                inv = &InverterControl.Inv2;
                break;
            case inv3:
                inv = &InverterControl.Inv3;
                break;
            case inv4:
                inv = &InverterControl.Inv4;
                break;
            default:
                break;
            }
            InverterControl_Update_con(inv, be1, be2);

            msg.B.rstBe1 = false;
            msg.B.setBe1 = false;
            msg.B.rstBe2 = false;
            msg.B.setBe2 = false;

            /*Reset the flag*/
            msg.B.control = false;
        }

        if (InverterControl_msg_rx_1.updated == CanMsg_overridden)
        {
            // TODO: overridden message routine
        }

        /*Reset the flag*/
        InverterControl_msg_rx_1.updated = CanMsg_none;
        // TODO: tx msg: status ack
    }
#endif
}

#ifdef INVCON_TEST
void InverterControl_test_1000ms(void)
{
    static int test = 0;
    if (test == 0)
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
static void InverterControl_Startup_inv(InverterControl_Inverter_t *inv)
{
}

/***Private Inline Function Implementations***********************************/
static inline __attribute__((always_inline)) void InverterControl_Update_con(InverterControl_Inverter_t *inv,
                                                                             InverterControl_Inverter_io_control_t be1,
                                                                             InverterControl_Inverter_io_control_t be2)
{
    inv->be1.control = be1;
    inv->be2.control = be2;
}

static inline __attribute__((always_inline)) void InverterControl_Update_io(InverterControl_Inverter_t *inv)
{
    if (inv->be1.control == InverterControl_io_reset)
    {
        InverterControl_Off_be1(inv);
    }
    else if (inv->be1.control == InverterControl_io_set)
    {
        InverterControl_On_be1(inv);
    }
    inv->be1.control = InverterControl_io_none;

    if (inv->be2.control == InverterControl_io_reset)
    {
        InverterControl_Off_be2(inv);
    }
    else if (inv->be2.control == InverterControl_io_set)
    {
        InverterControl_On_be2(inv);
    }
    inv->be2.control = InverterControl_io_none;

    InverterControl_Read_ba3(inv);
    inv->ba3.control = InverterControl_io_none;
}

static inline __attribute__((always_inline)) void InverterControl_On_be1(InverterControl_Inverter_t *inv)
{
    HAL_GPIO_WritePin(inv->be1.GPIOx, inv->be1.GPIO_Pin, GPIO_PIN_SET);
    inv->be1.status = (bool)HAL_GPIO_ReadPin(inv->be1.GPIOx, inv->be1.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be1(InverterControl_Inverter_t *inv)
{
    HAL_GPIO_WritePin(inv->be1.GPIOx, inv->be1.GPIO_Pin, GPIO_PIN_RESET);
    inv->be1.status = (bool)HAL_GPIO_ReadPin(inv->be1.GPIOx, inv->be1.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_On_be2(InverterControl_Inverter_t *inv)
{
    HAL_GPIO_WritePin(inv->be2.GPIOx, inv->be2.GPIO_Pin, GPIO_PIN_SET);
    inv->be2.status = (bool)HAL_GPIO_ReadPin(inv->be2.GPIOx, inv->be2.GPIO_Pin);
}

static inline __attribute__((always_inline)) void InverterControl_Off_be2(InverterControl_Inverter_t *inv)
{
    HAL_GPIO_WritePin(inv->be2.GPIOx, inv->be2.GPIO_Pin, GPIO_PIN_RESET);
    inv->be2.status = (bool)HAL_GPIO_ReadPin(inv->be2.GPIOx, inv->be2.GPIO_Pin);
}

static inline __attribute__((always_inline)) bool InverterControl_Read_ba3(InverterControl_Inverter_t *inv)
{
    inv->ba3.status = (bool)HAL_GPIO_ReadPin(inv->ba3.GPIOx, inv->ba3.GPIO_Pin);
    return inv->ba3.status;
}

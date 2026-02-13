/********************************************************************************************************
 * @file    board_8258_dongle.h
 *
 * @brief   This is the header file for board_8258_dongle
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************/

#define ZCL_BASIC_MFG_NAME      {11,'T','e','l','i','n','k','-','p','v','v','x'}
#define ZCL_BASIC_MODEL_ID      {15,'Z','G','8','0','7','Z','_','z'}

/***************************************************************/
/* CLK */
#define CLOCK_SYS_CLOCK_HZ          48000000 // 24000000, 32000000, 48000000

/* PA */
#define PA_ENABLE               1
#define PA_TX                   GPIO_PC4
#define PA_RX                   GPIO_PC3

//KEY
#define	BUTTON1                 GPIO_PD4
#define PD4_FUNC                AS_GPIO
#define PD4_OUTPUT_ENABLE       0
#define PD4_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PD4     PM_PIN_PULLUP_10K

//All LED as GPIO.
#define LED_LIGHT_ON		    0
#define LED_LIGHT_OFF		    (!LED_LIGHT_ON)
#define LED_LIGHT		        GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_DATA_OUT			LED_LIGHT_OFF
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

// ADC
#if 1//VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  GPIO_PB0
#endif

/*** Configure  GPIO Vbat ***/
#define SHL_ADC_VBAT        1  // "B0P" in adc.h
#define GPIO_VBAT           GPIO_PB0 // missing pin on case TLSR8251F512ET24
#define PB0_INPUT_ENABLE    1
#define PB0_DATA_OUT        1
#define PB0_OUTPUT_ENABLE   1
#define PB0_FUNC            AS_GPIO
#define PULL_WAKEUP_SRC_PB0 PM_PIN_PULLUP_10K

// UART
#if ZBHCI_UART
#define UART_TX_PIN             UART_TX_PB1
#define UART_RX_PIN             UART_RX_PB7

#define UART_PIN_CFG()          uart_gpio_set(UART_TX_PIN, UART_RX_PIN) //uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
#define	DEBUG_INFO_TX_PIN       GPIO_PC6//print
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02
};

#define	KB_MAP_NORMAL           {{VK_SW1,}}

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {0}
#define KB_SCAN_PINS            {BUTTON1}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
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

/**********************************************************************
 * App configuration
 */
/* Debug mode */
#define	UART_PRINTF_MODE                        0
#define USB_PRINTF_MODE                         0

/* HCI interface */
#define	ZBHCI_UART                              0

/* BDB */
#define TOUCHLINK_SUPPORT                       1
#define FIND_AND_BIND_SUPPORT                   0

/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * 1) if MCU_CORE_826x is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * and there is no need to configure the detection IO port;
 * 2) if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 * 3) if MCU_CORE_B91 is defined, the DRV_ADC_BASE_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be connected to the target under test,
 * such as VCC.
 */
#define VOLTAGE_DETECT_ENABLE                   0

/* Flash protect module */
/* Only the firmware area will be locked, the NV data area will not be locked.
 * For details, please refer to drv_flash.c file.
 */
#define FLASH_PROTECT_ENABLE                    0

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE                  0

/* UART module */
#if ZBHCI_UART
#define	MODULE_UART_ENABLE                      1
#endif

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
#define ZBHCI_EN                                1
#endif

/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

#define ZB_ROUTER_ROLE      1

/* Chip IDs */
#define TLSR_8267                       0x00
#define TLSR_8269                       0x01
#define TLSR_8258_512K                  0x02
#define TLSR_8258_1M                    0x03
#define TLSR_8278                       0x04
#define TLSR_B91                        0x05
#define TLSR_B92                        0x06
#define TLSR_TL721X                     0x07
#define TLSR_TL321X                     0x08


/* FLASH 1M map:
              1M
0x100000  ------------
         |  MAC_Addr  |
 0xFF000 |------------|
         | F_CFG_Info | // FACTORY_CFG_BASE_ADD
 0xFE000 |------------|
         | U_Cfg_Info | // 0xFC000 CFG_FACTORY_RST_CNT
 0xFC000 |------------|
         | USER_DATA  |
 0x96000 |------------|
         |     NV     |
 0x80000 |------------|
         |            |
         |            |
         |  OTA_Image | 256K
         |            |
         |            |
 0x40000 |------------|
         |            | // 0x32000 FLASH_ADDR_TAB_GPIOS
         |            |
         |  Firmware  | 256K
         |            |
         |            |
 0x00000  ------------
*/
#define BEGIN_USER_DATA_F1M            0x96000   // begin address for saving energy
#define END_USER_DATA_F1M              0xFC000   // end address for saving energy
/*  Flash 512k map:
     0x80000  ------------
             |            |
             |    NV_2    |
             |            |
     0x7A000 |------------|
             | U_Cfg_Info | // 0x79000 CFG_FACTORY_RST_CNT
     0x78000 |------------|
             | F_CFG_Info | // 0x77000 FACTORY_CFG_BASE_ADD
     0x77000 |------------|
             |  MAC_Addr  |
     0x76000 |------------|
             | USER_DATA  |
     0x72000 |------------|
             |            |
             |  OTA_Image | 200k
             |            |
     0x40000 |------------|
             |            |
             |    NV_1    |
             |            |
     0x34000 |------------|
             |   free     | // 0x32000 FLASH_ADDR_TAB_GPIOS
     0x32000 |------------|
             |            |
             |  Firmware  | 200k
             |            |
     0x00000  ------------
 */
#define BEGIN_USER_DATA_F512K             0x72000 // begin address for saving energy
#define END_USER_DATA_F512K               0x76000 // end address for saving energy
#define FLASH_ADDR_TAB_GPIOS		  	  0x32000
/** Store zigbee information in flash:
 ********************************************************************************************************
 *   Module ID                  |          512K Flash               |              1M Flash             |
 * -----------------------------|-----------------------------------|-----------------------------------|
 * NV_MODULE_ZB_INFO            |        0x34000 - 0x36000          |	      0x80000 - 0x82000         |
 * NV_MODULE_ADDRESS_TABLE      |        0x36000 - 0x38000          |         0x82000 - 0x84000         |
 * NV_MODULE_APS                |        0x38000 - 0x3a000          |         0x84000 - 0x86000         |
 * NV_MODULE_ZCL                |        0x3a000 - 0x3c000          |         0x86000 - 0x88000         |
 * NV_MODULE_NWK_FRAME_COUNT	|        0x3c000 - 0x3e000          |         0x88000 - 0x8a000         |
 * NV_MODULE_OTA                |        0x3e000 - 0x40000          |         0x8a000 - 0x8c000         |
 * NV_MODULE_APP                |        0x7a000 - 0x7c000          |         0x8c000 - 0x8e000         |
 * NV_MODULE_KEYPAIR            |        0x7c000 - 0x80000          |         0x8e000 - 0x96000         |
 *                              |    *16K - can store 127 nodes     |     *32K - can store 302 nodes    |
 * NV_MAX_MODULS
 */

/**********************************************************************
 * Board configuration
 */
#if (BOARD == BOARD_ZG807Z)
    #include "board_zg807z.h"
#endif

#if defined(MCU_CORE_8258)
    #define CHIP_TYPE               TLSR_8258_512K //TLSR_8258_1M
    #if (CHIP_TYPE == TLSR_8258_1M)
        #define FLASH_CAP_SIZE_1M       1
    #endif
#endif

/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"

/**********************************************************************
 * ZCL cluster configuration
 */
#define ZCL_POWER_CFG_SUPPORT					0
#define ZCL_ON_OFF_SUPPORT                      1
#define ZCL_GROUP_SUPPORT                       1
#define ZCL_SCENE_SUPPORT                       0
#define ZCL_OTA_SUPPORT                         1
#define ZCL_GP_SUPPORT                          1
#if TOUCHLINK_SUPPORT
#define ZCL_ZLL_COMMISSIONING_SUPPORT           1
#endif
//test
#define AF_TEST_ENABLE                          0

/**********************************************************************
 * EV configuration
 */
typedef enum {
    EV_POLL_ED_DETECT,
    EV_POLL_HCI,
    EV_POLL_IDLE,
    EV_POLL_MAX,
} ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

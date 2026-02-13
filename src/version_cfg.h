/********************************************************************************************************
 * @file    version_cfg.h
 *******************************************************************************************************/
#pragma once


#define APP_RELEASE				0x00	//0x12 -> BCD app release "1.2"
#define APP_BUILD				0x01	//0x34 -> BCD app build "3.4"

#define STACK_RELEASE			0x30	//BCD stack release 3.0
#define STACK_BUILD				0x01	//BCD stack build 01

/**********************************************************************
 * Board definitions
 */
#ifndef MCU_CORE_8258
#define MCU_CORE_8258
#endif

/* Board ID */
#define BOARD_ZG807Z	1

/* Board define */
#define BOARD	BOARD_ZG807Z

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
//#define MANUFACTURER_CODE_TELINK        0x1141 //Telink ID
#define MANUFACTURER_CODE_TELINK	0xEA60	// 60000 Telink-pvvx
#define	IMAGE_TYPE					(0x0200 | BOARD)
#define	FILE_VERSION				((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)

/* Pre-compiled link configuration. */
/* APP image address. */
#define BOOT_LOADER_MODE                0
/* Boot loader address. */
#define BOOT_LOADER_IMAGE_ADDR          0x0
#if (BOOT_LOADER_MODE)
	#error "Boot loader - Not released!"
    #define APP_IMAGE_ADDR              0x8000
#else
    #define APP_IMAGE_ADDR              0x0
#endif
#define IS_BOOT_LOADER_IMAGE				0
#define RESV_FOR_APP_RAM_CODE_SIZE			0
#define IMAGE_OFFSET						APP_IMAGE_ADDR

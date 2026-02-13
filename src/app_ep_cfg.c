/********************************************************************************************************
 * @file    app_ep_cfg.c
 *
 * @brief   This is the source file for appEpCfg
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

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "app.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#if !defined(ZCL_BASIC_MFG_NAME) || !defined(ZCL_BASIC_MODEL_ID)
#error "defined ZCL_BASIC_MODEL_ID & ZCL_BASIC_MFG_NAME !"
#endif

#ifndef ZCL_BASIC_SW_BUILD_ID

#define ZCL_BASIC_SW_BUILD_ID   {9 \
                                ,'0' \
                                ,'0' \
                                ,'0'+(BOARD / 10) \
                                ,'0'+(BOARD % 10)\
                                ,'-' \
                                ,'0'+(APP_RELEASE>>4) \
                                ,'0'+(APP_RELEASE & 0xf) \
                                ,'0'+(APP_BUILD>>4) \
                                ,'0'+(APP_BUILD & 0xf) \
                                }

#endif

#ifndef ZCL_BASIC_DATE_CODE
#define ZCL_BASIC_DATE_CODE     {8,'2','0','2','6','0','1','0','1'}
#endif

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME      {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID      {8,'T','L','S','R','x','x','x','x'}
#endif

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 app_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
#ifdef ZCL_POWER_CFG
	ZCL_CLUSTER_GEN_POWER_CFG,
#endif
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 app_outClusterList[] = {
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_IN_CLUSTER_NUM      (sizeof(app_inClusterList) / sizeof(app_inClusterList[0]))
#define APP_OUT_CLUSTER_NUM     (sizeof(app_outClusterList) / sizeof(app_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_simpleDesc = {
    HA_PROFILE_ID,                      /* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
    APP_ENDPOINT,              			/* Endpoint */
    1,                                  /* Application device version */
    0,                                  /* Reserved */
    APP_IN_CLUSTER_NUM,         /* Application input cluster count */
    APP_OUT_CLUSTER_NUM,        /* Application output cluster count */
    (u16 *)app_inClusterList,   /* Application input cluster list */
    (u16 *)app_outClusterList,  /* Application output cluster list */
};

/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs = {
    .zclVersion   = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = BOARD,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
#ifdef ZCL_POWER_CFG
    .powerSource    = POWER_SOURCE_BATTERY,
#else
	.powerSource 	= POWER_SOURCE_MAINS_1_PHASE,
#endif
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] = {
    { ZCL_ATTRID_BASIC_ZCL_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.zclVersion},
    { ZCL_ATTRID_BASIC_APP_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.appVersion},
    { ZCL_ATTRID_BASIC_STACK_VER,         ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,            ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.hwVersion},
    { ZCL_ATTRID_BASIC_MFR_NAME,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.manuName},
    { ZCL_ATTRID_BASIC_MODEL_ID,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.modelId},
    { ZCL_ATTRID_BASIC_POWER_SOURCE,      ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.powerSource},
    { ZCL_ATTRID_BASIC_DEV_ENABLED,       ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,       ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.swBuildId},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_BASIC_ATTR_NUM      sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_POWER_CFG

/* power */
zcl_powerAttr_t g_zcl_powerAttrs =
{
    .batteryVoltage    = 33, //in 100 mV units, 0xff - unknown
    .batteryPercentage = 200 //in 0,5% units, 0xff - unknown
};

const zclAttrInfo_t powerCfg_attrTbl[] =
{
	{ ZCL_ATTRID_BATTERY_VOLTAGE,      		   ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,	(u8*)&g_zcl_powerAttrs.batteryVoltage},
	{ ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_powerAttrs.batteryPercentage},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_POWER_CFG_ATTR_NUM		 sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs = {
    .identifyTime = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] = {
    { ZCL_ATTRID_IDENTIFY_TIME,           ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM   sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs = {
    .nameSupport = 0,
};

const zclAttrInfo_t group_attrTbl[] = {
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,      ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8*)&g_zcl_groupAttrs.nameSupport },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_GROUP_ATTR_NUM      sizeof(group_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs = {
    .sceneCount   = 0,
    .currentScene = 0,
    .currentGroup = 0x0000,
    .sceneValid   = FALSE,
    .nameSupport  = 0,
};

const zclAttrInfo_t scene_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,       ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_sceneAttrs.sceneCount },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,     ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_sceneAttrs.currentScene },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,     ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&g_zcl_sceneAttrs.currentGroup },
    { ZCL_ATTRID_SCENE_SCENE_VALID,       ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_sceneAttrs.sceneValid },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,      ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8*)&g_zcl_sceneAttrs.nameSupport },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_ATTR_NUM      sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_ON_OFF
/* On/Off */
zcl_onOffAttr_t g_zcl_onOffAttrs = {
    .onOff              = 0x00,
    .globalSceneControl	= 1,
    .onTime             = 0x0000,
    .offWaitTime        = 0x0000,
    .startUpOnOff       = ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON,
};

const zclAttrInfo_t onOff_attrTbl[] = {
    { ZCL_ATTRID_ONOFF,                   ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs.onOff},
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,    ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs.globalSceneControl},
    { ZCL_ATTRID_ON_TIME,                 ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs.onTime},
    { ZCL_ATTRID_OFF_WAIT_TIME,           ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs.offWaitTime},
    { ZCL_ATTRID_START_UP_ONOFF,          ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs.startUpOnOff},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_ONOFF_ATTR_NUM      sizeof(onOff_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/**
 *  @brief Definition for simple light ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,              MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,    basic_attrTbl,          zcl_basic_register, app_basicCb},
#ifdef ZCL_POWER_CFG
	{ZCL_CLUSTER_GEN_POWER_CFG,			 MANUFACTURER_CODE_NONE, ZCL_POWER_CFG_ATTR_NUM, powerCfg_attrTbl,		zcl_powerCfg_register,	app_powerCfgCb},
#endif
    {ZCL_CLUSTER_GEN_IDENTIFY,           MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,       zcl_identify_register, app_identifyCb},
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,             MANUFACTURER_CODE_NONE, ZCL_GROUP_ATTR_NUM,    group_attrTbl,          zcl_group_register, NULL},
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,             MANUFACTURER_CODE_NONE, ZCL_SCENE_ATTR_NUM,    scene_attrTbl,          zcl_scene_register, app_sceneCb},
#endif
#ifdef ZCL_ON_OFF
    {ZCL_CLUSTER_GEN_ON_OFF,             MANUFACTURER_CODE_NONE, ZCL_ONOFF_ATTR_NUM,    onOff_attrTbl,          zcl_onOff_register, app_onOffCb},
#endif
};

u8 APP_CB_CLUSTER_NUM = (sizeof(g_appClusterList) / sizeof(g_appClusterList[0]));


/**********************************************************************
 * FUNCTIONS
 */


/*********************************************************************
 * @fn      zcl_onOffAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_save(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
    bool needSave = FALSE;
    zcl_nv_onOff_t zcl_nv_onOff;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    if (st == NV_SUCC) {
        if ((zcl_nv_onOff.onOff != g_zcl_onOffAttrs.onOff) ||
            (zcl_nv_onOff.startUpOnOff != g_zcl_onOffAttrs.startUpOnOff)) {
            needSave = TRUE;
        }
    } else if (st == NV_ITEM_NOT_FOUND) {
        needSave = TRUE;
    }

    if (needSave) {
        zcl_nv_onOff.onOff = g_zcl_onOffAttrs.onOff;
        zcl_nv_onOff.startUpOnOff = g_zcl_onOffAttrs.startUpOnOff;

        st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_onOffAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_restore(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
    zcl_nv_onOff_t zcl_nv_onOff;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    if (st == NV_SUCC) {
        g_zcl_onOffAttrs.onOff = zcl_nv_onOff.onOff;
        g_zcl_onOffAttrs.startUpOnOff = zcl_nv_onOff.startUpOnOff;
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      populate_date_code
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
__attribute__((optimize("-Os")))
void populate_date_code(void) {
	u8 month;
	if (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n') month = 1;
	else if (__DATE__[0] == 'F') month = 2;
	else if (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') month = 3;
	else if (__DATE__[0] == 'A' && __DATE__[1] == 'p') month = 4;
	else if (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') month = 5;
	else if (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') month = 6;
	else if (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') month = 7;
	else if (__DATE__[0] == 'A' && __DATE__[1] == 'u') month = 8;
	else if (__DATE__[0] == 'S') month = 9;
	else if (__DATE__[0] == 'O') month = 10;
	else if (__DATE__[0] == 'N') month = 11;
	else if (__DATE__[0] == 'D') month = 12;

	g_zcl_basicAttrs.dateCode[1] = __DATE__[7];
	g_zcl_basicAttrs.dateCode[2] = __DATE__[8];
	g_zcl_basicAttrs.dateCode[3] = __DATE__[9];
	g_zcl_basicAttrs.dateCode[4] = __DATE__[10];
	g_zcl_basicAttrs.dateCode[5] = '0' + month / 10;
	g_zcl_basicAttrs.dateCode[6] = '0' + month % 10;
	g_zcl_basicAttrs.dateCode[7] = __DATE__[4] >= '0' ? (__DATE__[4]) : '0';
	g_zcl_basicAttrs.dateCode[8] = __DATE__[5];
}

/*********************************************************************
 * @fn      zcl_appAttrsInit
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
void zcl_appAttrsInit(void)
{
	populate_date_code();
    zcl_onOffAttr_restore();
}



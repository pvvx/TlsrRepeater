/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for app
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
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "gp.h"
#include "app.h"
#include "app_button.h"
#include "app_light.h"
//#include "factory_reset.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**
 *  @brief Definition for BDB finding and binding cluster
 */
u16 bdb_findBindClusterList[] =
{
#ifdef ZCL_POWER_CFG
	ZCL_CLUSTER_GEN_POWER_CFG,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

/**
 *  @brief Definition for BDB finding and binding cluster number
 */
#define FIND_AND_BIND_CLUSTER_NUM		(sizeof(bdb_findBindClusterList)/sizeof(bdb_findBindClusterList[0]))

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
app_ctx_t gLightCtx;

#ifdef ZCL_OTA
extern ota_callBack_t app_otaCb;

//running code firmware information
ota_preamble_t app_otaInfo = {
    .fileVer          = FILE_VERSION,
    .imageType        = IMAGE_TYPE,
    .manufacturerCode = MANUFACTURER_CODE_TELINK,
};
#endif

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,                 //start device cnf cb
    NULL,                               //reset cnf cb
    NULL,                               //device announce indication cb
    app_leaveIndHandler,        //leave ind cb
    app_leaveCnfHandler,        //leave cnf cb
    app_nwkUpdateIndicateHandler,//nwk update ind cb
    NULL,                               //permit join ind cb
    NULL,                               //nlme sync cnf cb
    NULL,                               //tc join ind cb
    NULL,                               //tc detects that the frame counter is near limit
    app_nwkStatusIndHandler,    //nwk status ind cb
};

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key     = (u8 *)tcLinkKeyCentralDefault,         //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key     = (u8 *)linkKeyDistributedMaster,//use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key     = (u8 *)touchLinkKeyMaster,           //use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
    .touchlinkEnable = 1,                       /* enable touch-link */
#else
    .touchlinkEnable = 0,                       /* disable touch-link */
#endif
    .touchlinkChannel = DEFAULT_CHANNEL,        /* touch-link default operation channel for target */
    .touchlinkLqiThreshold = 0xA0,              /* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */
ev_timer_event_t *appAttrsStoreTimerEvt = NULL;


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
    /* Initialize ZB stack */
    zb_init();

    /* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
    af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

    /* Initialize ZCL layer */
    /* Register Incoming ZCL Foundation command/response messages */
    zcl_init(app_zclProcessIncomingMsg);

    /* Register endPoint */
    af_endpointRegister(APP_ENDPOINT, (af_simple_descriptor_t *)&app_simpleDesc, zcl_rx_handler, NULL);

    /* Initialize or restore attributes, this must before 'zcl_register()' */
    zcl_appAttrsInit();
    zcl_reportingTabInit();


    /* Register ZCL specific cluster information */
    zcl_register(APP_ENDPOINT, APP_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_appClusterList);

#if ZCL_GP_SUPPORT
    /* Initialize GP */
    gp_init(APP_ENDPOINT);
#endif

#if ZCL_OTA_SUPPORT
    /* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&app_simpleDesc, &app_otaInfo, &app_otaCb);
#endif
}

s32 appAttrsStoreTimerCb(void *arg)
{
    zcl_onOffAttr_save();
    appAttrsStoreTimerEvt = NULL;
    return -1;
}

void appAttrsStoreTimerStart(void)
{
    if (appAttrsStoreTimerEvt) {
        TL_ZB_TIMER_CANCEL(&appAttrsStoreTimerEvt);
    }
    appAttrsStoreTimerEvt = TL_ZB_TIMER_SCHEDULE(appAttrsStoreTimerCb, NULL, 200);
}

void appAttrsChk(void)
{
    if (gLightCtx.lightAttrsChanged) {
        gLightCtx.lightAttrsChanged = FALSE;
        if (zb_isDeviceJoinedNwk()) {
            appAttrsStoreTimerStart();
        }
    }
}

void app_task(void)
{
    app_key_handler();
//    localPermitJoinState();

    if (BDB_STATE_GET() == BDB_STATE_IDLE) {
        //factoryRst_handler();

        report_handler();

#if 1/* NOTE: If set to '1', the latest status of lighting will be stored. */
        appAttrsChk();
#endif
    }
}

static void appSysException(void)
{
#if 1
    SYSTEM_RESET();
#else
    led_on(LED_POWER);
    while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
    (void)isRetention;

    u32 reportableChange = 0;

    /* Register except handler for test */
    sys_exceptHandlerRegister(appSysException);

    /* Initialize LEDs*/
//    led_init();

#if PA_ENABLE
    /* external RF PA used */
    rf_paInit(PA_TX, PA_RX);
#endif

    light_init();

    //factoryRst_init();

    /* Initialize Stack */
    stack_init();

    /* Initialize user application */
    user_app_init();

    /* Adjust light state to default attributes*/
    light_adjust();

    /* User's Task */
#if ZBHCI_EN
    zbhciInit();
    ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
    ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
    if (bdb_preInstallCodeLoad(&gLightCtx.tcLinkKey.keyType, gLightCtx.tcLinkKey.key) == RET_OK) {
        g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = gLightCtx.tcLinkKey.keyType;
        g_bdbCommissionSetting.linkKey.tcLinkKey.key = gLightCtx.tcLinkKey.key;
    }
    /* Set default reporting configuration */
#ifdef ZCL_POWER_CFG
    bdb_defaultReportingCfg(
    	APP_ENDPOINT,
		HA_PROFILE_ID,
		ZCL_CLUSTER_GEN_POWER_CFG,
		ZCL_ATTRID_BATTERY_VOLTAGE,
		360,
		10800,
		(u8 *)&reportableChange
	);
    bdb_defaultReportingCfg(
    	APP_ENDPOINT,
		HA_PROFILE_ID,
		ZCL_CLUSTER_GEN_POWER_CFG,
		ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,
		360,
		10800,
		(u8 *)&reportableChange
	);
#endif
#ifdef ZCL_ON_OFF
    bdb_defaultReportingCfg(
    	APP_ENDPOINT,
		HA_PROFILE_ID,
		ZCL_CLUSTER_GEN_ON_OFF,
		ZCL_ATTRID_ONOFF,
		1,
		120,
		(u8 *)&reportableChange); //min = 1s, max = 120s
#endif
    bdb_findBindMatchClusterSet(FIND_AND_BIND_CLUSTER_NUM, bdb_findBindClusterList);

    /* Initialize BDB */
    bdb_init((af_simple_descriptor_t *)&app_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
}


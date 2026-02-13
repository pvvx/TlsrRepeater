/********************************************************************************************************
 * @file    zcl_appCb.c
 *
 * @brief   This is the source file for zcl_appCb
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
#include "ota.h"
#include "app.h"
#include "app_light.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */



/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#if 0 //def ZCL_READ
static void app_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void app_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
//static void app_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd);
#endif
#if 0 //def ZCL_REPORT
static void app_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd);
static void app_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void app_zclReportCmd(zclReportCmd_t *pReportCmd);
#endif
//static void app_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif


/**********************************************************************
 * FUNCTIONS
 */
void app_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);

/*********************************************************************
 * @fn      app_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
    //printf("app_zclProcessIncomingMsg\n");

    switch (pInHdlrMsg->hdr.cmd) {
#if 0 // def ZCL_READ
    case ZCL_CMD_READ_RSP:
        app_zclReadRspCmd(pInHdlrMsg->attrCmd);
        break;
#endif
#ifdef ZCL_WRITE
    case ZCL_CMD_WRITE:
    case ZCL_CMD_WRITE_NO_RSP:
        app_zclWriteReqCmd(pInHdlrMsg->msg->indInfo.cluster_id, pInHdlrMsg->attrCmd);
        break;
//    case ZCL_CMD_WRITE_RSP:
//        app_zclWriteRspCmd(pInHdlrMsg->attrCmd);
//        break;
#endif
#if 0 //def ZCL_REPORT
    case ZCL_CMD_CONFIG_REPORT:
        app_zclCfgReportCmd(pInHdlrMsg->attrCmd);
        break;
    case ZCL_CMD_CONFIG_REPORT_RSP:
        app_zclCfgReportRspCmd(pInHdlrMsg->attrCmd);
        break;
    case ZCL_CMD_REPORT:
        app_zclReportCmd(pInHdlrMsg->attrCmd);
        break;
#endif
//    case ZCL_CMD_DEFAULT_RSP:
//        app_zclDfltRspCmd(pInHdlrMsg->attrCmd);
//        break;
    default:
        break;
    }
}

#if 0 // def ZCL_READ
/*********************************************************************
 * @fn      app_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd)
{
    //printf("app_zclReadRspCmd\n");
}
#endif

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      app_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param
 *
 * @return  None
 */
static void app_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
    u8 numAttr = pWriteReqCmd->numAttr;
    zclWriteRec_t *attr = pWriteReqCmd->attrList;

    if (clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_START_UP_ONOFF) {
                zcl_onOffAttr_save();
            }
        }
    } else if (clusterId == ZCL_CLUSTER_GEN_IDENTIFY) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_IDENTIFY_TIME) {
                app_zclIdentifyCmdHandler(APP_ENDPOINT, 0xFFFE, g_zcl_identifyAttrs.identifyTime);
            }
        }
    }
}
#endif

#if 0
/*********************************************************************
 * @fn      app_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd)
{
    //printf("app_zclWriteRspCmd\n");
}
#endif

#if 0
/*********************************************************************
 * @fn      app_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd)
{
    //printf("app_zclDfltRspCmd\n");
}
#endif

#if 0 // def ZCL_REPORT
/*********************************************************************
 * @fn      app_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd)
{
    //printf("app_zclCfgReportCmd\n");
}

/*********************************************************************
 * @fn      app_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    //printf("app_zclCfgReportRspCmd\n");
}

/*********************************************************************
 * @fn      app_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReportCmd(zclReportCmd_t *pReportCmd)
{
    //printf("app_zclReportCmd\n");
}
#endif

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      app_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT) {
        //Reset all the attributes of all its clusters to factory defaults
        //zcl_nv_attr_reset();
    }

    //return ZCL_STA_SUCCESS;
    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}
#endif

#ifdef ZCL_IDENTIFY
s32 app_zclIdentifyTimerCb(void *arg)
{
    if (g_zcl_identifyAttrs.identifyTime <= 0) {
        light_blink_stop();

        identifyTimerEvt = NULL;
        return -1;
    }
    g_zcl_identifyAttrs.identifyTime--;
    return 0;
}

void app_zclIdentifyTimerStop(void)
{
    if (identifyTimerEvt) {
        TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
    }
}

/*********************************************************************
 * @fn      app_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void app_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
    g_zcl_identifyAttrs.identifyTime = identifyTime;

    if (identifyTime == 0) {
        app_zclIdentifyTimerStop();
        light_blink_stop();
    } else {
        if (!identifyTimerEvt) {
            light_blink_start(identifyTime, 500, 500);
            identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(app_zclIdentifyTimerCb, NULL, 1000);
        }
    }
}

/*********************************************************************
 * @fn      app_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void app_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
    u8 effectId = pTriggerEffect->effectId;
    //u8 effectVariant = pTriggerEffect->effectVariant;

    switch (effectId) {
    case IDENTIFY_EFFECT_BLINK:
        light_blink_start(1, 500, 500);
        break;
    case IDENTIFY_EFFECT_BREATHE:
        light_blink_start(15, 300, 700);
        break;
    case IDENTIFY_EFFECT_OKAY:
        light_blink_start(2, 250, 250);
        break;
    case IDENTIFY_EFFECT_CHANNEL_CHANGE:
        light_blink_start(1, 500, 7500);
        break;
    case IDENTIFY_EFFECT_FINISH_EFFECT:
        light_blink_start(1, 300, 700);
        break;
    case IDENTIFY_EFFECT_STOP_EFFECT:
        light_blink_stop();
        break;
    default:
        break;
    }
}

/*********************************************************************
 * @fn      app_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (pAddrInfo->dstEp == APP_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_IDENTIFY:
                app_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
                break;
            case ZCL_CMD_TRIGGER_EFFECT:
                app_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
                break;
            default:
                break;
            }
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif

/*********************************************************************
 * @fn      app_powerCfgCb
 *
 * @brief   Handler for ZCL Power Configuration command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
//	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
//	}

	return ZCL_STA_SUCCESS;
}

/********************************************************************************************************
 * @file    app_light.c
 *
 * @brief   This is the source file for appCtrl
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
#include "app_light.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */

/**********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * FUNCTIONS
 */
extern void app_onOffInit(void);

/*********************************************************************
 * @fn      light_adjust
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_adjust(void)
{
    app_onOffInit();
}

/*********************************************************************
 * @fn      light_init
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_init(void)
{
	gpio_write(LED_LIGHT, LED_LIGHT_OFF);
}


/*********************************************************************
 * @fn      hwLight_onOffUpdate
 *
 * @brief
 *
 * @param   onOff - onOff attribute value
 *
 * @return  None
 */
static void hwLight_onOffUpdate(bool onOff)
{
    if (onOff) {
    	gpio_write(LED_LIGHT, LED_LIGHT_ON);
    } else {
    	gpio_write(LED_LIGHT, LED_LIGHT_OFF);
    }
}

/*********************************************************************
 * @fn      light_onOffUpdate
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void light_onOffUpdate(void)
{
    zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();
    hwLight_onOffUpdate(pOnOff->onOff);
}

/*********************************************************************
 * @fn      light_refresh
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_refresh(lightSta_e sta)
{
    switch (sta) {
#if ZCL_ON_OFF_SUPPORT
    case LIGHT_STA_ON_OFF:
        light_onOffUpdate();
        break;
#endif
    default:
        return;
        break;
    }

    gLightCtx.lightAttrsChanged = TRUE;
}


/*********************************************************************
 * @fn      light_blink_TimerEvtCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 light_blink_TimerEvtCb(void *arg)
{
    u32 interval = 0;

    gLightCtx.sta = !gLightCtx.sta;
    if (gLightCtx.sta) {
        hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
        interval = gLightCtx.ledOnTime;
    } else {
        hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
        interval = gLightCtx.ledOffTime;
    }

    if (gLightCtx.sta == gLightCtx.oriSta) {
        if (gLightCtx.times) {
            gLightCtx.times--;
            if (gLightCtx.times <= 0) {
                if (gLightCtx.oriSta) {
                    hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
                } else {
                    hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
                }

                gLightCtx.timerLedEvt = NULL;
                return -1;
            }
        }
    }

    return interval;
}

/*********************************************************************
 * @fn      light_blink_start
 *
 * @brief
 *
 * @param   times       - counts
 * @param   ledOnTime	- on times, ms
 * @param   ledOffTime	- off times, ms
 *
 * @return  None
 */
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    zcl_onOffAttr_t *pOnoff = zcl_onoffAttrGet();

    if (!gLightCtx.timerLedEvt) {
        gLightCtx.times = times;
        gLightCtx.ledOnTime = ledOnTime;
        gLightCtx.ledOffTime = ledOffTime;

        gLightCtx.oriSta = pOnoff->onOff;

        gLightCtx.sta = gLightCtx.oriSta;
        interval = gLightCtx.sta ? ledOnTime : ledOffTime;

        gLightCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(light_blink_TimerEvtCb, NULL, interval);
    }
}

/*********************************************************************
 * @fn      light_blink_stop
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_blink_stop(void)
{
    if (gLightCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&gLightCtx.timerLedEvt);

        gLightCtx.times = 0;
        if (gLightCtx.oriSta) {
            hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
        } else {
            hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
        }
    }
}



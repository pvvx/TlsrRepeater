/*
 * battery.c
 *
 *  Created on: 18 нояб. 2023 г.
 *      Author: pvvx
 */
#include "zcl_include.h"

//#include "tl_common.h"
#include "app.h"
#include "battery.h"

#if ZCL_POWER_CFG_SUPPORT
measured_battery_t measured_battery;
#endif

#define _BAT_SPEED_CODE_SEC_ //_attribute_ram_code_sec_ // for speed

#define BAT_AVERAGE_COUNT_SHL	10 // 4,5,6,7,8,9,10,11,12 -> 16,32,64,128,256,512,1024,2048,4096

_BAT_SPEED_CODE_SEC_
//__attribute__((optimize("-Os")))
void battery_detect(bool startup_flg)
{
	u32 battery_level = BATTERY_LOW_POWER;
	if(startup_flg)
		battery_level = BATTERY_START_POWER;
	adc_channel_init(SHL_ADC_VBAT);
#if ZCL_POWER_CFG_SUPPORT
	measured_battery.mv = get_adc_mv(0);
	if(measured_battery.mv < battery_level)
#else
	if(get_adc_mv(0) < battery_level)
#endif
	{
#if PM_ENABLE
 #if USE_DISPLAY
		display_off();
 #endif
 #if USE_SENSOR_TH
		sensor_go_sleep();
 #endif
#endif
		drv_pm_longSleep(PM_SLEEP_MODE_DEEPSLEEP, PM_WAKEUP_SRC_TIMER, LOW_POWER_SLEEP_TIME_ms);
	}
#if ZCL_POWER_CFG_SUPPORT
	measured_battery.summ += measured_battery.mv;
	measured_battery.cnt++;
	if(measured_battery.cnt >= (1<<BAT_AVERAGE_COUNT_SHL)) {
		measured_battery.average_mv = measured_battery.summ >> BAT_AVERAGE_COUNT_SHL;
		measured_battery.summ -= measured_battery.average_mv;
		measured_battery.cnt--;
	} else {
		measured_battery.average_mv = measured_battery.summ / measured_battery.cnt;
	}
	if(measured_battery.average_mv > BATTERY_LOW_POWER) {
		battery_level = ((measured_battery.average_mv - BATTERY_LOW_POWER) * 20000) / ((BATTERY_MAX_POWER - BATTERY_LOW_POWER)*100);
		if(battery_level > 200)
			battery_level = 200;
	} else
		battery_level = 0;

	g_zcl_powerAttrs.batteryVoltage = (u8)((measured_battery.average_mv + 50) / 100);
    g_zcl_powerAttrs.batteryPercentage = (u8) battery_level;
#endif
}

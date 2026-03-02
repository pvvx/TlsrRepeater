/*
 * battery.c
 *
 *  Created on: 18 нояб. 2023 г.
 *      Author: pvvx
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "app.h"
#include "battery.h"

#ifdef ZCL_POWER_CFG
measured_battery_t measured_battery;
#endif

#define _BAT_SPEED_CODE_SEC_ //_attribute_ram_code_sec_ // for speed

_BAT_SPEED_CODE_SEC_
//__attribute__((optimize("-Os")))
void battery_detect(bool startup_flg)
{
	u32 battery_level = BATTERY_LOW_POWER;
	if(startup_flg)
		battery_level = BATTERY_START_POWER;
	adc_channel_init(SHL_ADC_VBAT);
#ifdef ZCL_POWER_CFG
	measured_battery.mv = get_adc_mv(0);
	if(measured_battery.mv < battery_level)
#else
	if(get_adc_mv(0) < battery_level)
#endif
	{
#if PM_ENABLE
		drv_pm_sleep(PM_SLEEP_MODE_DEEPSLEEP, 0, clock_time() + LOW_POWER_SLEEP_TIME_ms * CLOCK_16M_SYS_TIMER_CLK_1MS);
#else
		sleep_ms(1000);
		SYSTEM_RESET();
#endif
	}
#ifdef ZCL_POWER_CFG
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

/******************************************************************************
 * @file app_operator.c
 *
 * @brief
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_operator.h"

/*******************************************************************************
 * @brief Constants
 ******************************************************************************/
#define APP_OPERATOR_PRIORITY      (configMAX_PRIORITIES - 1)
#define APP_OPERATOR_LOOP_RUN_MS   1000U

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_operator_heartbeat ( void *pvParameters )
{
	// Activate power on bt module
	lib_device_bt_power_on_off ( true );

	// Enable pairing
	lib_device_bt_pairing_on_off ( true );

	while (1)
	{
		vTaskDelay ( APP_OPERATOR_LOOP_RUN_MS );

		// Read status
		if ( true == lib_device_bt_is_connected () )
		{
			// CONNECTED !!

			// Disable pairing
			lib_device_bt_pairing_on_off ( false );
		}
		else if ( true == lib_device_bt_is_bat_low () )
		{
			// BAT LOW !!
		}
		else if ( true == lib_device_bt_is_pairing () )
		{
			// PAIRING
		}
		else
		{
			// UNDEFINED
		}

		// Read command
		if ( true == lib_device_bt_is_boost_on () )
		{
			// BOOST !!
		}
		else if ( true == lib_device_bt_is_power_on () )
		{
			// POWER !!
		}
		else
		{
			// OFF !!
		}
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
BaseType_t app_operator_init ( void )
{
	BaseType_t result = pdFAIL;

	result = xTaskCreate ( app_operator_heartbeat, "operator_module", configMINIMAL_STACK_SIZE + 20, NULL, APP_OPERATOR_PRIORITY, NULL);

	lib_device_bt_init();

	return result;
}

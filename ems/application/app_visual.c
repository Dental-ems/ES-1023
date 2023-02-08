/******************************************************************************
 * @file app_visual.c
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

#include "app_visual.h"

/*******************************************************************************
 * @brief Constants
 ******************************************************************************/
#define APP_VISUAL_BLINK_RUN_MS	1000U
#define APP_VISUAL_PRIORITY 	(configMAX_PRIORITIES - 1)

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_visual_heartbeat ( void *pvParameters )
{
	while (1)
	{
		if ( 1 == ES_1023_status )
		{
			app_visual_mb_run ();
		}
		else
		{
			app_visual_mb_error ();
		}
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
BaseType_t app_visual_init ( void )
{
	BaseType_t result = pdFAIL;

	ES_1023_status = 0;

	lib_led_mb_init();

	result = xTaskCreate ( app_visual_heartbeat, "Visual module", configMINIMAL_STACK_SIZE + 10, NULL, APP_VISUAL_PRIORITY, NULL);

	return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_visual_mb_run ( void )
{
	drv_io_led_main_high ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS / 10 );

	lib_led_mb_toogle ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS / 10 );

	lib_led_mb_toogle ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS / 10 );

	lib_led_mb_toogle ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS / 10 * (10 - 3) );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_visual_mb_error ( void )
{
	lib_led_mb_toogle ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS );
}

/******************************************************************************
 * @file app_satellite.c
 *
 * @brief
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_satellite.h"

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
APP_SATELLITE_CTX app_satellite_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_satellite_heartbeat ( void *pvParameters )
{
	while (1)
	{
		vTaskDelay ( APP_SATELLITE_PERIOD_MS );
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool app_satellite_init ( QueueHandle_t* app_satellite_handle )
{
	bool result = false;

	memset ( &app_satellite_ctx, 0, sizeof ( APP_SATELLITE_CTX ) );

	result = misc_task_create ( app_satellite_heartbeat, "satellite_module", APP_SATELLITE_PRIORITY );
	if ( result != false )
	{
		app_satellite_ctx.handle = misc_queue_create ();

		*app_satellite_handle = app_satellite_ctx.handle;

		lib_remote_af_init ();
	}

	return result;
}

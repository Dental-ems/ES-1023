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
#include <string.h>

#include "app_operator.h"

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
static APP_OPERATOR_CTX app_operator_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_operator_heartbeat ( void *pvParameters )
{
	// Activate power on bt module
	lib_device_bt_power_on_off ( true );

	// Enable pairing
	lib_device_bt_pairing_on_off ( true );

	// Take tick reference for timeout pairing
	app_operator_ctx.pairing_since = xTaskGetTickCount ( );

	while (1)
	{
		// Read status
		if ( true == lib_device_bt_is_connected () )
		{
			// CONNECTED !!
			app_operator_ctx.status.in_use = 3;

			// Disable pairing
			lib_device_bt_pairing_on_off ( false );
		}
		else if ( true == lib_device_bt_is_bat_low () )
		{
			// BAT LOW !!
			app_operator_ctx.status.in_use = 2;
		}
		else if ( true == lib_device_bt_is_pairing () )
		{
			// PAIRING
			app_operator_ctx.status.in_use = 1;

			if ( ( xTaskGetTickCount ( ) - app_operator_ctx.pairing_since ) > APP_OPERATOR_PAIR_TIMEOUT_MS )
			{
				// Abort pairing because of timeout
				lib_device_bt_pairing_on_off ( false );
			}
		}
		else
		{
			// UNDEFINED
			app_operator_ctx.status.in_use = 0;
		}

		// Only when pedal is connected
		if ( app_operator_ctx.status.in_use > 1 )
		{
			// Read command
			if ( true == lib_device_bt_is_boost_on () )
			{
				// BOOST !!
				app_operator_ctx.command.in_use = 1;
			}
			else if ( true == lib_device_bt_is_power_on () )
			{
				// POWER !!
				app_operator_ctx.command.in_use = 2;
			}
			else
			{
				// OFF !!
				app_operator_ctx.command.in_use = 0;
			}
		}
		else
		{
			// OFF !!
			app_operator_ctx.command.in_use = 0;
		}

		app_operator_notify ();

		vTaskDelay ( APP_OPERATOR_LOOP_RUN_MS );
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool app_operator_init ( QueueHandle_t* app_operator_handle )
{
	bool result = false;

	memset ( &app_operator_ctx, 0, sizeof ( APP_OPERATOR_CTX ) );

	result = misc_task_create ( app_operator_heartbeat, "operator_module", APP_OPERATOR_PRIORITY );
	if ( result != false )
	{
		app_operator_ctx.handle = misc_queue_create ();

		*app_operator_handle = app_operator_ctx.handle;

		lib_device_bt_init();
	}

	return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_operator_set_visual_queue ( QueueHandle_t app_visual_handle )
{
	app_operator_ctx.visual_handle = app_visual_handle;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_operator_notify ( void )
{
	MISC_MSG_BODY operator_msg;

	memset ( &operator_msg, 0, sizeof ( MISC_MSG_BODY ) );

	// Notification management : status
	if ( app_operator_ctx.status.in_use != app_operator_ctx.status.sent )
	{
		operator_msg.key   = 1;
		operator_msg.value = app_operator_ctx.status.in_use;

		if ( pdPASS == misc_send_dest ( app_operator_ctx.visual_handle, &operator_msg ) )
		{
			app_operator_ctx.status.sent = app_operator_ctx.status.in_use;
		}
	}

	// Notification management : command
	if ( app_operator_ctx.command.in_use != app_operator_ctx.command.sent )
	{
		operator_msg.key   = 2;
		operator_msg.value = app_operator_ctx.command.in_use;

		if ( pdPASS == misc_send_dest ( app_operator_ctx.visual_handle, &operator_msg ) )
		{
			app_operator_ctx.command.sent = app_operator_ctx.command.in_use;
		}
	}
}

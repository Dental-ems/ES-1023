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
#define APP_OPERATOR_LOOP_RUN_MS   100U

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
APP_OPERATOR_CTX app_operator_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_operator_heartbeat ( void *pvParameters )
{
	// Activate power on bt module
	lib_device_bt_power_on_off ( true );

	// Enable pairing
	lib_device_bt_pairing_on_off ( true );
	// TODO : add tick monitoring to stop pairing

	while (1)
	{
		// Read status
		if ( true == lib_device_bt_is_connected () )
		{
			// CONNECTED !!
			app_operator_ctx.status.in_use = 0;

			// Disable pairing
			lib_device_bt_pairing_on_off ( false );
		}
		else if ( true == lib_device_bt_is_bat_low () )
		{
			// BAT LOW !!
			app_operator_ctx.status.in_use = 1;
		}
		else if ( true == lib_device_bt_is_pairing () )
		{
			// PAIRING
			app_operator_ctx.status.in_use = 2;
		}
		else
		{
			// UNDEFINED
			app_operator_ctx.status.in_use = 3;
		}

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

		app_operator_notify ();

		vTaskDelay ( APP_OPERATOR_LOOP_RUN_MS );
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
BaseType_t app_operator_init ( QueueHandle_t* app_operator_handle )
{
	BaseType_t result = pdFAIL;

	memset ( &app_operator_ctx, 0, sizeof ( APP_OPERATOR_CTX ) );

	result = xTaskCreate ( app_operator_heartbeat, "operator_module", configMINIMAL_STACK_SIZE + 200, NULL, APP_OPERATOR_PRIORITY, NULL);

	app_operator_ctx.handle = xQueueCreate ( 2, sizeof (APP_MSG) );

	*app_operator_handle = app_operator_ctx.handle;

	lib_device_bt_init();

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
BaseType_t app_operator_send_external ( QueueHandle_t handle_dest, APP_MSG* operator_msg )
{
	BaseType_t result = pdFAIL;

	if ( handle_dest != NULL )
	{
		result = xQueueSendToBack ( handle_dest, ( void * ) operator_msg, ( TickType_t ) 0 );
	}

	return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_operator_notify ( void )
{
	APP_MSG operator_msg;

	memset ( &operator_msg, 0, sizeof ( APP_MSG ) );

	// Notification management : status
	if ( app_operator_ctx.status.in_use != app_operator_ctx.status.sent )
	{
		operator_msg.key   = 1;
		operator_msg.value = app_operator_ctx.status.in_use;

		if ( pdPASS == app_operator_send_external ( app_operator_ctx.visual_handle, &operator_msg ) )
		{
			app_operator_ctx.status.sent = app_operator_ctx.status.in_use;
		}
	}

	// Notification management : command
	if ( app_operator_ctx.command.in_use != app_operator_ctx.command.sent )
	{
		operator_msg.key   = 2;
		operator_msg.value = app_operator_ctx.command.in_use;

		if ( pdPASS == app_operator_send_external ( app_operator_ctx.visual_handle, &operator_msg ) )
		{
			app_operator_ctx.command.sent = app_operator_ctx.command.in_use;
		}
	}
}

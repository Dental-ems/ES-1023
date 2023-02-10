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
#define APP_VISUAL_PRIORITY 	   (configMAX_PRIORITIES - 2)
#define APP_VISUAL_BLINK_RUN_MS	   1000U

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
typedef struct
{
	uint8_t key;
	uint8_t value;
} APP_MSG;

typedef struct
{
	QueueHandle_t handle;

	uint8_t status;
	uint16_t next_period;
} APP_VISUAL_CTX;

APP_VISUAL_CTX app_visual_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_visual_heartbeat ( void *pvParameters )
{
	BaseType_t result = pdFAIL;

	APP_MSG operator_msg;
	memset ( &operator_msg, 0, sizeof ( APP_MSG ) );

	while (1)
	{
		if ( 0 == app_visual_ctx.status )
		{
			app_visual_mb_run ();
		}
		else if ( 20 == app_visual_ctx.status )
		{
			app_visual_mb_boost ();
		}
		else if ( 40 == app_visual_ctx.status )
		{
			app_visual_mb_power ();
		}
		else
		{
			app_visual_mb_error ();
		}

		result = xQueueReceive ( app_visual_ctx.handle, &operator_msg, ( TickType_t ) 0 );
		if ( pdPASS == result )
		{
			app_visual_ctx.status = operator_msg.value * operator_msg.key * 10;
		}

		vTaskDelay ( app_visual_ctx.next_period );
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
BaseType_t app_visual_init ( QueueHandle_t* app_visual_handle )
{
	BaseType_t result = pdFAIL;

	memset ( &app_visual_ctx, 0, sizeof ( APP_VISUAL_CTX ) );

	result = xTaskCreate ( app_visual_heartbeat, "visual_module", configMINIMAL_STACK_SIZE + 200, NULL, APP_VISUAL_PRIORITY, NULL);

	app_visual_ctx.handle = xQueueCreate ( 2, sizeof (APP_MSG) );

	*app_visual_handle = app_visual_ctx.handle;

	lib_led_mb_init();

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

	app_visual_ctx.next_period = APP_VISUAL_BLINK_RUN_MS / 10 * (10 - 3);
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_visual_mb_error ( void )
{
	lib_led_mb_toogle ();

	app_visual_ctx.next_period = APP_VISUAL_BLINK_RUN_MS;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_visual_mb_power ( void )
{
	lib_led_mb_enable ();

	app_visual_ctx.next_period = APP_VISUAL_BLINK_RUN_MS;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_visual_mb_boost ( void )
{
	lib_led_mb_toogle ();

	vTaskDelay ( APP_VISUAL_BLINK_RUN_MS / 10 );

	lib_led_mb_toogle ();

	app_visual_ctx.next_period = APP_VISUAL_BLINK_RUN_MS / 10;
}

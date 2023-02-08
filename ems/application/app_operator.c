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
#define APP_OPERATOR_PRIORITY (configMAX_PRIORITIES - 2)

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_operator_heartbeat ( void *pvParameters )
{
	while (1)
	{
		;
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
BaseType_t app_operator_init ( void )
{
	BaseType_t result = pdFAIL;

	result = xTaskCreate ( app_operator_heartbeat, "operator module", configMINIMAL_STACK_SIZE + 10, NULL, APP_OPERATOR_PRIORITY, NULL);

	lib_device_bt_init();

	return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/

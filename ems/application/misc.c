/******************************************************************************
 * @file misc.c
 *
 *  Created on: 10 february 2023
 *      Author: dcardinale
 *****************************************************************************/

#include "misc.h"

/******************************************************************************
 * @brief
 *****************************************************************************/
QueueHandle_t misc_queue_create ( void )
{
	return xQueueCreate ( MISC_QUEUE_MSG_NBR, sizeof ( MISC_MSG_BODY ) );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool misc_task_create ( void* callback, char* name_str, uint8_t piority )
{
	BaseType_t result = pdFAIL;

	result = xTaskCreate ( callback, name_str, configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES - piority, NULL);

	if ( pdPASS == result )
		return true;
	else
		return false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool misc_get_msg ( QueueHandle_t handle_dest, MISC_MSG_BODY* msg_body )
{
	BaseType_t result = pdFAIL;

	if ( handle_dest != NULL )
	{
		result = xQueueReceive ( handle_dest, msg_body, ( TickType_t ) 0 );
	}

	if ( pdPASS == result )
		return true;
	else
		return false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool misc_send_dest ( QueueHandle_t handle_dest, MISC_MSG_BODY* msg_body )
{
	BaseType_t result = pdFAIL;

	if ( handle_dest != NULL )
	{
		result = xQueueSendToBack ( handle_dest, ( void * ) msg_body, ( TickType_t ) 0 );
	}

	if ( pdPASS == result )
		return true;
	else
		return false;
}

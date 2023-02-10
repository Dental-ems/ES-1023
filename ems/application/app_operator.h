/******************************************************************************
 * @file app_operator.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_OPERATOR_H_
#define APP_OPERATOR_H_

#include "queue.h"

#include "../lib/lib_device_bt.h"

typedef struct
{
	uint8_t key;
	uint8_t value;
} APP_MSG;

typedef struct
{
	uint8_t sent;
	uint8_t in_use;
} information;

typedef struct
{
	QueueHandle_t handle;

	information command;
	information status;

	QueueHandle_t visual_handle;
} APP_OPERATOR_CTX;

BaseType_t app_operator_init 		( QueueHandle_t* app_operator_handle );

void app_operator_set_visual_queue  ( QueueHandle_t app_visual_handle );
BaseType_t app_operator_send_external ( QueueHandle_t handle_dest, APP_MSG* operator_msg );

void app_operator_notify 			( void );

#endif /* APP_OPERATOR_H_ */

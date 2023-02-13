/******************************************************************************
 * @file app_operator.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_OPERATOR_H_
#define APP_OPERATOR_H_

#include "../lib/lib_device_bt.h"

#include "misc.h"

#define APP_OPERATOR_PRIORITY         1U
#define APP_OPERATOR_LOOP_RUN_MS      100U
#define APP_OPERATOR_PAIR_TIMEOUT_MS  (3*60000U)

typedef struct
{
	uint8_t sent;
	uint8_t in_use;
} information;

typedef struct
{
	QueueHandle_t 	handle;
	information 	command;
	information 	status;
	QueueHandle_t 	visual_handle;
	TickType_t 	    pairing_since;
} APP_OPERATOR_CTX;

bool  app_operator_init 		 	 ( QueueHandle_t* app_operator_handle );
void  app_operator_set_visual_queue  ( QueueHandle_t app_visual_handle );
void  app_operator_notify 		     ( void );

#endif /* APP_OPERATOR_H_ */

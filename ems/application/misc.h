/******************************************************************************
 * @file misc.c
 *
 *  Created on: 10 february 2023
 *      Author: dcardinale
 *****************************************************************************/

#ifndef MISC_H_
#define MISC_H_

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define MISC_QUEUE_MSG_NBR    2

typedef struct
{
    uint8_t key;
    uint8_t value;
} MISC_MSG_BODY;

QueueHandle_t misc_queue_create ( void );
bool          misc_task_create  ( void* callback, char* name_str, uint8_t piority );
bool          misc_get_msg      ( QueueHandle_t handle_dest, MISC_MSG_BODY* msg_body );
bool          misc_send_dest    ( QueueHandle_t handle_dest, MISC_MSG_BODY* msg_body );

#endif /* APPLICATION_MISC_H_ */

/******************************************************************************
 * @file app_satellite.h
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_SATELLITE_H_
#define APP_SATELLITE_H_

#include "../lib/lib_remote_af.h"

#include "misc.h"

#define APP_SATELLITE_PRIORITY 	   	3U
#define APP_SATELLITE_PERIOD_MS		(5*1000U)

typedef struct
{
	QueueHandle_t 	   handle;
	LIB_REMOTE_AF_ITEM airflow;
} APP_SATELLITE_CTX;

bool app_satellite_init 			 ( QueueHandle_t* app_satellite_handle );
void app_satellite_update_encoder_af ( void );

#endif /* APP_SATELLITE_H_ */

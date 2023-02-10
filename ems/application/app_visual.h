/******************************************************************************
 * @file app_visual.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_VISUAL_H_
#define APP_VISUAL_H_

#include "queue.h"

#include "../lib/lib_led_mb.h"

BaseType_t  app_visual_init     ( QueueHandle_t* app_visual_handle );

void 		app_visual_mb_run   ( void );
void 		app_visual_mb_error ( void );
void        app_visual_mb_power ( void );
void        app_visual_mb_boost ( void );

#endif /* APP_VISUAL_H_ */

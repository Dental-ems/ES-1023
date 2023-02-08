/******************************************************************************
 * @file app_visual.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_VISUAL_H_
#define APP_VISUAL_H_

#include "../lib/lib_led_mb.h"

extern uint8_t ES_1023_status;

BaseType_t  app_visual_init     ( void );
void 		app_visual_mb_run   ( void );
void 		app_visual_mb_error ( void );

#endif /* APP_VISUAL_H_ */

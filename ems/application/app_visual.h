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

#include "misc.h"

#define APP_VISUAL_PRIORITY        2U
#define APP_VISUAL_BLINK_RUN_MS    1000U

typedef struct
{
    QueueHandle_t    handle;
    uint8_t          status;
    uint16_t         next_period;
} APP_VISUAL_CTX;

bool  app_visual_init     ( QueueHandle_t* app_visual_handle );
void  app_visual_mb_run   ( void );
void  app_visual_mb_error ( void );
void  app_visual_mb_power ( void );
void  app_visual_mb_boost ( void );

#endif /* APP_VISUAL_H_ */

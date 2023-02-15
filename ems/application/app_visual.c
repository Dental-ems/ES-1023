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
#include <string.h>

#include "app_visual.h"

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
static APP_VISUAL_CTX app_visual_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_visual_heartbeat ( void *pvParameters )
{
    MISC_MSG_BODY visual_msg;

    memset ( &visual_msg, 0, sizeof ( MISC_MSG_BODY ) );

    while (1)
    {
        if ( 13 == app_visual_ctx.status )
        {
            app_visual_mb_run ();
        }
        else if ( 21 == app_visual_ctx.status )
        {
            app_visual_mb_boost ();
        }
        else if ( 22 == app_visual_ctx.status )
        {
            app_visual_mb_power ();
        }
        else
        {
            app_visual_mb_error ();
        }

        if ( true == misc_get_msg ( app_visual_ctx.handle, &visual_msg ) )
        {
            app_visual_ctx.status = visual_msg.value + visual_msg.key * 10;
        }

        vTaskDelay ( app_visual_ctx.next_period );
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool app_visual_init ( QueueHandle_t* app_visual_handle )
{
    bool result = false;

    memset ( &app_visual_ctx, 0, sizeof ( APP_VISUAL_CTX ) );

    result = misc_task_create ( app_visual_heartbeat, "visual_module", APP_VISUAL_PRIORITY );
    if ( result != false )
    {
        app_visual_ctx.handle = misc_queue_create ();

        *app_visual_handle = app_visual_ctx.handle;

        lib_led_mb_init();
    }

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

/*******************************************************************************
 * @file    ES-1023.c
 * @brief   Application entry point
 *
 * @author  Damien Cardinale
 * @email   dcardinale@ems-ch.ch
 *
 * @date    08 february 2023
 *******************************************************************************/
#include <stdio.h>

#include "board.h"
#include "peripherals.h"
#include "clock_config.h"
#include "LPC5528.h"
#include "fsl_power.h"

#include "FreeRTOS.h"
#include "task.h"

#include "../ems/application/app_visual.h"
#include "../ems/application/app_operator.h"
#include "../ems/application/app_satellite.h"

/*******************************************************************************
 * @brief Constants
 ******************************************************************************/

/*******************************************************************************
 * @brief Global
 ******************************************************************************/
QueueHandle_t app_visual_handle    = NULL;
QueueHandle_t app_operator_handle  = NULL;
QueueHandle_t app_satellite_handle = NULL;

/*******************************************************************************
 * @brief Prototypes
 ******************************************************************************/
void EMS_task_manager ( void );

/*******************************************************************************
 * @brief
 ******************************************************************************/
int main ( void )
{
    // Board pin, clock, debug console init
    // set BOD VBAT level to 1.65V
    POWER_SetBodVbatLevel ( kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false );

    // Init board hardware
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals ();

    EMS_task_manager ();
    app_operator_set_visual_queue ( app_visual_handle );

    vTaskStartScheduler ();

    while (1)
    {
        ;
    }

    return 0;
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void EMS_task_manager ( void )
{
    BaseType_t result = pdFAIL;

    result = app_visual_init ( &app_visual_handle );

    if ( pdPASS == result ) result = app_operator_init ( &app_operator_handle );

    if ( pdPASS == result ) result = app_satellite_init ( &app_satellite_handle );

    if ( result != pdPASS )
    {
        while (1)
        {
            ;
        }
    }
}

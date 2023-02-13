/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * @file    ES-1023.c
 * @brief   Application entry point
 *
 * @author  Damien Cardinale
 * @email   dcardinale@ems-ch.ch
 * @date    08.02.2023
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
    BOARD_BootClockFRO12M ();
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

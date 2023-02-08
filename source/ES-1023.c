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
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC5528.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_power.h"

#include "FreeRTOS.h"
#include "task.h"

#define my_task_PRIORITY (configMAX_PRIORITIES - 1)
static void my_task ( void *pvParameters );

/*******************************************************************************
 * @brief   Application entry point
 ******************************************************************************/
int main(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    // Board pin, clock, debug console init
    // set BOD VBAT level to 1.65V
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);

    // enable clock for GPIO
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    // Init board hardware
    BOARD_InitBootPins();
    BOARD_BootClockFRO12M();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    // Init FSL debug console.
    BOARD_InitDebugConsole();
#endif

    // Init output LED GPIO
    GPIO_PortInit(GPIO, BOARD_INITPINS_LED_MAIN_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, &led_config);
    GPIO_PinWrite(GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, 1);

    // Port masking
    GPIO_PortMaskedSet(GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0x0000FFFF);
    GPIO_PortMaskedWrite(GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0xFFFFFFFF);

    BaseType_t result = xTaskCreate(my_task, "my_task", configMINIMAL_STACK_SIZE + 10, NULL, my_task_PRIORITY, NULL);

	if ( result != pdPASS)
	{
		while (1)
		{
			;
		}
	}

	vTaskStartScheduler();

	while (1)
	{
		;
	}

	return 0;
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
static void my_task ( void *pvParameters )
{
	while (1)
	{
		GPIO_PortToggle ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 1u << BOARD_INITPINS_LED_MAIN_PIN );

		vTaskDelay(500U);
	}
}

/******************************************************************************
 * @file drv_io.c
 *
 * @brief
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "pin_mux.h"
#include "fsl_gpio.h"

#include "drv_io.h"

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_init ( void )
{
    // enable clock for GPIO
    CLOCK_EnableClock ( kCLOCK_Gpio0 );
    CLOCK_EnableClock ( kCLOCK_Gpio1 );

    BOARD_InitBootPins ();

    drv_io_led_main_init ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_led_main_init ( void )
{
    gpio_pin_config_t led_main_cfg = {
        kGPIO_DigitalOutput,
        0,
    };

    // Init output LED GPIO
    GPIO_PortInit ( GPIO, BOARD_INITPINS_LED_MAIN_PORT );
    GPIO_PinInit  ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, &led_main_cfg );
    GPIO_PinWrite ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, 1 );

    // Port masking
    GPIO_PortMaskedSet   ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0x0000FFFF );
    GPIO_PortMaskedWrite ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0xFFFFFFFF );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_led_main_toogle ( void )
{
	GPIO_PortToggle ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 1u << BOARD_INITPINS_LED_MAIN_PIN );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_led_main_high ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, 1 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_led_main_low ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, 0 );
}

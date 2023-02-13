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

static bool drv_io_is_ready = false;

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_init ( void )
{
	if ( false == drv_io_is_ready )
	{
		// enable clock for GPIO
		CLOCK_EnableClock ( kCLOCK_Gpio0 );
		CLOCK_EnableClock ( kCLOCK_Gpio1 );

		BOARD_InitBootPins ();

		GPIO_PortInit ( GPIO, BOARD_INITPINS_LED_MAIN_PORT );

#if 0 // Ports are all the same
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_OFF_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_PAIRING_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_S1_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_S2_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_P1_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_BTM_P2_PORT );
		GPIO_PortInit ( GPIO, BOARD_INITPINS_UART_SAT_DE_GPIO );
#endif

		// Port masking
		//GPIO_PortMaskedSet   ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0x0000FFFF );
		//GPIO_PortMaskedWrite ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, 0xFFFFFFFF );

		drv_io_is_ready = true;
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_led_main_init ( void )
{
	drv_io_init ();

    gpio_pin_config_t led_main_cfg = {
        kGPIO_DigitalOutput,
        0,
    };

	GPIO_PinInit ( GPIO, BOARD_INITPINS_LED_MAIN_PORT, BOARD_INITPINS_LED_MAIN_PIN, &led_main_cfg );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_btm_init ( CALLBACK_BTM callback_btm_pin_s1, CALLBACK_BTM callback_btm_pin_s2, CALLBACK_BTM callback_btm_pin_p1, CALLBACK_BTM callback_btm_pin_p2 )
{
	drv_io_init ();

    gpio_pin_config_t btm_off_cfg = {
        kGPIO_DigitalOutput,
        0,
    };

    gpio_pin_config_t btm_pairing_cfg = {
        kGPIO_DigitalOutput,
        0,
    };

    gpio_pin_config_t btm_s1_cfg = {
        kGPIO_DigitalInput,
    };

    gpio_pin_config_t btm_s2_cfg = {
    		kGPIO_DigitalInput,
    };

    gpio_pin_config_t btm_p1_cfg = {
    		kGPIO_DigitalInput,
    };

    gpio_pin_config_t btm_p2_cfg = {
    		kGPIO_DigitalInput,
    };

    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_OFF_PORT, BOARD_INITPINS_BTM_OFF_PIN, &btm_off_cfg );
    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_PAIRING_PORT, BOARD_INITPINS_BTM_PAIRING_PIN, &btm_pairing_cfg );
    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_S1_PORT, BOARD_INITPINS_BTM_S1_PIN, &btm_s1_cfg );
    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_S2_PORT, BOARD_INITPINS_BTM_S2_PIN, &btm_s2_cfg );
    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_P1_PORT, BOARD_INITPINS_BTM_P1_PIN, &btm_p1_cfg );
    GPIO_PinInit ( GPIO, BOARD_INITPINS_BTM_P2_PORT, BOARD_INITPINS_BTM_P2_PIN, &btm_p2_cfg );

    // Connect trigger sources to PINT
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, BOARD_INITPINS_BTM_IT_S1_PIN );
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt1, BOARD_INITPINS_BTM_IT_S2_PIN );
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt2, BOARD_INITPINS_BTM_IT_P1_PIN );
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt3, BOARD_INITPINS_BTM_IT_P2_PIN );

    // Initialize PINT
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableBothEdges, callback_btm_pin_s1 );
    PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableBothEdges, callback_btm_pin_s2 );
    PINT_PinInterruptConfig(PINT, kPINT_PinInt2, kPINT_PinIntEnableBothEdges, callback_btm_pin_p1 );
    PINT_PinInterruptConfig(PINT, kPINT_PinInt3, kPINT_PinIntEnableBothEdges, callback_btm_pin_p2 );

    // Enable callbacks for PINT
    PINT_EnableCallback(PINT);
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

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_btm_power_on ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_BTM_OFF_PORT, BOARD_INITPINS_BTM_OFF_PIN, 0 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_btm_power_off ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_BTM_OFF_PORT, BOARD_INITPINS_BTM_OFF_PIN, 1 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_btm_pairing_on ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_BTM_PAIRING_PORT, BOARD_INITPINS_BTM_PAIRING_PIN, 1 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_io_btm_pairing_off ( void )
{
	GPIO_PinWrite ( GPIO, BOARD_INITPINS_BTM_PAIRING_PORT, BOARD_INITPINS_BTM_PAIRING_PIN, 0 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t drv_io_btm_get_s1 ( void )
{
	return (uint8_t)GPIO_PinRead ( GPIO, BOARD_INITPINS_BTM_S1_PORT, BOARD_INITPINS_BTM_S1_PIN );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t drv_io_btm_get_s2 ( void )
{
	return (uint8_t)GPIO_PinRead ( GPIO, BOARD_INITPINS_BTM_S2_PORT, BOARD_INITPINS_BTM_S2_PIN );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t drv_io_btm_get_p1 ( void )
{
	return (uint8_t)GPIO_PinRead ( GPIO, BOARD_INITPINS_BTM_P1_PORT, BOARD_INITPINS_BTM_P1_PIN );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t drv_io_btm_get_p2 ( void )
{
	return (uint8_t)GPIO_PinRead ( GPIO, BOARD_INITPINS_BTM_P1_PORT, BOARD_INITPINS_BTM_P1_PIN );
}

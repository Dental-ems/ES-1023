/******************************************************************************
 * @file drv_io.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef DRV_IO_H_
#define DRV_IO_H_

#include "fsl_pint.h"
#include "fsl_inputmux.h"

typedef void (*CALLBACK_BTM)( pint_pin_int_t pintr, uint32_t pmatch_status );

void drv_io_init          ( void );
void drv_io_led_main_init ( void );
void drv_io_en_5v_init    ( void );
void drv_io_btm_init      ( CALLBACK_BTM callback_btm_pin_s1,
                            CALLBACK_BTM callback_btm_pin_s2,
                            CALLBACK_BTM callback_btm_pin_p1,
                            CALLBACK_BTM callback_btm_pin_p2 );

void drv_io_led_main_toogle ( void );
void drv_io_led_main_high   ( void );
void drv_io_led_main_low    ( void );
void drv_io_en_5v_high      ( void );
void drv_io_btm_pairing_on  ( void );
void drv_io_btm_pairing_off ( void );
void drv_io_btm_power_on    ( void );
void drv_io_btm_power_off   ( void );
uint8_t drv_io_btm_get_s1   ( void );
uint8_t drv_io_btm_get_s2   ( void );
uint8_t drv_io_btm_get_p1   ( void );
uint8_t drv_io_btm_get_p2   ( void );

void drv_io_it_s1 ( pint_pin_int_t pintr, uint32_t pmatch_status );

#endif /* DRV_IO_H_ */

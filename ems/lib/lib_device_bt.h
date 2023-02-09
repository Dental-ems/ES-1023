/******************************************************************************
 * @file lib_device_bt.h
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef LIB_DEVICE_BT_H_
#define LIB_DEVICE_BT_H_

#include "../drv/drv_io.h"

typedef struct
{
	uint8_t pin_s1;
	uint8_t pin_s2;
	uint8_t pin_p1;
	uint8_t pin_p2;
} LIB_DEVICE_BTM_CTX;

void lib_device_bt_init			  ( void );
void lib_device_bt_pairing_on_off ( bool on_off );
void lib_device_bt_power_on_off   ( bool on_off );

void lib_device_update_s1 ( pint_pin_int_t pintr, uint32_t pmatch_status );
void lib_device_update_s2 ( pint_pin_int_t pintr, uint32_t pmatch_status );
void lib_device_update_p1 ( pint_pin_int_t pintr, uint32_t pmatch_status );
void lib_device_update_p2 ( pint_pin_int_t pintr, uint32_t pmatch_status );

bool lib_device_bt_is_connected ( void );
bool lib_device_bt_is_power_on  ( void );
bool lib_device_bt_is_boost_on  ( void );
bool lib_device_bt_is_bat_low   ( void );
bool lib_device_bt_is_pairing   ( void );

#endif /* LIB_DEVICE_BT_H_ */

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

void lib_device_bt_init			  ( void );
void lib_device_bt_pairing_on_off ( bool on_off );

#endif /* LIB_DEVICE_BT_H_ */

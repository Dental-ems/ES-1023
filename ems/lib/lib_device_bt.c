/******************************************************************************
 * @file lib_device_bt.c
 *
 * @brief
 *
 *  Created on: 08 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "../lib/lib_device_bt.h"

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_device_bt_init ( void )
{
	drv_io_btm_init ();

	drv_io_btm_power_on ();
}

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_device_bt_pairing_on_off ( bool on_off )
{
	if ( true == on_off )
	{
		drv_io_btm_pairing_on ();
	}
	else
	{
		drv_io_btm_pairing_off ();
	}
}

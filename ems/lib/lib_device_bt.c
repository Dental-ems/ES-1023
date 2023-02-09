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
#include <string.h>

#include "../lib/lib_device_bt.h"

/******************************************************************************
 * @brief Variables
 ******************************************************************************/
LIB_DEVICE_BTM_CTX lib_device_btm_ctx;

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_device_bt_init ( void )
{
	memset ( &lib_device_btm_ctx, 0, sizeof (LIB_DEVICE_BTM_CTX) );

	drv_io_btm_init ( lib_device_update_s1,
					  lib_device_update_s2,
					  lib_device_update_p1,
					  lib_device_update_p2 );

	lib_device_bt_power_on_off ( false );
}

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_device_bt_power_on_off ( bool on_off )
{
	if ( true == on_off )
	{
		drv_io_btm_power_on ();
	}
	else
	{
		drv_io_btm_power_off ();
	}
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

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_device_update_s1 ( pint_pin_int_t pintr, uint32_t pmatch_status )
{
	lib_device_btm_ctx.pin_s1 = drv_io_btm_get_s1 ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_device_update_s2 ( pint_pin_int_t pintr, uint32_t pmatch_status )
{
	lib_device_btm_ctx.pin_s2 = drv_io_btm_get_s2 ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_device_update_p1 ( pint_pin_int_t pintr, uint32_t pmatch_status )
{
	lib_device_btm_ctx.pin_p1 = drv_io_btm_get_p1 ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_device_update_p2 ( pint_pin_int_t pintr, uint32_t pmatch_status )
{
	lib_device_btm_ctx.pin_p2 = drv_io_btm_get_p2 ();
}

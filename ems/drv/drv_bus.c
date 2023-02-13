/******************************************************************************
 * @file drv_bus.c
 *
 * @brief
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "drv_bus.h"

static bool drv_bus_is_ready = false;

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_init ( void )
{
	if ( false == drv_bus_is_ready )
	{
		drv_bus_is_ready = true;
	}
}

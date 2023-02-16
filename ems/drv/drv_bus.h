/******************************************************************************
 * @file drv_bus.h
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef DRV_BUS_H_
#define DRV_BUS_H_

#include "fsl_usart.h"

#define DRV_BUS_ADDR_MASTER       0x03

#define DRV_BUS_ADDR_SLAVE_PZ     0x81
#define DRV_BUS_ADDR_SLAVE_AF     0x82

#define DRV_BUS_UART_SAT_ID       USART7

typedef void (*DRV_BUS_CALLBACK)( USART_Type *base, usart_handle_t *handle, status_t status, void *userData );

void drv_bus_init_master    ( DRV_BUS_CALLBACK uart_callback );
void drv_bus_transmit_start ( void );
void drv_bus_transmit_end   ( void );
void drv_bus_transmit_run   ( uint8_t* buffer, size_t lenght );
void drv_bus_receive_run    ( uint8_t* buffer, size_t lenght );
void drv_bus_send_to_slave  ( uint16_t slave_addr );

#endif /* DRV_BUS_H_ */

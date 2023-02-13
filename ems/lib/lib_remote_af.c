/******************************************************************************
 * @file lib_remote_af.c
 *
 * @brief
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "../lib/lib_remote_af.h"

/******************************************************************************
 * @brief
 ******************************************************************************/
static LIB_REMOTE_AF lib_remote_af_ctx;

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_af_init ( void )
{
	memset ( &lib_remote_af_ctx, 0, sizeof ( LIB_REMOTE_AF ) );

	drv_bus_init_master ( lib_remote_af_callback );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_af_exchange ( uint8_t* data_buf_req, uint8_t* data_buf_resp )
{
    // Start receiving
	drv_bus_receive_run ( data_buf_resp, LIB_REMOTE_AF_TRAME_LEN_BYTES + 1 );

    // Open transmitter
	drv_bus_transmit_start ();

    // Write data on transmitter
	drv_bus_transmit_run ( data_buf_req, LIB_REMOTE_AF_TRAME_LEN_BYTES );

    // Start transmitting
	lib_remote_af_transmit ();

	// Close transmitter
	drv_bus_transmit_end ();

	// Send to Airflow slave
	drv_bus_send_to_slave ( DRV_BUS_ADDR_SLAVE_AF );

    // Waiting for response
	lib_remote_af_receive ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_af_transmit ( void )
{
    while ( lib_remote_af_ctx.flag_transmit == false ) { }

    lib_remote_af_ctx.flag_transmit = false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_af_receive ( void )
{
    while ( lib_remote_af_ctx.flag_receive == false ) { }

    lib_remote_af_ctx.flag_receive = false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_af_encode ( void )
{
	// TODO : encode request
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_af_decode ( void )
{
	// TODO : decode response
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_af_callback ( USART_Type *base, usart_handle_t *handle, status_t status, void *userData )
{
    if ( kStatus_USART_TxIdle == status )
    {
        lib_remote_af_ctx.flag_transmit = true;
    }

    if ( kStatus_USART_RxIdle == status )
    {
    	lib_remote_af_ctx.flag_receive = true;
    }
}

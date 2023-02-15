/******************************************************************************
 * @file lib_remote_pz.c
 *
 * @brief
 *
 *  Created on: 14 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "../lib/lib_remote_pz.h"

/******************************************************************************
 * @brief
 ******************************************************************************/
static LIB_REMOTE_PZ_CTX lib_remote_pz_ctx;

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_pz_init ( void )
{
	memset ( &lib_remote_pz_ctx, 0, sizeof ( LIB_REMOTE_PZ_CTX ) );

	drv_bus_init_master ( lib_remote_pz_callback );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_pz_exchange ( uint8_t* data_buf_req, uint8_t* data_buf_resp )
{
    // Start receiving
	drv_bus_receive_run ( data_buf_resp, LIB_REMOTE_PZ_TRAME_LEN_RESP );

    // Open transmitter
	drv_bus_transmit_start ();

    // Write data on transmitter
	drv_bus_transmit_run ( data_buf_req, LIB_REMOTE_PZ_TRAME_LEN_REQ );

    // Start transmitting
	lib_remote_af_transmit ();

	// Send to Piezon slave
	drv_bus_send_to_slave ( DRV_BUS_ADDR_SLAVE_PZ );

	// Close transmitter
	drv_bus_transmit_end ();

    // Waiting for response
	lib_remote_af_receive ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_pz_transmit ( void )
{
	// TODO : remove blocking procedure
	// tip : use freertos peripheral option

    while ( lib_remote_pz_ctx.flag_transmit == false ) { }

    lib_remote_pz_ctx.flag_transmit = false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void lib_remote_pz_receive ( void )
{
	// TODO : remove blocking procedure
	// tip : use freertos peripheral option

    while ( lib_remote_pz_ctx.flag_receive == false ) { }

    lib_remote_pz_ctx.flag_receive = false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool lib_remote_pz_encode ( LIB_REMOTE_PZ_LL_REQ* msg_to_encode, uint8_t version )
{
	return lib_remote_af_encode ( msg_to_encode, version );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool lib_remote_pz_decode ( LIB_REMOTE_PZ_LL_RESP* msg_to_decode )
{
	return lib_remote_af_decode ( msg_to_decode );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_extract_encoder ( uint8_t* data )
{
	return lib_remote_af_extract_encoder ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_checksum ( LIB_REMOTE_PZ_LL_HEADER header_to_checksum )
{
	return lib_remote_af_checksum ( header_to_checksum );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_pz_callback ( USART_Type *base, usart_handle_t *handle, status_t status, void *user_data )
{
    if ( kStatus_USART_TxIdle == status )
    {
        lib_remote_pz_ctx.flag_transmit = true;
    }

    if ( kStatus_USART_RxIdle == status )
    {
    	lib_remote_pz_ctx.flag_receive = true;
    }
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
bool lib_remote_pz_request ( uint8_t req_type, LIB_REMOTE_PZ_LL_REQ* msg_req, LIB_REMOTE_PZ_LL_RESP* msg_resp )
{
	bool    result = false;
	uint8_t buffer_in[LIB_REMOTE_PZ_TRAME_LEN_REQ];
	uint8_t buffer_out[LIB_REMOTE_PZ_TRAME_LEN_RESP];

	msg_req->header.slave_addr   = DRV_BUS_ADDR_SLAVE_PZ;
	msg_req->header.request_type = req_type;

	if ( true == lib_remote_pz_encode ( msg_req, LIB_REMOTE_PZ_TRAME_VERSION_01 ) )
	{
		memcpy ( msg_req, buffer_in, LIB_REMOTE_PZ_TRAME_LEN_REQ );

		lib_remote_pz_exchange ( buffer_in, buffer_out );

		memcpy ( buffer_out, msg_resp, LIB_REMOTE_PZ_TRAME_LEN_RESP );

		result = lib_remote_pz_decode ( msg_resp );
	}

	return result;
}

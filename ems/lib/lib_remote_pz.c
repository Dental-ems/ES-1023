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

#include "lib_remote_pz.h"

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

    drv_bus_init_master ( );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_pz_exchange ( uint8_t* data_buf_req, uint8_t* data_buf_resp )
{
    drv_bus_send_to_slave ( DRV_BUS_ADDR_SLAVE_PZ );

    // Write data on transmitter
    drv_bus_transmit_send ( data_buf_req, LIB_REMOTE_PZ_TRAME_LEN_REQ );

    // Start receiving
    drv_bus_transmit_receive ( data_buf_resp, LIB_REMOTE_PZ_TRAME_LEN_RESP );
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
uint8_t lib_remote_pz_extract_holder_hook ( uint8_t* data )
{
    return lib_remote_af_extract_holder_hook ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_extract_holder_conn ( uint8_t* data )
{
    return lib_remote_af_extract_holder_conn ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_extract_holder_rfid ( uint8_t* data )
{
    return lib_remote_af_extract_holder_rfid ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_extract_rfid_state ( uint8_t* data )
{
    return lib_remote_af_extract_rfid_state ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint32_t lib_remote_pz_extract_hall_voltage ( uint8_t* data )
{
    return lib_remote_af_extract_hall_voltage ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint32_t lib_remote_pz_extract_detect_voltage ( uint8_t* data )
{
    return lib_remote_af_extract_detect_voltage ( data );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_pz_checksum ( uint8_t* trame, uint8_t size )
{
    return lib_remote_af_checksum ( trame, size );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
bool lib_remote_pz_request ( uint8_t req_type, LIB_REMOTE_PZ_LL_REQ* msg_req, LIB_REMOTE_PZ_LL_RESP* msg_resp )
{
    bool    result = false;
    uint8_t buffer_write[LIB_REMOTE_PZ_TRAME_LEN_REQ];
    uint8_t buffer_read [LIB_REMOTE_PZ_TRAME_LEN_RESP];

    msg_req->header.slave_addr   = DRV_BUS_ADDR_SLAVE_PZ;
    msg_req->header.request_type = req_type;

    if ( true == lib_remote_pz_encode ( msg_req, LIB_REMOTE_PZ_TRAME_VERSION_01 ) )
    {
        memcpy ( buffer_write, msg_req, LIB_REMOTE_PZ_TRAME_LEN_REQ );

        lib_remote_pz_exchange ( buffer_write, buffer_read );

        memcpy ( msg_resp, buffer_read, LIB_REMOTE_PZ_TRAME_LEN_RESP );

        result = lib_remote_pz_decode ( msg_resp );
    }

    return result;
}

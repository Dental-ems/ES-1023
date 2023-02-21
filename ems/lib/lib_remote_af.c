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
static LIB_REMOTE_AF_CTX lib_remote_af_ctx;

/******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_af_init ( void )
{
    memset ( &lib_remote_af_ctx, 0, sizeof ( LIB_REMOTE_AF_CTX ) );

    drv_bus_init_master ( );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void lib_remote_af_exchange ( uint8_t* data_buf_req, uint8_t* data_buf_resp )
{
    // Start receiving
    drv_bus_receive_run ( data_buf_resp, LIB_REMOTE_AF_TRAME_LEN_RESP );

    // Open transmitter
    drv_bus_transmit_start ();

    // Send to Airflow slave
    drv_bus_send_to_slave ( DRV_BUS_ADDR_SLAVE_AF );

    // Write data on transmitter
    drv_bus_transmit_run ( data_buf_req, LIB_REMOTE_AF_TRAME_LEN_REQ );

    // Waiting while transmitting
    drv_bus_transmit ();

    // Close transmitter
    drv_bus_transmit_end ();

    // Waiting for response
    drv_bus_receive ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool lib_remote_af_encode ( LIB_REMOTE_AF_LL_REQ* msg_to_encode, uint8_t version )
{
    bool result = false;

    if ( msg_to_encode != NULL )
    {
        if ( LIB_REMOTE_AF_TRAME_VERSION_01 == version )
        {
            if ( ( msg_to_encode->header.request_type > LIB_REMOTE_AF_REQ_UNDEF ) && ( msg_to_encode->header.request_type < LIB_REMOTE_AF_REQ_MAX ) )
            {
                msg_to_encode->header.master_addr      = DRV_BUS_ADDR_MASTER;
                msg_to_encode->header.protocol_version = version;

                msg_to_encode->end_of_frame = LIB_REMOTE_AF_TRAME_EOF;
                msg_to_encode->checksum     = lib_remote_af_checksum ( (uint8_t*)msg_to_encode, 4 );
            }

            result = true;
        }
    }

    return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool lib_remote_af_decode ( LIB_REMOTE_AF_LL_RESP* msg_to_decode )
{
    bool result = false;

    if ( msg_to_decode != NULL )
    {
        if ( LIB_REMOTE_AF_TRAME_VERSION_01 == msg_to_decode->header.protocol_version )
        {
            if ( LIB_REMOTE_AF_RESULT_ACK == msg_to_decode->result )
            {
                if ( ( msg_to_decode->header.slave_addr > 0 ) && ( msg_to_decode->header.master_addr > 0 ) )
                {
                    // Switch because of response with same header
                    uint8_t temp = msg_to_decode->header.slave_addr;
                    msg_to_decode->header.slave_addr  = msg_to_decode->header.master_addr;
                    msg_to_decode->header.master_addr = temp;

                    if ( ( msg_to_decode->header.request_type > LIB_REMOTE_AF_REQ_UNDEF ) && ( msg_to_decode->header.request_type < LIB_REMOTE_AF_REQ_MAX ) )
                    {
                        if ( lib_remote_af_checksum ( (uint8_t*)msg_to_decode, 9 ) == msg_to_decode->checksum )
                        {
                            result = true;
                        }
                    }
                }
            }
        }
    }

    return result;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_extract_encoder ( uint8_t* data )
{
    uint32_t value = 0;

    value += data[2] << 16;
    value += data[1] << 8;
    value += data[0];

    return value;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_extract_holder_status ( uint8_t* data )
{
    return data[0];
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_extract_holder_conn ( uint8_t* data )
{
    return data[1] + LIB_REMOTE_AF_HOLDER_CONNECTED_NO ;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_extract_holder_rfid ( uint8_t* data )
{
    return data[2] + LIB_REMOTE_AF_HOLDER_DETECTED_NO;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_extract_rfid_status ( uint8_t* data )
{
    return data[0];
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint32_t lib_remote_af_extract_hall_voltage ( uint8_t* data )
{
    uint32_t value = 0;

    value += data[2] << 16;
    value += data[1] << 8;
    value += data[0];

    return value;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint32_t lib_remote_af_extract_detect_voltage ( uint8_t* data )
{
    uint32_t value = 0;

    value += data[2] << 16;
    value += data[1] << 8;
    value += data[0];

    return value;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
uint8_t lib_remote_af_checksum ( uint8_t* trame, uint8_t size )
{
    uint8_t sum_computed = 0;

    sum_computed = trame[0];

    for ( int i=1 ; i<size ; i++ )
    {
        sum_computed ^= trame[i];
    }

    sum_computed &= LIB_REMOTE_AF_TRAME_CHECKSUM;

    return sum_computed;
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
bool lib_remote_af_request ( uint8_t req_type, LIB_REMOTE_AF_LL_REQ* msg_req, LIB_REMOTE_AF_LL_RESP* msg_resp )
{
    bool    result = false;
    uint8_t buffer_write[LIB_REMOTE_AF_TRAME_LEN_REQ];
    uint8_t buffer_read [LIB_REMOTE_AF_TRAME_LEN_RESP];

    memcpy ( buffer_write, 0, sizeof ( uint8_t ) * LIB_REMOTE_AF_TRAME_LEN_REQ  );
    memcpy (  buffer_read, 0, sizeof ( uint8_t ) * LIB_REMOTE_AF_TRAME_LEN_RESP );

    msg_req->header.slave_addr   = DRV_BUS_ADDR_SLAVE_AF;
    msg_req->header.request_type = req_type;

    if ( true == lib_remote_af_encode ( msg_req, LIB_REMOTE_AF_TRAME_VERSION_01 ) )
    {
        memcpy ( buffer_write, msg_req, LIB_REMOTE_AF_TRAME_LEN_REQ );

        lib_remote_af_exchange ( buffer_write, buffer_read );

        memcpy ( msg_resp, buffer_read, LIB_REMOTE_AF_TRAME_LEN_RESP );

        result = lib_remote_af_decode ( msg_resp );
    }

    return result;
}

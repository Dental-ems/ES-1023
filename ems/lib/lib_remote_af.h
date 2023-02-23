/******************************************************************************
 * @file lib_remote_af.h
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef LIB_REMOTE_AF_H_
#define LIB_REMOTE_AF_H_

#include "../drv/drv_bus.h"

#define LIB_REMOTE_AF_TRAME_LEN_REQ       6U
#define LIB_REMOTE_AF_TRAME_LEN_RESP      12U

#define LIB_REMOTE_AF_TRAME_EOF           0x0D
#define LIB_REMOTE_AF_TRAME_CHECKSUM      0x7F

#define LIB_REMOTE_AF_TRAME_VERSION_01    0x01

enum LIB_REMOTE_AF_REQ
{
    LIB_REMOTE_AF_REQ_UNDEF=0,
    LIB_REMOTE_AF_REQ_HOLDER,
    LIB_REMOTE_AF_REQ_ENCODER,
    LIB_REMOTE_AF_REQ_RFID,
    LIB_REMOTE_AF_REQ_VOLT,
    LIB_REMOTE_AF_REQ_DETECT,
    LIB_REMOTE_AF_REQ_LED_ON,
    LIB_REMOTE_AF_REQ_LED_OFF,
    LIB_REMOTE_AF_REQ_MAX
};

enum LIB_REMOTE_AF_RESULT
{
    LIB_REMOTE_AF_RESULT_UNDEF=0,
    LIB_REMOTE_AF_RESULT_ACK=0x0A,
    LIB_REMOTE_AF_RESULT_NACK=0x0F,
    LIB_REMOTE_AF_RESULT_MAX
};

enum LIB_REMOTE_AF_HOLDER
{
    LIB_REMOTE_AF_HOLDER_UNDEF=0,
    LIB_REMOTE_AF_HOLDER_HOOKED_PZ,
    LIB_REMOTE_AF_HOLDER_HOOKED_AF,
    LIB_REMOTE_AF_HOLDER_HOOKED_NONE,
    LIB_REMOTE_AF_HOLDER_CONNECTED_NO,
    LIB_REMOTE_AF_HOLDER_CONNECTED_YES,
    LIB_REMOTE_AF_HOLDER_DETECTED_NO,
    LIB_REMOTE_AF_HOLDER_DETECTED_YES,
    LIB_REMOTE_AF_HOLDER_MAX
};

typedef struct
{
    uint8_t slave_addr;
    uint8_t master_addr;
    uint8_t protocol_version;
    uint8_t request_type;
} LIB_REMOTE_AF_LL_HEADER;

typedef struct
{
    LIB_REMOTE_AF_LL_HEADER header;
    uint8_t                 checksum;
    uint8_t                 end_of_frame;
} LIB_REMOTE_AF_LL_REQ;

typedef struct
{
    LIB_REMOTE_AF_LL_HEADER header;
    uint8_t                 result;
    uint8_t                 data[4];
    uint8_t                 reserved;
    uint8_t                 checksum;
    uint8_t                 end_of_frame;
} LIB_REMOTE_AF_LL_RESP;

typedef struct
{
    ;
} LIB_REMOTE_AF_CTX;

void     lib_remote_af_init                   ( void );
void     lib_remote_af_exchange               ( uint8_t* data_buf_req, uint8_t* data_buf_resp );
bool     lib_remote_af_encode                 ( LIB_REMOTE_AF_LL_REQ*  msg_to_encode, uint8_t version );
bool     lib_remote_af_decode                 ( LIB_REMOTE_AF_LL_RESP* msg_to_decode );
uint8_t  lib_remote_af_checksum               ( uint8_t* trame, uint8_t size );
uint8_t  lib_remote_af_extract_encoder        ( uint8_t* data );
uint8_t  lib_remote_af_extract_holder_hook    ( uint8_t* data );
uint8_t  lib_remote_af_extract_holder_conn    ( uint8_t* data );
uint8_t  lib_remote_af_extract_holder_rfid    ( uint8_t* data );
uint8_t  lib_remote_af_extract_rfid_state     ( uint8_t* data );
uint32_t lib_remote_af_extract_hall_voltage   ( uint8_t* data );
uint32_t lib_remote_af_extract_detect_voltage ( uint8_t* data );
bool     lib_remote_af_request                ( uint8_t req_type, LIB_REMOTE_AF_LL_REQ* msg_req, LIB_REMOTE_AF_LL_RESP* msg_resp );

#endif /* LIB_REMOTE_AF_H_ */

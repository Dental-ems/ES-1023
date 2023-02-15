/******************************************************************************
 * @file lib_remote_pz.h
 *
 *  Created on: 14 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef LIB_REMOTE_PZ_H_
#define LIB_REMOTE_PZ_H_

#include "../drv/drv_bus.h"
#include "lib_remote_af.h"

#define LIB_REMOTE_PZ_TRAME_LEN_RESP   LIB_REMOTE_AF_TRAME_LEN_REQ
#define LIB_REMOTE_PZ_TRAME_LEN_REQ    LIB_REMOTE_AF_TRAME_LEN_RESP
#define LIB_REMOTE_PZ_TRAME_EOF        LIB_REMOTE_PZ_TRAME_EOF
#define LIB_REMOTE_PZ_TRAME_CHECKSUM   LIB_REMOTE_AF_TRAME_CHECKSUM
#define LIB_REMOTE_PZ_TRAME_VERSION_01 0x01

enum LIB_REMOTE_PZ_REQ
{
    LIB_REMOTE_PZ_REQ_UNDEF=0,
    LIB_REMOTE_PZ_REQ_HOLDER,
    LIB_REMOTE_PZ_REQ_ENCODER,
    LIB_REMOTE_PZ_REQ_RFID,
    LIB_REMOTE_PZ_REQ_VOLT,
    LIB_REMOTE_PZ_REQ_DETECT,
    LIB_REMOTE_PZ_REQ_LED_ON,
    LIB_REMOTE_PZ_REQ_LED_OFF,
    LIB_REMOTE_PZ_REQ_MAX
};

typedef LIB_REMOTE_AF_LL_HEADER LIB_REMOTE_PZ_LL_HEADER;
typedef LIB_REMOTE_AF_LL_REQ    LIB_REMOTE_PZ_LL_REQ;
typedef LIB_REMOTE_AF_LL_RESP   LIB_REMOTE_PZ_LL_RESP;
typedef LIB_REMOTE_AF_CTX       LIB_REMOTE_PZ_CTX;

void    lib_remote_pz_init            ( void );
void    lib_remote_pz_exchange        ( uint8_t* data_buf_req, uint8_t* data_buf_resp );
void    lib_remote_pz_transmit        ( void );
void    lib_remote_pz_receive         ( void );
bool    lib_remote_pz_encode          ( LIB_REMOTE_PZ_LL_REQ*  msg_to_encode, uint8_t version );
bool    lib_remote_pz_decode          ( LIB_REMOTE_PZ_LL_RESP* msg_to_decode );
uint8_t lib_remote_pz_checksum        ( LIB_REMOTE_PZ_LL_HEADER header_to_checksum );
uint8_t lib_remote_pz_extract_encoder ( uint8_t* data );
void    lib_remote_pz_callback        ( USART_Type *base, usart_handle_t *handle, status_t status, void *user_data );
bool    lib_remote_pz_request         ( uint8_t req_type, LIB_REMOTE_PZ_LL_REQ* msg_req, LIB_REMOTE_PZ_LL_RESP* msg_resp );

#endif /* LIB_REMOTE_PZ_H_ */

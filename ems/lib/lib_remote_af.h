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

#define LIB_REMOTE_AF_TRAME_EOF 	      0x0D
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
	uint8_t checksum;
	uint8_t end_of_frame;
} LIB_REMOTE_AF_LL_REQ;

typedef struct
{
	LIB_REMOTE_AF_LL_HEADER header;
	uint8_t ack_nack;
	uint8_t reserved;
	uint8_t data[4];
	uint8_t checksum;
	uint8_t end_of_frame;
} LIB_REMOTE_AF_LL_RESP;

typedef struct
{
	uint32_t encoder;
} LIB_REMOTE_AF_ITEM;

typedef struct
{
	bool 	flag_transmit;
	bool 	flag_receive;
} LIB_REMOTE_AF_CTX;

void lib_remote_af_init 	          ( void );
void lib_remote_af_exchange           ( uint8_t* data_buf_req, uint8_t* data_buf_resp );
void lib_remote_af_transmit           ( void );
void lib_remote_af_receive 	          ( void );
bool lib_remote_af_encode 	          ( LIB_REMOTE_AF_LL_REQ*  msg_to_encode, uint8_t version );
bool lib_remote_af_decode 	          ( LIB_REMOTE_AF_LL_RESP* msg_to_decode );
uint8_t lib_remote_af_checksum        ( LIB_REMOTE_AF_LL_HEADER header_to_checksum );
uint8_t lib_remote_af_extract_encoder ( uint8_t* data );

void lib_remote_af_callback  ( USART_Type *base, usart_handle_t *handle, status_t status, void *userData );

#endif /* LIB_REMOTE_AF_H_ */

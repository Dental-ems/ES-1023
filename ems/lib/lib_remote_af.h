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

#define LIB_REMOTE_AF_TRAME_LEN_BYTES   6U

typedef struct
{
	bool 	flag_transmit;
	bool 	flag_receive;
} LIB_REMOTE_AF;

void lib_remote_af_init 	 ( void );

void lib_remote_af_exchange  ( uint8_t* data_buf_req, uint8_t* data_buf_resp );
void lib_remote_af_transmit  ( void );
void lib_remote_af_receive 	 ( void );
void lib_remote_af_encode    ( void );
void lib_remote_af_decode    ( void );

void lib_remote_af_callback  ( USART_Type *base, usart_handle_t *handle, status_t status, void *userData );

#endif /* LIB_REMOTE_AF_H_ */

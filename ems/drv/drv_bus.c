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

#include "pin_mux.h"
#include "fsl_gpio.h"

#include "drv_bus.h"

/******************************************************************************
 * @brief
 *****************************************************************************/
static bool drv_bus_is_ready = false;
static bool flag_send        = false;
static bool flag_receive     = false;
static usart_handle_t drv_bus_handle;

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_init_master ( void )
{
    if ( false == drv_bus_is_ready )
    {
        usart_config_t    satellite_uart_config;
        gpio_pin_config_t sat_de_config = {
            kGPIO_DigitalOutput,
            0,
        };

        // Initialize DE pin for uart transmitting
        GPIO_PinInit ( GPIO, BOARD_INITPINS_UART_SAT_DE_PORT, BOARD_INITPINS_UART_SAT_DE_PIN, &sat_de_config );

        // Initialize uart instance configuration
        USART_GetDefaultConfig ( &satellite_uart_config );
        satellite_uart_config.baudRate_Bps = 115200;
        satellite_uart_config.enableTx     = true;
        satellite_uart_config.enableRx     = true;

        USART_Init ( DRV_BUS_UART_SAT_ID, &satellite_uart_config, CLOCK_GetFlexCommClkFreq(7U) );

        //USART_Enable9bitMode ( DRV_BUS_UART_SAT_ID, true );

        // Configure Master address
        USART_SetMatchAddress ( DRV_BUS_UART_SAT_ID, DRV_BUS_ADDR_MASTER );

        // Enable match address
        USART_EnableMatchAddress ( DRV_BUS_UART_SAT_ID, true );

        // Create usart handle
        USART_TransferCreateHandle ( DRV_BUS_UART_SAT_ID, &drv_bus_handle, drv_bus_transmit_callback, NULL );

        drv_bus_is_ready = true;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_transmit_start ( void )
{
    GPIO_PinWrite ( GPIO, BOARD_INITPINS_UART_SAT_DE_PORT, BOARD_INITPINS_UART_SAT_DE_PIN, 1 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_transmit_end ( void )
{
    GPIO_PinWrite ( GPIO, BOARD_INITPINS_UART_SAT_DE_PORT, BOARD_INITPINS_UART_SAT_DE_PIN, 0 );
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_transmit_send ( uint8_t* buffer, size_t lenght )
{
    // Open transmit window
    drv_bus_transmit_start ();

    usart_transfer_t drv_bus_sender;

    drv_bus_sender.data     = buffer;
    drv_bus_sender.dataSize = lenght;

    USART_TransferSendNonBlocking ( DRV_BUS_UART_SAT_ID, &drv_bus_handle, &drv_bus_sender );

    drv_bus_send_run ();

    // Close transmit window
    drv_bus_transmit_end ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_transmit_receive ( uint8_t* buffer, size_t lenght )
{
    usart_transfer_t drv_bus_receiver;

    drv_bus_receiver.data     = buffer;
    drv_bus_receiver.dataSize = lenght;

    USART_TransferReceiveNonBlocking ( DRV_BUS_UART_SAT_ID, &drv_bus_handle, &drv_bus_receiver, NULL );

    drv_bus_receive_run ();
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_send_to_slave ( uint16_t slave_addr )
{
    USART_SendAddress ( DRV_BUS_UART_SAT_ID, slave_addr );
}

/*******************************************************************************
 * @brief
 ******************************************************************************/
void drv_bus_transmit_callback ( USART_Type *base, usart_handle_t *handle, status_t status, void *user_data )
{
    if ( kStatus_USART_TxIdle == status )
    {
        flag_send = true;
    }

    if ( kStatus_USART_RxIdle == status )
    {
        flag_receive = true;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_send_run ( void )
{
    // TODO : remove blocking procedure
    // tip : use freertos peripheral option

    while ( flag_send == false ) { }

    flag_send = false;
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void drv_bus_receive_run ( void )
{
    // TODO : remove blocking procedure
    // tip : use freertos peripheral option

    while ( flag_receive == false ) { }

    flag_receive = false;
}

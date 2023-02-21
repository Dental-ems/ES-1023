/******************************************************************************
 * @file app_satellite.c
 *
 * @brief
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_satellite.h"

#warning TO BE MOVE TO OTHER MODULE
#include "../lib/lib_alim.h"

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
static APP_SATELLITE_CTX app_satellite_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_satellite_heartbeat ( void *pvParameters )
{
    lib_alim_enable ();

    vTaskDelay ( APP_SATELLITE_PERIOD_MS );

    app_satellite_cmd_light_pz_on_off ( false );

    while (1)
    {
        vTaskDelay ( APP_SATELLITE_PERIOD_MS );

        // Airflow management
        app_satellite_update_encoder_af ();

        if ( false == app_satellite_ctx.airflow.encoder.has_ref )
        {
            app_satellite_offset_af ();
        }
        else
        {
            app_satellite_compute_water_setting_af ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_update_holder_af ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_rfid_af ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_hall_af ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_detect_af ();
        }

        vTaskDelay ( APP_SATELLITE_PERIOD_MS );

        // Piezon management
        app_satellite_update_encoder_pz ();

        if ( false == app_satellite_ctx.piezon.encoder.has_ref )
        {
            app_satellite_offset_pz ();
        }
        else
        {
            app_satellite_compute_water_setting_pz ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_update_holder_pz ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_cmd_light_pz_on_off ( true );

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_rfid_pz ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_hall_pz ();

            vTaskDelay ( APP_SATELLITE_PERIOD_MS );

            app_satellite_detect_pz ();
        }

        vTaskDelay ( APP_SATELLITE_PERIOD_MS );
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_update_encoder_af ( void )
{
    LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
    LIB_REMOTE_AF_LL_RESP msg_from_airflow;

    if ( true == lib_remote_af_request ( LIB_REMOTE_AF_REQ_ENCODER, &msg_to_airflow, &msg_from_airflow ) )
    {
        app_satellite_ctx.airflow.encoder.raw = lib_remote_af_extract_encoder ( msg_from_airflow.data );

        app_satellite_ctx.airflow.encoder.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_update_holder_af ( void )
{
    LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
    LIB_REMOTE_AF_LL_RESP msg_from_airflow;

    if ( true == lib_remote_af_request ( LIB_REMOTE_AF_REQ_HOLDER, &msg_to_airflow, &msg_from_airflow ) )
    {
        app_satellite_ctx.airflow.holder.status = lib_remote_af_extract_holder_status ( msg_from_airflow.data );
        app_satellite_ctx.airflow.holder.conn   = lib_remote_af_extract_holder_conn   ( msg_from_airflow.data );
        app_satellite_ctx.airflow.holder.rfid   = lib_remote_af_extract_holder_rfid   ( msg_from_airflow.data );

        if ( app_satellite_ctx.airflow.holder.status > LIB_REMOTE_AF_HOLDER_UNDEF )
        {
            app_satellite_ctx.airflow.holder.cpt++;
        }
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_update_encoder_pz ( void )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_ENCODER, &msg_to_piezon, &msg_from_piezon ) )
    {
        app_satellite_ctx.piezon.encoder.raw = lib_remote_pz_extract_encoder ( msg_from_piezon.data );

        app_satellite_ctx.piezon.encoder.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_update_holder_pz ( void )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_HOLDER, &msg_to_piezon, &msg_from_piezon ) )
    {
        app_satellite_ctx.piezon.holder.status = lib_remote_pz_extract_holder_status ( msg_from_piezon.data );
        app_satellite_ctx.piezon.holder.conn   = lib_remote_pz_extract_holder_conn   ( msg_from_piezon.data );
        app_satellite_ctx.piezon.holder.rfid   = lib_remote_pz_extract_holder_rfid   ( msg_from_piezon.data );

        if ( app_satellite_ctx.piezon.holder.status > LIB_REMOTE_AF_HOLDER_UNDEF )
        {
            app_satellite_ctx.piezon.holder.cpt++;
        }
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_cmd_light_pz_on_off ( bool on_off )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == on_off )
    {
        lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_LED_ON, &msg_to_piezon, &msg_from_piezon );
    }
    else
    {
        lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_LED_OFF, &msg_to_piezon, &msg_from_piezon );
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_rfid_af ( void )
{
    LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
    LIB_REMOTE_AF_LL_RESP msg_from_airflow;

    if ( true == lib_remote_af_request ( LIB_REMOTE_AF_REQ_RFID, &msg_to_airflow, &msg_from_airflow ) )
    {
        app_satellite_ctx.airflow.rfid.status = lib_remote_af_extract_rfid_status ( msg_from_airflow.data );

        app_satellite_ctx.airflow.rfid.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_hall_af ( void )
{
    LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
    LIB_REMOTE_AF_LL_RESP msg_from_airflow;

    if ( true == lib_remote_af_request ( LIB_REMOTE_AF_REQ_VOLT, &msg_to_airflow, &msg_from_airflow ) )
    {
        app_satellite_ctx.airflow.hall.voltage = lib_remote_af_extract_hall_voltage ( msg_from_airflow.data );

        app_satellite_ctx.airflow.hall.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_detect_af ( void )
{
    LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
    LIB_REMOTE_AF_LL_RESP msg_from_airflow;

    if ( true == lib_remote_af_request ( LIB_REMOTE_AF_REQ_DETECT, &msg_to_airflow, &msg_from_airflow ) )
    {
        app_satellite_ctx.airflow.detect.voltage = lib_remote_af_extract_detect_voltage ( msg_from_airflow.data );

        app_satellite_ctx.airflow.detect.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_rfid_pz ( void )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_RFID, &msg_to_piezon, &msg_from_piezon ) )
    {
        app_satellite_ctx.piezon.rfid.status = lib_remote_af_extract_rfid_status ( msg_from_piezon.data );

        app_satellite_ctx.piezon.rfid.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_hall_pz ( void )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_VOLT, &msg_to_piezon, &msg_from_piezon ) )
    {
        app_satellite_ctx.piezon.hall.voltage = lib_remote_af_extract_hall_voltage ( msg_from_piezon.data );

        app_satellite_ctx.piezon.hall.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_detect_pz ( void )
{
    LIB_REMOTE_PZ_LL_REQ  msg_to_piezon;
    LIB_REMOTE_PZ_LL_RESP msg_from_piezon;

    if ( true == lib_remote_pz_request ( LIB_REMOTE_PZ_REQ_DETECT, &msg_to_piezon, &msg_from_piezon ) )
    {
        app_satellite_ctx.piezon.detect.voltage = lib_remote_af_extract_detect_voltage ( msg_from_piezon.data );

        app_satellite_ctx.piezon.detect.cpt++;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_compute_water_setting_af ( void )
{
    int16_t value_normalized = 0;

    value_normalized = (int16_t)( app_satellite_ctx.airflow.encoder.raw - app_satellite_ctx.airflow.encoder.offset );
    value_normalized *= APP_SATELLITE_STEP_CW_COEF;
    value_normalized += APP_SATELLITE_STEP_DIM;
    value_normalized &= 0xFF;

    app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_UNDEF;

    if ( value_normalized >= app_satellite_ctx.airflow.encoder.step3 )
    {
        app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_03;
    }

    if ( value_normalized >= app_satellite_ctx.airflow.encoder.step2 )
    {
        app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_02;
    }

    if ( value_normalized >= app_satellite_ctx.airflow.encoder.step1 )
    {
        app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_01;
    }

    if ( value_normalized > app_satellite_ctx.airflow.encoder.over )
    {
        app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_OVER;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_offset_af ( void )
{
    if ( app_satellite_ctx.airflow.encoder.cpt > APP_SATELLITE_REF_NB_READ )
    {
        if ( ( app_satellite_ctx.airflow.encoder.raw >= APP_SATELLITE_STEP_REF_MIN ) && ( app_satellite_ctx.airflow.encoder.raw <= APP_SATELLITE_STEP_REF_MAX ) )
        {
            // Save offset from raw value
            app_satellite_ctx.airflow.encoder.offset = app_satellite_ctx.airflow.encoder.raw;

            // Compute steps references
            app_satellite_ctx.airflow.encoder.step3 = 0 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.airflow.encoder.step2 = 1 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.airflow.encoder.step1 = 2 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.airflow.encoder.over  = 3 * APP_SATELLITE_STEP_DELTA;

            // Done
            app_satellite_ctx.airflow.encoder.has_ref = true;
        }
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_compute_water_setting_pz ( void )
{
    int16_t value_normalized = 0;

    value_normalized = (int16_t)( app_satellite_ctx.piezon.encoder.raw - app_satellite_ctx.piezon.encoder.offset );
    value_normalized *= APP_SATELLITE_STEP_CW_ANTI;
    value_normalized += APP_SATELLITE_STEP_DIM;
    value_normalized &= 0xFF;

    app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_UNDEF;

    if ( value_normalized >= app_satellite_ctx.piezon.encoder.step3 )
    {
        app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_03;
    }

    if ( value_normalized >= app_satellite_ctx.piezon.encoder.step2 )
    {
        app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_02;
    }

    if ( value_normalized >= app_satellite_ctx.piezon.encoder.step1 )
    {
        app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_01;
    }

    if ( value_normalized > app_satellite_ctx.piezon.encoder.over )
    {
        app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_OVER;
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_offset_pz ( void )
{
    if ( app_satellite_ctx.piezon.encoder.cpt > APP_SATELLITE_REF_NB_READ )
    {
        if ( ( app_satellite_ctx.piezon.encoder.raw >= APP_SATELLITE_STEP_REF_MIN ) && ( app_satellite_ctx.piezon.encoder.raw <= APP_SATELLITE_STEP_REF_MAX ) )
        {
            // Save offset from raw value
            app_satellite_ctx.piezon.encoder.offset = app_satellite_ctx.piezon.encoder.raw;

            // Compute steps references
            app_satellite_ctx.piezon.encoder.step3 = 0 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.piezon.encoder.step2 = 1 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.piezon.encoder.step1 = 2 * APP_SATELLITE_STEP_DELTA;
            app_satellite_ctx.piezon.encoder.over  = 3 * APP_SATELLITE_STEP_DELTA;

            // Done
            app_satellite_ctx.piezon.encoder.has_ref = true;
        }
    }
}

/******************************************************************************
 * @brief
 *****************************************************************************/
bool app_satellite_init ( QueueHandle_t* app_satellite_handle )
{
    bool result = false;

    memset ( &app_satellite_ctx, 0, sizeof ( APP_SATELLITE_CTX ) );

    result = misc_task_create ( app_satellite_heartbeat, "satellite_module", APP_SATELLITE_PRIORITY );
    if ( result != false )
    {
        app_satellite_ctx.handle = misc_queue_create ();

        *app_satellite_handle = app_satellite_ctx.handle;

        lib_remote_af_init ();

        lib_remote_pz_init ();

        lib_alim_init ();
    }

    return result;
}

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

/*******************************************************************************
 * @brief Variables
 ******************************************************************************/
static APP_SATELLITE_CTX app_satellite_ctx;

/******************************************************************************
 * @brief
 *****************************************************************************/
static void app_satellite_heartbeat ( void *pvParameters )
{
	while (1)
	{
		app_satellite_update_encoder_af ();

		if ( false == app_satellite_ctx.airflow.encoder.has_ref )
		{
			app_satellite_take_step_reference ();
		}
		else
		{
			app_satellite_compute_water_setting ();
		}

		vTaskDelay ( APP_SATELLITE_PERIOD_MS );
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_update_encoder_af ( void )
{
	uint8_t satelline_buffer_in[LIB_REMOTE_AF_TRAME_LEN_REQ];
	uint8_t satelline_buffer_out[LIB_REMOTE_AF_TRAME_LEN_RESP];
	LIB_REMOTE_AF_LL_REQ  msg_to_airflow;
	LIB_REMOTE_AF_LL_RESP msg_from_airflow;

	// Message to get encoder from airflow
	msg_to_airflow.header.slave_addr   = DRV_BUS_ADDR_SLAVE_AF;
	msg_to_airflow.header.request_type = LIB_REMOTE_AF_REQ_ENCODER;

	if ( true == lib_remote_af_encode ( &msg_to_airflow, LIB_REMOTE_AF_TRAME_VERSION_01 ) )
	{
		memcpy ( &msg_to_airflow, satelline_buffer_in, LIB_REMOTE_AF_TRAME_LEN_REQ );

		lib_remote_af_exchange ( satelline_buffer_in, satelline_buffer_out );

		memcpy ( satelline_buffer_out, &msg_from_airflow, LIB_REMOTE_AF_TRAME_LEN_RESP );

		if ( true == lib_remote_af_decode ( &msg_from_airflow ) )
		{
			app_satellite_ctx.airflow.encoder.raw = lib_remote_af_extract_encoder ( msg_from_airflow.data );

			if ( app_satellite_ctx.airflow.encoder.raw > APP_SATELLITE_STEP_TOL )
			{
				app_satellite_ctx.airflow.encoder.cpt++;
			}
		}
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_compute_water_setting ( void )
{
	app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_UNDEF;

	if ( app_satellite_ctx.airflow.encoder.raw <= app_satellite_ctx.airflow.encoder.low )
	{
		app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_LOW;
	}

	if ( app_satellite_ctx.airflow.encoder.raw >= ( app_satellite_ctx.airflow.encoder.step1 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_01;
	}

	if ( app_satellite_ctx.airflow.encoder.raw >= ( app_satellite_ctx.airflow.encoder.step2 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_02;
	}

	if ( app_satellite_ctx.airflow.encoder.raw >= ( app_satellite_ctx.airflow.encoder.step3 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_03;
	}

	if ( app_satellite_ctx.airflow.encoder.raw >= app_satellite_ctx.airflow.encoder.high )
	{
		app_satellite_ctx.airflow.water_in_use = APP_SATELLITE_WATER_SETTING_HIGH;
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_take_step_reference ( void )
{
	if ( app_satellite_ctx.airflow.encoder.cpt > APP_SATELLITE_NB_READ_FOR_REF )
	{
		if ( ( app_satellite_ctx.airflow.encoder.raw > APP_SATELLITE_STEP_REF_MIN ) && ( app_satellite_ctx.airflow.encoder.raw < APP_SATELLITE_STEP_REF_MAX ) )
		{
			// Compute step references for water settings
			app_satellite_ctx.airflow.encoder.step3 = app_satellite_ctx.airflow.encoder.raw;
			app_satellite_ctx.airflow.encoder.step2 = app_satellite_ctx.airflow.encoder.step3 - APP_SATELLITE_STEP_DELTA;
			app_satellite_ctx.airflow.encoder.step1 = app_satellite_ctx.airflow.encoder.step2 - APP_SATELLITE_STEP_DELTA;

			// Compute limit lower and higher
			app_satellite_ctx.airflow.encoder.low  = app_satellite_ctx.airflow.encoder.step1 - APP_SATELLITE_STEP_TOL;
			app_satellite_ctx.airflow.encoder.high = app_satellite_ctx.airflow.encoder.step3 + APP_SATELLITE_STEP_TOL;

			app_satellite_ctx.airflow.encoder.has_ref = true;
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
	}

	return result;
}

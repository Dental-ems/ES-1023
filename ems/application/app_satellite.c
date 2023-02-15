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
		// Airflow management
		app_satellite_update_encoder_af ();

		if ( false == app_satellite_ctx.airflow.encoder.has_ref )
		{
			app_satellite_take_step_reference_af ();
		}
		else
		{
			app_satellite_compute_water_setting_af ();
		}

		// Piezon management
		app_satellite_update_encoder_pz ();

		if ( false == app_satellite_ctx.piezon.encoder.has_ref )
		{
			app_satellite_take_step_reference_pz ();
		}
		else
		{
			app_satellite_compute_water_setting_pz ();
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

		if ( app_satellite_ctx.airflow.encoder.raw > APP_SATELLITE_STEP_TOL )
		{
			app_satellite_ctx.airflow.encoder.cpt++;
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

		if ( app_satellite_ctx.piezon.encoder.raw > APP_SATELLITE_STEP_TOL )
		{
			app_satellite_ctx.piezon.encoder.cpt++;
		}
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_compute_water_setting_af ( void )
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
void app_satellite_compute_water_setting_pz ( void )
{
	app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_UNDEF;

	if ( app_satellite_ctx.piezon.encoder.raw <= app_satellite_ctx.piezon.encoder.low )
	{
		app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_LOW;
	}

	if ( app_satellite_ctx.piezon.encoder.raw >= ( app_satellite_ctx.piezon.encoder.step1 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_01;
	}

	if ( app_satellite_ctx.piezon.encoder.raw >= ( app_satellite_ctx.piezon.encoder.step2 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_02;
	}

	if ( app_satellite_ctx.piezon.encoder.raw >= ( app_satellite_ctx.piezon.encoder.step3 - APP_SATELLITE_STEP_TOL ) )
	{
		app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_03;
	}

	if ( app_satellite_ctx.piezon.encoder.raw >= app_satellite_ctx.piezon.encoder.high )
	{
		app_satellite_ctx.piezon.water_in_use = APP_SATELLITE_WATER_SETTING_HIGH;
	}
}

/******************************************************************************
 * @brief
 *****************************************************************************/
void app_satellite_take_step_reference_af ( void )
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
void app_satellite_take_step_reference_pz ( void )
{
	if ( app_satellite_ctx.piezon.encoder.cpt > APP_SATELLITE_NB_READ_FOR_REF )
	{
		if ( ( app_satellite_ctx.piezon.encoder.raw > APP_SATELLITE_STEP_REF_MIN ) && ( app_satellite_ctx.piezon.encoder.raw < APP_SATELLITE_STEP_REF_MAX ) )
		{
			// Compute step references for water settings
			app_satellite_ctx.piezon.encoder.step3 = app_satellite_ctx.piezon.encoder.raw;
			app_satellite_ctx.piezon.encoder.step2 = app_satellite_ctx.piezon.encoder.step3 - APP_SATELLITE_STEP_DELTA;
			app_satellite_ctx.piezon.encoder.step1 = app_satellite_ctx.piezon.encoder.step2 - APP_SATELLITE_STEP_DELTA;

			// Compute limit lower and higher
			app_satellite_ctx.piezon.encoder.low  = app_satellite_ctx.piezon.encoder.step1 - APP_SATELLITE_STEP_TOL;
			app_satellite_ctx.piezon.encoder.high = app_satellite_ctx.piezon.encoder.step3 + APP_SATELLITE_STEP_TOL;

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
	}

	return result;
}

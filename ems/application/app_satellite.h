/******************************************************************************
 * @file app_satellite.h
 *
 *  Created on: 13 february 2023
 *      Author: dcardinale
 *
 *****************************************************************************/

#ifndef APP_SATELLITE_H_
#define APP_SATELLITE_H_

#include "../lib/lib_remote_af.h"
#include "../lib/lib_remote_pz.h"

#include "misc.h"

#define APP_SATELLITE_PRIORITY          3U
#define APP_SATELLITE_PERIOD_MS         (5*1000U)
#define APP_SATELLITE_STEP_DELTA        5555U   // must be changed
#define APP_SATELLITE_STEP_TOL          44U     // must be changed
#define APP_SATELLITE_STEP_REF_MIN      333U    // must be changed
#define APP_SATELLITE_STEP_REF_MAX      99999U  // must be changed
#define APP_SATELLITE_NB_READ_FOR_REF   3U

enum APP_SATELLITE_WATER_SETTING
{
    APP_SATELLITE_WATER_SETTING_UNDEF=0,
    APP_SATELLITE_WATER_SETTING_01,
    APP_SATELLITE_WATER_SETTING_02,
    APP_SATELLITE_WATER_SETTING_03,
    APP_SATELLITE_WATER_SETTING_LOW,
    APP_SATELLITE_WATER_SETTING_HIGH,
    APP_SATELLITE_WATER_SETTING_MAX
};

typedef struct
{
    uint32_t raw;
    uint32_t cpt;
    uint8_t  has_ref;
    uint8_t  step1;
    uint8_t  step2;
    uint8_t  step3;
    uint8_t  low;
    uint8_t  high;
} APP_SATELLITE_ENCODER;

typedef struct
{
    APP_SATELLITE_ENCODER encoder;
    uint8_t               water_in_use;
} APP_SATELLITE_ITEM;

typedef struct
{
    QueueHandle_t      handle;
    APP_SATELLITE_ITEM airflow;
    APP_SATELLITE_ITEM piezon;
} APP_SATELLITE_CTX;

bool app_satellite_init                     ( QueueHandle_t* app_satellite_handle );
void app_satellite_update_encoder_af        ( void );
void app_satellite_update_encoder_pz        ( void );
void app_satellite_take_step_reference_af   ( void );
void app_satellite_take_step_reference_pz   ( void );
void app_satellite_compute_water_setting_af ( void );
void app_satellite_compute_water_setting_pz ( void );

#endif /* APP_SATELLITE_H_ */

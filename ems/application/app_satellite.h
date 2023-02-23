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

#define APP_SATELLITE_PERIOD_MS         (200U)

#define APP_SATELLITE_STEP_DELTA        80U
#define APP_SATELLITE_STEP_REF_MIN      10U
#define APP_SATELLITE_STEP_REF_MAX      250U
#define APP_SATELLITE_STEP_DIM          256U
#define APP_SATELLITE_STEP_CW_COEF      (-1)
#define APP_SATELLITE_STEP_CW_ANTI      (1)

#define APP_SATELLITE_REF_NB_READ      3U

enum APP_SATELLITE_WATER_SETTING
{
    APP_SATELLITE_WATER_SETTING_UNDEF=0,
    APP_SATELLITE_WATER_SETTING_01,
    APP_SATELLITE_WATER_SETTING_02,
    APP_SATELLITE_WATER_SETTING_03,
    APP_SATELLITE_WATER_SETTING_OVER,
    APP_SATELLITE_WATER_SETTING_MAX
};

typedef struct
{
    uint8_t  raw;
    uint8_t  offset;
    uint32_t cpt;
    uint8_t  has_ref;
    uint8_t  step1;
    uint8_t  step2;
    uint8_t  step3;
    uint8_t  over;
} APP_SATELLITE_ENCODER;

typedef struct
{
    uint32_t cpt;
    uint8_t  hook;
    uint8_t  conn;
    uint8_t  rfid;
} APP_SATELLITE_HOLDER;

typedef struct
{
    uint32_t cpt;
    uint8_t  status;
} APP_SATELLITE_RFID;

typedef struct
{
    uint32_t cpt;
    uint32_t voltage;
} APP_SATELLITE_HALL;

typedef struct
{
    uint32_t cpt;
    uint32_t voltage;
} APP_SATELLITE_DETECT;

typedef struct
{
    APP_SATELLITE_ENCODER encoder;
    APP_SATELLITE_HOLDER  holder;
    APP_SATELLITE_RFID    rfid;
    APP_SATELLITE_HALL    hall;
    APP_SATELLITE_DETECT  detect;
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
void app_satellite_update_holder_af         ( void );
void app_satellite_update_holder_pz         ( void );
void app_satellite_cmd_light_pz_on_off      ( bool on_off );
void app_satellite_rfid_af                  ( void );
void app_satellite_rfid_pz                  ( void );
void app_satellite_hall_af                  ( void );
void app_satellite_hall_pz                  ( void );
void app_satellite_detect_af                ( void );
void app_satellite_detect_pz                ( void );
void app_satellite_offset_af                ( void );
void app_satellite_offset_pz                ( void );
void app_satellite_compute_water_setting_af ( void );
void app_satellite_compute_water_setting_pz ( void );

#endif /* APP_SATELLITE_H_ */

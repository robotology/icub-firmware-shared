/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: Valentina Gaggero, Marco Accame
 * email:   valentina.gaggero@iit.it, marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EOANALOGSENSORS_H_
#define _EOANALOGSENSORS_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoAnalogSensors.h
    @brief      This header file gives
    @author     marco.accame@iit.it
    @date       09/06/2011
**/

/** @defgroup eo_cevcwervcrev5555 Data structure for analog sensors
    Tcecece

    @{
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOarray.h"
#include "EoUtilities.h"
#include "EoMeasures.h"

#include "EoBoards.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types -------------------------------------------------------------------------

// use eoas_sensor2string() and eoas_string2sensor() to convert to / from eOas_sensor_t and associated string 
typedef enum
{
    eoas_strain                 = 0,    // associated string is: "eoas_strain"
    eoas_mais                   = 1,    // etc ... the string is equal to the enum
    eoas_accel_mtb_int          = 2,   
    eoas_accel_mtb_ext          = 3,    
    eoas_gyros_mtb_ext          = 4,    
    eoas_accel_st_lis3x         = 5,    
    eoas_gyros_st_l3g4200d      = 6, 
    eoas_imu_acc                = 7,   
    eoas_imu_mag                = 8,  
    eoas_imu_gyr                = 9,
    eoas_imu_eul                = 10,
    eoas_imu_qua                = 11,
    eoas_imu_lia                = 12,
    eoas_imu_grv                = 13,
    eoas_imu_status             = 14,
    eoas_temperature            = 15,
    eoas_psc_angle              = 16,
    eoas_pos_angle              = 17,
    // add in here eoas_xxxnameetc
    eoas_unknown                = 254,  
    eoas_none                   = 255   
} eOas_sensor_t;

enum { eoas_sensors_numberof = 18 };


/** @typedef    typedef enum eOas_entity_t;
    @brief      It contains all the possible entities in analog sensors.
                See definition of eOas_strain_t, and eOas_mais_t for explanations
 **/
typedef enum
{
    eoas_entity_strain                      = 0,
    eoas_entity_mais                        = 1,
    eoas_entity_temperature                 = 2,    
    eoas_entity_inertial                    = 3,
    eoas_entity_inertial3                   = 4,
    eoas_entity_psc                         = 5,
    eoas_entity_pos                         = 6
} eOas_entity_t;

enum { eoas_entities_numberof = 7 };


// -- all the possible enum


typedef enum
{
    eoas_strainmode_txcalibrateddatacontinuously        = 0,
    eoas_strainmode_acquirebutdonttx                    = 1,
    eoas_strainmode_txuncalibrateddatacontinuously      = 3,
    eoas_strainmode_txalldatacontinuously               = 4
} eOas_strainmode_t;

typedef enum
{
    eoas_strain_formatofvalues_fullscale                = 0,
    eoas_strain_formatofvalues_calibrated               = 1,
    eoas_strain_formatofvalues_uncalibrated             = 2
} eOas_strain_formatofvalues_t;

typedef enum
{
    eoas_maismode_txdatacontinuously                    = 0,
    eoas_maismode_acquirebutdonttx                      = 1,
    eoas_maismode_dontacquiredonttx                     = 5
} eOas_maismode_t;


typedef enum
{
    eoas_maisresolution_08                              = 0,
    eoas_maisresolution_16                              = 1,
    eoas_maisresolution_debug                           = 2       // this mode set mais board in debug mode: it sends triangular and square waves.
} eOas_maisresolution_t;


// -- verification of enum values vs icub can protocol

EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_txdatacontinuously == 0))
EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_acquirebutdonttx == 1))
EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_dontacquiredonttx == 5))


// -- all the possible data service structures

/** @typedef    typedef struct eOas_upto15analog08vals_t
    @brief      eOas_upto15analog08vals_t contains measurements of generic analog sensors with 8 bit resolution, as the
                default behaviour of mais.
                note: a more generic type would be an EOarray. however, 4 bytes are required for the head, thus
                only 12 bytes are available to fit in
 **/
typedef struct                  // size is: 1+1*15+0 = 16
{
    uint8_t                     number;             /**< the number of values */
    uint8_t                     value[15];          /**< the values */
} eOas_upto15analog08vals_t;    //EO_VERIFYsizeof(eOas_upto15analog08vals_t, 16)


/** @typedef    typedef struct eOas_arrayofupto12bytes_t
    @brief      eOas_arrayofupto12bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 12 bytes of the strain
 **/
typedef struct                  // size is: 4+12 = 16
{
    eOarray_head_t              head;               /**< the head */
    uint8_t                     data[12];           /**< the data: upto 12 U8, or 6 U16, or 3 U32 */
} eOas_arrayofupto12bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto12bytes_t, 16)


/** @typedef    typedef struct eOas_arrayofupto20bytes_t
    @brief      eOas_arrayofupto20bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 15 bytes of the mais
 **/
typedef struct                  // size is: 4+20 = 24
{
    eOarray_head_t              head;               /**< the head */
    uint8_t                     data[20];           /**< the data: upto 20 U8, or 10 U16, or 5 U32 */
} eOas_arrayofupto20bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto20bytes_t, 24)


/** @typedef    typedef struct eOas_arrayofupto36bytes_t
    @brief      eOas_arrayofupto36bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 15 halfwords of mais
 **/
typedef struct                  // size is: 4+36+0 = 40
{
    eOarray_head_t              head;               /**< the head */
    uint8_t                     data[36];           /**< the data: upto 36 U8, or 18 U16, or 9 U32 */
} eOas_arrayofupto36bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto36bytes_t, 40)


// -- the definition of a strain entity

/** @typedef    typedef uint8_t  eOas_strainId_t
    @brief      eOas_strainId_t contains the values required to identify a strain sensor board in robot.
 **/
typedef uint8_t  eOas_strainId_t;


typedef struct
{
    eOenum08_t                      mode;                                   /**< use values from eOas_strainmode_t */
    uint8_t                         datarate;                               /**< in ms from 1 up to 210 */
    eObool_t                        signaloncefullscale;                    /**< if eobool_true, then the strains signals its full scale*/
    uint8_t                         filler01[1];
} eOas_strain_config_t;             //EO_VERIFYsizeof(eOas_strain_config_t, 4)


typedef struct
{
    uint8_t                         filler04[4];
} eOas_strain_inputs_t;             //EO_VERIFYsizeof(eOas_strain_inputs_t, 4)


typedef struct                      // size is: 16+16+16+0 = 48
{
    eOas_arrayofupto12bytes_t       fullscale;                              /**< the full scale as an array of three forces and three torques each of 2 bytes */
    eOas_arrayofupto12bytes_t       calibratedvalues;                       /**< the calibrated values as an array of three forces and three torques each of 2 bytes */
    eOas_arrayofupto12bytes_t       uncalibratedvalues;                     /**< the uncalibrated values as an array of three forces and three torques each of 2 bytes */
} eOas_strain_status_t;             //EO_VERIFYsizeof(eOas_strain_status_t, 48)


typedef struct                      // size is: 4+4+48+0 = 56
{
    eOas_strain_config_t            config;
    eOas_strain_inputs_t            inputs;
    eOas_strain_status_t            status;
} eOas_strain_t;                    //EO_VERIFYsizeof(eOas_strain_t, 56)



// -- the definition of a mais entity


/** @typedef    typedef uint8_t  eOas_maisId_t
    @brief      eOas_maisId_t contains the values required to identify a mais sensor board in robot.
 **/
typedef uint8_t  eOas_maisId_t;

typedef struct
{
    eOenum08_t                      mode;                               /**< use values from eOas_maismode_t */
    uint8_t                         datarate;                           /**< in ms from 1 upto 210 */
    eOenum08_t                      resolution;                         /**< use values from eOas_maisresolution_t */
    uint8_t                         filler01[1];
} eOas_mais_config_t;               //EO_VERIFYsizeof(eOas_mais_config_t, 4)


typedef struct
{
    uint8_t                         filler04[4];
} eOas_mais_inputs_t;               //EO_VERIFYsizeof(eOas_mais_inputs_t, 4)


/*
   NOTE: mais board sends 2 can frame: the first contains values of channels between 0 to 6 (7 values) and the second contains values of channel between 7 to 14 (8 vals);
         they are saved the15values.data[].
*/

typedef struct                      // size is: 40+0 = 40
{
    eOas_arrayofupto36bytes_t       the15values;                        /**< the 15 values of the mais, either at 1 byte or 2 bytes resolution. */
} eOas_mais_status_t;               //EO_VERIFYsizeof(eOas_mais_status_t, 40)

typedef struct                      // size is: 4+4+40+0 = 48
{
    eOas_mais_config_t              config;
    eOas_mais_inputs_t              inputs;
    eOas_mais_status_t              status;
} eOas_mais_t;                      //EO_VERIFYsizeof(eOas_mais_t, 48)




// -- the definition of a temperature entity


/** @typedef    typedef uint8_t  eOas_temperatureId_t
    @brief      eOas_temperatureId_t contains the values required to identify a temperature sensor in robot.
 **/

typedef uint8_t  eOas_temperatureId_t;


typedef enum
{
    eoas_temperature_t1                     = eoas_temperature,
    eoas_temperature_unknown                = eoas_unknown,
    eoas_temperature_none                   = eoas_none
} eOas_temperature_type_t;



// this struct describes a single temperature sensor
typedef struct
{
    uint8_t                     typeofboard;    // use eObrd_type_t
    eObrd_location_t            on;             // the location of the board
    uint8_t                     typeofsensor;   // use eOas_temperature_type_t
    uint8_t                     ffu;            // for future use
} eOas_temperature_descriptor_t;  EO_VERIFYsizeof(eOas_temperature_descriptor_t, 4)


// we use this struct to send activate-verify command to the board
enum { eOas_temperature_descriptors_maxnumber = 7 };
typedef struct
{   
    eOarray_head_t                      head;
    eOas_temperature_descriptor_t       data[eOas_temperature_descriptors_maxnumber];
} eOas_temperature_arrayof_descriptors_t; EO_VERIFYsizeof(eOas_temperature_arrayof_descriptors_t, 32)


// we use this struct to send activate-verify command to the board
enum { eOas_temperature_boardinfos_maxnumber = 2 };
typedef struct
{   //6*2=12
    eObrd_info_t                    data[eOas_temperature_boardinfos_maxnumber];
} eOas_temperature_setof_boardinfos_t; EO_VERIFYsizeof(eOas_temperature_setof_boardinfos_t, 12)


// this struct describes the data acquired by a single temperature sensor
typedef struct
{
    uint8_t     id;             // an id which is shared by transmitter and receiver. it can be a unique number or an index inside the array-of-sensors
    uint8_t     typeofsensor;   // use eOas_temperature_type_t
    int16_t     value;          // each unit is 0.1 Celsius (maybe move to 0.01 kelvin ...)
    uint32_t    timestamp;      // the time in msec of reception of this data. it is circular in about an hour 
} eOas_temperature_data_t;      EO_VERIFYsizeof(eOas_temperature_data_t, 8)


// nota di marco.accame: enabled kept at 16 bits so that we can have eOas_temperature_descriptors_maxnumber up to 15.
typedef struct
{
    uint16_t                    datarate;       // it specifies the acquisition rate in seconds with accepted range. 
    uint16_t                    enabled;        // bitmask of enabled sensors with reference to array-of-sensors
} eOas_temperature_config_t;    EO_VERIFYsizeof(eOas_temperature_config_t, 4)

// we use this struct to broadcast the status
enum { eOas_temperature_data_maxnumber = 2 };
typedef struct
{   // 4+(8*2) = 68
    eOarray_head_t                  head;
    eOas_temperature_data_t         data[eOas_temperature_data_maxnumber];
} eOas_temperature_arrayof_data_t;  EO_VERIFYsizeof(eOas_temperature_arrayof_data_t, 20)


typedef struct
{
    eOas_temperature_arrayof_data_t arrayofdata;    // it is the most recent reading of the temperature sensors which are related to this entity
} eOas_temperature_status_t;        EO_VERIFYsizeof(eOas_temperature_status_t, 20)


typedef struct
{
    uint8_t                     enable;         /**< use 0 or 1*/
    uint8_t                     filler[3];
} eOas_temperature_commands_t;  EO_VERIFYsizeof(eOas_temperature_commands_t, 4)


typedef struct                      // size is: 4+20+4 = 28
{
    eOas_temperature_config_t       config;
    eOas_temperature_status_t       status;
    eOas_temperature_commands_t     cmmnds;
} eOas_temperature_t;               EO_VERIFYsizeof(eOas_temperature_t, 28)


// -- the definition of a inertial sensor

typedef enum
{
    eoas_inertial_accel_mtb_int             = eoas_accel_mtb_int,
    eoas_inertial_accel_mtb_ext             = eoas_accel_mtb_ext,
    eoas_inertial_gyros_mtb_ext             = eoas_gyros_mtb_ext,
    eoas_inertial_accel_ems_st_lis3x        = eoas_accel_st_lis3x,
    eoas_inertial_gyros_ems_st_l3g4200d     = eoas_gyros_st_l3g4200d,
    eoas_inertial_unknown                   = eoas_unknown,
    eoas_inertial_none                      = eoas_none
} eOas_inertial_type_t;



// this struct describes a single inertial sensor
typedef struct
{
    eObrd_location_t            on;
    uint8_t                     type; // use relevant entry of eOas_inertial_type_t
} eOas_inertial_descriptor_t;   EO_VERIFYsizeof(eOas_inertial_descriptor_t, 2)


// we use this struct to send activate-verify command to the board
enum { eOas_inertials_maxnumber = 48 };
typedef struct
{
    eOarray_head_t                  head;
    eOas_inertial_descriptor_t      data[eOas_inertials_maxnumber];
} eOas_inertial_arrayof_sensors_t;  EO_VERIFYsizeof(eOas_inertial_arrayof_sensors_t, 100)


// this struct describes the data acquired by a single intertial sensor
typedef struct
{
    uint64_t    timestamp;      // the time in usec of reception of this data
    uint16_t    id;             // an id which is shared by transmitter and receiver. it can be a unique number or an index inside the array-of-sensors
    int16_t     x;              /**< x value */
    int16_t     y;              /**< y value */
    int16_t     z;              /**< z value */
} eOas_inertial_data_t;         EO_VERIFYsizeof(eOas_inertial_data_t, 16)


typedef struct
{
    uint8_t                         datarate;       /**< it specifies the acquisition rate in ms with accepted range [10, 200]. bug: if 250 the mtb emits every 35  */
    uint8_t                         filler[7];
    uint64_t                        enabled;        // bitmask of enabled sensors with reference to array-of-sensors
} eOas_inertial_config_t;           EO_VERIFYsizeof(eOas_inertial_config_t, 16)


typedef struct
{
    eOas_inertial_data_t           data;   /**< it is the most recent reading of the inertial sensors which are related to this entity */
} eOas_inertial_status_t;          EO_VERIFYsizeof(eOas_inertial_status_t, 16)


typedef struct
{
    uint8_t                     enable;         /**< use 0 or 1*/
    uint8_t                     filler[7];
} eOas_inertial_commands_t;     EO_VERIFYsizeof(eOas_inertial_commands_t, 8)


typedef struct                      // size is: 16+16+8 = 40
{
    eOas_inertial_config_t         config;
    eOas_inertial_status_t         status;
    eOas_inertial_commands_t       cmmnds;
} eOas_inertial_t;                 EO_VERIFYsizeof(eOas_inertial_t, 40)


// -- the definition of a inertial3 sensor
// inertial3 sensor contains the new inertial sensors generated by a imu, plus the old ones


typedef enum
{
    // old sensors. i keep them just for future merging of eOas_inertial_type_t into eOas_inertial3_type_t
    eoas_inertial3_accel_mtb_int            = eoas_accel_mtb_int,
    eoas_inertial3_accel_mtb_ext            = eoas_accel_mtb_ext,
    eoas_inertial3_gyros_mtb_ext            = eoas_gyros_mtb_ext,
    eoas_inertial3_accel_ems_st_lis3x       = eoas_accel_st_lis3x,
    eoas_inertial3_gyros_ems_st_l3g4200d    = eoas_gyros_st_l3g4200d,
    // new imu-based sensors
    eoas_inertial3_imu_acc                  = eoas_imu_acc,     
    eoas_inertial3_imu_mag                  = eoas_imu_mag,
    eoas_inertial3_imu_gyr                  = eoas_imu_gyr,
    eoas_inertial3_imu_eul                  = eoas_imu_eul,
    eoas_inertial3_imu_qua                  = eoas_imu_qua,
    eoas_inertial3_imu_lia                  = eoas_imu_lia,
    eoas_inertial3_imu_grv                  = eoas_imu_grv,
    eoas_inertial3_imu_status               = eoas_imu_status,
    
    eoas_inertial3_unknown                  = eoas_unknown,
    eoas_inertial3_none                     = eoas_none
} eOas_inertial3_type_t;



// this struct describes a single inertial3 sensor
typedef struct
{
    uint8_t                     typeofboard;    // use eObrd_type_t
    eObrd_location_t            on;             // the location of the board
    uint8_t                     typeofsensor;   // use eOas_inertial3_type_t
    uint8_t                     ffu;            // for future use
} eOas_inertial3_descriptor_t;  EO_VERIFYsizeof(eOas_inertial3_descriptor_t, 4)


// we use this struct to send activate-verify command to the board
enum { eOas_inertials3_descriptors_maxnumber = 32 };
typedef struct
{   //4+24*4=100
    eOarray_head_t                  head;
    eOas_inertial3_descriptor_t     data[eOas_inertials3_descriptors_maxnumber];
} eOas_inertial3_arrayof_descriptors_t; EO_VERIFYsizeof(eOas_inertial3_arrayof_descriptors_t, 132)


// we use this struct to send activate-verify command to the board
enum { eOas_inertials3_boardinfos_maxnumber = 4 };
typedef struct
{   //6*4=24
    eObrd_info_t                    data[eOas_inertials3_boardinfos_maxnumber];
} eOas_inertial3_setof_boardinfos_t; EO_VERIFYsizeof(eOas_inertial3_setof_boardinfos_t, 24)

typedef struct
{
    uint8_t gyr :2;
    uint8_t acc :2;
    uint8_t mag :2;
    uint8_t dum :2;
}eOas_inertial3_calibStatus;

typedef union
{
    uint8_t                     general;
    eOas_inertial3_calibStatus  calib;
} eOas_inertial3_sensorstatus_t; EO_VERIFYsizeof(eOas_inertial3_sensorstatus_t, 1)

// this struct describes the data acquired by a single intertial sensor
typedef struct
{
    uint8_t     id;             // an id which is shared by transmitter and receiver. it can be a unique number or an index inside the array-of-sensors
    uint8_t     typeofsensor;   // use eOas_inertial3_type_t
    eOas_inertial3_sensorstatus_t     status;
    uint8_t     seq;            // maybe we can uses it to expand the timestamp to up of about 256 hours ...
    uint32_t    timestamp;      // the time in msec of reception of this data. it is circular in about an hour time
    int16_t     w;              /**< w value ... used only by eoas_inertial3_imu_qua */
    int16_t     x;              /**< x value */
    int16_t     y;              /**< y value */
    int16_t     z;              /**< z value */
} eOas_inertial3_data_t;         EO_VERIFYsizeof(eOas_inertial3_data_t, 16)


// enabled is 32 bits hence eOas_inertials3_descriptors_maxnumber <= 32.
typedef struct
{
    uint8_t                         datarate;       /**< it specifies the acquisition rate in ms with accepted range [10, 200]. bug: if 250 the mtb emits every 35  */
    uint8_t                         numberofitemstofillateachtick; // if 0, we fill the entire arrayofdata. else if 1 we fill just arrayofdata[0].
    uint8_t                         filler[2];
    uint32_t                        enabled;        // bitmask of enabled sensors with reference to array-of-sensors
} eOas_inertial3_config_t;          EO_VERIFYsizeof(eOas_inertial3_config_t, 8)


enum { eOas_inertials3_data_maxnumber = 4 };
typedef struct
{   // 4+(16*4) = 68
    eOarray_head_t                  head;
    eOas_inertial3_data_t           data[eOas_inertials3_data_maxnumber];
} eOas_inertial3_arrayof_data_t;    EO_VERIFYsizeof(eOas_inertial3_arrayof_data_t, 68)


typedef struct
{
    eOas_inertial3_arrayof_data_t   arrayofdata;   /**< it is the most recent reading of the inertial sensors which are related to this entity */
} eOas_inertial3_status_t;          EO_VERIFYsizeof(eOas_inertial3_status_t, 68)


typedef struct
{
    uint8_t                     enable;         /**< use 0 or 1*/
    uint8_t                     filler[3];
} eOas_inertial3_commands_t;    EO_VERIFYsizeof(eOas_inertial3_commands_t, 4)


typedef struct                      // size is: 8+68+4 = 80
{
    eOas_inertial3_config_t        config;
    eOas_inertial3_status_t        status;
    eOas_inertial3_commands_t      cmmnds;
} eOas_inertial3_t;                EO_VERIFYsizeof(eOas_inertial3_t, 80)


// -- the definition of a psc sensor service

enum { eOas_psc_boards_maxnumber = 3 };
typedef struct
{
    eObrd_canlocation_t                 canloc[eOas_psc_boards_maxnumber];
} eOas_psc_canLocationsInfo_t;

// a single psc sensor contains an angle expressed in deci-degrees 
// this struct describes the data acquired by a single psc sensor
typedef struct
{
    int16_t     value;              // so far the only value. we use int16 which contains a deci-degree.
} eOas_psc_data_t;                  EO_VERIFYsizeof(eOas_psc_data_t, 2)

typedef struct
{
    uint8_t                         datarate;       /**< it specifies the acquisition rate in ms  */
    uint8_t                         filler[3];
} eOas_psc_config_t;                EO_VERIFYsizeof(eOas_psc_config_t, 4)


enum { eOas_psc_data_maxnumber = 6 };
typedef struct
{   // 4+(2*6) = 16
    eOarray_head_t                  head;
    eOas_psc_data_t                 data[eOas_psc_data_maxnumber];
} eOas_psc_arrayof_data_t;          EO_VERIFYsizeof(eOas_psc_arrayof_data_t, 16)


typedef struct
{
    eOas_psc_arrayof_data_t         arrayofdata;   /**< it is the most recent reading which are related to this entity */
} eOas_psc_status_t;                EO_VERIFYsizeof(eOas_psc_status_t, 16)


typedef struct
{
    uint8_t                         enable;         /**< use 0 or 1*/
    uint8_t                         filler[3];
} eOas_psc_commands_t;              EO_VERIFYsizeof(eOas_psc_commands_t, 4)

typedef struct                      // size is: 4+16+4 = 24
{
    eOas_psc_config_t               config;
    eOas_psc_status_t               status;
    eOas_psc_commands_t             cmmnds;
} eOas_psc_t;                       EO_VERIFYsizeof(eOas_psc_t, 24)


// -- the definition of a pos sensor service

// we use this struct to send activate-verify command to the board
//enum { eOas_pos_boardinfos_maxnumber = 4 };
//typedef struct
//{   //6*4=24
//    eObrd_info_t                    data[eOas_pos_boardinfos_maxnumber];
//} eOas_pos_setof_boardinfos_t; EO_VERIFYsizeof(eOas_pos_setof_boardinfos_t, 24)

//enum { eOas_pos_boards_maxnumber = 3 };
//typedef struct
//{
//    eObrd_canproperties_t           canprop[eOas_pos_boards_maxnumber];
//} eOas_pos_canPropertiesInfo_t;

enum { eOas_pos_boards_maxnumber = 1 };
typedef struct
{
    eObrd_canlocation_t                 canloc[eOas_pos_boards_maxnumber];
} eOas_pos_canLocationsInfo_t;

// a single pos sensor contains an angle expressed in deci-degrees 
// this struct describes the data acquired by a single pos sensor
typedef struct
{
    int16_t     value;              // so far the only value. we use int16 which contains a deci-degree.
} eOas_pos_data_t;                  EO_VERIFYsizeof(eOas_pos_data_t, 2)

typedef struct
{
    uint8_t                         datarate;       /**< it specifies the acquisition rate in ms  */
    uint8_t                         filler[3];
} eOas_pos_config_t;                EO_VERIFYsizeof(eOas_pos_config_t, 4)


enum { eOas_pos_data_maxnumber = 14 };
typedef struct
{   // 4+(2*14) = 32
    eOarray_head_t                  head;
    eOas_pos_data_t                 data[eOas_pos_data_maxnumber];
} eOas_pos_arrayof_data_t;          EO_VERIFYsizeof(eOas_pos_arrayof_data_t, 32)


typedef struct
{
    eOas_pos_arrayof_data_t         arrayofdata;   /**< it is the most recent reading which are related to this entity */
} eOas_pos_status_t;                EO_VERIFYsizeof(eOas_pos_status_t, 32)


typedef struct
{
    uint8_t                         enable;         /**< use 0 or 1*/
    uint8_t                         filler[3];
} eOas_pos_commands_t;              EO_VERIFYsizeof(eOas_pos_commands_t, 4)

typedef struct                      // size is: 4+32+4 = 40
{
    eOas_pos_config_t               config;
    eOas_pos_status_t               status;
    eOas_pos_commands_t             cmmnds;
} eOas_pos_t;                       EO_VERIFYsizeof(eOas_pos_t, 40)

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


extern const char * eoas_sensor2string(eOas_sensor_t sensor);

extern eOas_sensor_t eoas_string2sensor(const char * string);

extern eOresult_t eoas_inertial3_setof_boardinfos_clear(eOas_inertial3_setof_boardinfos_t *set);
extern eOresult_t eoas_inertial3_setof_boardinfos_add(eOas_inertial3_setof_boardinfos_t *set, const eObrd_info_t *brdinfo); 
extern const eObrd_info_t * eoas_inertial3_setof_boardinfos_find(const eOas_inertial3_setof_boardinfos_t *set, eObrd_cantype_t brdtype);

extern uint8_t eoas_inertial3_supportedboards_numberof(void);
extern eObrd_cantype_t eoas_inertial3_supportedboards_gettype(uint8_t pos);

extern icubCanProto_imu_sensor_t eoas_inertial3_imu_to_canproto(eOas_inertial3_type_t t);
extern eOas_inertial3_type_t eoas_inertial3_canproto_to_imu(uint8_t v);

extern eOresult_t eoas_temperature_setof_boardinfos_clear(eOas_temperature_setof_boardinfos_t *set);
extern eOresult_t eoas_temperature_setof_boardinfos_add(eOas_temperature_setof_boardinfos_t *set, const eObrd_info_t *brdinfo); 
extern const eObrd_info_t * eoas_temperature_setof_boardinfos_find(const eOas_temperature_setof_boardinfos_t *set, eObrd_cantype_t brdtype);

extern uint8_t eoas_temperature_supportedboards_numberof(void);
extern eObrd_cantype_t eoas_temperature_supportedboards_gettype(uint8_t pos);


/** @}
    end of group eo_cevcwervcrev5555
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


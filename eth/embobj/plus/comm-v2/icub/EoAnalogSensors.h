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


// - public #define  --------------------------------------------------------------------------------------------------





// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef enum eOas_entity_t;
    @brief      It contains all the possible entities in analog sensors.
                See definition of eOas_strain_t, and eOas_mais_t for explanations
 **/
typedef enum
{
    eoas_entity_strain                      = 0,
    eoas_entity_mais                        = 1,
    eoas_entity_extorque                    = 2,
    eoas_entity_inertial                    = 3
} eOas_entity_t; 

enum { eoas_entities_numberof = 4 };

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

EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_txdatacontinuously == 0));
EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_acquirebutdonttx == 1));
EO_VERIFYproposition(ismaismodeCompatiblewithicubcanproto, (eoas_maismode_dontacquiredonttx == 5));


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
} eOas_upto15analog08vals_t;    //EO_VERIFYsizeof(eOas_upto15analog08vals_t, 16);


/** @typedef    typedef struct eOas_arrayofupto12bytes_t
    @brief      eOas_arrayofupto12bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 12 bytes of the strain
 **/
typedef struct                  // size is: 4+12 = 16 
{
    eOarray_head_t              head;               /**< the head */ 
    uint8_t                     data[12];           /**< the data: upto 12 U8, or 6 U16, or 3 U32 */
} eOas_arrayofupto12bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto12bytes_t, 16);


/** @typedef    typedef struct eOas_arrayofupto20bytes_t
    @brief      eOas_arrayofupto20bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 15 bytes of the mais
 **/
typedef struct                  // size is: 4+20 = 24 
{
    eOarray_head_t              head;               /**< the head */ 
    uint8_t                     data[20];           /**< the data: upto 20 U8, or 10 U16, or 5 U32 */
} eOas_arrayofupto20bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto20bytes_t, 24);


/** @typedef    typedef struct eOas_arrayofupto36bytes_t
    @brief      eOas_arrayofupto36bytes_t contains measurements of generic analog sensors with 8/16/32 bit resolution and is able to contain the 15 halfwords of mais
 **/
typedef struct                  // size is: 4+36+0 = 40 
{
    eOarray_head_t              head;               /**< the head */ 
    uint8_t                     data[36];           /**< the data: upto 36 U8, or 18 U16, or 9 U32 */
} eOas_arrayofupto36bytes_t;    //EO_VERIFYsizeof(eOas_arrayofupto36bytes_t, 40);


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
} eOas_strain_config_t;             //EO_VERIFYsizeof(eOas_strain_config_t, 4);


typedef struct                      
{
    uint8_t                         filler04[4];                               
} eOas_strain_inputs_t;             //EO_VERIFYsizeof(eOas_strain_inputs_t, 4);


typedef struct                      // size is: 16+16+16+0 = 48                     
{
    eOas_arrayofupto12bytes_t       fullscale;                              /**< the full scale as an array of three forces and three torques each of 2 bytes */
    eOas_arrayofupto12bytes_t       calibratedvalues;                       /**< the calibrated values as an array of three forces and three torques each of 2 bytes */
    eOas_arrayofupto12bytes_t       uncalibratedvalues;                     /**< the uncalibrated values as an array of three forces and three torques each of 2 bytes */                  
} eOas_strain_status_t;             //EO_VERIFYsizeof(eOas_strain_status_t, 48);


typedef struct                      // size is: 4+4+48+0 = 56
{
    eOas_strain_config_t            config;
    eOas_strain_inputs_t            inputs;    
    eOas_strain_status_t            status;
} eOas_strain_t;                    //EO_VERIFYsizeof(eOas_strain_t, 56);



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
} eOas_mais_config_t;               //EO_VERIFYsizeof(eOas_mais_config_t, 4);


typedef struct                      
{
    uint8_t                         filler04[4];                               
} eOas_mais_inputs_t;               //EO_VERIFYsizeof(eOas_mais_inputs_t, 4);


/* 
   NOTE: mais board sends 2 can frame: the first contains values of channels between 0 to 6 (7 values) and the second contains values of channel between 7 to 14 (8 vals); 
         they are saved the15values.data[]. 
*/

typedef struct                      // size is: 40+0 = 40                     
{
    eOas_arrayofupto36bytes_t       the15values;                        /**< the 15 values of the mais, either at 1 byte or 2 bytes resolution. */                  
} eOas_mais_status_t;               //EO_VERIFYsizeof(eOas_mais_status_t, 40);

typedef struct                      // size is: 4+4+40+0 = 48
{
    eOas_mais_config_t              config;
    eOas_mais_inputs_t              inputs;    
    eOas_mais_status_t              status;
} eOas_mais_t;                      //EO_VERIFYsizeof(eOas_mais_t, 48);




// -- the definition of a extorque entity


/** @typedef    typedef uint8_t  eOas_extorqueId_t
    @brief      eOas_extorqueId_t contains the values required to identify a extorque sensor board in robot.
 **/
typedef uint8_t  eOas_extorqueId_t;

typedef struct                      
{
    uint8_t                         jointindex;
    uint8_t                         filler03[3];                          
} eOas_extorque_config_t;           //EO_VERIFYsizeof(eOas_extorque_config_t, 4);


typedef struct                      
{
    eOmeas_torque_t                 torquevalue;
    uint8_t                         filler02[2];
} eOas_extorque_inputs_t;           //EO_VERIFYsizeof(eOas_extorque_inputs_t, 4);


typedef struct                      // size is: 4+4+0 = 8
{
    eOas_extorque_config_t          config;
    eOas_extorque_inputs_t          inputs;    
} eOas_extorque_t;                  //EO_VERIFYsizeof(eOas_extorque_t, 8);



// -- the definition of a inertial sensor


typedef enum 
{
    eoas_inertial_type_none          = 0,
    eoas_inertial_type_accelerometer = 1,
    eoas_inertial_type_gyroscope     = 2
} eOas_inertial_type_t;


enum { eoas_inertial_pos_max_numberof = 63 };

enum { eoas_inertial_pos_offsetleft = 0, eoas_inertial_pos_offsetright = 24, eoas_inertial_pos_offsetcentral = 48 };

/** @typedef    typedef enum eOas_inertial_position_t
    @brief      contains a unique id for every possible inertial sensor positioned on iCub. So far we can host
                up to 63 different positions. The actual positions on iCub are documented on http://wiki.icub.org/wiki/Distributed_Inertial_sensing
                where one must look for the tags 10B12, 10B13 etc. The mapping on CAN for the ETH robot v3 is written aside. 
 **/
typedef enum
{
    eoas_inertial_pos_none                  = 0,
    
    // left arm
    eoas_inertial_pos_l_hand                = 1+eoas_inertial_pos_offsetleft,       // label 1B7    canloc = (CAN2, 14) 
    eoas_inertial_pos_l_forearm_1           = 2+eoas_inertial_pos_offsetleft,       // label 1B8    canloc = (CAN2, 12)
    eoas_inertial_pos_l_forearm_2           = 3+eoas_inertial_pos_offsetleft,       // label 1B9    canloc = (CAN2, 13)
    eoas_inertial_pos_l_upper_arm_1         = 4+eoas_inertial_pos_offsetleft,       // label 1B10   canloc = (CAN2,  9)
    eoas_inertial_pos_l_upper_arm_2         = 5+eoas_inertial_pos_offsetleft,       // label 1B11   canloc = (CAN2, 11)
    eoas_inertial_pos_l_upper_arm_3         = 6+eoas_inertial_pos_offsetleft,       // label 1B12   canloc = (CAN2, 10)
    eoas_inertial_pos_l_upper_arm_4         = 7+eoas_inertial_pos_offsetleft,       // label 1B13   canloc = (CAN2,  8)
    // left leg
    eoas_inertial_pos_l_foot_1              = 8+eoas_inertial_pos_offsetleft,       // label 10B12  canloc = (CAN2, 13)
    eoas_inertial_pos_l_foot_2              = 9+eoas_inertial_pos_offsetleft,       // label 10B13  canloc = (CAN2, 12)
    eoas_inertial_pos_l_lower_leg_1         = 10+eoas_inertial_pos_offsetleft,      // label 10B8   canloc = (CAN2,  8)
    eoas_inertial_pos_l_lower_leg_2         = 11+eoas_inertial_pos_offsetleft,      // label 10B9   canloc = (CAN2,  9)  
    eoas_inertial_pos_l_lower_leg_3         = 12+eoas_inertial_pos_offsetleft,      // label 10B10  canloc = (CAN2, 10)
    eoas_inertial_pos_l_lower_leg_4         = 13+eoas_inertial_pos_offsetleft,      // label 10B11  canloc = (CAN2, 11)
    eoas_inertial_pos_l_upper_leg_1         = 14+eoas_inertial_pos_offsetleft,      // label 10B1   canloc = (CAN1,  1)
    eoas_inertial_pos_l_upper_leg_2         = 15+eoas_inertial_pos_offsetleft,      // label 10B2   canloc = (CAN1,  2)
    eoas_inertial_pos_l_upper_leg_3         = 16+eoas_inertial_pos_offsetleft,      // label 10B3   canloc = (CAN1,  3)
    eoas_inertial_pos_l_upper_leg_4         = 17+eoas_inertial_pos_offsetleft,      // label 10B4   canloc = (CAN1,  4)
    eoas_inertial_pos_l_upper_leg_5         = 18+eoas_inertial_pos_offsetleft,      // label 10B5   canloc = (CAN1,  5)
    eoas_inertial_pos_l_upper_leg_6         = 19+eoas_inertial_pos_offsetleft,      // label 10B6   canloc = (CAN1,  6)
    eoas_inertial_pos_l_upper_leg_7         = 20+eoas_inertial_pos_offsetleft,      // label 10B7   canloc = (CAN1,  7)
    
    // right arm
    eoas_inertial_pos_r_hand                = 1+eoas_inertial_pos_offsetright,      // label 2B7    canloc = (CAN2, 14)
    eoas_inertial_pos_r_forearm_1           = 2+eoas_inertial_pos_offsetright,      // label 2B8    canloc = (CAN2, 12)
    eoas_inertial_pos_r_forearm_2           = 3+eoas_inertial_pos_offsetright,      // label 2B9    canloc = (CAN2, 13)
    eoas_inertial_pos_r_upper_arm_1         = 4+eoas_inertial_pos_offsetright,      // label 2B10   canloc = (CAN2,  9)
    eoas_inertial_pos_r_upper_arm_2         = 5+eoas_inertial_pos_offsetright,      // label 2B11   canloc = (CAN2, 11)
    eoas_inertial_pos_r_upper_arm_3         = 6+eoas_inertial_pos_offsetright,      // label 2B12   canloc = (CAN2, 10)
    eoas_inertial_pos_r_upper_arm_4         = 7+eoas_inertial_pos_offsetright,      // label 2B13   canloc = (CAN2,  8)
    // right leg
    eoas_inertial_pos_r_foot_1              = 8+eoas_inertial_pos_offsetright,      // label 11B12  canloc = (CAN2, 13)
    eoas_inertial_pos_r_foot_2              = 9+eoas_inertial_pos_offsetright,      // label 11B13  canloc = (CAN2, 12)
    eoas_inertial_pos_r_lower_leg_1         = 10+eoas_inertial_pos_offsetright,     // label 11B8   canloc = (CAN2,  8)
    eoas_inertial_pos_r_lower_leg_2         = 11+eoas_inertial_pos_offsetright,     // label 11B9   canloc = (CAN2,  9)
    eoas_inertial_pos_r_lower_leg_3         = 12+eoas_inertial_pos_offsetright,     // label 11B10  canloc = (CAN2, 10)
    eoas_inertial_pos_r_lower_leg_4         = 13+eoas_inertial_pos_offsetright,     // label 11B11  canloc = (CAN2, 11)
    eoas_inertial_pos_r_upper_leg_1         = 14+eoas_inertial_pos_offsetright,     // label 11B1   canloc = (CAN1,  1)
    eoas_inertial_pos_r_upper_leg_2         = 15+eoas_inertial_pos_offsetright,     // label 11B2   canloc = (CAN1,  2)
    eoas_inertial_pos_r_upper_leg_3         = 16+eoas_inertial_pos_offsetright,     // label 11B3   canloc = (CAN1,  3)
    eoas_inertial_pos_r_upper_leg_4         = 17+eoas_inertial_pos_offsetright,     // label 11B5   canloc = (CAN1,  5)
    eoas_inertial_pos_r_upper_leg_5         = 18+eoas_inertial_pos_offsetright,     // label 11B4   canloc = (CAN1,  4)
    eoas_inertial_pos_r_upper_leg_6         = 19+eoas_inertial_pos_offsetright,     // label 11B6   canloc = (CAN1,  6)
    eoas_inertial_pos_r_upper_leg_7         = 20+eoas_inertial_pos_offsetright,     // label 11B7   canloc = (CAN1,  7)
    
    // central parts
    eoas_inertial_pos_chest_1               = 1+eoas_inertial_pos_offsetcentral,    // 9B7
    eoas_inertial_pos_chest_2               = 2+eoas_inertial_pos_offsetcentral,    // 9B8
    eoas_inertial_pos_chest_3               = 3+eoas_inertial_pos_offsetcentral,    // 9B9
    eoas_inertial_pos_chest_4               = 4+eoas_inertial_pos_offsetcentral     // 9B10
    
} eOas_inertial_position_t;


typedef struct
{
    uint64_t    timestamp;      // the time in usec of reception of this data
    uint8_t     type;           // use eOas_inertial_type_t.
    uint8_t     position;       // use eOas_inertial_position_t    
    int16_t     x;              /**< x value */
    int16_t     y;              /**< y value */
    int16_t     z;              /**< z value */
} eOas_inertial_data_t;         EO_VERIFYsizeof(eOas_inertial_data_t, 16);


typedef struct
{
    uint8_t                         canmapofsupportedsensors[2][15];  // contains in pos i,j: eoas_inertial_pos_none if no mtb is at that address, else another value of eOas_inertial_position_t
    uint8_t                         filler[2];
} eOas_inertial_serviceconfig_t;    EO_VERIFYsizeof(eOas_inertial_serviceconfig_t, 32);


#define EOAS_ENABLEPOS(pos)                 ((uint64_t)0x1 << (pos))
#define EOAS_ISPOSENABLED(mask, pos)        (((((uint64_t)(mask)) & ((uint64_t)0x1 << (pos)) )) ? 1 : 0)

typedef struct
{
    uint64_t                        enabled;        /**< fill with bitwise OR of EOAS_ENABLEPOS(pos), where pos is from eOas_inertial_position_t */ 
    uint8_t                         datarate;       /**< it specifies the acquisition rate in ms with accepted range [10, 200]. bug: if 250 the mtb emits every 35  */
    uint8_t                         filler[7];  
} eOas_inertial_sensorsconfig_t;    EO_VERIFYsizeof(eOas_inertial_sensorsconfig_t, 16);


typedef struct                      
{
    eOas_inertial_serviceconfig_t   service;
    eOas_inertial_sensorsconfig_t   sensors;     
} eOas_inertial_config_t;           EO_VERIFYsizeof(eOas_inertial_config_t, 48);



typedef struct                      
{
    eOas_inertial_data_t            data;       /**< it is the most recent reading of the inertial sensors which are related to this entity */
} eOas_inertial_status_t;           EO_VERIFYsizeof(eOas_inertial_status_t, 16);


typedef struct                  
{
    uint8_t                     enable;         /**< use 0 or 1*/ 
    uint8_t                     filler[7];
} eOmc_inertial_commands_t;     EO_VERIFYsizeof(eOmc_inertial_commands_t, 8);



typedef struct                      // size is: 48+16+8 = 72
{
    eOas_inertial_config_t          config;
    eOas_inertial_status_t          status;
    eOmc_inertial_commands_t        cmmnds;
} eOas_inertial_t;                  EO_VERIFYsizeof(eOas_inertial_t, 72);


// - others unused possible entities

/** @typedef    typedef struct eOas_aea_t
    @brief      eOas_aea_t contains measurements of the absolute encoder which so far is managed only by the ems 
    @warning    This struct must be of fixed and size and multiple of 4.
 **/
typedef struct                  // size is: 4+0 = 4
{
    uint32_t                    value;            // used with ... see hal_encoder.h for bitfield formatting  
} eOas_aea_t;                   //EO_VERIFYsizeof(eOas_aea_t, 4);


/** @typedef    typedef struct eOas_forcetorque_t
    @brief      eOas_forcetorque_t contains measurements of force on xyz and of torque on 3 components 
 **/
typedef struct                  // size is: 4+2*3+2*3+0 = 16 
{
    uint32_t                    identifier;         /**< the identifier of the origin of measure: so far only strain */ 
    eOmeas_force_t              force[3];           /**< the force split in x, y, and z components */
    eOmeas_torque_t             torque[3];          /**< the torque split in three components*/
} eOas_forcetorque_t;           //EO_VERIFYsizeof(eOas_forcetorque_t, 16);



// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section



/** @}            
    end of group eo_cevcwervcrev5555  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


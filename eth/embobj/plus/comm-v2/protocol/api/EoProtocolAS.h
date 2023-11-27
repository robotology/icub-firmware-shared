/*
 * Copyright (C) 2013 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
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
#ifndef _EOPROTOCOLAS_H_
#define _EOPROTOCOLAS_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoProtocolAS.h
    @brief      This header file gives the ...
    @author     marco.accame@iit.it
    @date       06/05/2013
**/

/** @defgroup eo_EoProtocolAS Protocol for analog sensors endpoint 
    This file contains tags, rw mode, and callback functions specific of the eoprot_endpoint_analogsensors endpoint.
    
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoProtocol.h"
#include "EoAnalogSensors.h"
#include "EOnv.h"
#include "EOrop.h"




// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types ------------------------------------------------------------------------- 


enum { eoprot_version_as_major = 1, eoprot_version_as_minor = 9 };


enum { eoprot_entities_as_numberof = eoas_entities_numberof };


// - entity strain


/** @typedef    typedef enum eOprot_tag_as_strain_t
    @brief      It contains the tags for all variables of the strain entity.
                See definition of eOas_strain_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_strain_wholeitem                                  = 0,
    eoprot_tag_as_strain_config                                     = 1,
    eoprot_tag_as_strain_status                                     = 2,
    eoprot_tag_as_strain_status_fullscale                           = 3,
    eoprot_tag_as_strain_status_calibratedvalues                    = 4,
    eoprot_tag_as_strain_status_uncalibratedvalues                  = 5
} eOprot_tag_as_strain_t;

enum { eoprot_tags_as_strain_numberof = 6 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_strain_t
    @brief      It contains the rw modes for all variables of the strain entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_strain_t.
 **/
typedef enum
{
    eoprot_rwm_as_strain_wholeitem                                  = eo_nv_rwmode_RO,
    eoprot_rwm_as_strain_config                                     = eo_nv_rwmode_RW,
    eoprot_rwm_as_strain_status                                     = eo_nv_rwmode_RO,
    eoprot_rwm_as_strain_status_fullscale                           = eo_nv_rwmode_RO,
    eoprot_rwm_as_strain_status_calibratedvalues                    = eo_nv_rwmode_RO,
    eoprot_rwm_as_strain_status_uncalibratedvalues                  = eo_nv_rwmode_RO    
} eOprot_rwm_as_strain_t; 

enum { eoprot_rwms_as_strain_numberof = 6 };  // it MUST be equal to the number of rw modes. 



// - entity mais
 

/** @typedef    typedef enum eOprot_tag_as_mais_t
    @brief      It contains the tags for all variables in the mais entity.
                See definition of eOas_mais_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_mais_wholeitem                                    = 0,
    eoprot_tag_as_mais_config                                       = 1,
    eoprot_tag_as_mais_config_mode                                  = 2,
    eoprot_tag_as_mais_config_datarate                              = 3,
    eoprot_tag_as_mais_config_resolution                            = 4,
    eoprot_tag_as_mais_status                                       = 5,
    eoprot_tag_as_mais_status_the15values                           = 6
} eOprot_tag_as_mais_t;

enum { eoprot_tags_as_mais_numberof = 7 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_mais_t
    @brief      It contains the rw modes for all variables of the mais entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_mais_t.
 **/
typedef enum
{
    eoprot_rwm_as_mais_wholeitem                                    = eo_nv_rwmode_RO,
    eoprot_rwm_as_mais_config                                       = eo_nv_rwmode_RW,
    eoprot_rwm_as_mais_config_mode                                  = eo_nv_rwmode_RW,
    eoprot_rwm_as_mais_config_datarate                              = eo_nv_rwmode_RW,
    eoprot_rwm_as_mais_config_resolution                            = eo_nv_rwmode_RW,
    eoprot_rwm_as_mais_status                                       = eo_nv_rwmode_RO,
    eoprot_rwm_as_mais_status_the15values                           = eo_nv_rwmode_RO
} eOprot_rwm_as_mais_t; 

enum { eoprot_rwms_as_mais_numberof = 7 };  // it MUST be equal to the number of rw modes. 



// - entity temperature


/** @typedef    typedef enum eOprot_tag_as_temperature_t
    @brief      It contains the tags for all variables of the temperature entity.
                See definition of eOas_temperature_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_temperature_wholeitem                                = 0,
    eoprot_tag_as_temperature_config                                   = 1,
    eoprot_tag_as_temperature_status                                   = 2,
    eoprot_tag_as_temperature_cmmnds_enable                            = 3
} eOprot_tag_as_temperature_t;

enum { eoprot_tags_as_temperature_numberof = 4 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_temperature_t
    @brief      It contains the rw modes for all variables of the temperature entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_temperature_t.
 **/
typedef enum
{
    eoprot_rwm_as_temperature_wholeitem                                = eo_nv_rwmode_RO,
    eoprot_rwm_as_temperature_config                                   = eo_nv_rwmode_RW,
    eoprot_rwm_as_temperature_status                                   = eo_nv_rwmode_RO,
    eoprot_rwm_as_temperature_cmmnds_enable                            = eo_nv_rwmode_RW  
} eOprot_rwm_as_temperature_t; 

enum { eoprot_rwms_as_temperature_numberof = 4 };  // it MUST be equal to the number of rw modes. 


// - entity inertial3


/** @typedef    typedef enum eOprot_tag_as_inertial3_t
    @brief      It contains the tags for all variables of the inertial3 entity.
                See definition of eOas_inertial3_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_inertial3_wholeitem                                = 0,
    eoprot_tag_as_inertial3_config                                   = 1,
    eoprot_tag_as_inertial3_status                                   = 2,
    eoprot_tag_as_inertial3_cmmnds_enable                            = 3
} eOprot_tag_as_inertial3_t;

enum { eoprot_tags_as_inertial3_numberof = 4 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_inertial3_t
    @brief      It contains the rw modes for all variables of the inertial3 entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_inertial3_t.
 **/
typedef enum
{
    eoprot_rwm_as_inertial3_wholeitem                                = eo_nv_rwmode_RO,
    eoprot_rwm_as_inertial3_config                                   = eo_nv_rwmode_RW,
    eoprot_rwm_as_inertial3_status                                   = eo_nv_rwmode_RO,
    eoprot_rwm_as_inertial3_cmmnds_enable                            = eo_nv_rwmode_RW    
} eOprot_rwm_as_inertial3_t; 

enum { eoprot_rwms_as_inertial3_numberof = 4 };  // it MUST be equal to the number of rw modes. 

// - entity psc


/** @typedef    typedef enum eOprot_tag_as_psc_t
    @brief      It contains the tags for all variables of the psc entity.
                See definition of eOas_psc_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_psc_wholeitem                                     = 0,
    eoprot_tag_as_psc_config                                        = 1,
    eoprot_tag_as_psc_status                                        = 2,
    eoprot_tag_as_psc_cmmnds_enable                                 = 3
} eOprot_tag_as_psc_t;

enum { eoprot_tags_as_psc_numberof = 4 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_psc_t
    @brief      It contains the rw modes for all variables of the psc entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_psc_t.
 **/
typedef enum
{
    eoprot_rwm_as_psc_wholeitem                                     = eo_nv_rwmode_RO,
    eoprot_rwm_as_psc_config                                        = eo_nv_rwmode_RW,
    eoprot_rwm_as_psc_status                                        = eo_nv_rwmode_RO,
    eoprot_rwm_as_psc_cmmnds_enable                                 = eo_nv_rwmode_RW    
} eOprot_rwm_as_psc_t; 

enum { eoprot_rwms_as_psc_numberof = 4 };  // it MUST be equal to the number of rw modes. 



// - entity pos


/** @typedef    typedef enum eOprot_tag_as_pos_t
    @brief      It contains the tags for all variables of the psc entity.
                See definition of eOas_pos_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_pos_wholeitem                                     = 0,
    eoprot_tag_as_pos_config                                        = 1,
    eoprot_tag_as_pos_status                                        = 2,
    eoprot_tag_as_pos_cmmnds_enable                                 = 3
} eOprot_tag_as_pos_t;

enum { eoprot_tags_as_pos_numberof = 4 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_pos_t
    @brief      It contains the rw modes for all variables of the pos entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_pos_t.
 **/
typedef enum
{
    eoprot_rwm_as_pos_wholeitem                                     = eo_nv_rwmode_RO,
    eoprot_rwm_as_pos_config                                        = eo_nv_rwmode_RW,
    eoprot_rwm_as_pos_status                                        = eo_nv_rwmode_RO,
    eoprot_rwm_as_pos_cmmnds_enable                                 = eo_nv_rwmode_RW    
} eOprot_rwm_as_pos_t; 

enum { eoprot_rwms_as_pos_numberof = 4 };  // it MUST be equal to the number of rw modes. 


// - entity ft


/** @typedef    typedef enum eOprot_tag_as_ft_t
    @brief      It contains the tags for all variables of the ft entity.
                See definition of eOas_ft_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_ft_config                                        = 0,
    eoprot_tag_as_ft_cmmnds_enable                                 = 1,
    eoprot_tag_as_ft_status                                        = 2,
    eoprot_tag_as_ft_status_timedvalue                             = 3,
    eoprot_tag_as_ft_status_fullscale                              = 4
} eOprot_tag_as_ft_t;

enum { eoprot_tags_as_ft_numberof = 5 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_ft_t
    @brief      It contains the rw modes for all variables of the ft entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_ft_t.
 **/
typedef enum
{
    eoprot_rwm_as_ft_config                                        = eo_nv_rwmode_RW,
    eoprot_rwm_as_ft_cmmnds_enable                                 = eo_nv_rwmode_RW,
    eoprot_rwm_as_ft_status                                        = eo_nv_rwmode_RO,
    eoprot_rwm_as_ft_status_timedvalue                             = eo_nv_rwmode_RO,
    eoprot_rwm_as_ft_status_fullscale                              = eo_nv_rwmode_RO    
} eOprot_rwm_as_ft_t; 

enum { eoprot_rwms_as_ft_numberof = 5 };  // it MUST be equal to the number of rw modes. 

// - entity battery


/** @typedef    typedef enum eOprot_tag_as_battery_t
    @brief      It contains the tags for all variables of the battery entity.
                See definition of eOas_battery_t (and its fields) in file EoAnalogSensors.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_as_battery_config                                        = 0,
    eoprot_tag_as_battery_cmmnds_enable                                 = 1,
    eoprot_tag_as_battery_status                                        = 2,
    eoprot_tag_as_battery_status_timedvalue                             = 3,
} eOprot_tag_as_battery_t;

enum { eoprot_tags_as_battery_numberof = 4 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_as_battery_t
    @brief      It contains the rw modes for all variables of the ft entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_as_battery_t.
 **/
typedef enum
{
    eoprot_rwm_as_battery_config                                        = eo_nv_rwmode_RW,
    eoprot_rwm_as_battery_cmmnds_enable                                 = eo_nv_rwmode_RW,
    eoprot_rwm_as_battery_status                                        = eo_nv_rwmode_RO,
    eoprot_rwm_as_battery_status_timedvalue                             = eo_nv_rwmode_RO,
} eOprot_rwm_as_battery_t; 

enum { eoprot_rwms_as_battery_numberof = 4 };  // it MUST be equal to the number of rw modes. 



// - structures implementing the endpoints

/** @typedef    typedef struct eOprot_template_as_t;
    @brief      It is a template for the organisation of strain, mais etc entities in the analog sensors endpoint.
                The effective number depends on the board.
 **/
typedef struct                  // 56*1+48*1+8*1 = 112              
{
    eOas_strain_t               strain[1]; 
    eOas_mais_t                 mais[1];
    eOas_temperature_t          temperature[1];
    eOas_inertial3_t            inertial3[1];
    eOas_psc_t                  psc[1];
    eOas_pos_t                  pos[1];
    eOas_ft_t                   ft[1];
    eOas_battery_t           battery[1];
} eOprot_template_as_t;         //EO_VERIFYsizeof(eOprot_template_as_t, 112);
  
  

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------

// suggested name for onsay in analog sensors. the function is not defined.
extern void eoprot_fun_ONSAY_as(const EOnv* nv, const eOropdescriptor_t* rd);


// - declaration of extern overridable functions ----------------------------------------------------------------------
// but if EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME is defined, then these functions are not defined.

extern void eoprot_fun_INITIALISE_as(eOprotIP_t ip, void *ram);

// -- strain 

extern void eoprot_fun_INIT_as_strain_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_strain_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_strain_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_strain_status_fullscale(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_status_fullscale(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_strain_status_calibratedvalues(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_status_calibratedvalues(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_strain_status_uncalibratedvalues(const EOnv* nv);
extern void eoprot_fun_UPDT_as_strain_status_uncalibratedvalues(const EOnv* nv, const eOropdescriptor_t* rd);

// -- mais

extern void eoprot_fun_INIT_as_mais_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_config_mode(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_config_mode(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_config_datarate(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_config_datarate(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_config_resolution(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_config_resolution(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_mais_status_the15values(const EOnv* nv);
extern void eoprot_fun_UPDT_as_mais_status_the15values(const EOnv* nv, const eOropdescriptor_t* rd);

// -- temperature

extern void eoprot_fun_INIT_as_temperature_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_temperature_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_temperature_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_temperature_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_temperature_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_temperature_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_temperature_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_temperature_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);

// -- inertial3

extern void eoprot_fun_INIT_as_inertial3_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_inertial3_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_inertial3_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_inertial3_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_inertial3_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_inertial3_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_inertial3_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_inertial3_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);


// -- psc

extern void eoprot_fun_INIT_as_psc_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_psc_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_psc_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_psc_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_psc_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_psc_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_psc_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_psc_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);

// -- pos

extern void eoprot_fun_INIT_as_pos_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_as_pos_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_pos_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_pos_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_pos_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_pos_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_pos_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_pos_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);

// -- ft

extern void eoprot_fun_INIT_as_ft_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_ft_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_ft_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_ft_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_ft_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_ft_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_ft_status_fullscale(const EOnv* nv);
extern void eoprot_fun_UPDT_as_ft_status_fullscale(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_ft_status_timedvalue(const EOnv* nv);
extern void eoprot_fun_UPDT_as_ft_status_timedvalue(const EOnv* nv, const eOropdescriptor_t* rd);

// -- battery

extern void eoprot_fun_INIT_as_battery_config(const EOnv* nv);
extern void eoprot_fun_UPDT_as_battery_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_battery_cmmnds_enable(const EOnv* nv);
extern void eoprot_fun_UPDT_as_battery_cmmnds_enable(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_battery_status(const EOnv* nv);
extern void eoprot_fun_UPDT_as_battery_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_as_battery_status_timedvalue(const EOnv* nv);
extern void eoprot_fun_UPDT_as_battery_status_timedvalue(const EOnv* nv, const eOropdescriptor_t* rd);
/** @}            
    end of group eo_EoProtocolAS  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



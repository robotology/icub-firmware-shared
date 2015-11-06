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
#ifndef _EOPROTOCOLMC_H_
#define _EOPROTOCOLMC_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoProtocolMC.h
    @brief      This header file gives the ...
    @author     marco.accame@iit.it
    @date       06/05/2013
**/

/** @defgroup eo_EoProtocolMC Protocol for motion control endpoint 
    This file contains tags, rw mode, and callback functions specific of the eoprot_endpoint_motioncontrol endpoint.
    
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoProtocol.h"
#include "EoMotionControl.h"
#include "EOnv.h"
#include "EOrop.h"




// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types ------------------------------------------------------------------------- 


enum { eoprot_version_mc_major = 1, eoprot_version_mc_minor = 11 };

enum { eoprot_entities_mc_numberof = eomc_entities_numberof };

// - entity joint

/** @typedef    typedef enum eOprot_tag_mc_joint_t
    @brief      It contains the tags for all variables of the joint entity.
                See definition of eOmc_joint_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mc_joint_wholeitem                                   =  0,
    eoprot_tag_mc_joint_config                                      =  1,
    eoprot_tag_mc_joint_config_pidposition                          =  2,
    eoprot_tag_mc_joint_config_pidvelocity                          =  3,
    eoprot_tag_mc_joint_config_pidtorque                            =  4,
    eoprot_tag_mc_joint_config_limitsofjoint                        =  5,
    eoprot_tag_mc_joint_config_impedance                            =  6,
    eoprot_tag_mc_joint_config_motor_params                         =  7,
    eoprot_tag_mc_joint_config_tcfiltertype                         =  8,
    eoprot_tag_mc_joint_status                                      =  9,
    eoprot_tag_mc_joint_status_basic                                = 10,
    eoprot_tag_mc_joint_status_ofpid                                = 11,
    eoprot_tag_mc_joint_status_controlmodestatus                    = 12,
    eoprot_tag_mc_joint_status_interactionmodestatus                = 13,
    eoprot_tag_mc_joint_status_ismotiondone                         = 14,
    eoprot_tag_mc_joint_inputs                                      = 15,
    eoprot_tag_mc_joint_inputs_externallymeasuredtorque             = 16,
    eoprot_tag_mc_joint_cmmnds_calibration                          = 17,
    eoprot_tag_mc_joint_cmmnds_setpoint                             = 18,
    eoprot_tag_mc_joint_cmmnds_stoptrajectory                       = 19,
    eoprot_tag_mc_joint_cmmnds_controlmode                          = 20,
    eoprot_tag_mc_joint_cmmnds_interactionmode                      = 21
} eOprot_tag_mc_joint_t;

enum { eoprot_tags_mc_joint_numberof = 22 };  // it MUST be equal to the number of tags


/** @typedef    typedef enum eOprot_rwm_mc_joint_t
    @brief      It contains the rw modes for all variables of the joint entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mc_joint_t.
                See definition of eOmc_joint_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_rwm_mc_joint_wholeitem                                   = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_config                                      = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_pidposition                          = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_pidvelocity                          = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_pidtorque                            = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_limitsofjoint                        = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_impedance                            = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_motor_params                         = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_config_tcfiltertype                         = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_status                                      = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_status_basic                                = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_status_ofpid                                = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_status_controlmodestatus                    = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_status_interactionmodestatus                = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_status_ismotiondone                         = eo_nv_rwmode_RO,
    eoprot_rwm_mc_joint_inputs                                      = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_inputs_externallymeasuredtorque             = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_cmmnds_calibration                          = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_cmmnds_setpoint                             = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_cmmnds_stoptrajectory                       = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_cmmnds_controlmode                          = eo_nv_rwmode_RW,
    eoprot_rwm_mc_joint_cmmnds_interactionmode                      = eo_nv_rwmode_RW
} eOprot_rwm_mc_joint_t;    

enum { eoprot_rwms_mc_joint_numberof = 22 };  // it MUST be equal to the number of rw modes  


// - definition of the motor


/** @typedef    typedef enum eOprot_tag_mc_motor_t
    @brief      It contains the tags for all variables of the motor entity.
                See definition of eOmc_joint_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mc_motor_wholeitem                                   = 0,
    eoprot_tag_mc_motor_config                                      = 1,
    eoprot_tag_mc_motor_config_maxcurrentofmotor                    = 2,
    eoprot_tag_mc_motor_config_gearboxratio                         = 3,
    eoprot_tag_mc_motor_config_rotorencoder                         = 4,
    eoprot_tag_mc_motor_status                                      = 5,
    eoprot_tag_mc_motor_status_basic                                = 6
} eOprot_tag_mc_motor_t;

enum { eoprot_tags_mc_motor_numberof = 7 };   // it MUST be equal to the number of tags 


/** @typedef    typedef enum eOprot_rwm_mc_motor_t
    @brief      IIt contains the rw modes for all variables of the motor entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mc_motor_t.
                See definition of eOmc_motor_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_rwm_mc_motor_wholeitem                                   = eo_nv_rwmode_RO,
    eoprot_rwm_mc_motor_config                                      = eo_nv_rwmode_RW,
    eoprot_rwm_mc_motor_config_maxcurrentofmotor                    = eo_nv_rwmode_RW,
    eoprot_rwm_mc_motor_config_gearboxratio                         = eo_nv_rwmode_RW,
    eoprot_rwm_mc_motor_config_rotorencoder                         = eo_nv_rwmode_RW,
    eoprot_rwm_mc_motor_status                                      = eo_nv_rwmode_RO,
    eoprot_rwm_mc_motor_status_basic                                = eo_nv_rwmode_RO
} eOprot_rwm_mc_motor_t;  

enum { eoprot_rwms_mc_motor_numberof = 7 };   // it MUST be equal to the number of rw modes



// - definition of the controller



/** @typedef    typedef enum eOprot_tag_mc_controller_t
    @brief      It contains the tags for all variables of the controller entity.
                See definition of eOmc_controller_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
     eoprot_tag_mc_controller_wholeitem                              = 0,
     eoprot_tag_mc_controller_config                                 = 1,
     eoprot_tag_mc_controller_config_jointcoupling                   = 2,
     eoprot_tag_mc_controller_status                                 = 3
} eOprot_tag_mc_controller_t;

enum { eoprot_tags_mc_controller_numberof = 4 };  // it MUST be equal to the number of tags


/** @typedef    typedef enum eOprot_rwm_mc_controller_t
    @brief      It contains the rw modes for all variables of the motor entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mc_controller_t.
                See definition of eOmc_controller_t (and its fields) in file EoMotionControl.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_rwm_mc_controller_wholeitem                              = eo_nv_rwmode_RO,
    eoprot_rwm_mc_controller_config                                 = eo_nv_rwmode_RW,
    eoprot_rwm_mc_controller_config_jointcoupling                   = eo_nv_rwmode_RW,
    eoprot_rwm_mc_controller_status                                 = eo_nv_rwmode_RO
} eOprot_rwm_mc_controller_t; 

enum { eoprot_rwms_mc_controller_numberof = 4 };  // it MUST be equal to the number of rw modes 


// - structures implementing the endpoints


/** @typedef    typedef struct eOprot_template_mc_t;
    @brief      It is a template for the organisation of joint, motor, controller entities in the motion control endpoint.
                The effective number depends on the board.
 **/
typedef struct                  // 188*1+48*1+80 = 316              
{
    eOmc_joint_t                joints[1]; 
    eOmc_motor_t                motors[1];
    eOmc_controller_t           thecontroller;
} eOprot_template_mc_t;         //EO_VERIFYsizeof(eOprot_template_mc_t, 316);


  
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------

// suggested name for onsay in motion control. the function is not defined.
extern void eoprot_fun_ONSAY_mc(const EOnv* nv, const eOropdescriptor_t* rd);

// - declaration of extern overridable functions ----------------------------------------------------------------------
// but if EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME is defined, then these functions are not defined.

extern void eoprot_fun_INITIALISE_mc(eOprotIP_t ip, void *ram);


// -- joint

extern void eoprot_fun_INIT_mc_joint_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config_pidposition(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_pidposition(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config_pidvelocity(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_pidvelocity(const EOnv* nv, const eOropdescriptor_t* rd);
   
extern void eoprot_fun_INIT_mc_joint_config_pidtorque(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_pidtorque(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config_limitsofjoint(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_limitsofjoint(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config_impedance(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_impedance(const EOnv* nv, const eOropdescriptor_t* rd);


extern void eoprot_fun_INIT_mc_joint_config_motor_params(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_motor_params(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_config_tcfiltertype(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_config_tcfiltertype(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_status_basic(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status_basic(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_status_ofpid(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status_ofpid(const EOnv* nv, const eOropdescriptor_t* rd);


extern void eoprot_fun_INIT_mc_joint_status_controlmodestatus(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status_controlmodestatus(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_status_interactionmodestatus(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status_interactionmodestatus(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_status_ismotiondone(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_status_ismotiondone(const EOnv* nv, const eOropdescriptor_t* rd);

   

extern void eoprot_fun_INIT_mc_joint_inputs(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_inputs(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_inputs_externallymeasuredtorque(const EOnv* nv);   
extern void eoprot_fun_UPDT_mc_joint_inputs_externallymeasuredtorque(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_cmmnds_calibration(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_cmmnds_calibration(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_cmmnds_setpoint(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_cmmnds_setpoint(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_cmmnds_stoptrajectory(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_cmmnds_stoptrajectory(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_cmmnds_controlmode(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_cmmnds_controlmode(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_joint_cmmnds_interactionmode(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_joint_cmmnds_interactionmode(const EOnv* nv, const eOropdescriptor_t* rd);

// -- motor

extern void eoprot_fun_INIT_mc_motor_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_config_maxcurrentofmotor(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_config_maxcurrentofmotor(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_config_gearboxratio(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_config_gearboxratio(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_config_rotorencoder(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_config_rotorencoder(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_motor_status_basic(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_motor_status_basic(const EOnv* nv, const eOropdescriptor_t* rd);

// -- controller

extern void eoprot_fun_INIT_mc_controller_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_controller_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_controller_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_controller_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_controller_config_jointcoupling(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_controller_config_jointcoupling(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mc_controller_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mc_controller_status(const EOnv* nv, const eOropdescriptor_t* rd);


/** @}            
    end of group eo_EoProtocolMC  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



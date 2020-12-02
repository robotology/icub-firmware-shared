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
#ifndef _EOPROTOCOLMN_H_
#define _EOPROTOCOLMN_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoProtocolMN.h
    @brief      This header file gives the ...
    @author     marco.accame@iit.it
    @date       06/05/2013
**/

/** @defgroup eo_EoProtocolMN Protocol for management endpoint 
    This file contains tags, rw mode, and callback functions specific of the eoprot_endpoint_management endpoint.
    
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoProtocol.h"
#include "EoManagement.h"
#include "EOnv.h"
#include "EOrop.h"



// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types ------------------------------------------------------------------------- 


enum { eoprot_version_mn_major = 2, eoprot_version_mn_minor = 17 };


enum { eoprot_entities_mn_numberof = eomn_entities_numberof };


// - entity comm


/** @typedef    typedef enum eOprot_tag_mn_comm_t
    @brief      It contains the tags for all variables of the comm entity.
                See definition of eOmn_comm_t (and its fields) in file EoManagement.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mn_comm_wholeitem                                    = 0, 
    eoprot_tag_mn_comm_status                                       = 1, 
    eoprot_tag_mn_comm_status_managementprotocolversion             = 2,
    eoprot_tag_mn_comm_cmmnds_command_querynumof                    = 3,
    eoprot_tag_mn_comm_cmmnds_command_queryarray                    = 4,
    eoprot_tag_mn_comm_cmmnds_command_replynumof                    = 5,
    eoprot_tag_mn_comm_cmmnds_command_replyarray                    = 6,
    eoprot_tag_mn_comm_cmmnds_command_config                        = 7
} eOprot_tag_mn_comm_t;

enum { eoprot_tags_mn_comm_numberof = 8 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_mn_comm_t
    @brief      It contains the rw modes for all variables of the comm entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mn_comm_t.
 **/
typedef enum
{
    eoprot_rwm_mn_comm_wholeitem                                    = eo_nv_rwmode_RO,
    eoprot_rwm_mn_comm_status                                       = eo_nv_rwmode_RO,
    eoprot_rwm_mn_comm_status_managementprotocolversion             = eo_nv_rwmode_RO,
    eoprot_rwm_mn_comm_cmmnds_command_querynumof                    = eo_nv_rwmode_RW,
    eoprot_rwm_mn_comm_cmmnds_command_queryarray                    = eo_nv_rwmode_RW,
    eoprot_rwm_mn_comm_cmmnds_command_replynumof                    = eo_nv_rwmode_RW,
    eoprot_rwm_mn_comm_cmmnds_command_replyarray                    = eo_nv_rwmode_RW,
    eoprot_rwm_mn_comm_cmmnds_command_config                        = eo_nv_rwmode_RW    
} eOprot_rwm_mn_comm_t; 

enum { eoprot_rwms_mn_comm_numberof = 8 };  // it MUST be equal to the number of rw modes. 



// - entity appl
  

/** @typedef    typedef enum eOprot_tag_mn_appl_t
    @brief      It contains the tags for all variables of the appl entity.
                See definition of eOmn_appl_t (and its fields) in file EoManagement.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mn_appl_wholeitem                                    = 0,
    eoprot_tag_mn_appl_config                                       = 1,
    eoprot_tag_mn_appl_config_txratedivider                         = 2,
    eoprot_tag_mn_appl_status                                       = 3,
    eoprot_tag_mn_appl_cmmnds_go2state                              = 4,
    eoprot_tag_mn_appl_cmmnds_timeset                               = 5
} eOprot_tag_mn_appl_t;

enum { eoprot_tags_mn_appl_numberof = 6 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_mn_appl_t
    @brief      It contains the rw modes for all variables of the appl entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mn_appl_t.
 **/
typedef enum
{
    eoprot_rwm_mn_appl_wholeitem                                    = eo_nv_rwmode_RO,
    eoprot_rwm_mn_appl_config                                       = eo_nv_rwmode_RW,
    eoprot_rwm_mn_appl_config_txratedivider                         = eo_nv_rwmode_RW,
    eoprot_rwm_mn_appl_status                                       = eo_nv_rwmode_RO,
    eoprot_rwm_mn_appl_cmmnds_go2state                              = eo_nv_rwmode_RW,
    eoprot_rwm_mn_appl_cmmnds_timeset                               = eo_nv_rwmode_RW
} eOprot_rwm_mn_appl_t; 

enum { eoprot_rwms_mn_appl_numberof = 6 };  // it MUST be equal to the number of rw modes. 


// - entity info
  
/** @typedef    typedef enum eOprot_tag_mn_info_t
    @brief      It contains the tags for all variables of the appl entity.
                See definition of eOmn_appl_t (and its fields) in file EoManagement.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mn_info_wholeitem                                    = 0,
    eoprot_tag_mn_info_config                                       = 1,
    eoprot_tag_mn_info_config_enabled                               = 2,
    eoprot_tag_mn_info_status                                       = 3,
    eoprot_tag_mn_info_status_basic                                 = 4
} eOprot_tag_mn_info_t;

enum { eoprot_tags_mn_info_numberof = 5 };  // it MUST be equal to the number of tags. 

/** @typedef    typedef enum eOprot_rwm_mn_appl_t
    @brief      It contains the rw modes for all variables of the appl entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mn_appl_t.
 **/
typedef enum
{
    eoprot_rwm_mn_info_wholeitem                                    = eo_nv_rwmode_RO,
    eoprot_rwm_mn_info_config                                       = eo_nv_rwmode_RW,
    eoprot_rwm_mn_info_config_enabled                               = eo_nv_rwmode_RW,
    eoprot_rwm_mn_info_status                                       = eo_nv_rwmode_RO,
    eoprot_rwm_mn_info_status_basic                                 = eo_nv_rwmode_RO
} eOprot_rwm_mn_info_t; 

enum { eoprot_rwms_mn_info_numberof = 5 };  // it MUST be equal to the number of rw modes. 


// - entity service


/** @typedef    typedef enum eOprot_tag_mn_service_t
    @brief      It contains the tags for all variables of the service entity.
                See definition of eOmn_service_t (and its fields) in file EoManagement.h for explanation of the variables.
 **/
typedef enum
{
    eoprot_tag_mn_service_wholeitem                                     = 0, 
    eoprot_tag_mn_service_status_commandresult                          = 1, 
    eoprot_tag_mn_service_cmmnds_command                                = 2
} eOprot_tag_mn_service_t;

enum { eoprot_tags_mn_service_numberof = 3 };  // it MUST be equal to the number of tags. 


/** @typedef    typedef enum eOprot_rwm_mn_service_t
    @brief      It contains the rw modes for all variables of the service entity. There must be a one-to-one
                correspondence to the values in eOprot_tag_mn_service_t.
 **/
typedef enum
{
    eoprot_rwm_mn_service_wholeitem                                     = eo_nv_rwmode_RO,
    eoprot_rwm_mn_service_status_commandresult                          = eo_nv_rwmode_RO,
    eoprot_rwm_mn_service_cmmnds_command                                = eo_nv_rwmode_RW 
} eOprot_rwm_mn_service_t; 

enum { eoprot_rwms_mn_service_numberof = 3 };  // it MUST be equal to the number of rw modes. 


// - memory organization in the endpoint
  
  
// omissis

  
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOprot_EPcfg_t eoprot_mn_basicEPcfg;


// - declaration of extern public functions ---------------------------------------------------------------------------

// suggested name for onsay in management. the function is not defined.
extern void eoprot_fun_ONSAY_mn(const EOnv* nv, const eOropdescriptor_t* rd);


// - declaration of extern overridable functions ----------------------------------------------------------------------
// but if EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME is defined, then these functions are not defined.

extern void eoprot_fun_INITIALISE_mn(eOprotIP_t ip, void *ram);

// - comm

extern void eoprot_fun_INIT_mn_comm_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_comm_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_comm_status_managementprotocolversion(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_status_managementprotocolversion(const EOnv* nv, const eOropdescriptor_t* rd);
 
extern void eoprot_fun_INIT_mn_comm_cmmnds_command_querynumof(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_cmmnds_command_querynumof(const EOnv* nv, const eOropdescriptor_t* rd);
 
extern void eoprot_fun_INIT_mn_comm_cmmnds_command_queryarray(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_cmmnds_command_queryarray(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_comm_cmmnds_command_replynumof(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_cmmnds_command_replynumof(const EOnv* nv, const eOropdescriptor_t* rd);
  
extern void eoprot_fun_INIT_mn_comm_cmmnds_command_replyarray(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_cmmnds_command_replyarray(const EOnv* nv, const eOropdescriptor_t* rd);
   
extern void eoprot_fun_INIT_mn_comm_cmmnds_command_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_comm_cmmnds_command_config(const EOnv* nv, const eOropdescriptor_t* rd);


// - appl

extern void eoprot_fun_INIT_mn_appl_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_appl_config_txratedivider(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_config_txratedivider(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_appl_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_appl_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_appl_cmmnds_go2state(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_cmmnds_go2state(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_appl_cmmnds_timeset(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_appl_cmmnds_timeset(const EOnv* nv, const eOropdescriptor_t* rd);

// - info

extern void eoprot_fun_INIT_mn_info_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_info_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_info_config(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_info_config(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_info_config_enabled(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_info_config_enabled(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_info_status(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_info_status(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_info_status_basic(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_info_status_basic(const EOnv* nv, const eOropdescriptor_t* rd);



// - service

extern void eoprot_fun_INIT_mn_service_wholeitem(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_service_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd);

extern void eoprot_fun_INIT_mn_service_status_commandresult(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_service_status_commandresult(const EOnv* nv, const eOropdescriptor_t* rd);

 
extern void eoprot_fun_INIT_mn_service_cmmnds_command(const EOnv* nv);
extern void eoprot_fun_UPDT_mn_service_cmmnds_command(const EOnv* nv, const eOropdescriptor_t* rd);


/** @}            
    end of group eo_EoProtocolMN  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



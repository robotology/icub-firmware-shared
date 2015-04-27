/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
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
#ifndef _EOPROTOCOLCONFIGURATOR_H_
#define _EOPROTOCOLCONFIGURATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOprotocolConfigurator.h
    @brief      This header file implements public interface to ...
    @author     marco.accame@iit.it
    @date       05/08/2014
**/

/** @defgroup eodoxy_EOprotocolConfigurator EOprotocolConfigurator
    cefcewqfcew
      
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

#include "EoProtocol.h"


#include "EOnvSet.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types -------------------------------------------------------------------------    


/** @typedef    typedef const struct EOprotocolConfigurator_hid EOprotocolConfigurator
    @brief      EOaction is an opaque struct. It is used to implement data abstraction for the  
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOprotocolConfigurator_hid EOprotocolConfigurator;


typedef struct
{   // for each endpoint and each entity it tells how many to use
    eOnvBRD_t       board;
    eObool_t        ep_management_is_present;
    uint8_t         en_mn_entity_comm_numberof;
    uint8_t         en_mn_entity_appl_numberof;
    uint8_t         en_mn_entity_info_numberof;
    
    eObool_t        ep_motioncontrol_is_present;
    uint8_t         en_mc_entity_joint_numberof;
    uint8_t         en_mc_entity_motor_numberof;
    uint8_t         en_mc_entity_controller_numberof;
    
    eObool_t        ep_analogsensors_is_present;
    uint8_t         en_as_entity_strain_numberof;
    uint8_t         en_as_entity_mais_numberof;
    uint8_t         en_as_entity_extorque_numberof;
    
    eObool_t        ep_skin_is_present;   
    uint8_t         en_sk_entity_skin_numberof;
} eOprotconfig_cfg_t;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

// able to accomodate worst case (12 joints/motors, 1 strain, 1 mais, 1 extorques, 1 skin)... even if it uses a lot of ram ...
extern const eOprotconfig_cfg_t eo_protconfig_cfg_default; // = { ... }; 


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern EOprotocolConfigurator* eo_protconfig_New(const eOprotconfig_cfg_t* cfg)
    @brief      creates the object EOprotocolConfigurator, whihc is used to obtain the eOnvset_BRDcfg_t data structure
                used by the EOtransceiver.  
    @param      cfg             the required protocol information        
    @return     a valid object or NULL in case of failure
 **/
extern EOprotocolConfigurator* eo_protconfig_New(const eOprotconfig_cfg_t* cfg);


/** @fn         extern void eo_protconfig_Delete(EOprotocolConfigurator *p)
    @brief      deletes the object EOprotocolConfigurator ... but also the eOnvset_BRDcfg_t data structure used to configure
                the EOtransceiver, but also to run it. THUS: the call of eo_protconfig_Delete() makes the EOtransceiver unusable.  
    @param      p               the object        
 **/
extern void eo_protconfig_Delete(EOprotocolConfigurator *p);
 
 
/** @fn         extern eOnvset_BRDcfg_t* eo_protconfig_BRDcfg_Get(EOprotocolConfigurator* p)
    @brief      retrieves the eOnvset_BRDcfg_t data structure used to configure the EOtransceiver, but also to run it. 
    @param      p               the object      
    @return     a valid pointer or NULL in case of failure    
 **/ 
extern eOnvset_BRDcfg_t* eo_protconfig_BRDcfg_Get(EOprotocolConfigurator* p);


/** @}            
    end of group eodoxy_EOprotocolConfigurator 
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




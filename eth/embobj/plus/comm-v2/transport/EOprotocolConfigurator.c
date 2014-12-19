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

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "EoCommon.h"
#include "string.h"
#include "EOtheMemoryPool.h"
#include "EOtheErrorManager.h"

#include "EOvector_hid.h" 

#include "EoProtocol.h"
#include "EoProtocolMN.h"
#include "EoProtocolMC.h"
#include "EoProtocolAS.h"
#include "EoProtocolSK.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOprotocolConfigurator.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOprotocolConfigurator_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

const eOprotconfig_cfg_t eo_protconfig_cfg_default =
{
    EO_INIT(.board)                                 eo_prot_BRDdummy,
    EO_INIT(.ep_management_is_present)              eobool_true,
    EO_INIT(.en_mn_entity_comm_numberof)            1,        
    EO_INIT(.en_mn_entity_appl_numberof)            1,
    EO_INIT(.en_mn_entity_info_numberof)            1,
    
    EO_INIT(.ep_motioncontrol_is_present)           eobool_true,
    EO_INIT(.en_mc_entity_joint_numberof)           12,
    EO_INIT(.en_mc_entity_motor_numberof)           12,
    EO_INIT(.en_mc_entity_controller_numberof)      1,
    
    EO_INIT(.ep_analogsensors_is_present)           eobool_true,
    EO_INIT(.en_as_entity_strain_numberof)          1,
    EO_INIT(.en_as_entity_mais_numberof)            1,
    EO_INIT(.en_as_entity_extorque_numberof)        1,
    
    EO_INIT(.ep_skin_is_present)                    eobool_true,   
    EO_INIT(.en_sk_entity_skin_numberof)            2
};


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOprotocolConfigurator";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOprotocolConfigurator* eo_protconfig_New(const eOprotconfig_cfg_t* cfg)
{
    EOprotocolConfigurator *p = NULL;  

    if(NULL == cfg)
    {
        cfg = &eo_protconfig_cfg_default;
    }
    
    if(eo_prot_BRDdummy == cfg->board)
    {
        return(NULL);
    }
    
    
    // i get the memory for the object
    p = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOprotocolConfigurator), 1);
    
    p->nvsetdevbuilder = eo_nvsetdevbuilder_New(cfg->board);
    
    memcpy(&p->config, cfg, sizeof(eOprotconfig_cfg_t));
    
    // ok, now i load everything
    
    // load mn ...
    if(eobool_true == p->config.ep_management_is_present)
    {
        eo_nvsetdevbuilder_ENDPOINTpushback(p->nvsetdevbuilder, eoprot_endpoint_management);
        if(p->config.en_mn_entity_comm_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_management, eoprot_entity_mn_comm, p->config.en_mn_entity_comm_numberof);
        }
        if(p->config.en_mn_entity_appl_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_management, eoprot_entity_mn_appl, p->config.en_mn_entity_appl_numberof);
        }  
        if(p->config.en_mn_entity_info_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_management, eoprot_entity_mn_info, p->config.en_mn_entity_info_numberof);
        }          
    }
    
    // load mc ...
    if(eobool_true == p->config.ep_motioncontrol_is_present)
    {
        eo_nvsetdevbuilder_ENDPOINTpushback(p->nvsetdevbuilder, eoprot_endpoint_motioncontrol);
        if(p->config.en_mc_entity_joint_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, p->config.en_mc_entity_joint_numberof);
        }
        if(p->config.en_mc_entity_motor_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_motioncontrol, eoprot_entity_mc_motor, p->config.en_mc_entity_motor_numberof);
        } 
        if(p->config.en_mc_entity_controller_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_motioncontrol, eoprot_entity_mc_controller, p->config.en_mc_entity_controller_numberof);
        }          
    }    
    
     // load as ...
    if(eobool_true == p->config.ep_analogsensors_is_present)
    {
        eo_nvsetdevbuilder_ENDPOINTpushback(p->nvsetdevbuilder, eoprot_endpoint_analogsensors);
        if(p->config.en_as_entity_strain_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_analogsensors, eoprot_entity_as_strain, p->config.en_as_entity_strain_numberof);
        }
        if(p->config.en_as_entity_mais_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_analogsensors, eoprot_entity_as_mais, p->config.en_as_entity_mais_numberof);
        } 
        if(p->config.en_as_entity_extorque_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_analogsensors, eoprot_entity_as_extorque, p->config.en_as_entity_extorque_numberof);
        }          
    }

     // load sk ...
    if(eobool_true == p->config.ep_skin_is_present)
    {
        eo_nvsetdevbuilder_ENDPOINTpushback(p->nvsetdevbuilder, eoprot_endpoint_skin);
        if(p->config.en_sk_entity_skin_numberof > 0)
        {
            eo_nvsetdevbuilder_ENTITYpushback(p->nvsetdevbuilder, eoprot_endpoint_skin, eoprot_entity_sk_skin, p->config.en_sk_entity_skin_numberof);
        }    
    } 


    // ok, now prepare everything
    eo_nvsetdevbuilder_Prepare(p->nvsetdevbuilder);
    
    return(p);
}



extern void eo_protconfig_Delete(EOprotocolConfigurator *p)
{
    if(NULL == p)
    {
        return;
    } 
    
    if(NULL == p->nvsetdevbuilder)
    {
        return;
    }
    
    eo_nvsetdevbuilder_Delete(p->nvsetdevbuilder);
       
    memset(p, 0, sizeof(EOprotocolConfigurator));    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;    
}
    

extern eOnvset_DEVcfg_t* eo_protconfig_DEVcfg_Get(EOprotocolConfigurator* p)
{
    if(NULL == p)
    {
        return(NULL);
    }
    
    eOnvset_DEVcfg_t *ret = eo_nvsetdevbuilder_DEVcfg_Get(p->nvsetdevbuilder);
    
    // we can clean some memory used in the EOprotocolConfigurator object. however we cannot delete the p->nvsetdevbuilder.
    
    // marco.accame: we cannot clean anything in EOprotocolConfigurator. we could remove the eOprotconfig_cfg_t member, as it is
    //               used only inside eo_protconfig_New(), but ... we dont save much memory.

    return(ret);
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------

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
    eOprot_EPcfg_t epcfg = {0};

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
    
    p->nvsetbrdbuilder = eo_nvsetbrdbuilder_New(cfg->board);
    
    memcpy(&p->config, cfg, sizeof(eOprotconfig_cfg_t));
    
    // ok, now i load everything
    
    // load mn ...
//    if(eobool_true == p->config.ep_management_is_present)
//    {
//        epcfg.endpoint = eoprot_endpoint_management;
//        memset(epcfg.numberofentities, 0, sizeof(epcfg.numberofentities));
//        epcfg.numberofentities[0] = p->config.en_mn_entity_comm_numberof;
//        epcfg.numberofentities[1] = p->config.en_mn_entity_appl_numberof;
//        epcfg.numberofentities[2] = p->config.en_mn_entity_info_numberof;
//       
//        eo_nvsetbrdbuilder_EP_Load(p->nvsetbrdbuilder, &epcfg);
//    }

    // mn is always present
    eo_nvsetbrdbuilder_EP_Load(p->nvsetbrdbuilder, (eOprot_EPcfg_t*)&eoprot_mn_basicEPcfg);
    
    // load mc ...
    if(eobool_true == p->config.ep_motioncontrol_is_present)
    {       
        epcfg.endpoint = eoprot_endpoint_motioncontrol;
        memset(epcfg.numberofentities, 0, sizeof(epcfg.numberofentities));
        epcfg.numberofentities[0] = p->config.en_mc_entity_joint_numberof;
        epcfg.numberofentities[1] = p->config.en_mc_entity_motor_numberof;
        epcfg.numberofentities[2] = p->config.en_mc_entity_controller_numberof;
       
        eo_nvsetbrdbuilder_EP_Load(p->nvsetbrdbuilder, &epcfg);         
    }    
    
     // load as ...
    if(eobool_true == p->config.ep_analogsensors_is_present)
    {
        epcfg.endpoint = eoprot_endpoint_analogsensors;
        memset(epcfg.numberofentities, 0, sizeof(epcfg.numberofentities));
        epcfg.numberofentities[0] = p->config.en_as_entity_strain_numberof;
        epcfg.numberofentities[1] = p->config.en_as_entity_mais_numberof;
        epcfg.numberofentities[2] = p->config.en_as_entity_extorque_numberof;
       
        eo_nvsetbrdbuilder_EP_Load(p->nvsetbrdbuilder, &epcfg);            
    }

     // load sk ...
    if(eobool_true == p->config.ep_skin_is_present)
    {
        epcfg.endpoint = eoprot_endpoint_skin;
        memset(epcfg.numberofentities, 0, sizeof(epcfg.numberofentities));
        epcfg.numberofentities[0] = p->config.en_sk_entity_skin_numberof;
       
        eo_nvsetbrdbuilder_EP_Load(p->nvsetbrdbuilder, &epcfg);          
    } 
    
    return(p);
}


extern void eo_protconfig_Delete(EOprotocolConfigurator *p)
{
    if(NULL == p)
    {
        return;
    } 
    
    if(NULL == p->nvsetbrdbuilder)
    {
        return;
    }
    
    eo_nvsetbrdbuilder_Delete(p->nvsetbrdbuilder);
       
    memset(p, 0, sizeof(EOprotocolConfigurator));    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;    
}
    

extern eOnvset_BRDcfg_t* eo_protconfig_BRDcfg_Get(EOprotocolConfigurator* p)
{
    eOnvset_BRDcfg_t *ret = NULL;
    if(NULL == p)
    {
        return(NULL);
    }
    
    ret = eo_nvsetbrdbuilder_BRDcfg_Get(p->nvsetbrdbuilder);
    
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

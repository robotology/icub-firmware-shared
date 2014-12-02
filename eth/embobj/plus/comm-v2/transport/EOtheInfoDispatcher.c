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

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "EoCommon.h"
#include "string.h"
#include "stdio.h"
#include "EOtheMemoryPool.h"
#include "EOtheErrorManager.h"
#include "EOnv.h"
#include "EOrop.h"
#include "EOVtheSystem.h"
#include "EoProtocol.h"
#include "EoProtocolMN.h"
#include "EoError.h"

#include "eventviewer.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheInfoDispatcher.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheInfoDispatcher_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static void s_eo_infodispatcher_overflow_init(EOtheInfoDispatcher* p);
static void s_eo_infodispatcher_overflow_clear(EOtheInfoDispatcher* p);
static void s_eo_infodispatcher_overflow_fill(EOtheInfoDispatcher* p);

static eOresult_t s_eo_infodispatcher_transmit(EOtheInfoDispatcher* p, eOmn_info_status_t* info);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOtheInfoDispatcher";
 
static EOtheInfoDispatcher s_eo_theinfodispatcher = 
{
    EO_INIT(.vectorOfinfostatus)        NULL,
    EO_INIT(.overflow)                  NULL,
    EO_INIT(.nvinfostatus)              NULL,
    EO_INIT(.nvinfostatusbasic)         NULL
};


const eOinfodispatcher_cfg_t eo_infodispatcher_cfg_default = 
{
    EO_INIT(.capacity)                  8,
    EO_INIT(.transmitter)               NULL
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

//void infodisp_loadrop(void) {}
 
extern EOtheInfoDispatcher * eo_infodispatcher_Initialise(const eOinfodispatcher_cfg_t *cfg) 
{
    
    if(NULL != s_eo_theinfodispatcher.vectorOfinfostatus)
    {
        return(&s_eo_theinfodispatcher);
    }

    if(NULL == cfg)
    {
        cfg = &eo_infodispatcher_cfg_default;
    }
    
    if(0 == cfg->capacity)
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_infodispatcher_Initialise(): 0 cfg->capacity", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    }
    
    if(NULL == cfg->transmitter)
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_infodispatcher_Initialise(): NULL cfg->transmitter", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    }

    
    // 1. init the infostatus vector, overflow, transmitter etc.

    s_eo_theinfodispatcher.vectorOfinfostatus = eo_vector_New(sizeof(eOmn_info_status_t), cfg->capacity, NULL, NULL, NULL, NULL);    
    s_eo_theinfodispatcher.overflow = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_auto, sizeof(eOmn_info_status_t), 1);
    s_eo_theinfodispatcher.infostatus = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_auto, sizeof(eOmn_info_status_t), 1);
    
    s_eo_theinfodispatcher.transmitter = cfg->transmitter;
    EOnvSet* nvset = eo_transmitter_GetNVset(s_eo_theinfodispatcher.transmitter);   
    s_eo_theinfodispatcher.nvinfostatus = eo_nv_New();    
    eo_nvset_NV_Get(nvset, eok_ipv4addr_localhost, 
                    eoprot_ID_get(eoprot_endpoint_management, eoprot_entity_mn_info, 0, eoprot_tag_mn_info_status),
                    s_eo_theinfodispatcher.nvinfostatus
                    );   
    s_eo_theinfodispatcher.nvinfostatusbasic = eo_nv_New();
    eo_nvset_NV_Get(nvset, eok_ipv4addr_localhost, 
                    eoprot_ID_get(eoprot_endpoint_management, eoprot_entity_mn_info, 0, eoprot_tag_mn_info_status_basic),
                    s_eo_theinfodispatcher.nvinfostatusbasic
                    ); 
    
    // now i init the overflow

    s_eo_infodispatcher_overflow_init(&s_eo_theinfodispatcher);
    
    
    s_eo_theinfodispatcher.ropsizeinfostatusbasic = eo_rop_compute_size(eok_ropctrl_basic, eo_ropcode_say, sizeof(eOmn_info_basic_t));
    // s_eo_theinfodispatcher.ropsizeinfostatusbasic = sizeof(eOrophead_t) + sizeof(eOmn_info_basic_t);
    s_eo_theinfodispatcher.ropsizeinfostatus = eo_rop_compute_size(eok_ropctrl_basic, eo_ropcode_say, sizeof(eOmn_info_status_t));
    // s_eo_theinfodispatcher.ropsizeinfostatus = sizeof(eOrophead_t) + sizeof(eOmn_info_status_t);

    // in here i build the ropstream which i want to send out to the transmitter. i use a single buffer and then i shape it later
    s_eo_theinfodispatcher.ropstream = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_auto, s_eo_theinfodispatcher.ropsizeinfostatus, 1);
    
    
    // i get the pointers to header and data. i known from the protocol that the data is just after the header. 
    // then ... i dont use signature and time in the rop
    s_eo_theinfodispatcher.rophead = (eOrophead_t*)s_eo_theinfodispatcher.ropstream;
    s_eo_theinfodispatcher.ropdata = s_eo_theinfodispatcher.ropstream + sizeof(eOrophead_t);
    
    // init the rophead to contain a eok_ropctrl_basic and a eo_ropcode_say. 
    // the id32 and the size are to be configured each time we send a ropstream. for now are for eOmn_info_basic_t
    s_eo_theinfodispatcher.rophead->ctrl.confinfo   = eo_ropconf_none;
    s_eo_theinfodispatcher.rophead->ctrl.plustime   = 0;
    s_eo_theinfodispatcher.rophead->ctrl.plussign   = 0;
    s_eo_theinfodispatcher.rophead->ctrl.rqsttime   = 0;
    s_eo_theinfodispatcher.rophead->ctrl.rqstconf   = 0;
    s_eo_theinfodispatcher.rophead->ctrl.version    = EOK_ROP_VERSION_0;
    s_eo_theinfodispatcher.rophead->ropc            = eo_ropcode_say;  
    s_eo_theinfodispatcher.rophead->id32            = eoprot_ID_get(eoprot_endpoint_management, eoprot_entity_mn_info, 0, eoprot_tag_mn_info_status_basic);
    s_eo_theinfodispatcher.rophead->dsiz            = sizeof(eOmn_info_basic_t);
    
    
    //eventviewer_load(ev_ID_first_usrdef+15, infodisp_loadrop);

    
    return(&s_eo_theinfodispatcher);        
}    


extern EOtheInfoDispatcher * eo_infodispatcher_GetHandle(void) 
{
    if(NULL != s_eo_theinfodispatcher.vectorOfinfostatus)
    {
        return(&s_eo_theinfodispatcher);
    }
    else
    {    
        return(NULL);
    }
}


extern eOresult_t eo_infodispatcher_Put(EOtheInfoDispatcher* p, eOmn_info_properties_t* props, const char* extra)
{
    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }
    
    if(NULL == props) 
    {
        return(eores_NOK_nullpointer);
    }
    
    if(eobool_true == eo_vector_Full(s_eo_theinfodispatcher.vectorOfinfostatus))
    {
        // manage an overflow
        s_eo_infodispatcher_overflow_fill(&s_eo_theinfodispatcher);
        return(eores_NOK_generic); 
    }

    
    // prepare the infostatus ...
    
    s_eo_theinfodispatcher.infostatus->basic.timestamp = eov_sys_LifeTimeGet(eov_sys_GetHandle());
    memcpy(&s_eo_theinfodispatcher.infostatus->basic.properties, props, sizeof(eOmn_info_properties_t));
    if(NULL != extra)
    {
        memcpy(s_eo_theinfodispatcher.infostatus->extra, extra, sizeof(s_eo_theinfodispatcher.infostatus->extra));
    }
    else
    {   // must tell that there is no extra
        EOMN_INFO_PROPERTIES_FLAGS_set_extraformat(s_eo_theinfodispatcher.infostatus->basic.properties.flags, eomn_info_extraformat_none);
    }
    
    // put infostatus inside the vector
    
    eo_vector_PushBack(s_eo_theinfodispatcher.vectorOfinfostatus, s_eo_theinfodispatcher.infostatus);
    
    return(eores_OK);
}


extern eOresult_t eo_infodispatcher_Send(EOtheInfoDispatcher* p, uint16_t number, uint16_t* numberofsent)
{
    eOresult_t res = eores_NOK_generic;
    uint16_t nn = 0;
    uint16_t i = 0;
    eOmn_info_status_t *info = NULL;
    
    if(NULL == p) 
    {
        if(NULL != numberofsent)
        {
            *numberofsent = nn;
        }
        return(eores_NOK_nullpointer);
    }
    
    if(0 != s_eo_theinfodispatcher.overflow->basic.timestamp)
    {
        res = s_eo_infodispatcher_transmit(p, s_eo_theinfodispatcher.overflow);      
        if(eores_OK == res)
        {
            s_eo_infodispatcher_overflow_clear(p);
            nn++;
        }
        else
        {
            if(NULL != numberofsent)
            {
                *numberofsent = nn;
            }
            return(eores_OK); 
        }    
    }
    
    for(i=0; i<number; i++)
    {       
        // get the first info in the vector
        info = (eOmn_info_status_t*) eo_vector_Front(s_eo_theinfodispatcher.vectorOfinfostatus);
        if(NULL == info)
        {
            // we dont have anything inside the vector: quit loop
            break;             
        }
        
        // ok, we have a info item: i try to send it
        
        res = s_eo_infodispatcher_transmit(p, info);  
     
        if(eores_OK == res)
        {   
            // ok: i could transmit it. thus, 1. increase number of sent items, and 2. remove the front
            eo_vector_PopFront(s_eo_theinfodispatcher.vectorOfinfostatus);                        
            nn++;
        }
        else
        {   
            // i could not send it: quit
            break; 
        }    

        
    }
    
    
    if(NULL != numberofsent)
    {
        *numberofsent = nn;
    }    
    
    return(eores_OK);       
}




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static void s_eo_infodispatcher_overflow_init(EOtheInfoDispatcher* p)
{
    memset(p->overflow, 0, sizeof(eOmn_info_status_t));
    
    p->overflow->basic.timestamp                = 0;
    p->overflow->basic.properties.code          = eoerror_code_get(eoerror_category_System, eoerror_value_SYS_dispatcherfifooverflow);
    p->overflow->basic.properties.param         = 0;
    
    EOMN_INFO_PROPERTIES_FLAGS_set_type(p->overflow->basic.properties.flags, eomn_info_type_error);
    EOMN_INFO_PROPERTIES_FLAGS_set_source(p->overflow->basic.properties.flags, eomn_info_source_board);
    EOMN_INFO_PROPERTIES_FLAGS_set_address(p->overflow->basic.properties.flags, 0);
    EOMN_INFO_PROPERTIES_FLAGS_set_extraformat(p->overflow->basic.properties.flags, eomn_info_extraformat_none);
    EOMN_INFO_PROPERTIES_FLAGS_set_futureuse(p->overflow->basic.properties.flags, 0);    
        
}


static void s_eo_infodispatcher_overflow_clear(EOtheInfoDispatcher* p)
{   
    p->overflow->basic.timestamp = 0;
    
    p->overflow->basic.properties.param = 0;
}


static void s_eo_infodispatcher_overflow_fill(EOtheInfoDispatcher* p)
{
    // manage an overflow ... but only if another one has not just happened
    if(0 == p->overflow->basic.timestamp)
    {
        p->overflow->basic.timestamp = eov_sys_LifeTimeGet(eov_sys_GetHandle());            
    }
    
    p->overflow->basic.properties.param ++;
}


static eOresult_t s_eo_infodispatcher_transmit(EOtheInfoDispatcher* p, eOmn_info_status_t* info)
{  
#if 1
    
    eOresult_t res = eores_NOK_generic;
    uint16_t size = 0;
    
    //evEntityId_t prev = eventviewer_switch_to(ev_ID_first_usrdef+15);
    
    // when i transmit a say, i want that the nv contains the value, thus ... let me copy the whole status. 
    eo_nv_Set(s_eo_theinfodispatcher.nvinfostatus, info, eobool_true, eo_nv_upd_dontdo);    
     
    if(eomn_info_extraformat_none == EOMN_INFO_PROPERTIES_FLAGS_get_extraformat(info->basic.properties.flags))
    {
        s_eo_theinfodispatcher.rophead->dsiz = sizeof(eOmn_info_basic_t); //eo_nv_Size(s_eo_theinfodispatcher.nvinfostatusbasic); //sizeof(eOmn_info_basic_t);
        s_eo_theinfodispatcher.rophead->id32 = eo_nv_GetID32(s_eo_theinfodispatcher.nvinfostatusbasic);  
        memcpy(s_eo_theinfodispatcher.ropdata, info, sizeof(eOmn_info_basic_t));
        size = p->ropsizeinfostatusbasic;        
    }
    else
    {
        s_eo_theinfodispatcher.rophead->dsiz = sizeof(eOmn_info_status_t); //eo_nv_Size(s_eo_theinfodispatcher.nvinfostatus); //sizeof(eOmn_info_status_t);
        s_eo_theinfodispatcher.rophead->id32 = eo_nv_GetID32(s_eo_theinfodispatcher.nvinfostatus);  
        memcpy(s_eo_theinfodispatcher.ropdata, info, sizeof(eOmn_info_status_t));        
        size = p->ropsizeinfostatus;        
    }
    
    

    res = eo_transmitter_occasional_rops_LoadStream(p->transmitter, p->ropstream, size);
    
    //eventviewer_switch_to(prev);
    
    return(res);
    
#else
    // must: attempt loading the info inside the proper netvar, then into the transmitter as a occasional rop
 
    //evEntityId_t prev = eventviewer_switch_to(ev_ID_first_usrdef+15);
    
    eOresult_t res = eores_NOK_generic;
    eOnvID32_t id32 = eo_nv_ID32dummy;
    
    // i set the whole status
    eo_nv_Set(s_eo_theinfodispatcher.nvinfostatus, info, eobool_true, eo_nv_upd_dontdo);
    
    if(eomn_info_extraformat_none == info->basic.properties.extraformat)
    {
        id32 = eo_nv_GetID32(s_eo_theinfodispatcher.nvinfostatusbasic);
    }
    else
    {
        id32 =  eo_nv_GetID32(s_eo_theinfodispatcher.nvinfostatus);
    }

    eOropdescriptor_t ropdes = {0};
    memcpy(&ropdes, &eok_ropdesc_basic, sizeof(eOropdescriptor_t));
    
    ropdes.ropcode  = eo_ropcode_say;
    ropdes.id32     = id32;
    
    //evEntityId_t prev = eventviewer_switch_to(ev_ID_first_usrdef+15);
    
    res = eo_transmitter_occasional_rops_Load(p->transmitter, &ropdes);
    
  
    //eventviewer_switch_to(prev);
    
    return(res);     
    
#endif    
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





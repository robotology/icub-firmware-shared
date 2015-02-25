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
#include "stdio.h"
#include "EOtheMemoryPool.h"
#include "EOtheErrorManager.h"
#include "EoError.h"
#include "EOnv_hid.h"
#include "EOrop_hid.h"
#include "EOVtheSystem.h"




// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOproxy.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOproxy_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#if defined(EO_TAILOR_CODE_FOR_ARM)
    #define EONV_DONT_USE_EOV_MUTEX_FUNCTIONS
#endif


#if defined(EONV_DONT_USE_EOV_MUTEX_FUNCTIONS)
    #define eov_mutex_Take(a, b)   
    #define eov_mutex_Release(a)
#endif


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

typedef struct              // 24+24=48
{
    eOropdescriptor_t       ropdes; // ropdes.time contains the expiry time ...
    EOnv                    nv;
    eOproxy_params_t        params;
} eo_proxy_ropdes_plus_t;   //EO_VERIFYsizeof(eo_proxy_ropdes_plus_t, 56); 

typedef struct
{
    uint32_t    id32;
    uint32_t    sign;
} eo_proxy_search_key_t;


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_eo_proxy_forward_set(EOproxy *p, EOrop *rop, EOrop *ropout);

static eOresult_t s_eo_proxy_forward_rst(EOproxy *p, EOrop *rop, EOrop *ropout);

static eOresult_t s_eo_proxy_forward_ask(EOproxy *p, EOrop *rop, EOrop *ropout);

static eOresult_t s_matching_rule_id32(void *item, void *param);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOproxy";
 
const eOproxy_cfg_t eo_proxy_cfg_default = 
{
    EO_INIT(.mode)                          eoproxy_mode_disabled,
    EO_INIT(.capacityoflistofropdes)        8, 
    EO_INIT(.replyroptimeout)               10*1000, 
    EO_INIT(.mutex_fn_new)                  NULL,
    EO_INIT(.transceiver)                   NULL
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOproxy* eo_proxy_New(const eOproxy_cfg_t *cfg) 
{    
    EOproxy *retptr = NULL;   

    if(NULL == cfg)
    {    
        cfg = &eo_proxy_cfg_default;
        //eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, s_eobj_ownname, "cfg is NULL");
    }
    
    if(eoproxy_mode_disabled == cfg->mode)
    {
        return(NULL);
    }
    
    // i get the memory for the object
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOproxy), 1);
    
    memcpy(&retptr->config, cfg, sizeof(eOproxy_cfg_t));
    
    // i get the list ...
    
    retptr->transceiver = cfg->transceiver;
    
    retptr->listofropdes    = (0 == cfg->capacityoflistofropdes) ? (NULL) : (eo_list_New(sizeof(eo_proxy_ropdes_plus_t), cfg->capacityoflistofropdes, NULL, 0, NULL, NULL));
    
    if(NULL != cfg->mutex_fn_new)
    {
        retptr->mtx = cfg->mutex_fn_new();
    }
           
    return(retptr);       
}    


extern void eo_proxy_Delete(EOproxy *p) 
{    
    if(NULL == p)
    {
        return;
    }
    
    if(NULL != p->mtx)
    {
        eov_mutex_Delete(p->mtx);
    }
    
    if(NULL != p->listofropdes)
    {
        eo_list_Delete(p->listofropdes);
    }
   
    memset(p, 0, sizeof(EOproxy));
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;   
}    


extern eOresult_t eo_proxy_ROP_Forward(EOproxy *p, EOrop* rop, EOrop* ropout)
{
    eOresult_t res = eores_NOK_generic;
    EOnv *nv = NULL;
    eOerrmanDescriptor_t errdes = {0};
	errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
    errdes.sourceaddress    = 0;
    errdes.code             = eoerror_code_get(eoerror_category_System, eoerror_value_SYS_proxy_forward_fails);
    errdes.par16            = 0; 
    errdes.par64            = 0; 
	 
    
    if((NULL == p) || (NULL == rop))
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        return(eores_NOK_nullpointer);
    }

    nv = &rop->netvar;
    
    if(eobool_false == eo_nv_IsProxied(nv))
    {
        errdes.par16 = (eo_list_Capacity(p->listofropdes) << 8) | (eo_list_Size(p->listofropdes));
        errdes.par64 = ((uint64_t)rop->ropdes.signature << 32) | (rop->ropdes.id32);
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        return(eores_NOK_generic);
    }

    switch(rop->ropdes.ropcode)
    {
        case eo_ropcode_ask:
        {
            res = s_eo_proxy_forward_ask(p, rop, ropout);
        } break;
            
        case eo_ropcode_set:
        {
            res = s_eo_proxy_forward_set(p, rop, ropout);
        } break; 

        case eo_ropcode_rst:
        {
            res = s_eo_proxy_forward_rst(p, rop, ropout);
        } break; 

        default:
        {
            res = eores_NOK_generic;
        } break;
        
    }
    
    if(eores_OK != res)
    {
        errdes.par16 = (eo_list_Capacity(p->listofropdes) << 8) | (eo_list_Size(p->listofropdes));
        errdes.par64 = ((uint64_t)rop->ropdes.signature << 32) | (rop->ropdes.id32);
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);       
    }
    
    return(res);
}

extern eOproxy_params_t * eo_proxy_Params_Get(EOproxy *p, eOnvID32_t id32)
{
    eOproxy_params_t *par = NULL;
    
    EOlistIter* li = NULL;
    eo_proxy_search_key_t skey = {0};
    eo_proxy_ropdes_plus_t *item = NULL;
    
    eOerrmanDescriptor_t errdes = {0};
	errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
    errdes.sourceaddress    = 0;
    errdes.code             = eoerror_code_get(eoerror_category_System, eoerror_value_SYS_proxy_ropdes_notfound);
    errdes.par16            = 0; 
    errdes.par64            = 0; 

    skey.id32 = id32;
    skey.sign = EOK_uint32dummy;
    
    if(NULL == p)
    {
        //eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        return(par);
    }
    
    eov_mutex_Take(p->mtx, eok_reltimeINFINITE);
    
    li = eo_list_Find(p->listofropdes, s_matching_rule_id32, &skey);

    if(NULL == li)
    {   // there is no entry with id32 in the list ... i cannot give teh param back
        eov_mutex_Release(p->mtx);
        
        errdes.par16 = (eo_list_Capacity(p->listofropdes) << 8) | (eo_list_Size(p->listofropdes));
        errdes.par64 = (id32);
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        
        return(par);
    }
    
    item = (eo_proxy_ropdes_plus_t*) eo_list_At(p->listofropdes, li);       
    eov_mutex_Release(p->mtx);   

    return(&item->params);   
}


extern eOresult_t eo_proxy_ReplyROP_Load(EOproxy *p, eOnvID32_t id32, void *data)
{
    eOresult_t res = eores_NOK_generic;
    EOlistIter* li = NULL;
    eo_proxy_search_key_t skey = {0};
    eo_proxy_ropdes_plus_t *item = NULL;
    eOerrmanDescriptor_t errdes = {0};
	errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
    errdes.sourceaddress    = 0;
    errdes.code             = eoerror_code_get(eoerror_category_System, eoerror_value_SYS_proxy_reply_fails);
    errdes.par16            = 0; 
    errdes.par64            = 0; 

    skey.id32 = id32;
    skey.sign = EOK_uint32dummy;
        
    if(NULL == p)
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        return(eores_NOK_nullpointer);
    }
        
    eov_mutex_Take(p->mtx, eok_reltimeINFINITE);
    
    li = eo_list_Find(p->listofropdes, s_matching_rule_id32, &skey);

    if(NULL == li)
    {   // there is no entry with id32 in the list ... i dont load any reply rop
        eov_mutex_Release(p->mtx);
        
        errdes.par16 = (eo_list_Capacity(p->listofropdes) << 8) | (eo_list_Size(p->listofropdes));
        //errdes.par64 = ((uint64_t)signature << 32) | (id32);
        errdes.par64 = (id32);
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
        
        return(eores_NOK_generic);
    }
    
    item = (eo_proxy_ropdes_plus_t*) eo_list_At(p->listofropdes, li);
    
    if(NULL != data)
    {
        eo_nv_Set(&item->nv, data, eobool_true, eo_nv_upd_dontdo);   
    }

    // item->ropdes is already ok (opc is say, id32 is ..., etc.) ... i just change the time
    item->ropdes.time = 0;    
    res = eo_transceiver_ReplyROP_Load(p->config.transceiver, &item->ropdes);
    if(eores_OK != res)
    {
        errdes.par16 = 0;
        //errdes.par64 = ((uint64_t)signature << 32) | (id32);
        errdes.par64 = (id32);
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, NULL, NULL, &errdes);
    }
    
    eo_list_Erase(p->listofropdes, li);
    
    eov_mutex_Release(p->mtx);
    
    return(res);    
}
    
    
extern eOresult_t eo_proxy_Tick(EOproxy *p)
{   
    eo_proxy_ropdes_plus_t* item = NULL;
    eOabstime_t timenow = 0;

    if(NULL == p)
    {
        return(eores_NOK_nullpointer);
    }
    
    timenow = eov_sys_LifeTimeGet(eov_sys_GetHandle());
    
    eov_mutex_Take(p->mtx, eok_reltimeINFINITE);
    
    // i assume that the items are in expiry order, thus i get the front and i keep on removing until timenow is higher than item->ropdes.time          
    item = (eo_proxy_ropdes_plus_t*) eo_list_Front(p->listofropdes);   
    while((NULL != item) && (timenow > item->ropdes.time))
    {
        eo_list_PopFront(p->listofropdes);
        item = (eo_proxy_ropdes_plus_t*) eo_list_Front(p->listofropdes);
    }
    
    eov_mutex_Release(p->mtx);

    return(eores_OK);    
}    


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static eOresult_t s_eo_proxy_forward_set(EOproxy *p, EOrop *rop, EOrop *ropout)
{
    EOnv *nv = &rop->netvar;
    eOropdescriptor_t* ropdes = &rop->ropdes;
    eOresult_t res = eo_nv_hid_SetROP(nv, ropdes->data, eo_nv_upd_ifneeded, ropdes);   
    
    // if operation was successfull, ... fill a possible reply 
    if(eores_OK == res)  
    {                
        // mark the ropcode of the reply (if any) to be ack
        if((1 == rop->stream.head.ctrl.rqstconf) && (NULL != ropout))
        {
            ropout->stream.head.ctrl.confinfo = eo_ropconf_ack;
        } 
    }    
    
    return(res);
}


static eOresult_t s_eo_proxy_forward_rst(EOproxy *p, EOrop *rop, EOrop *ropout)
{
    EOnv *nv = &rop->netvar;
    eOropdescriptor_t* ropdes = &rop->ropdes;
    eOresult_t res = eo_nv_hid_ResetROP(nv, eo_nv_upd_ifneeded, ropdes);
    
    // if operation was successfull, ... fill a possible reply 
    if(eores_OK == res)  
    {                
        // mark the ropcode of the reply (if any) to be ack
        if((1 == rop->stream.head.ctrl.rqstconf) && (NULL != ropout))
        {
            ropout->stream.head.ctrl.confinfo = eo_ropconf_ack;
        } 
    }    
     
    return(res);
}

static eOresult_t s_eo_proxy_forward_ask(EOproxy *p, EOrop *rop, EOrop *ropout)
{
    EOnv *nv = &rop->netvar;
    eOropdescriptor_t* ropdes = &rop->ropdes;
    eOresult_t res = eores_NOK_generic;
    eo_proxy_ropdes_plus_t ropdesplus;
    eOabstime_t timenow = eov_sys_LifeTimeGet(eov_sys_GetHandle());;
     
    eov_mutex_Take(p->mtx, eok_reltimeINFINITE);
    
    if(eobool_true != eo_list_Full(p->listofropdes))
    {   // we can process the ask        
        res = eores_OK;       
    }
    else
    {
        res = eores_NOK_generic;    
    }
    
    
    if(eores_OK != res)
    {
        // in such a case, if the ask was with ack-required ... we dont send a nak
        #define DONT_SEND_NAK
        #if defined(DONT_SEND_NAK)
        eo_rop_Reset(ropout); 
        #else
        // mark the ropcode of the reply (if any) to be nak
        if((1 == rop->stream.head.ctrl.rqstconf) && (NULL != ropout))
        {
            ropout->stream.head.ctrl.confinfo = eo_ropconf_nak;
        } 
        #endif       
        
        eov_mutex_Release(p->mtx);
        return(res);
    }
    
    // if we can process the ask we dont send a roput, not even a ack. thus we reset the rop so that the caller of roxy object cannot send it out
    eo_rop_Reset(ropout); 
       
    // we prepare the ropdes for transmission.
    ropdesplus.ropdes.control.confinfo  = eo_ropconf_none;
    ropdesplus.ropdes.control.plustime  = ropdes->control.rqsttime;
    ropdesplus.ropdes.control.plussign  = ropdes->control.plussign;
    ropdesplus.ropdes.control.rqsttime  = 0;
    ropdesplus.ropdes.control.rqstconf  = 0;
    ropdesplus.ropdes.control.version   = ropdes->control.version;
    ropdesplus.ropdes.ropcode           = eo_ropcode_say;
    ropdesplus.ropdes.size              = 0;
    ropdesplus.ropdes.id32              = ropdes->id32;
    ropdesplus.ropdes.data              = NULL;
    ropdesplus.ropdes.signature         = ropdes->signature;
    // in ropdes.time we put the time at which we want the entry to expire    
    if(eok_reltimeINFINITE == p->config.replyroptimeout)
    {
        ropdesplus.ropdes.time = EOK_uint64dummy;   // so that the the check of higher than any measured time always gives false
    } 
    else
    {
        ropdesplus.ropdes.time = timenow + p->config.replyroptimeout;
    }    

    // we copy the nv
    memcpy(&ropdesplus.nv, nv, sizeof(EOnv));
    
    // clear the param
    memset(&ropdesplus.params, 0, sizeof(ropdesplus.params));
       
    // now we insert the item in the list according to expiry time. 
    // since the timeouts  are all equal, i just append the item at the end of the list
    eo_list_PushBack(p->listofropdes, &ropdesplus);
     
    eov_mutex_Release(p->mtx); 

    // now we update the nv. it is in here so that inside the callback we can send the reply
    res = eo_nv_hid_UpdateROP(nv, eo_nv_upd_always, ropdes);
    
    return(res);
}


static eOresult_t s_matching_rule_id32(void *item, void *param)
{
    eo_proxy_ropdes_plus_t *inside = (eo_proxy_ropdes_plus_t*)item;
    eo_proxy_search_key_t *skeyptr = (eo_proxy_search_key_t*)param;
    uint32_t insidesignature = (EOK_uint32dummy == skeyptr->sign) ? (EOK_uint32dummy) : (inside->ropdes.signature);

    if((inside->ropdes.id32 == skeyptr->id32) && (insidesignature == skeyptr->sign))
    {
        return(eores_OK);
    }
    else
    {
        return(eores_NOK_generic);
    }
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





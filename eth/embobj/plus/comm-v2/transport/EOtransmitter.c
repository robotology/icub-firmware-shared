/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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
#include "EOtheParser.h"
#include "EOtheFormer.h"
#include "EOropframe_hid.h"
#include "EOnv_hid.h"
#include "EOrop_hid.h"
#include "EOVtheSystem.h"
#include "EOtheErrorManager.h"
#include "EOvector.h"
#include "EoProtocol.h"
#include "EOVmutex.h"
#include "EOlist.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOtransmitter.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOtransmitter_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#if defined(EO_TAILOR_CODE_FOR_ARM)
//    #define EONV_DONT_USE_EOV_MUTEX_FUNCTIONS
#endif


#if defined(EONV_DONT_USE_EOV_MUTEX_FUNCTIONS)
    #define eov_mutex_Take(a, b)   
    #define eov_mutex_Release(a)
#endif



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

//static eOresult_t s_eo_transmitter_listmatching_rule(void *item, void *param);

static eOresult_t s_eo_transmitter_entitymatchingrule_rule(void *item, void *param);

static eOresult_t s_eo_transmitter_ropmatchingrule_rule(void *item, void *param);

static void s_eo_transmitter_list_updaterop_in_ropframe(void *item, void *param);

static void s_eo_transmitter_list_shiftdownropinfo(void *item, void *param);

static eOresult_t s_eo_transmitter_rops_Load(EOtransmitter *p, eOropdescriptor_t* ropdesc, EOropframe* intoropframe, EOVmutexDerived *mtx);

static EOropframe * s_eo_transmitter_id32_to_typeofregulars(EOtransmitter* p, eOprotID32_t id32, eo_transm_regropframe_t *ropframetype);

static EOropframe * s_eo_transmitter_get_cycled_regropframe(EOtransmitter* p, uint16_t *ropsinside);


static uint16_t s_eo_transmitter_get_maxsizeof_regularsropframe(EOtransmitter *p);

static eObool_t s_eo_transmitter_regulars_canadd_rop(EOtransmitter *p, eo_transm_regropframe_t type, uint16_t ropbytes);

static void s_eo_transmitter_regulars_reset_sizes(EOtransmitter *p);

static void s_eo_transmitter_regulars_update_sizes(EOtransmitter *p, eo_transm_regropframe_t type, int16_t ropbytes);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOtransmitter";

const eOtransmitter_cfg_t eo_transmitter_cfg_default = 
{
    EO_INIT(.sizes)
    {
        EO_INIT(.capacityoftxpacket)            512, 
        EO_INIT(.capacityofropframeregulars)    256, 
        EO_INIT(.capacityofropframeoccasionals) 256,
        EO_INIT(.capacityofropframereplies)     256,
        EO_INIT(.capacityofrop)                 128, 
        EO_INIT(.maxnumberofregularrops)        16
    },
    EO_INIT(.ipv4addr)                      EO_COMMON_IPV4ADDR_LOCALHOST,
    EO_INIT(.ipv4port)                      10001,
    EO_INIT(.protection)                    eo_transmitter_protection_none,
    EO_INIT(.mutex_fn_new)                  NULL,
    EO_INIT(.agent)                         NULL
};


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


// marco.accame on 30oct15: see following comment about how many bytes in regulars
// we can tx at most p->effectivecapacityofregulars = (cfg->sizes.capacityofropframeregulars-28) bytes in regulars inside the ropframe to tx.
// when we form the regulars they can be:
// a. the _standard only,         [when for instance we dont have motion control as in boards with skin only]
// b. the _cycle0of only,         [if we launch only motion control device]
// c. the _cycle1of only,         [if we launch only motion control for left/right hand].
// d. the concatenation of _standard and _cycle0of,    [in most cases when motion control device is launched together with another device] 
// e. the concatenation of _standard and _cycle1of.    [for left/rigth hand motion control plus another device (skin or mais)].
// thus, how do we verify that we can accept a regular rop? in two ways:
// 1. we check that their number is lower than cfg->sizes.maxnumberofregularrops (which is the capacity of list listofregropinfo),
// 2. we must check that the totalsize of bytes used by the regulars in any combination a, .., e is lower than effectivecapacityofregulars = (capacityofropframeregulars-28)
//    the total max size is thus ... sizeof_standard + max(sizeof_cycle0of, sizeof_cycle1of). and i must keep updated these three sizes.
// moreover, i may have the rops distributed not evenly in these three containers. how do i partition them? best case is to give p->effectivecapacityofregulars to the three of them.
// in this way i can allocate all the space in the udp packet in only one container. for instance, i can create a larger skin status....
// on the other hand we may waste memory. a good compromise is using 75% for all. see TAG(*1234*)
// marco.accame on 03july2025: i dont care anymore to waste memory, i prefer to allocate max capacity for the mc or else -> use 100%
#define EOTRANSMITTER_ROPFRAMES_use100percent

 
extern EOtransmitter* eo_transmitter_New(const eOtransmitter_cfg_t *cfg)
{
    EOtransmitter *retptr = NULL;   
    uint16_t capacityofregularsubframes = 0;

    if(NULL == cfg)
    {    
        cfg = &eo_transmitter_cfg_default;
    }
    
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != cfg->agent), "eo_transmitter_New(): NULL agent", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    
    eo_errman_Assert(eo_errman_GetHandle(), (cfg->sizes.capacityoftxpacket > eo_ropframe_sizeforZEROrops), "eo_transmitter_New(): capacityoftxpacket is too small", s_eobj_ownname, &eo_errman_DescrWrongParamLocal); 
    
    // i get the memory for the object
    retptr = (EOtransmitter*) eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOtransmitter), 1);
    
    retptr->txpacket                = eo_packet_New(cfg->sizes.capacityoftxpacket);
    retptr->ropframereadytotx       = eo_ropframe_New();
    retptr->ropframeregulars_standard  = eo_ropframe_New();
    retptr->ropframeregulars_cycle0of  = eo_ropframe_New();
    retptr->ropframeregulars_cycle1of  = eo_ropframe_New();
    retptr->ropframeregulars_cycle2of  = eo_ropframe_New();
    retptr->ropframeoccasionals     = eo_ropframe_New();
    retptr->ropframereplies         = eo_ropframe_New();
    retptr->roptmp                  = eo_rop_New(cfg->sizes.capacityofrop);
    retptr->agent                   = cfg->agent;
    retptr->nvset                   = eo_agent_GetNVset(cfg->agent);
    retptr->confmanager             = eo_agent_GetConfirmationManager(cfg->agent);
    retptr->ipv4addr                = cfg->ipv4addr;
    retptr->ipv4port                = cfg->ipv4port;
    // TAG(*1234*) : begin
    // i split capacityofropframeregulars into equal parts for the three buffers. however, we have eo_ropframe_sizeforZEROrops which is minimum number.
#if defined(EOTRANSMITTER_ROPFRAMES_use100percent)
    capacityofregularsubframes = cfg->sizes.capacityofropframeregulars;
#else
    capacityofregularsubframes = eo_ropframe_capacity2effectivecapacity(3*cfg->sizes.capacityofropframeregulars/4) + eo_ropframe_sizeforZEROrops;
#endif    
    // TAG(*1234*) : end
    retptr->bufferropframeregulars_standard = (0 == capacityofregularsubframes) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, capacityofregularsubframes, 1));
    retptr->bufferropframeregulars_cycle0of = (0 == capacityofregularsubframes) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, capacityofregularsubframes, 1));
    retptr->bufferropframeregulars_cycle1of = (0 == capacityofregularsubframes) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, capacityofregularsubframes, 1));
    retptr->bufferropframeregulars_cycle2of = (0 == capacityofregularsubframes) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, capacityofregularsubframes, 1));
    
    retptr->bufferropframeoccasionals = (0 == cfg->sizes.capacityofropframeoccasionals) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, cfg->sizes.capacityofropframeoccasionals, 1));
    retptr->bufferropframereplies   = (0 == cfg->sizes.capacityofropframereplies) ? (NULL) : ((uint8_t*)eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, cfg->sizes.capacityofropframereplies, 1));
    retptr->listofregropinfo        = (0 == cfg->sizes.maxnumberofregularrops) ? (NULL) : (eo_list_New(sizeof(eo_transm_regrop_info_t), cfg->sizes.maxnumberofregularrops, NULL, 0, NULL, NULL));
    retptr->currenttime             = 0;
    retptr->tx_seqnum               = 0;

    eo_ropframe_Load(retptr->ropframeregulars_standard, retptr->bufferropframeregulars_standard, eo_ropframe_sizeforZEROrops, capacityofregularsubframes);
    eo_ropframe_Clear(retptr->ropframeregulars_standard);
    eo_ropframe_Load(retptr->ropframeregulars_cycle0of, retptr->bufferropframeregulars_cycle0of, eo_ropframe_sizeforZEROrops, capacityofregularsubframes);
    eo_ropframe_Clear(retptr->ropframeregulars_cycle0of);    
    eo_ropframe_Load(retptr->ropframeregulars_cycle1of, retptr->bufferropframeregulars_cycle1of, eo_ropframe_sizeforZEROrops, capacityofregularsubframes);
    eo_ropframe_Clear(retptr->ropframeregulars_cycle1of);    
    eo_ropframe_Load(retptr->ropframeregulars_cycle2of, retptr->bufferropframeregulars_cycle2of, eo_ropframe_sizeforZEROrops, capacityofregularsubframes);
    eo_ropframe_Clear(retptr->ropframeregulars_cycle2of);      
    eo_ropframe_Load(retptr->ropframeoccasionals, retptr->bufferropframeoccasionals, eo_ropframe_sizeforZEROrops, cfg->sizes.capacityofropframeoccasionals);
    eo_ropframe_Clear(retptr->ropframeoccasionals);
    eo_ropframe_Load(retptr->ropframereplies, retptr->bufferropframereplies, eo_ropframe_sizeforZEROrops, cfg->sizes.capacityofropframereplies);
    eo_ropframe_Clear(retptr->ropframereplies);


    {   // we set the content of ropframereadytotx with the same memory used by txpacket, so that when we operate on 
        // ropframereadytotx then we prepare the txpacket.
        uint8_t *data;
        uint16_t size;
        uint16_t capacity;
        
        eo_packet_Payload_Get(retptr->txpacket, &data, &size);
        eo_packet_Capacity_Get(retptr->txpacket, &capacity);
    
        eo_ropframe_Load(retptr->ropframereadytotx, data, eo_ropframe_sizeforZEROrops, capacity); // dont use size because size is now zero.
        eo_ropframe_Clear(retptr->ropframereadytotx);
        
        if(eobool_true != eo_ropframe_IsValid(retptr->ropframereadytotx))
        {
            eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_transmitter_New(): ropframeready2tx is not valid", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
        }

        // the destination ipv4addr and ipv4port are constant and are the ones passed through configuration
        eo_packet_Addressing_Set(retptr->txpacket, retptr->ipv4addr, retptr->ipv4port);
    } 

    if((NULL != cfg->mutex_fn_new) && (eo_transmitter_protection_total == cfg->protection))
    {
        retptr->mtx_replies     = cfg->mutex_fn_new();
        retptr->mtx_regulars    = cfg->mutex_fn_new();
        retptr->mtx_occasionals = cfg->mutex_fn_new(); 
        retptr->mtx_roptmp      = cfg->mutex_fn_new();
    }
    else
    {
        retptr->mtx_replies     = NULL;
        retptr->mtx_regulars    = NULL;
        retptr->mtx_occasionals = NULL;
        retptr->mtx_roptmp      = NULL;
    }
    
#if defined(USE_DEBUG_EOTRANSMITTER)
    // DEBUG
    retptr->debug.txropframeistoobigforthepacket = 0;
#endif
    
    retptr->lasterror = 0;
    retptr->lasterror_info0 = 0;
    retptr->lasterror_info1 = 0;
    retptr->lasterror_info2 = 0;
    
    retptr->txdecimationprogressive = 0;
    retptr->txdecimationreplies = 1;
    retptr->txdecimationoccasionals = 1;
    retptr->txdecimationregulars = 1;

    s_eo_transmitter_regulars_reset_sizes(retptr);
    
    retptr->effectivecapacityofregulars = eo_ropframe_capacity2effectivecapacity(cfg->sizes.capacityofropframeregulars);
    retptr->txregularsprogressive = 0;
    
    return(retptr);
}


extern void eo_transmitter_Delete(EOtransmitter *p)
{
    if(NULL == p)
    {
        return;
    }
    
    if(NULL == p->txpacket)
    {
        return;
    }
    
    if(NULL != p->mtx_replies)
    {
        eov_mutex_Delete(p->mtx_replies);
    }
    if(NULL != p->mtx_regulars)
    {
        eov_mutex_Delete(p->mtx_regulars);
    }
    if(NULL != p->mtx_occasionals)
    {
        eov_mutex_Delete(p->mtx_occasionals);
    }
    if(NULL != p->mtx_roptmp)
    {
        eov_mutex_Delete(p->mtx_roptmp);        
    }   

    if(NULL != p->listofregropinfo)
    {
        eo_list_Delete(p->listofregropinfo);
    }     
    if(NULL != p->bufferropframeregulars_standard)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(), p->bufferropframeregulars_standard);
        p->bufferropframeregulars_standard = NULL;
    }
    if(NULL != p->bufferropframeregulars_cycle0of)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(), p->bufferropframeregulars_cycle0of);
        p->bufferropframeregulars_cycle0of = NULL;
    } 
    if(NULL != p->bufferropframeregulars_cycle1of)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(), p->bufferropframeregulars_cycle1of);
        p->bufferropframeregulars_cycle1of = NULL;
    } 
    if(NULL != p->bufferropframeregulars_cycle2of)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(), p->bufferropframeregulars_cycle2of);
        p->bufferropframeregulars_cycle2of = NULL;
    }     
    if(NULL != p->bufferropframeoccasionals)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(),  p->bufferropframeoccasionals);
        p->bufferropframeoccasionals = NULL;
    }
    if(NULL != p->bufferropframereplies)
    {
        eo_mempool_Delete(eo_mempool_GetHandle(),  p->bufferropframereplies);
        p->bufferropframereplies = NULL;
    }  
    
    eo_rop_Delete(p->roptmp);
    
    eo_ropframe_Delete(p->ropframereadytotx);
    eo_ropframe_Delete(p->ropframeregulars_standard);
    eo_ropframe_Delete(p->ropframeregulars_cycle0of);
    eo_ropframe_Delete(p->ropframeregulars_cycle1of);
    eo_ropframe_Delete(p->ropframeregulars_cycle2of);
    eo_ropframe_Delete(p->ropframeoccasionals);
    eo_ropframe_Delete(p->ropframereplies);
   
    eo_packet_Delete(p->txpacket);
        
    memset(p, 0, sizeof(EOtransmitter));
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;
}


extern EOnvSet* eo_transmitter_GetNVset(EOtransmitter *p)
{
    if(NULL == p) 
    {
        return(NULL);
    }  

    return(p->nvset);
}


extern eOsizecntnr_t eo_transmitter_regular_rops_Size(EOtransmitter *p)
{
    eOsizecntnr_t size = 0;
    
    if(NULL == p) 
    {
        return(0);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(0);
    }
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    size = eo_list_Size(p->listofregropinfo);

    eov_mutex_Release(p->mtx_regulars);
    
    return(size);   
}


extern eOsizecntnr_t eo_transmitter_regular_rops_Size_with_ep(EOtransmitter *p, eOnvEP8_t ep)
{
    eOsizecntnr_t retvalue = 0;
    uint16_t size = 0;
    uint32_t i = 0;
    EOlistIter* li = NULL;
    eOnvID32_t id32 = 0;
    
    if(NULL == p) 
    {
        return(0);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(0);
    }

    if(eoprot_endpoint_all == ep)
    {
        return(eo_transmitter_regular_rops_Size(p));
    }

    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    size = eo_list_Size(p->listofregropinfo);
    li = eo_list_Begin(p->listofregropinfo);
    for(i=0; i<size; i++) 
    { 
        eo_transm_regrop_info_t *item = (eo_transm_regrop_info_t*) eo_list_At(p->listofregropinfo, li);
        li = eo_list_Next(p->listofregropinfo, li);
        
        id32 = eo_nv_GetID32(&item->thenv);
        if(ep == eoprot_ID2endpoint(id32))
        {
            retvalue ++;
        }               
    }            

    eov_mutex_Release(p->mtx_regulars);

    
    return(retvalue);   
}


extern eOresult_t eo_transmitter_regular_rops_arrayid32_Get(EOtransmitter *p, uint16_t start, EOarray* array)
{
    uint16_t size = 0;
    uint16_t array_capacity = 0;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_NOK_nullpointer);
    }
    
    if(sizeof(eOnvID32_t) != eo_array_ItemSize(array))
    {
        return(eores_NOK_generic);
    }
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    size = eo_list_Size(p->listofregropinfo);
    array_capacity = eo_array_Capacity(array);
    array_capacity = array_capacity;
    
    eo_array_Reset(array);
    
    if(start > size)
    {
        // do nothing
    }
    else
    {
        eOnvID32_t id32 = 0;
        uint32_t count = 0;
        uint32_t i=0;
        EOlistIter* li = eo_list_Begin(p->listofregropinfo);
        for(i=0; i<size; i++)
        { 
            eo_transm_regrop_info_t *item = (eo_transm_regrop_info_t*) eo_list_At(p->listofregropinfo, li);
            li = eo_list_Next(p->listofregropinfo, li);
            id32 = eo_nv_GetID32(&item->thenv);
            
            //if(ep == eoprot_ID2endpoint(id32))
            {
                count ++;
                if(count > start)
                {
                    eo_array_PushBack(array, &id32); 
                }
            }         
        }  
    }

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}


extern eOresult_t eo_transmitter_regular_rops_arrayid32_ep_Get(EOtransmitter *p, eOnvEP8_t ep, uint16_t start, EOarray* array)
{
    uint16_t size = 0;
    uint16_t array_capacity = 0;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_NOK_nullpointer);
    }
    
    if(sizeof(eOnvID32_t) != eo_array_ItemSize(array))
    {
        return(eores_NOK_generic);
    }

    if(eoprot_endpoint_all == ep)
    {
        return(eo_transmitter_regular_rops_arrayid32_Get(p, start, array));
    }

    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    size = eo_list_Size(p->listofregropinfo);
    array_capacity = eo_array_Capacity(array);
    array_capacity = array_capacity;
    
    eo_array_Reset(array);
    
    if(start > size)
    {
        // do nothing
    }
    else
    {      
        eOnvID32_t id32 = 0;
        uint32_t count = 0;
        uint32_t i=0;
        EOlistIter* li = eo_list_Begin(p->listofregropinfo);
        for(i=0; i<size; i++)
        { 
            eo_transm_regrop_info_t *item = (eo_transm_regrop_info_t*) eo_list_At(p->listofregropinfo, li);
            li = eo_list_Next(p->listofregropinfo, li);
            id32 = eo_nv_GetID32(&item->thenv);
            
            if(ep == eoprot_ID2endpoint(id32))
            {
                count ++;
                if(count > start)
                {
                    eo_array_PushBack(array, &id32); 
                }
            }         
        }            
    }

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}


extern eOresult_t eo_transmitter_regular_rops_Load(EOtransmitter *p, eOropdescriptor_t* ropdesc)
{
    eo_transm_regrop_info_t regropinfo;
    eOropdescriptor_t ropdescriptor;
    eOresult_t res;
    uint16_t usedbytes;
    uint16_t remainingbytes;
    uint16_t ropstarthere;
    uint16_t ropsize;
    EOnv nv;
    EOnv* tmpnvptr = NULL;
    eo_transm_regropframe_t regropframe2use_type = eo_transm_regropframe_standard;
    EOropframe* regropframe2use = NULL;

    if((NULL == p) || (NULL == ropdesc)) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {    // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_NOK_generic);
    }
    
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);

    // work on the list ...     
    if(eobool_true == eo_list_Full(p->listofregropinfo))
    {   // we have reached cfg->maxnumberofregularrops
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);
    }
    

    // for searching inside listofregropinfo we need only those three fields: ropcode, id, ep
    ropdescriptor.ropcode   = ropdesc->ropcode;
    ropdescriptor.id32      = ropdesc->id32;

    
    // search for ropcode+ep+id. if found, then ... return a non NULL iterator and dont do anything because it means that the rop is already inside
    if(NULL != eo_list_Find(p->listofregropinfo, s_eo_transmitter_ropmatchingrule_rule, &ropdescriptor))
    {   // it is already inside ...
        eov_mutex_Release(p->mtx_regulars);
        return(eores_OK);
    }    
    
    // else ... prepare a temporary variable eo_transm_regrop_info_t to be put inside the list.
    // and wait success of rop + insetrtion in frame
    
    memcpy(&ropdescriptor, ropdesc, sizeof(eOropdescriptor_t));
    ropdescriptor.control.rqstconf  = 0;                // VERY IMPORTANT: the regulars cannot ask for confirmation.
    ropdescriptor.control.confinfo  = eo_ropconf_none;  // VERY IMPORTANT: the regulars cannot be a ack/nack
    ropdescriptor.control.version   = EOK_ROP_VERSION_0;
    
      
    res = eo_nvset_NV_Get(  (p->nvset),  
                            ropdescriptor.id32,
                            &nv
                            );   

    // if the nvset does not have the triple (ip, ep, id) then we return an error because we cannot form the rop
    if(eores_OK != res)
    {
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);
    } 

    // force size to be coherent with the nv. the size is always used, even if there is no data to transmit
    ropdescriptor.size = eo_nv_Size(&nv);    
    
    // now we have the nv. we set its value in local ram
    if(eobool_true == eo_rop_ropcode_has_data(ropdescriptor.ropcode))
    { 
        eOnvOwnership_t nvownership = eo_rop_get_ownership(ropdescriptor.ropcode, eo_ropconf_none, eo_rop_dir_outgoing);        
        if(eo_nv_ownership_local == nvownership)
        {   // if the nv is local, then take data from nv, thus no need to write the data field of the nv using ropdescriptor.data.
            ropdescriptor.data = NULL;   // set ropdescriptor.data to NULL to force eo_agent_OutROPfromNV() to get data from EOnv
        }
        else
        {   // if the nv is remote, then the data must be passed inside ropdescriptor.data
            
            // so far we dont support that the device regularly sends commands such as set<remotevar, value>. it can send ask<remotevar> however.
            // marco.accame on Nov 17 2014: it can regularly sends a ask<remotevar>, even if this mechanisms is not used ... and maybe will never be used ...
            eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, "eo_transmitter_regular_rops_Load(): cant load a regular ROP of remote variable w/ payload", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);
            
            eov_mutex_Release(p->mtx_regulars);
            return(eores_NOK_generic);
            
            // however, if we allow a sending of rop<remotevar, value> ... we must have a descriptor.data not NULL
            //if(NULL == ropdescriptor.data)
            //{
            //    eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_transmitter_regular_rops_Load(): cant have NULL ropdes->data if nv is remote", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);
            //}          
        }
    }
    else
    {   // dont need to send data
        ropdescriptor.data = NULL;
    }

    // lock tmprop
    eov_mutex_Take(p->mtx_roptmp, eok_reltimeINFINITE);
    
    res = eo_agent_OutROPprepare(p->agent, &nv, &ropdescriptor, p->roptmp, &usedbytes);   
    
    // if we cannot prepare the rop ... we quit
    if(eores_OK != res)
    {
        eov_mutex_Release(p->mtx_roptmp);
        eov_mutex_Release(p->mtx_regulars);
        return(res);
    }
    

    // extract the reference to the associated netvar
    tmpnvptr = eo_rop_GetNV(p->roptmp);
    

    // choose the relevant regular ropframe. that depends on the id32 of the ropdescriptor 
    regropframe2use = s_eo_transmitter_id32_to_typeofregulars(p, ropdescriptor.id32, &regropframe2use_type);
    
    // see if we have space for this rop. as we transmit always a standard with one between cycled0of / cycled1of, we need verify
    // with knowledge of regropframe2use_type and of usedbytes. 
    if(eobool_false == s_eo_transmitter_regulars_canadd_rop(p, regropframe2use_type, usedbytes))
    {   // cannot load the rop because we dont have usedbytes anymore
        eov_mutex_Release(p->mtx_roptmp);
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);        
    }
           
    // put the rop inside the relevant regular ropframe         
    res = eo_ropframe_ROP_Add(regropframe2use, p->roptmp, &ropstarthere, &ropsize, &remainingbytes);
    // if we cannot add the rop, then we quit ....
    if(eores_OK != res)
    {
        eov_mutex_Release(p->mtx_roptmp);
        eov_mutex_Release(p->mtx_regulars);
        return(res);
    }
    
    // i am sure that ropsize is equal to usedbytes, thus i dont verify with an assert ...
    
    // 3. prepare a regropinfo variable to be put inside the list    
    regropinfo.ropcode                  = ropdescriptor.ropcode;    
    regropinfo.hasdata2update           = eo_rop_datafield_is_present(&(p->roptmp->stream.head)); 
    regropinfo.regropframetype          = regropframe2use_type;
    regropinfo.ropframe                 = regropframe2use;
    regropinfo.ropstarthere             = ropstarthere;
    regropinfo.ropsize                  = ropsize;
    regropinfo.timeoffsetinsiderop      = (0 == p->roptmp->stream.head.ctrl.plustime) ? (EOK_uint16dummy) : (ropsize - 8); //if we have time, then it is in teh last 8 bytes
    memcpy(&regropinfo.thenv, tmpnvptr, sizeof(EOnv));


    // push back regropinfo inside the list.
    eo_list_PushBack(p->listofregropinfo, &regropinfo);
    
    // increment size of the relevant regular ropframe
    s_eo_transmitter_regulars_update_sizes(p, regropframe2use_type, +regropinfo.ropsize); // with a + we increment
    
    eov_mutex_Release(p->mtx_roptmp);
    eov_mutex_Release(p->mtx_regulars);  
    
    return(eores_OK);   
}


extern eOresult_t eo_transmitter_regular_rops_Unload(EOtransmitter *p, eOropdescriptor_t* ropdesc)//eOropcode_t ropcode, eOnvEP_t nvep, eOnvID_t nvid)
{
    eo_transm_regrop_info_t regropinfo;
    eOropdescriptor_t ropdescriptor;
    EOlistIter *li = NULL;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_NOK_generic);
    }

    // work on the list ... 
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    if(eobool_true == eo_list_Empty(p->listofregropinfo))
    {
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);
    }
    
    // need only ropcode, ep and id to search for inside the list listofregropinfo
    ropdescriptor.ropcode       = ropdesc->ropcode;
    ropdescriptor.id32          = ropdesc->id32;

      
    // search for ropcode+nvep+nvid. if not found, then ... return OK and dont do anything.
    li = eo_list_Find(p->listofregropinfo, s_eo_transmitter_ropmatchingrule_rule, &ropdescriptor);
    if(NULL == li)
    {   // it is not inside ...
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);
    }
    
    // copy what is inside the list into a temporary variable
    memcpy(&regropinfo, eo_list_At(p->listofregropinfo, li), sizeof(eo_transm_regrop_info_t));
    
    // for each element after li: (name is afterli) retrieve it and modify its content so that ropstarthere is decremented by regropinfo.ropsize ...
    // but only if ... the element is inside the same regropframe. that is done in function s_eo_transmitter_list_shiftdownropinfo()
    eo_list_ExecuteFromIter(p->listofregropinfo, s_eo_transmitter_list_shiftdownropinfo, &regropinfo, eo_list_Next(p->listofregropinfo, li));
    
    // remove the element indexedby li
    eo_list_Erase(p->listofregropinfo, li);
    
    // inside the p->ropframeregulars: remove a rop of regropinfo.ropsize which starts at regropinfo.ropstartshere. use a _friend method in here defined.
    //                                 you must: decrement the nrops by 1, decrement the size by regropinfo.ropsize, ... else in header and private variable ...
    //                                           and finally make a memmove down by regropinfo.ropsize.
    

    eo_ropframe_ROP_Rem(regropinfo.ropframe, regropinfo.ropstarthere, regropinfo.ropsize);
    
    // decrement the size of relevant ropframe
    s_eo_transmitter_regulars_update_sizes(p, (eo_transm_regropframe_t)regropinfo.regropframetype, -regropinfo.ropsize); // with a -regropinfo.ropsize we decrement

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}


extern eOresult_t eo_transmitter_regular_rops_entity_Unload(EOtransmitter *p, eOnvEP8_t ep8, eOnvENT_t ent)
{
    eo_transm_regrop_info_t regropinfo;
    EOlistIter *li = NULL;
    uint32_t id32 = 0;
    uint8_t size = 0;
    uint8_t i = 0;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_NOK_generic);
    }

    // work on the list ... 
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    if(eobool_true == eo_list_Empty(p->listofregropinfo))
    {
        eov_mutex_Release(p->mtx_regulars);
        return(eores_NOK_generic);
    }
    
    // need only ropcode, ep and id to search for inside the list listofregropinfo
    id32 = ((uint32_t)ep8 << 24) | ((uint32_t)ent << 16);

      
    size = eo_list_Size(p->listofregropinfo);
    for(i=0; i<size; i++)
    {
        // search for ropcode+nvep+nvid. if not found, then ... break.
        li = eo_list_Find(p->listofregropinfo, s_eo_transmitter_entitymatchingrule_rule, &id32);
        if(NULL == li)
        {   // it is not inside ...
            break;
        }
        
        // copy what is inside the list into a temporary variable
        memcpy(&regropinfo, eo_list_At(p->listofregropinfo, li), sizeof(eo_transm_regrop_info_t));
        
        // for each element after li: (name is afterli) retrieve it and modify its content so that ropstarthere is decremented by regropinfo.ropsize ...
        // but only if ... the element is inside the same regropframe. that is done in function s_eo_transmitter_list_shiftdownropinfo()
        eo_list_ExecuteFromIter(p->listofregropinfo, s_eo_transmitter_list_shiftdownropinfo, &regropinfo, eo_list_Next(p->listofregropinfo, li));
        
        // remove the element indexedby li
        eo_list_Erase(p->listofregropinfo, li);
        
        // inside the p->ropframeregulars: remove a rop of regropinfo.ropsize which starts at regropinfo.ropstartshere. use a _friend method in here defined.
        //                                 you must: decrement the nrops by 1, decrement the size by regropinfo.ropsize, ... else in header and private variable ...
        //                                           and finally make a memmove down by regropinfo.ropsize.
        

        eo_ropframe_ROP_Rem(regropinfo.ropframe, regropinfo.ropstarthere, regropinfo.ropsize);
        
        // decrement the size of relevant ropframe
        s_eo_transmitter_regulars_update_sizes(p, (eo_transm_regropframe_t)regropinfo.regropframetype, -regropinfo.ropsize); // with a -regropinfo.ropsize we decrement    
    }

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}

extern eOresult_t eo_transmitter_regular_rops_Clear(EOtransmitter *p)
{
    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is room for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_OK);
    }
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    if(eobool_true == eo_list_Empty(p->listofregropinfo))
    {
        eov_mutex_Release(p->mtx_regulars);
        return(eores_OK);
    } 
    
    eo_list_Clear(p->listofregropinfo);
    
    eo_ropframe_Clear(p->ropframeregulars_standard);
    eo_ropframe_Clear(p->ropframeregulars_cycle0of);
    eo_ropframe_Clear(p->ropframeregulars_cycle1of);
    eo_ropframe_Clear(p->ropframeregulars_cycle2of);     
    
    s_eo_transmitter_regulars_reset_sizes(p);

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}

extern eOresult_t eo_transmitter_regular_rops_Refresh(EOtransmitter *p)
{
    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    if(NULL == p->listofregropinfo)
    {
        // in such a case there is not space for regular rops (for instance because the cfg->maxnumberofregularrops is zero)
        return(eores_OK);
    }
    
    eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
    
    if(eobool_true == eo_list_Empty(p->listofregropinfo))
    {
        eov_mutex_Release(p->mtx_regulars);
        return(eores_OK);
    } 
    
    p->currenttime = eov_sys_LifeTimeGet(eov_sys_GetHandle());
    
    // for each element in the list ... i do: ... see function
    eo_list_Execute(p->listofregropinfo, s_eo_transmitter_list_updaterop_in_ropframe, p);

    eov_mutex_Release(p->mtx_regulars);
    
    return(eores_OK);   
}


extern eOresult_t eo_transmitter_NumberofOutROPs(EOtransmitter *p, uint16_t *numberofreplies, uint16_t *numberofoccasionals, uint16_t *numberofregulars)
{
    if(NULL == p)
    {
        return(eores_NOK_nullpointer);
    }  
    
    if(NULL != numberofreplies)
    {
        if(0 == (p->txdecimationprogressive % p->txdecimationreplies))
        {
            eov_mutex_Take(p->mtx_replies, eok_reltimeINFINITE);
            *numberofreplies = eo_ropframe_ROP_NumberOf(p->ropframereplies);
            eov_mutex_Release(p->mtx_replies);
        }
        else
        {
            *numberofreplies = 0;
        }
    }   

    if(NULL != numberofoccasionals)
    {
        if(0 == (p->txdecimationprogressive % p->txdecimationoccasionals))
        {
            eov_mutex_Take(p->mtx_occasionals, eok_reltimeINFINITE);
            *numberofoccasionals = eo_ropframe_ROP_NumberOf(p->ropframeoccasionals);
            eov_mutex_Release(p->mtx_occasionals);
        }
        else
        {
            *numberofoccasionals = 0;
        }
    }   

    if(NULL != numberofregulars)
    {
        if(0 == (p->txdecimationprogressive % p->txdecimationregulars))
        {
            uint16_t cycledrops = 0;
            eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
            // we may have one of the cycled or not
            s_eo_transmitter_get_cycled_regropframe(p, &cycledrops);
            // but the standard is alwyas added
            *numberofregulars = eo_ropframe_ROP_NumberOf(p->ropframeregulars_standard) + cycledrops;
            eov_mutex_Release(p->mtx_regulars);
        }
        else
        {
            *numberofregulars = 0;
        }
        
    }  

    return(eores_OK);   
}    

    

extern eOresult_t eo_transmitter_outpacket_Prepare(EOtransmitter *p, uint16_t *numberofrops, eOtransmitter_ropsnumber_t *ropsnum)
{
    uint16_t remainingbytes;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }
    
    if(NULL != ropsnum)
    {
        ropsnum->numberofregulars = 0;
        ropsnum->numberofoccasionals = 0;
        ropsnum->numberofreplies = 0;       
    }
    
    // clear the content of the ropframe to transmit which uses the same storage of the packet ...
    eo_ropframe_Clear(p->ropframereadytotx);
    
//    // add to it the ropframe of regulars. keep it afterwards. dont clear it !!!
//    if(0 == (p->txdecimationprogressive % p->txdecimationregulars))
//    {
//        uint16_t nregulars = 0;
//        uint8_t n0 = 0;
//        uint8_t n1 = 0;
//        // refresh regulars ...    
//        eo_transmitter_regular_rops_Refresh(p);
//        // then copy regulars into the ropframe ready to be transmitted
//        eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
//        
//        // at first teh regulars which are always transmitted
//        eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_standard, &remainingbytes);
//        nregulars += eo_ropframe_ROP_NumberOf(p->ropframeregulars_standard);
//        
//        // then we may alternate
//        n0 = eo_ropframe_ROP_NumberOf(p->ropframeregulars_cycle0of);
//        n1 = eo_ropframe_ROP_NumberOf(p->ropframeregulars_cycle1of);
//        if(0 != (n0+n1))
//        {
//            if((0 != n0) && (0 != n1))
//            {   // we alternate.
//                if(0 == (p->txregularsprogressive % 2))
//                {   // we send the set0
//                    eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_cycle0of, &remainingbytes);    
//                    nregulars += n0;                     
//                }
//                else
//                {   // we send the set1
//                    eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_cycle1of, &remainingbytes);    
//                    nregulars += n1;                                          
//                }               
//            }
//            else
//            {   // we send only the non zero               
//                if(0 != n0)
//                {
//                    eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_cycle0of, &remainingbytes);    
//                    nregulars += n0;                    
//                }
//                if(0 != n1)
//                {
//                    eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_cycle1of, &remainingbytes);    
//                    nregulars += n1;                    
//                }
//            }
//                
//        }
//                
//        eov_mutex_Release(p->mtx_regulars);
//        
//        if(NULL != ropsnum)
//        {
//            ropsnum->numberofregulars = nregulars;
//        }
//        
//        p->txregularsprogressive ++;
//    }


    // add to it the ropframe of regulars. keep it afterwards. dont clear it !!!
    if(0 == (p->txdecimationprogressive % p->txdecimationregulars))
    {
        EOropframe* cycledregulars = NULL;
        uint16_t nregularscycled = 0;
        uint16_t nregulars = 0;

        // refresh all regulars ...    
        eo_transmitter_regular_rops_Refresh(p);
        
        // then copy regulars into the ropframe ready to be transmitted
        
        eov_mutex_Take(p->mtx_regulars, eok_reltimeINFINITE);
        
        // at first the standard regulars which are always transmitted
        eo_ropframe_Append(p->ropframereadytotx, p->ropframeregulars_standard, &remainingbytes);
        nregulars += eo_ropframe_ROP_NumberOf(p->ropframeregulars_standard);
        
        // then add the cycled one, if there are any
        cycledregulars = s_eo_transmitter_get_cycled_regropframe(p, &nregularscycled);
        if(NULL != cycledregulars)
        {
            eo_ropframe_Append(p->ropframereadytotx, cycledregulars, &remainingbytes);
            nregulars += nregularscycled;
        }
                
        eov_mutex_Release(p->mtx_regulars);
        
        if(NULL != ropsnum)
        {
            ropsnum->numberofregulars = nregulars;
        }
        
        // very important: increment the regulars progressive number. it is used to decide which cycling regular to get
        p->txregularsprogressive ++;
    }

    // add the ropframe of occasionals ... and then clear it
    if(0 == (p->txdecimationprogressive % p->txdecimationoccasionals))
    {
        eov_mutex_Take(p->mtx_occasionals, eok_reltimeINFINITE);
        if(NULL != ropsnum)
        {
            ropsnum->numberofoccasionals = eo_ropframe_ROP_NumberOf(p->ropframeoccasionals);
        }
        eo_ropframe_Append(p->ropframereadytotx, p->ropframeoccasionals, &remainingbytes);
        eo_ropframe_Clear(p->ropframeoccasionals);
        eov_mutex_Release(p->mtx_occasionals);
    }

    // add the ropframe of replies ... and then clear it
    if(0 == (p->txdecimationprogressive % p->txdecimationreplies))
    {
        eov_mutex_Take(p->mtx_replies, eok_reltimeINFINITE);
        if(NULL != ropsnum)
        {
            ropsnum->numberofreplies = eo_ropframe_ROP_NumberOf(p->ropframereplies);
        }        
        eo_ropframe_Append(p->ropframereadytotx, p->ropframereplies, &remainingbytes);
        eo_ropframe_Clear(p->ropframereplies);
        eov_mutex_Release(p->mtx_replies);
    }



    if(NULL != numberofrops)
    {
        // get the number of rops to tx    
        *numberofrops = eo_ropframe_ROP_NumberOf(p->ropframereadytotx);   
    }
    
    // finally we must increment the txdecimationprogressive
    p->txdecimationprogressive ++;
    
    return(eores_OK); 
}


extern eOresult_t eo_transmitter_TXdecimation_Set(EOtransmitter *p, uint8_t repliesTXdecimation, uint8_t regularsTXdecimation, uint8_t occasionalsTXdecimation)
{
    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }
    
    if(0 == repliesTXdecimation)
    {
        repliesTXdecimation = 1;
    }
    if(0 == regularsTXdecimation)
    {
        regularsTXdecimation = 1;
    }
    if(0 == occasionalsTXdecimation)
    {
        occasionalsTXdecimation = 1;
    }
    
    p->txdecimationreplies      = repliesTXdecimation;
    p->txdecimationregulars     = regularsTXdecimation;
    p->txdecimationoccasionals  = occasionalsTXdecimation;
    
    
    return(eores_NOK_nullpointer);       
}

extern eOresult_t eo_transmitter_outpacket_SetRemoteAddress(EOtransmitter *p, eOipv4addr_t remaddr, eOipv4port_t remport)
{
    if(NULL == p) 
    {
        return(eores_NOK_generic);
    }
    eOipv4addr_t ra = 0;
    eOipv4port_t rp = 0;
    eo_packet_Addressing_Get(p->txpacket, &ra, &rp);
        
    if((ra != remaddr) || (rp != remport))
    {
        p->ipv4addr = remaddr;
        p->ipv4port = remport;
        eo_packet_Addressing_Set(p->txpacket, remaddr, remport);
    }
    
    return(eores_OK);
}

extern eOresult_t eo_transmitter_outpacket_Get(EOtransmitter *p, EOpacket **outpkt)
{
    uint16_t size;

    if((NULL == p) || (NULL == outpkt)) 
    {
        return(eores_NOK_nullpointer);
    }

    
    // now add the age of the frame
    eo_ropframe_age_Set(p->ropframereadytotx, eov_sys_LifeTimeGet(eov_sys_GetHandle()));
        
    // add sequence number
    p->tx_seqnum++;
    eo_ropframe_seqnum_Set(p->ropframereadytotx, p->tx_seqnum);

    // now set the size of the packet according to what is inside the ropframe.
    eo_ropframe_Size_Get(p->ropframereadytotx, &size);
    eo_packet_Size_Set(p->txpacket, size);

#if defined(USE_DEBUG_EOTRANSMITTER)    
    {   // DEBUG    
        uint16_t capacity = 0;
        eo_packet_Capacity_Get(p->txpacket, &capacity);   
        if(size > capacity)
        {
            p->debug.txropframeistoobigforthepacket ++;
        }
    }
#endif
    
    // finally gives back the packet
    *outpkt = p->txpacket;


    // if the confirmation manager is active .. call it
    if(NULL != p->confmanager)
    {
        eo_confman_ConfirmationRequests_Process(p->confmanager, p->ipv4addr);
    }
        
    return(eores_OK);   
}

extern eOresult_t eo_transmitter_lasterror_Get(EOtransmitter *p, int32_t *err, int32_t *info0, int32_t *info1, int32_t *info2)
{
//    eOresult_t res;
    
    if((NULL == p) || (NULL == err) || (NULL == info0) || (NULL == info1))
    {
        return(eores_NOK_nullpointer);
    }
    
    *err    = p->lasterror;
    *info0  = p->lasterror_info0;
    *info1  = p->lasterror_info1;
    *info2  = p->lasterror_info2;
    
    
    p->lasterror        = 0;
    p->lasterror_info0  = 0;
    p->lasterror_info1  = 0;
    p->lasterror_info2  = 0;
    
    return(eores_OK);    
}

extern eOresult_t eo_transmitter_occasional_rops_Load(EOtransmitter *p, eOropdescriptor_t* ropdesc)
{   // we dont care about p->ropframeoccasionals being invalid because all controls are inside s_eo_transmitter_rops_Load().
    return(s_eo_transmitter_rops_Load(p, ropdesc, p->ropframeoccasionals, p->mtx_occasionals));
}


extern eOresult_t eo_transmitter_reply_rops_Load(EOtransmitter *p, eOropdescriptor_t* ropdesc)
{   // we dont care about p->ropframereplies being invalid because all controls are inside s_eo_transmitter_rops_Load().
    return(s_eo_transmitter_rops_Load(p, ropdesc, p->ropframereplies, p->mtx_replies));
}


extern eOresult_t eo_transmitter_reply_ropframe_Load(EOtransmitter *p, EOropframe* ropframe)
{
    eOresult_t res;
    uint16_t remainingbytes;

    if(NULL == p) 
    {
        return(eores_NOK_nullpointer);
    }  

    eov_mutex_Take(p->mtx_replies, eok_reltimeINFINITE);
    res = eo_ropframe_Append(p->ropframereplies, ropframe, &remainingbytes);
    eov_mutex_Release(p->mtx_replies);
    
    // replies cannot have a conf request flagged on, then there is no insertion inside the p->confrequests

    return(res);     
}


extern eOresult_t eo_transmitter_occasional_rops_LoadStream(EOtransmitter *p, uint8_t *stream, uint16_t size)
{    
    eOresult_t res;
    res = eo_ropframe_ROPdata_Add(p->ropframeoccasionals, stream, size, NULL);
    return(res);
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static eOresult_t s_eo_transmitter_ropmatchingrule_rule(void *item, void *param)
{
    eo_transm_regrop_info_t *inside = (eo_transm_regrop_info_t*)item;
    eOropdescriptor_t *targetrop = (eOropdescriptor_t*)param;

//    if((inside->thenv.id32 == targetrop->id32) && (inside->ropcode == targetrop->ropcode))
    if(inside->thenv.id32 == targetrop->id32)
    {
        return(eores_OK);
    }
    else
    {
        return(eores_NOK_generic);
    }
}

static eOresult_t s_eo_transmitter_entitymatchingrule_rule(void *item, void *param)
{
    eo_transm_regrop_info_t *inside = (eo_transm_regrop_info_t*)item;
    uint32_t *id32 = (uint32_t*)param;

    uint32_t id32_inside_masked = inside->thenv.id32 & 0xffff0000;
    uint32_t id32_target_masked = *id32 & 0xffff0000; 
    if(id32_inside_masked == id32_target_masked)
    {
        return(eores_OK);
    }
    else
    {
        return(eores_NOK_generic);
    }
}


static void s_eo_transmitter_list_updaterop_in_ropframe(void *item, void *param)
{
    eo_transm_regrop_info_t *inside = (eo_transm_regrop_info_t*)item;
    EOtransmitter *p = (EOtransmitter*)param;
    
    uint8_t *origofrop;
    uint8_t *dest;
    
    // retrieve the beginning of the ropstream inside the ropframe
    origofrop = eo_ropframe_hid_get_pointer_offset(inside->ropframe, inside->ropstarthere);
    dest = origofrop + sizeof(eOrophead_t);

    // if it has a data field ... copy from the nv to the ropstream
    if(eobool_true == inside->hasdata2update)
    {
        // by using eo_nv_hid_Fast_LocalMemoryGet() we use the protection which is configured
        // by the EOnvscfg object, and the concurrent access to the netvar is managed
        // internally the nv object.
        eo_nv_hid_Fast_LocalMemoryGet(&inside->thenv, dest);
        
        // with memcpy the copy from local buffer to dest is not protected, thus data format may be corrupt
        // in case any concurrent task is in the process of writing the local buffer.
        //  memcpy(dest, inside->nvloc, inside->capacity);
        // for debug: memset(dest, 0xaa, inside->capacity); 
    }
    
    
    // if it has a time field ... copy from the current time to the ropstream
    if(EOK_uint16dummy != inside->timeoffsetinsiderop)
    {
        eOabstime_t *time = (eOabstime_t*) (&origofrop[inside->timeoffsetinsiderop]);
        memcpy(time, &p->currenttime, sizeof(eOabstime_t));
    }
    
//    // if it requires a confirmation ... but it never requires a confirmation. 
//     if(...)
//     {
//         
//     }

}


static void s_eo_transmitter_list_shiftdownropinfo(void *item, void *param)
{
    eo_transm_regrop_info_t *after = (eo_transm_regrop_info_t*)item;
    eo_transm_regrop_info_t *theone2beremoved = (eo_transm_regrop_info_t*)param;

    // for each element after: modify its content so that ropstarthere is decremented by theone2beremoved.ropsize
    // but only if they have the same regropframetype

    if(after->ropframe == theone2beremoved->ropframe)
    {
        after->ropstarthere -= theone2beremoved->ropsize;
    }
}


static eOresult_t s_eo_transmitter_rops_Load(EOtransmitter *p, eOropdescriptor_t* ropdesc, EOropframe* intoropframe, EOVmutexDerived* mtx)
{
    // marco.accame on 23oct14: mtx protects the occasional or replies ropframe. p->mtx_roptmp protects the use of tmprop
    eOresult_t res;
    uint16_t usedbytes;
    uint16_t ropsize;
    uint16_t remainingbytes;   
    EOnv nv;
    eObool_t boolres = eobool_false;
    
    if((NULL == p) || (NULL == ropdesc)) 
    {
        if(NULL != p)
        {
            p->lasterror = 1;
        }
        return(eores_NOK_nullpointer);
    } 
    
    // marco.accame on 23oct14
    // must protect the reading of the ropframe. it has happened that boolres is false even for a good ropframe. 
    // reason is concurrent tx of the packet and call of this function
    eov_mutex_Take(mtx, eok_reltimeINFINITE);
    boolres = eo_ropframe_IsValid(intoropframe);
    eov_mutex_Release(mtx);
    
    if(eobool_false == boolres)
    {   // marco.accame: i added it on 15 may 2014 to exit from function if the ropframe does not have any data
        p->lasterror = 2;
        return(eores_NOK_generic);
    }
    
      
    res = eo_nvset_NV_Get(  (p->nvset),  
                            ropdesc->id32,
                            &nv
                            );   

    // if the nvset does not have the pair (ip, id) then we return an error because we cannot form the rop
    if(eores_OK != res)
    {
        p->lasterror = 3;
        return(eores_NOK_generic);
    } 

    // force size to be coherent with the nv. the size is always used, even if there is no data to transmit
    ropdesc->size = eo_nv_Size(&nv);    
    
    // now we have the nv. we set its value in local ram
    if(eobool_true == eo_rop_ropcode_has_data(ropdesc->ropcode))
    {  
        eOnvOwnership_t nvownership = eo_rop_get_ownership(ropdesc->ropcode, eo_ropconf_none, eo_rop_dir_outgoing);
        if(eo_nv_ownership_local == nvownership)
        {   // if the nv is local, then take data from nv, thus no need to write the data field of the nv using ropdesc->data.
            ropdesc->data = NULL;   // set ropdesc->data to NULL to force eo_agent_OutROPfromNV() to get data from EOnv
        }
        else
        {   // if the nv is remote, then the data must be passed inside ropdesc->data
            if(NULL == ropdesc->data)
            {
                eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, "s_eo_transmitter_rops_Load(): cant have NULL ropdes->data with rem ownership", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);
                return(eores_NOK_generic);
            }          
        }
    }
    else
    {   // dont need to send data
        ropdesc->data = NULL;
        ropdesc->size = 0;
    }


    // we begin the use in rw of p->tmprop: take its mutex ... we must avoid that a concurrent thread use it at the same time.
    eov_mutex_Take(p->mtx_roptmp, eok_reltimeINFINITE);
           
    res = eo_agent_OutROPprepare(p->agent, &nv, ropdesc, p->roptmp, &usedbytes);    
    
    if(eores_OK != res)
    {
        p->lasterror = 4;
        eov_mutex_Release(p->mtx_roptmp);
        return(res);
    }

    // put the rop inside the ropframe: protec ropframe vs concurrent use
    eov_mutex_Take(mtx, eok_reltimeINFINITE);
    res = eo_ropframe_ROP_Add(intoropframe, p->roptmp, NULL, &ropsize, &remainingbytes);
    eov_mutex_Release(mtx);
    
    // we dont use p->tmprop anymore: release its mutex
    eov_mutex_Release(p->mtx_roptmp);
    
    if(eores_OK != res)
    {
        uint16_t ss = 0;
        p->lasterror_info0 = ropsize;
        p->lasterror_info1 = remainingbytes;
        eo_ropframe_EffectiveCapacity_Get(intoropframe, &ss); // no need to protect using mutex as we read its capacity which stays constant all over the time
        p->lasterror_info2  = ss;
        p->lasterror = 5;
    }
    
    
 
    // if conf request is flagged on
    if((eores_OK == res) && (1 == ropdesc->control.rqstconf) && ((NULL != p->confmanager)))
    {
        if(eores_OK != eo_confman_ConfirmationRequest_Insert(p->confmanager, ropdesc))
        {
            eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, "s_eo_transmitter_rops_Load(): fails in processing a conf-request", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);
        }
    }
  
    return(res);   
}

static EOropframe * s_eo_transmitter_id32_to_typeofregulars(EOtransmitter* p, eOprotID32_t id32, eo_transm_regropframe_t *ropframetype)
{
    EOropframe* ret = NULL;
    
    if(eoprot_endpoint_motioncontrol == eoprot_ID2endpoint(id32))
    {   // we put in here joints, motors but also the controller  ...
        
        if(eoprot_entity_mc_controller == eoprot_ID2entity(id32))
        {
            *ropframetype = eo_transm_regropframe_standard;
            ret = p->ropframeregulars_standard;
        }
        else if(eoprot_ID2index(id32) < 4)
        {
            *ropframetype = eo_transm_regropframe_cycle0of;    
            ret = p->ropframeregulars_cycle0of;
        }
        else if(eoprot_ID2index(id32) < 8)
        {
            *ropframetype = eo_transm_regropframe_cycle1of;
            ret = p->ropframeregulars_cycle1of;
        } 
        else
        {
            *ropframetype = eo_transm_regropframe_cycle2of;
            ret = p->ropframeregulars_cycle2of;
        }         
    }
    else
    {
        *ropframetype = eo_transm_regropframe_standard; 
        ret = p->ropframeregulars_standard;        
    }

    return(ret);  
}

static EOropframe * s_eo_transmitter_get_cycled_regropframe(EOtransmitter* p, uint16_t *ropsinside)
{
    EOropframe* ret = NULL;
    uint16_t s0 = eo_ropframe_ROP_NumberOf(p->ropframeregulars_cycle0of);
    uint16_t s1 = eo_ropframe_ROP_NumberOf(p->ropframeregulars_cycle1of);
    uint16_t s2 = eo_ropframe_ROP_NumberOf(p->ropframeregulars_cycle2of);
    
    if(0 == (s0+s1+s2))
    {   // we dont have any
        ret = NULL;
        *ropsinside = 0;
    }
    else if(0 == (s1+s2))
    {   // we have only cycle0
        ret = p->ropframeregulars_cycle0of;
        *ropsinside = s0;
    } 
    else
    {   // we have more than one cycle. typically all 3. i must alternate
        uint8_t c = (p->txregularsprogressive % 3);
        switch(c)
        {
            default:            
            case 0:
            {   // we chose cycle0
                ret = p->ropframeregulars_cycle0of;
                *ropsinside = s0;
            } break;                
            case 1:
            {   // we chose cycle1
                ret = p->ropframeregulars_cycle1of;
                *ropsinside = s1;
            } break;  
            case 2:
            {   // we chose cycle2
                ret = p->ropframeregulars_cycle2of;
                *ropsinside = s2;
            } break;  
        }     
    }    
 
    return(ret);
}

static void s_eo_transmitter_regulars_reset_sizes(EOtransmitter *p)
{
    p->totalsizeofregulars_standard = 0;
    p->totalsizeofregulars_cycle0of = 0;
    p->totalsizeofregulars_cycle1of = 0;
    p->totalsizeofregulars_cycle2of = 0;
    p->maxsizeofregulars = 0;    
}

static uint16_t s_eo_transmitter_get_maxsizeof_regularsropframe(EOtransmitter *p)
{
    uint16_t m01 = EO_MAX(p->totalsizeofregulars_cycle0of, p->totalsizeofregulars_cycle1of);
    uint16_t m012 = EO_MAX(m01, p->totalsizeofregulars_cycle2of);
    return( p->totalsizeofregulars_standard + m012 );   
}

static eObool_t s_eo_transmitter_regulars_canadd_rop(EOtransmitter *p, eo_transm_regropframe_t type, uint16_t ropbytes)
{ 
    uint16_t std = 0;
    uint16_t cy0 = 0;
    uint16_t cy1 = 0;
    uint16_t cy2 = 0;
    
    std = p->totalsizeofregulars_standard;
    cy0 = p->totalsizeofregulars_cycle0of;
    cy1 = p->totalsizeofregulars_cycle1of;
    cy2 = p->totalsizeofregulars_cycle2of;

    switch(type)
    {
        case eo_transm_regropframe_standard:
        {
            std += ropbytes;
        } break;
        case eo_transm_regropframe_cycle0of:
        {
            cy0 += ropbytes;
        } break; 
        case eo_transm_regropframe_cycle1of:
        {
            cy1 += ropbytes;
        } break; 
        case eo_transm_regropframe_cycle2of:
        {
            cy2 += ropbytes;
        } break;         
    }

    uint16_t m01 = EO_MAX(cy0, cy1);
    uint16_t m012 =  EO_MAX(m01, cy2); 
    if( (std + m012) > p->effectivecapacityofregulars)
    {
        return(eobool_false);
    }
    
    return(eobool_true);          
}


static void s_eo_transmitter_regulars_update_sizes(EOtransmitter *p, eo_transm_regropframe_t type, int16_t ropbytes)
{
    switch(type)
    {
        case eo_transm_regropframe_standard:
        {
            p->totalsizeofregulars_standard += ropbytes;
        } break;
        case eo_transm_regropframe_cycle0of:
        {
            p->totalsizeofregulars_cycle0of += ropbytes;
        } break; 
        case eo_transm_regropframe_cycle1of:
        {
            p->totalsizeofregulars_cycle1of += ropbytes;
        } break;
        case eo_transm_regropframe_cycle2of:
        {
            p->totalsizeofregulars_cycle2of += ropbytes;
        } break;           
    }
    
    p->maxsizeofregulars = s_eo_transmitter_get_maxsizeof_regularsropframe(p);
}

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





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
#include "stdio.h"
#include "EOtheMemoryPool.h"
#include "EOtheErrorManager.h"
#include "EOnv_hid.h"
#include "EOrop_hid.h"




// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOdeviceTransceiver.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOdeviceTransceiver_hid.h" 


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

static EOnvSet* s_eo_devicetransceiver_nvset_get(EOdeviceTransceiver* p, const eOdevicetransceiver_cfg_t *cfg);

static void s_eo_devicetransceiver_nvset_release(EOdeviceTransceiver* p);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOdeviceTransceiver";
 


const eOdevicetransceiver_cfg_t eo_devicetransceiver_cfg_default = 
{
    EO_INIT(.nvsetbrdcfg)               NULL,
    EO_INIT(.remotehostipv4addr)        0,
    EO_INIT(.remotehostipv4port)        0,
    EO_INIT(.sizes)                     {0},
    EO_INIT(.mutex_fn_new)              NULL,
    EO_INIT(.transprotection)           eo_trans_protection_none,
    EO_INIT(.nvsetprotection)           eo_nvset_protection_none,
    EO_INIT(.proxycfg)                  NULL,
    EO_INIT(.extfn)                         
    {
        EO_INIT(.onerrorseqnumber)      NULL
    }
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOdeviceTransceiver * eo_devicetransceiver_New(const eOdevicetransceiver_cfg_t *cfg) 
{ 
    EOdeviceTransceiver* retptr  = NULL;
    eOtransceiver_cfg_t txrxcfg = eo_transceiver_cfg_default;

    if(NULL == cfg)
    {
        cfg = &eo_devicetransceiver_cfg_default;
    }
    
    if(NULL == cfg->nvsetbrdcfg)
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_devicetransceiver_New(): NULL nvsetbrdcfg", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    }
    
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOdeviceTransceiver), 1);

    if((0 == cfg->sizes.capacityoftxpacket) || (0 == cfg->sizes.capacityofrop) || (0 == cfg->sizes.capacityofropframeregulars) ||
       (0 == cfg->sizes.capacityofropframeoccasionals) || (0 == cfg->sizes.capacityofropframereplies) || (0 == cfg->sizes.maxnumberofregularrops))
    {
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_devicetransceiver_New(): one 0 cfg->sizes.field", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    }    
    
    // 1. init the proper transceiver cfg

    retptr->nvset = s_eo_devicetransceiver_nvset_get(retptr, cfg);
    
    retptr->boardnumber = cfg->nvsetbrdcfg->boardnum;
    

    txrxcfg.sizes.capacityoftxpacket            = cfg->sizes.capacityoftxpacket;
    txrxcfg.sizes.capacityofrop                 = cfg->sizes.capacityofrop;
    txrxcfg.sizes.capacityofropframeregulars    = cfg->sizes.capacityofropframeregulars;
    txrxcfg.sizes.capacityofropframeoccasionals = cfg->sizes.capacityofropframeoccasionals;
    txrxcfg.sizes.capacityofropframereplies     = cfg->sizes.capacityofropframereplies;
    txrxcfg.sizes.maxnumberofregularrops        = cfg->sizes.maxnumberofregularrops;
    txrxcfg.remipv4addr                         = cfg->remotehostipv4addr;
    txrxcfg.remipv4port                         = cfg->remotehostipv4port;
    txrxcfg.nvset                               = retptr->nvset;
    txrxcfg.confmancfg                          = NULL; // it does not have a confirmation manager cfg->confmancfg;
    txrxcfg.proxycfg                            = cfg->proxycfg;
    txrxcfg.mutex_fn_new                        = cfg->mutex_fn_new;
    txrxcfg.protection                          = cfg->transprotection;
    memcpy(&txrxcfg.extfn, &cfg->extfn, sizeof(eOtransceiver_extfn_t));
    
    retptr->transceiver = eo_transceiver_New(&txrxcfg);
    
    
    return(retptr);        
}    


extern void eo_devicetransceiver_Delete(EOdeviceTransceiver* p) 
{   
    if(NULL == p)
    {
        return;
    }
    
    if(NULL == p->transceiver)
    {
        return;
    }
    
    eo_transceiver_Delete(p->transceiver);
       
    s_eo_devicetransceiver_nvset_release(p);
    

    memset(p, 0, sizeof(EOdeviceTransceiver));
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);    
    return;    
}


extern EOtransceiver * eo_devicetransceiver_GetTransceiver(EOdeviceTransceiver* p)
{
    if(NULL != p)
    {
        return(p->transceiver);
    }
    else
    {    
        return(NULL);
    }     
}

extern EOnvSet * eo_devicetransceiver_GetNVset(EOdeviceTransceiver* p)
{
    if(NULL != p)
    {
        return(p->nvset);
    }
    else
    {    
        return(NULL);
    }        
}

extern eOnvBRD_t eo_devicetransceiver_GetBoardNumber(EOdeviceTransceiver* p)
{
    if(NULL != p)
    {
        return(p->boardnumber);
    }
    else
    {    
        return(eo_nv_BRDdummy);
    }          
}






// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

static EOnvSet* s_eo_devicetransceiver_nvset_get(EOdeviceTransceiver* p, const eOdevicetransceiver_cfg_t *cfg)
{
    EOnvSet* nvset = NULL;

    if(NULL != p->nvset)
    {   // if i call it more than once ... then i return the configuration but allocate and init only once 
        return(p->nvset);
    }

    nvset = eo_nvset_New(cfg->nvsetprotection, cfg->mutex_fn_new);
    eo_nvset_InitBRD_LoadEPs(nvset, eo_nvset_ownership_local, EO_COMMON_IPV4ADDR_LOCALHOST, (eOnvset_BRDcfg_t*)cfg->nvsetbrdcfg, eobool_true);           

    return(nvset);
}


static void s_eo_devicetransceiver_nvset_release(EOdeviceTransceiver* p)
{
    //const uint16_t numofdevices     = 1;    // one device only
    const uint16_t ondevindexzero   = 0;    // one device only

    if(NULL == p->nvset)
    {   // if i call it more than once ... 
        return;
    }

    // _delete also unit the brd etc.            
    eo_nvset_Delete(p->nvset);   
    p->nvset = NULL;  
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





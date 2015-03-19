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

#include "EOnv_hid.h" 

#include "EoProtocol.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOnvSet.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOnvSet_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#undef EO_NVSET_INIT_EVERY_NV

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_eo_nvset_PushBackDEVholder(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t boardnum, eOuint16_fp_voidp_uint8_t fptr_ep2index, uint16_t nendpoints, void *param);
static eOresult_t s_eo_nvset_onDEV_PushBackEP(EOnvSet* p, uint16_t ondevindex, eOnvset_EPcfg_t *cfgofep, eOnvBRD_t boardnum);

static eOresult_t s_eo_nvset_onDEV_ClearEPs(EOnvSet* p, uint16_t ondevindex);
static eOresult_t s_eo_nvset_PopBackDEVholder(EOnvSet* p);

static EOVmutexDerived* s_eo_nvset_get_nvmutex(EOnvSet* p, eOnvset_dev_t* thedevice, eOnvset_ep_t* theendpoint, eOnvID32_t id32);
static eOresult_t s_eo_nvset_hid_get_device(EOnvSet* p, eOipv4addr_t ip, eOnvset_dev_t** thedevice);
static eOresult_t s_eo_nvset_hid_get_device_endpoint_faster(EOnvSet* p, eOipv4addr_t ip, eOnvEP8_t ep8, eOnvset_dev_t** thedevice, eOnvset_ep_t** theendpoint);

static void s_eo_nvset_devicesowneship_change(EOnvSet *p, eOnvsetOwnership_t ownership);


extern uint16_t eo_nvset_hid_ip2index(EOnvSet* p, eOipv4addr_t ip);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOnvSet";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

 
extern EOnvSet* eo_nvset_New(uint16_t ndevices, eOnvset_protection_t prot, eov_mutex_fn_mutexderived_new mtxnew)
{
    EOnvSet *p = NULL;  

    eo_errman_Assert(eo_errman_GetHandle(), (0 != ndevices), "eo_nvset_New(): 0 ndevices", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);    

    // i get the memory for the object
    p = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOnvSet), 1);
    
    p->thedevices               = eo_vector_New(sizeof(eOnvset_dev_t*), ndevices, NULL, 0, NULL, NULL);
    p->theipaddresses           = eo_vector_New(sizeof(eOipv4addr_t), ndevices, NULL, 0, NULL, NULL);
    p->devindexoflocaldevice    = EOK_uint16dummy;
    p->devicesowneship          = eo_nvset_devicesownership_none;
    p->mtxderived_new           = mtxnew; 
    p->protection               = (NULL == mtxnew) ? (eo_nvset_protection_none) : (prot); 
    p->mtx_object               = (eo_nvset_protection_one_per_object == p->protection) ? (p->mtxderived_new()) : (NULL);

    return(p);
}


extern void eo_nvset_Delete(EOnvSet* p)
{   
    if(NULL == p)
    {
        return;
    }
    
    if(NULL == p->thedevices)
    {
        return;
    }
    
    if(NULL != p->mtx_object)
    {
        eov_mutex_Delete(p->mtx_object);
    }
    
    eo_vector_Delete(p->theipaddresses);
    eo_vector_Delete(p->thedevices);
    
    
    memset(p, 0, sizeof(EOnvSet));
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;
}

extern eOresult_t eo_nvset_DEVpushback(EOnvSet* p, uint16_t ondevindex, eOnvset_DEVcfg_t* cfgofdev, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress)
{
    uint16_t i = 0;
    uint16_t nendpoints = eo_constvector_Size(cfgofdev->vectorof_epcfg);
    s_eo_nvset_PushBackDEVholder(p, ownership, ipaddress, cfgofdev->boardnum, cfgofdev->fptr_ep2indexofepcfg, nendpoints, cfgofdev->param);
    
    if(NULL != cfgofdev->fptr_device_initialise)
    {
        eObool_t local = (eo_nvset_ownership_local == ownership) ? (eobool_true) : (eobool_false);
        cfgofdev->fptr_device_initialise(cfgofdev->param, local);
    }
    
    for(i=0; i<nendpoints; i++)
    {
        eOnvset_EPcfg_t* pepcfg = (eOnvset_EPcfg_t*) eo_constvector_At(cfgofdev->vectorof_epcfg, i);
        s_eo_nvset_onDEV_PushBackEP(p, ondevindex, pepcfg, cfgofdev->boardnum);        
    }
    
    return(eores_OK); 
}


extern eOresult_t eo_nvset_DEVpopback(EOnvSet* p, uint16_t ondevindex)
{   
    s_eo_nvset_onDEV_ClearEPs(p, ondevindex); // all the EPs added one by one with s_eo_nvset_onDEV_PushBackEP()
    
    s_eo_nvset_PopBackDEVholder(p);
    
    return(eores_OK); 
}


static eOresult_t s_eo_nvset_PushBackDEVholder(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t boardnum, eOuint16_fp_voidp_uint8_t fptr_ep2index, uint16_t nendpoints, void *param)
{
    eOnvset_dev_t *dev = NULL;
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    eo_errman_Assert(eo_errman_GetHandle(), (0 != nendpoints), "s_eo_nvset_PushBackDEVholder(): 0 nendpoints", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != fptr_ep2index), "s_eo_nvset_PushBackDEVholder(): NULL fptr_ep2index", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    
    eo_errman_Assert(eo_errman_GetHandle(), (eobool_false == eo_vector_Full(p->thedevices)), "s_eo_nvset_PushBackDEVholder(): thedevices is full", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);

    eo_errman_Assert(eo_errman_GetHandle(), (eobool_true != eo_vector_Find(p->theipaddresses, NULL, &ipaddress, NULL)),"s_eo_nvset_PushBackDEVholder(): ip already inside", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);

    s_eo_nvset_devicesowneship_change(p, ownership);

    if(eo_nvset_ownership_local == ownership)
    {
        p->devindexoflocaldevice   = eo_vector_Size(p->thedevices);
    }
    
    dev = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(eOnvset_dev_t), 1);
    
    dev->ipaddress              = ipaddress;
    dev->boardnum               = boardnum;
    dev->ownership              = ownership;
    dev->theendpoints_numberof  = nendpoints;
    dev->theendpoints           = eo_vector_New(sizeof(eOnvset_ep_t*), nendpoints, NULL, 0, NULL, NULL);    
    dev->fptr_ep2index          = fptr_ep2index;
    dev->ep2index_param         = param;
    dev->mtx_device             = (eo_nvset_protection_one_per_device == p->protection) ? p->mtxderived_new() : NULL;

    eo_vector_PushBack(p->thedevices, &dev);
    eo_vector_PushBack(p->theipaddresses, &ipaddress);

    return(eores_OK);
}

static eOresult_t s_eo_nvset_PopBackDEVholder(EOnvSet* p)
{
    eOnvset_dev_t *dev = NULL;
    eOnvset_dev_t **devitem = NULL;
    
    devitem = (eOnvset_dev_t**)eo_vector_Back(p->thedevices);
    dev = *devitem;
    
    eo_vector_Delete(dev->theendpoints);
    
    if(NULL != dev->mtx_device)
    {
        eov_mutex_Delete(dev->mtx_device);
    }
    
    eo_mempool_Delete(eo_mempool_GetHandle(), dev);
    
    // not needed so far
    eo_vector_PopBack(p->thedevices);
    eo_vector_PopBack(p->theipaddresses);
    
 
    return(eores_OK);
}

static eOresult_t s_eo_nvset_onDEV_PushBackEP(EOnvSet* p, uint16_t ondevindex, eOnvset_EPcfg_t *cfgofep, eOnvBRD_t boardnum)
{
    eOnvset_dev_t** thedev = NULL;
    eOnvset_ep_t *theendpoint = NULL;
    uint16_t epnvsnumberof = 0;
    eOres_fp_uint8_uint8_voidp_uint16_t fptr_loadram = NULL;
   
 
    if((NULL == p) || (NULL == cfgofep)) 
    {
        return(eores_NOK_nullpointer); 
    }
    
    //epnvsnumberof = cfgofep->protif->getvarsnumberof(boardnum, cfgofep->endpoint);
    epnvsnumberof = eoprot_endpoint_numberofvariables_get(boardnum, cfgofep->endpoint);
    
    eo_errman_Assert(eo_errman_GetHandle(), (0 != epnvsnumberof), "s_eo_nvset_onDEV_PushBackEP(): 0 epnvsnumberof", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal); 
    
    // ok: i go on
        
    thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
    
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != thedev), "s_eo_nvset_onDEV_PushBackEP(): ->thedevices is indexed in wrong pos", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);    
    eo_errman_Assert(eo_errman_GetHandle(), (eobool_false == eo_vector_Full((*thedev)->theendpoints)), "s_eo_nvset_onDEV_PushBackEP(): one of ->theendpoints is full", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);
     
    theendpoint = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(eOnvset_ep_t), 1);
    
    memcpy(&theendpoint->epcfg, cfgofep, sizeof(eOnvset_EPcfg_t));   
    theendpoint->epnvsnumberof      = epnvsnumberof;
    theendpoint->initted            = eobool_false;    
    theendpoint->epram              = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, theendpoint->epcfg.epram_sizeof, 1);
    theendpoint->mtx_endpoint       = (eo_nvset_protection_one_per_endpoint == p->protection) ? p->mtxderived_new() : NULL;
        
    // now we must load the ram in the endpoint
//    fptr_loadram = theendpoint->epcfg.protif->loadram;
//    eo_errman_Assert(eo_errman_GetHandle(), (NULL != fptr_loadram), "s_eo_nvset_onDEV_PushBackEP(): NULL fptr_loadram", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal); 
//    fptr_loadram((*thedev)->boardnum, theendpoint->epcfg.endpoint, theendpoint->epram, theendpoint->epcfg.epram_sizeof);
    
    eoprot_config_endpoint_ram((*thedev)->boardnum, theendpoint->epcfg.endpoint, theendpoint->epram, theendpoint->epcfg.epram_sizeof);
    
    // now add the vector of mtx if needed.
    if(eo_nvset_protection_one_per_netvar == p->protection)
    {
        uint16_t i;
        theendpoint->themtxofthenvs = eo_vector_New(sizeof(EOVmutexDerived*), epnvsnumberof, NULL, 0, NULL, NULL);
        for(i=0; i<epnvsnumberof; i++)
        {
            EOVmutexDerived* mtx = p->mtxderived_new();
            eo_vector_PushBack(theendpoint->themtxofthenvs, &mtx);           
        }
    }
   
    eo_vector_PushBack((*thedev)->theendpoints, &theendpoint);

    return(eores_OK);
}



static eOresult_t s_eo_nvset_onDEV_ClearEPs(EOnvSet* p, uint16_t ondevindex)
{
    eOnvset_dev_t** thedev = NULL;
    eOnvset_ep_t *theendpoint = NULL;
    
    uint16_t i = 0;    
    
    thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != thedev), "s_eo_nvset_onDEV_ClearEPs(): ->thedevices is indexed in wrong pos", s_eobj_ownname, &eo_errman_DescrRuntimeErrorLocal);    
    
    uint16_t endpointsnum = eo_vector_Size((*thedev)->theendpoints);
 
    for(i=0; i<endpointsnum; i++)
    {
        eOnvset_ep_t **ppep = (eOnvset_ep_t **)eo_vector_At((*thedev)->theendpoints, i);
        theendpoint = *ppep;
        
        // now i erase memory associated with this endpoint
        eo_mempool_Delete(eo_mempool_GetHandle(), theendpoint->epram);
        if(NULL != theendpoint->mtx_endpoint)
        {
            eov_mutex_Delete(theendpoint->mtx_endpoint);
        }
        if(NULL != theendpoint->themtxofthenvs)
        {
            uint16_t size = eo_vector_Size(theendpoint->themtxofthenvs);
            int j = 0;
            for(j=0; j<size; j++)
            {
                EOVmutexDerived** pmtx =  (EOVmutexDerived**)eo_vector_At(theendpoint->themtxofthenvs, j);
                eov_mutex_Delete(*pmtx);            
            } 
            eo_vector_Delete(theendpoint->themtxofthenvs);
        }
        
        // now i erase the endpoint        
         eo_mempool_Delete(eo_mempool_GetHandle(), theendpoint);
       
    }

    return(eores_OK);
}

extern eOresult_t eo_nvset_NVSinitialise(EOnvSet* p)
{
    eOnvset_dev_t** theDevice = NULL;
    eOnvset_ep_t** theEndpoint = NULL;
    EOVmutexDerived* mtx2use = NULL;
    
    uint16_t i, j;
    uint16_t ndev;
    uint16_t nendpoints;
    eOnvEP8_t ep08 = EOK_uint08dummy;
    eOvoid_fp_uint32_voidp_t initialise = NULL;


 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}

    ndev = eo_vector_Size(p->thedevices);
    mtx2use = (eo_nvset_protection_one_per_object == p->protection) ? (p->mtx_object) : (NULL);


    for(i=0; i<ndev; i++)
    {
        theDevice = (eOnvset_dev_t**) eo_vector_At(p->thedevices, i);
        nendpoints = eo_vector_Size((*theDevice)->theendpoints);
        
        mtx2use = (eo_nvset_protection_one_per_device == p->protection) ? ((*theDevice)->mtx_device) : (mtx2use);


        for(j=0; j<nendpoints; j++)
        {
            theEndpoint = (eOnvset_ep_t**) eo_vector_At((*theDevice)->theendpoints, j);
            
            ep08 = (*theEndpoint)->epcfg.endpoint;
            
            mtx2use = (eo_nvset_protection_one_per_endpoint == p->protection) ? ((*theEndpoint)->mtx_endpoint) : (mtx2use);                   

            if(eobool_false == ((*theEndpoint)->initted))
            {
                initialise  = (*theEndpoint)->epcfg.fptr_ram_initialise;
                (*theEndpoint)->initted = eobool_true;
                
                if(NULL != initialise)
                {
                    initialise((*theDevice)->ipaddress, (*theEndpoint)->epram);
                }
            }
#define EO_NVSET_INIT_EVERY_NV
#if defined(EO_NVSET_INIT_EVERY_NV)
            {   // put parenthesis to create a new scope and avoid errors in non c99 environments as windows
            EOnv thenv;
            uint16_t k = 0;
            EOnv_rom_t* rom = NULL;
            uint8_t* ram = NULL;
            uint16_t nvars = (*theEndpoint)->epnvsnumberof;
            eOipv4addr_t ip = (*theDevice)->ipaddress;
            uint8_t brd =  (*theDevice)->boardnum;
            eOnvID32_t id32 = EOK_uint32dummy;     
            uint32_t prog = 0;
            eObool_t proxied = eobool_false;
            eOvoid_fp_cnvp_cropdesp_t onsay = NULL;

            for(k=0; k<nvars; k++)
            {

                if(eo_nvset_protection_one_per_netvar == p->protection)
                {
                    uint32_t** addr = eo_vector_At((*theEndpoint)->themtxofthenvs, k);
                    mtx2use = (EOVmutexDerived*) (*addr);
                }
                
                prog = k;
                // - 0. the id32 
                //id32 = (*theEndpoint)->epcfg.protif->epgetid(brd, ep08, prog);   
                id32 = eoprot_endpoint_prognum2id(brd, ep08, prog);               
                if(EOK_uint32dummy == id32)
                {
                    continue;
                }

                // - 0+. proxied?
                //proxied = (*theEndpoint)->epcfg.protif->isvarproxied(brd, id32);  
                proxied = eoprot_variable_is_proxied(brd, id32);                
                // - 0++ onsay
                onsay = eoprot_onsay_endpoint_get(ep08); 
                // - 1. the rom
                //rom = (EOnv_rom_t*) (*theEndpoint)->epcfg.protif->getrom(brd, id32);
                rom = (EOnv_rom_t*) eoprot_variable_romof_get(brd, id32);
                // - 2. the ram
                //ram = (uint8_t*) ((*theEndpoint)->epcfg.protif->getram(brd, id32));
                ram = (uint8_t*) eoprot_variable_ramof_get(brd, id32);
                // - 3. the mtx
                mtx2use = s_eo_nvset_get_nvmutex(p, (*theDevice), (*theEndpoint), id32);

                // - load everything into the nv
                eo_nv_hid_Load(     &thenv,
                                    ip, //(*theDevice)->ipaddress,
                                    brd,
                                    proxied,
                                    id32,
                                    onsay,
                                    rom,
                                    ram,
                                    mtx2use
                              );                    
                
             
                eo_nv_Init(&thenv);                             
            }
            }   // put parenthesis to create a new scope and avoid errors in non c99 environments as windows
#endif //EO_NVSET_INIT_EVERY_NV                   
        }
            
    }

    return(eores_OK);
}

extern eOresult_t eo_nvset_NVSdeinitialise(EOnvSet* p)
{
    //#warning -> add code in here for eo_nvset_NVSdeinitialise()
    // so far it is not necessary to deinit    
    return(eores_OK);    
}


extern void* eo_nvset_RAMofEndpoint_Get(EOnvSet* p, eOipv4addr_t ip, eOnvEP8_t ep8)
{
    eOnvset_dev_t* theDevice = NULL;
    eOnvset_ep_t* theEndpoint = NULL;
 
    if((NULL == p)) 
    {
        return(NULL); 
    }

    // - retrieve the device and the endpoint
    if(eores_OK != s_eo_nvset_hid_get_device_endpoint_faster(p, ip, ep8, &theDevice, &theEndpoint))   
    {
        return(NULL);
    }
    
    return(theEndpoint->epram); 
}

extern void* eo_nvset_RAMofEntity_Get(EOnvSet* p, eOipv4addr_t ip, eOnvEP8_t ep8, eOnvENT_t ent, uint8_t index)
{
    eOnvset_dev_t* theDevice = NULL;
    eOnvset_ep_t* theEndpoint = NULL;
 
    if((NULL == p)) 
    {
        return(NULL); 
    }

    // - retrieve the device and the endpoint
    if(eores_OK != s_eo_nvset_hid_get_device_endpoint_faster(p, ip, ep8, &theDevice, &theEndpoint))   
    {
        return(NULL);
    }
    

//    return(eoprot_ep_entity_ram_extract(theDevice->boardnum, ep8, ent, theEndpoint->epram));
    
    return(eoprot_entity_ramof_get(theDevice->boardnum, ep8, ent, index));

}


extern void* eo_nvset_RAMofVariable_Get(EOnvSet* p, eOipv4addr_t ip, eOnvID32_t id32)
{
    eOnvset_dev_t* theDevice = NULL;
    eOnvset_ep_t* theEndpoint = NULL;
 
    if((NULL == p)) 
    {
        return(NULL); 
    }

    // - retrieve the device and the endpoint
    if(eores_OK != s_eo_nvset_hid_get_device_endpoint_faster(p, ip, eoprot_ID2endpoint(id32), &theDevice, &theEndpoint))   
    {
        return(NULL);
    }
      
    return(eoprot_variable_ramof_get(theDevice->boardnum, id32));
}

extern eOresult_t eo_nvset_BRD_Get(EOnvSet* p, eOipv4addr_t ip, eOnvBRD_t* brd)
{
    eOnvset_dev_t* theDevice = NULL;
 
    if((NULL == p) || (NULL == brd)) 
    {
        return(eores_NOK_nullpointer); 
    }

    // - retrieve the device. if the ip is not recognised, then ... eores_NOK_generic
    if(eores_OK != s_eo_nvset_hid_get_device(p, ip, &theDevice))   
    {
        return(eores_NOK_generic);
    }

    *brd = theDevice->boardnum;    

    return(eores_OK);    
}


extern eOresult_t eo_nvset_NV_Get(EOnvSet* p, eOipv4addr_t ip, eOnvID32_t id32, EOnv* thenv)
{
    eOnvset_dev_t* theDevice = NULL;
    eOnvset_ep_t* theEndpoint = NULL;
    eOnvEP8_t ep8 = eo_nv_hid_id32_extract_ep8(id32); 
    uint8_t brd = 0;
    eObool_t proxied = eobool_false;
    EOnv_rom_t* rom = NULL;
    uint8_t* ram = NULL;
    EOVmutexDerived* mtx2use = NULL;
    eOvoid_fp_cnvp_cropdesp_t onsay = NULL;

 
    if((NULL == p) || (NULL == thenv)) 
    {
        return(eores_NOK_nullpointer); 
    }

    // - retrieve the device and the endpoint. if the ep8 is not recognised, then ... eores_NOK_generic
    if(eores_OK != s_eo_nvset_hid_get_device_endpoint_faster(p, ip, ep8, &theDevice, &theEndpoint))   
    {
        return(eores_NOK_generic);
    }

    brd = theDevice->boardnum;
    
    // - verify that on the given endpoint there is a valid id32. if the id32 is not recognised, then ... eores_NOK_generic
    //if(eobool_false ==  theEndpoint->epcfg.protif->isidsupported(brd, id32))
    if(eobool_false == eoprot_id_isvalid(brd, id32))
    {
        return(eores_NOK_generic);       
    }
    
    // - retrieve from the device and endpoint what is required to form the netvar: con, ram, mtx, etc.   
    // - 0+. proxied?
    //proxied = theEndpoint->epcfg.protif->isvarproxied(brd, id32);     
    proxied = eoprot_variable_is_proxied(brd, id32);
    // - 0++. the onsay function
    onsay = eoprot_onsay_endpoint_get(ep8);   
    // - 1. the rom
    //rom = (EOnv_rom_t*) theEndpoint->epcfg.protif->getrom(brd, id32);
    rom = (EOnv_rom_t*) eoprot_variable_romof_get(brd, id32);
    // - 2. the ram
    //ram = (uint8_t*)theEndpoint->epcfg.protif->getram(brd, id32);
    ram = (uint8_t*) eoprot_variable_ramof_get(brd, id32);
    // - 3. the mtx
    mtx2use = s_eo_nvset_get_nvmutex(p, theDevice, theEndpoint, id32);
    
    
    // - final control about the validity of id32. it may be redundant but it is safer. for instance if the fptr_isepidsupported()
    //   does not take into account a removed tag and just checks that the tag-number is lower than the max allowed.
    
    if((NULL == rom) || (NULL == ram))  // mtx2use can be NULL
    {
        return(eores_NOK_generic); 
    }
    

    // - load everything into the nv
    eo_nv_hid_Load(     thenv,
                        ip, //(*theDevice)->ipaddress,
                        brd,
                        proxied,
                        id32,  
                        onsay,
                        rom,
                        ram,
                        mtx2use
                  );    

    return(eores_OK);
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


extern eOnvPROGnum_t eo_nvset_hid_EPprogressivenumber(EOnvSet* p, eOipv4addr_t ip, eOnvID32_t id32)
{
    eOnvset_dev_t* theDevice = NULL;
    eOnvset_ep_t* theEndpoint = NULL;
    
    eOnvEP8_t ep8 = eo_nv_hid_id32_extract_ep8(id32);
 
    if(NULL == p) 
    {
        return(EOK_uint32dummy); 
    }

    // - retrieve the device and the endpoint
    if(eores_OK != s_eo_nvset_hid_get_device_endpoint_faster(p, ip, ep8, &theDevice, &theEndpoint))   
    {
        return(EOK_uint32dummy);
    }
    
//    if(NULL == theEndpoint->epcfg.protif->epgetprognumber)
//    {
//        return(EOK_uint32dummy);
//    }
//    
//    return(theEndpoint->epcfg.protif->epgetprognumber(theDevice->boardnum, id32));
    return(eoprot_endpoint_id2prognum(theDevice->boardnum, id32));
}

extern uint16_t eo_nvset_hid_ip2index(EOnvSet* p, eOipv4addr_t ip)
{
    uint16_t index = 0;
    if(NULL == p)
    {
        return(EOK_uint16dummy);
    }
    if(eobool_true == eo_vector_Find(p->theipaddresses, NULL, &ip, &index))
    {
        return(index);  
    }
    return(EOK_uint16dummy);
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static EOVmutexDerived* s_eo_nvset_get_nvmutex(EOnvSet* p, eOnvset_dev_t* thedevice, eOnvset_ep_t* theendpoint, eOnvID32_t id32)
{
    EOVmutexDerived* mtx2use = NULL;
    
    if(eo_nvset_protection_none == p->protection)
    {
        mtx2use = NULL;
    }
    else
    {
        switch(p->protection)
        {
            case eo_nvset_protection_one_per_object:
            {
                mtx2use = p->mtx_object;
            } break;
            
            case eo_nvset_protection_one_per_device:
            {
                mtx2use = thedevice->mtx_device;
            } break;            
       
            case eo_nvset_protection_one_per_endpoint:
            {
                mtx2use = theendpoint->mtx_endpoint;
            } break;  
            
            case eo_nvset_protection_one_per_netvar:
            {
                //#warning .... i think of void* as a uint32_t*
                //uint32_t nvprognumber = theendpoint->epcfg.protif->epgetprognumber(thedevice->boardnum, id32);
                uint32_t nvprognumber = eoprot_endpoint_id2prognum(thedevice->boardnum, id32);
                uint32_t** addr = eo_vector_At(theendpoint->themtxofthenvs, nvprognumber);
                mtx2use = (EOVmutexDerived*) (*addr);
            } break;  
            
            default:
            {
                mtx2use = NULL;
            } break;
            
        }
    }
    
    return(mtx2use);  
}

static eOresult_t s_eo_nvset_hid_get_device(EOnvSet* p, eOipv4addr_t ip, eOnvset_dev_t** thedevice)
{
    eOnvset_dev_t** thedev = NULL;
    // get the device   
    uint16_t ondevindex = (eok_ipv4addr_localhost == ip) ? (p->devindexoflocaldevice) : (eo_nvset_hid_ip2index(p, ip));   
    if(EOK_uint16dummy == ondevindex)
    {
        return(eores_NOK_generic);
    }    
    thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
    if(NULL == thedev)
    {
        return(eores_NOK_nullpointer);
    }
 
   
    *thedevice      = *thedev;  

    return(eores_OK);
}

static eOresult_t s_eo_nvset_hid_get_device_endpoint_faster(EOnvSet* p, eOipv4addr_t ip, eOnvEP8_t ep8, eOnvset_dev_t** thedevice, eOnvset_ep_t** theendpoint)
{
    eOnvset_dev_t** thedev = NULL;
    uint16_t onendpointindex = EOK_uint16dummy;
    eOnvset_ep_t** theend = NULL;
    // get the device   
    uint16_t ondevindex = (eok_ipv4addr_localhost == ip) ? (p->devindexoflocaldevice) : (eo_nvset_hid_ip2index(p, ip));   
    if(EOK_uint16dummy == ondevindex)
    {
        return(eores_NOK_generic);
    }    
    thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
    if(NULL == thedev)
    {
        return(eores_NOK_nullpointer);
    }
 
    // get the endpoint
    onendpointindex = EOK_uint16dummy;
    if((NULL != (*thedev)->fptr_ep2index))
    {
        onendpointindex = (*thedev)->fptr_ep2index((*thedev)->ep2index_param, ep8);
    }
    // we dont do a full search because we must rely on the given function !!!
//     // if the fptr_ep2index is not present or if it fails ... we must do a full search
//     if(EOK_uint16dummy == onendpointindex)
//     {   // cannot fill w/ hash --> need to do an exhaustive search.
//         uint16_t j = 0;
//         uint16_t nendpoints = eo_vector_Size((*thedev)->theendpoints);
//         for(j=0; j<nendpoints; j++)
//         {
//             eOnvset_ep_t **tmpendpoint = (eOnvset_ep_t**) eo_vector_At((*thedev)->theendpoints, j);
//     
//             if(ep8 == (*tmpendpoint)->epcfg.endpoint)
//             {
//                 onendpointindex = j;
//                 break;
//             }
//         }
//     }    
    if(EOK_uint16dummy == onendpointindex)
    {
        return(eores_NOK_generic);
    }        
    theend = (eOnvset_ep_t**) eo_vector_At((*thedev)->theendpoints, onendpointindex);
    if(NULL == theend)
    {
        return(eores_NOK_nullpointer);
    }    

    
    *thedevice      = *thedev;
    *theendpoint    = *theend;
    

    return(eores_OK);
}


static void s_eo_nvset_devicesowneship_change(EOnvSet *p,  eOnvsetOwnership_t ownership)
{
    switch(p->devicesowneship)
    {
        case eo_nvset_devicesownership_none:
        {
            if(eo_nvset_ownership_local == ownership)
            {
                p->devicesowneship = eo_nvset_devicesownership_onelocal;    
            }
            else
            {
                p->devicesowneship = eo_nvset_devicesownership_someremote;
            }
        } break;

        case eo_nvset_devicesownership_onelocal:
        {
            if(eo_nvset_ownership_local == ownership)
            {
                eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "s_eo_nvset_devicesowneship_change(): at most one local", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
            }
            else
            {
                p->devicesowneship = eo_nvset_devicesownership_onelocalsomeremote;
            }
        } break;

        case eo_nvset_devicesownership_onelocalsomeremote:
        {
            if(eo_nvset_ownership_local == ownership)
            {
                eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "s_eo_nvset_devicesowneship_change() at most One local", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
            }
            else
            {
                p->devicesowneship = eo_nvset_devicesownership_onelocalsomeremote;
            }
        } break;

        case eo_nvset_devicesownership_someremote:
        {
            if(eo_nvset_ownership_local == ownership)
            {
                 p->devicesowneship = eo_nvset_devicesownership_onelocalsomeremote;    
            }
            else
            {
                p->devicesowneship = eo_nvset_devicesownership_someremote;
            }
        } break;

        default:
        {
            eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "s_eo_nvset_devicesowneship_change(): verify your code", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
        } break;

    }

}



// - not used ------------------------------------------------------------------------------------------------------------

// static eOresult_t s_eo_nvset_hid_get_device_endpoint(EOnvSet* p, eOipv4addr_t ip, eOnvEP_t ep, eOnvset_dev_t** thedevice, eOnvset_ep_t** theendpoint)
// {
//     // look for the indices for ip and ep. they are required to find the suitable eOnvset_dev_t and eOnvset_ep_t
//     uint16_t ondevindex;
//     uint16_t onendpointindex;
//     
//     if(eores_OK != s_eo_nvset_IPEP2indices(p, ip, ep, &ondevindex, &onendpointindex))
//     {
//         return(eores_NOK_generic);
//     }
//        
//     // - retrieve the device and the endpoint
//
//     eOnvset_dev_t** thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
//     if(NULL == thedev)
//     {
//         return(eores_NOK_nullpointer);
//     }
//     eOnvset_ep_t** theend = (eOnvset_ep_t**) eo_vector_At((*thedev)->theendpoints, onendpointindex);
//     if(NULL == theend)
//     {
//         return(eores_NOK_nullpointer);
//     }
//     
//     *thedevice      = *thedev;
//     *theendpoint    = *theend;
//     
//
//     return(eores_OK);
// }


// static eOresult_t s_eo_nvset_IPEP2indices(EOnvSet* p, eOipv4addr_t ip, eOnvEP_t ep, uint16_t* ondevindex, uint16_t* onendpointindex)
// {
//    // --- search for the index of the ip
//
//     if(eok_ipv4addr_localhost == ip)
//     {
//         *ondevindex = p->devindexoflocaldevice;
//     }
//     else
//     {
//         *ondevindex = eo_nvset_hid_ip2index(p, ip);
//     }
//
//     if(EOK_uint16dummy == *ondevindex)
//     {
//         return(eores_NOK_generic);
//     }
//
//     // --- search for the index of the ep
//
//     *onendpointindex = s_eo_nvset_ondevice_endpoint2index(p, *ondevindex, ep);
//     if(EOK_uint16dummy == *onendpointindex)
//     {
//         return(eores_NOK_generic);
//     }
//        
//     return(eores_OK);
// }

// extern uint16_t eo_nvset_hid_ondevice_endpoint2index(EOnvSet* p, uint16_t ondevindex, eOnvEP_t endpoint);
// extern uint16_t eo_nvset_hid_ondevice_onendpoint_id2progressivenumber(EOnvSet* p, uint16_t ondevindex, uint16_t onendpointindex, eOnvID_t id);

// extern uint16_t eo_nvset_hid_ondevice_endpoint2index(EOnvSet* p, uint16_t ondevindex, eOnvEP_t endpoint)
// {
//     if(NULL == p)
//     {
//         return(EOK_uint16dummy);
//     }

//     return(s_eo_nvset_ondevice_endpoint2index(p, ondevindex, endpoint));
// }


// extern uint16_t eo_nvset_hid_ondevice_onendpoint_id2progressivenumber(EOnvSet* p, uint16_t ondevindex, uint16_t onendpointindex, eOnvID_t id)
// {
//     if(NULL == p)
//     {
//         return(EOK_uint16dummy);
//     }

//     return(s_eo_nvset_ondevice_onendpoint_id2progressivenumber(p, ondevindex, onendpointindex, id));
// }

//static uint16_t s_eo_nvset_ondevice_onendpoint_id2progressivenumber(EOnvSet* p, uint16_t ondevindex, uint16_t onendpointindex, eOnvID_t id);
//static uint16_t s_eo_nvset_ondevice_onendpoint_id2progressivenumber(EOnvSet* p, uint16_t ondevindex, uint16_t onendpointindex, eOnvID_t id)
// {
// #if 1
// 		#warning ------------> write s_eo_nvset_ondevice_onendpoint_id2progressivenumber()
// #else	
//     eOnvset_dev_t** thedev = NULL;
//     eOnvset_ep_t **theendpoint = NULL;

//     thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
//     if(NULL == thedev)
//     {
//         return(EOK_uint08dummy);
//     }
//     theendpoint = (eOnvset_ep_t**) eo_vector_At((*thedev)->theendpoints, onendpointindex);
//     if(NULL == theendpoint)
//     {
//         return(EOK_uint16dummy);
//     }

//  
//     if((NULL != (*theendpoint)->hashfn_id2index))
//     {
//         uint16_t index = (*theendpoint)->hashfn_id2index(id);
//         if(EOK_uint16dummy != index)
//         {
//             return(index);
//         }        
//     }

//     if(1)
//     {   // cannot find with hash function, thus use exhaustive search
//         uint16_t k = 0;
//         uint16_t nvars = (*theendpoint)->epnvs_numberof;
//     
//         for(k=0; k<nvars; k++)
//         {
//             EOnv_con_t* nvcon = (EOnv_con_t*) eo_treenode_GetData((EOtreenode*) eo_constvector_At((*theendpoint)->thetreeofnvs_con, k));
//     
//             if(id == nvcon->id)
//             {
//                 return(k);
//             }
//         }

//     }
// 		
// 		
// #endif
//     return(EOK_uint16dummy);
// }

// static uint16_t s_eo_nvset_ondevice_endpoint2index(EOnvSet* p, uint16_t ondevindex, eOnvEP_t endpoint);
// static uint16_t s_eo_nvset_ondevice_endpoint2index(EOnvSet* p, uint16_t ondevindex, eOnvEP_t endpoint)
// {
//     eOnvset_dev_t** thedev = NULL;

//     thedev = (eOnvset_dev_t**) eo_vector_At(p->thedevices, ondevindex);
//     if(NULL == thedev)
//     {
//         return(EOK_uint16dummy);
//     }

//     if((NULL != (*thedev)->fptr_ep2index))
//     {
//         uint16_t index = (*thedev)->fptr_ep2index(endpoint);
//         if(EOK_uint16dummy != index)
//         {
//             return(index);
//         }        
//     }


//     if(1)
//     {   // cannot fill w/ hash --> need to do an exhaustive search.
//         uint16_t j = 0;
//         uint16_t nendpoints = eo_vector_Size((*thedev)->theendpoints);
//         for(j=0; j<nendpoints; j++)
//         {
//             eOnvset_ep_t **theendpoint = (eOnvset_ep_t**) eo_vector_At((*thedev)->theendpoints, j);
//     
//             if(endpoint == (*theendpoint)->endpoint)
//             {
//                 return(j);
//             }
//         }

//     }

//     return(EOK_uint16dummy);
// }

// extern eOnvsetDevicesOwnership_t eo_nvset_GetDevicesOwnership(EOnvSet* p)
// {
//  	if(NULL == p) 
// 	{
//         return(eo_nvset_devicesownership_none); 
// 	}
//     return(p->devicesowneship);
// }


// extern uint16_t eo_nvset_GetIndexOfLocalDEV(EOnvSet* p)
// {
//  	if(NULL == p) 
// 	{
// 		return(EOK_uint16dummy); 
// 	}
//     return(p->devindexoflocaldevice);
// }

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





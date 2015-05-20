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
#include "EoProtocolMN.h"

#include "EOconstvector_hid.h"


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

static const EOconstvector s_eonvset_constvectofEPcfgBasic = 
{
    EO_INIT(.capacity)        sizeof(eoprot_mn_basicEPcfg)/sizeof(eOprot_EPcfg_t),
    EO_INIT(.size)            sizeof(eoprot_mn_basicEPcfg)/sizeof(eOprot_EPcfg_t), 
    EO_INIT(.item_size)       sizeof(eOprot_EPcfg_t),
    EO_INIT(.dummy)           0,  
    EO_INIT(.stored_items)    (void*) &eoprot_mn_basicEPcfg,
    EO_INIT(.functions)       NULL     
};

const eOnvset_BRDcfg_t eonvset_BRDcfgBasic =
{
    EO_INIT(.boardnum)              99,
    EO_INIT(.dummy)                 {0, 0, 0},
    EO_INIT(.epcfg_constvect)       (EOconstvector*)&s_eonvset_constvectofEPcfgBasic    
};


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_eo_nvset_InitBRD(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t brdnum);

static eOresult_t s_eo_nvset_NVsOfEP_Initialise(EOnvSet* p, eOnvset_ep_t* endpoint, eOnvEP8_t ep08);

static eOresult_t s_eo_nvset_DeinitEPs(EOnvSet* p);
static eOresult_t s_eo_nvset_DeinitDEV(EOnvSet* p);

static EOVmutexDerived* s_eo_nvset_get_nvmutex(EOnvSet* p, eOnvID32_t id32);
static eOnvset_ep_t* s_eo_nvset_get_endpoint(EOnvSet* p, eOnvEP8_t ep8);
uint16_t s_eonvset_EP2INDEX(EOnvSet* p, uint8_t ep08);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOnvSet";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

 
extern EOnvSet* eo_nvset_New(eOnvset_protection_t prot, eov_mutex_fn_mutexderived_new mtxnew)
{
    EOnvSet *p = NULL;  

    // i get the memory for the object
    p = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_auto, sizeof(EOnvSet), 1);
    
    // i dont initialise yet the device. i simply rely on the fact that it contains all zero data.
    p->theboard.ipaddress       = 0;    
    p->mtxderived_new           = mtxnew; 
    p->protection               = (NULL == mtxnew) ? (eo_nvset_protection_none) : (prot); 

    return(p);
}


extern void eo_nvset_Delete(EOnvSet* p)
{   
    if(NULL == p)
    {
        return;
    }
    
    eo_nvset_DeinitBRD(p);
               
    memset(p, 0, sizeof(EOnvSet));
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;
}

extern eOresult_t eo_nvset_InitBRD(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t brdnum)
{
//    eOresult_t res = eores_NOK_generic;   
    if(NULL == p)
    {
        return(eores_NOK_generic);
    }
    
    if(0 != p->theboard.ipaddress)
    {   // already initted
        return(eores_NOK_generic);
    }
    
    if(0 == ipaddress)
    {   // the ipaddress must be non-zero
        return(eores_NOK_generic);
    }
    
    if(eo_nvset_ownership_local == ownership)
    {   // by this call we can later on retrieve a number for the local board.
        eoprot_config_board_local(brdnum);
    }
    else
    {   // by this call the eoprot library reserves space for a given board number
        if(eores_OK != eoprot_config_board_reserve(brdnum))
        {
            //#warning TBD: shall i call the error manager?
            return(eores_NOK_generic);
        }
    }    
    
    if(eores_OK != s_eo_nvset_InitBRD(p, ownership, ipaddress, brdnum))
    {
        return(eores_NOK_generic);
    }
    
    
    return(eores_OK); 
}


extern eOresult_t eo_nvset_BRDlocalsetnumber(EOnvSet* p, eOnvBRD_t brdnum)
{    
    if(NULL == p)
    {
        return(eores_NOK_generic);
    }
    
    if(0 == p->theboard.ipaddress)
    {   // not already initted
        return(eores_NOK_generic);
    }
    
    if(eo_nvset_ownership_local == p->theboard.ownership)
    {   // by this call we can later on retrieve a number for the local board.
        eoprot_config_board_local(brdnum);
        return(eores_OK); 
    }    
    
    return(eores_NOK_generic); 
}

extern eOresult_t eo_nvset_InitBRD_LoadEPs(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvset_BRDcfg_t* cfgofbrd, eObool_t initNVs)
{
    uint16_t nendpoints = 0;
    uint16_t i = 0;
    
    if((NULL == p) || (NULL == cfgofbrd))
    {
        return(eores_NOK_nullpointer);
    }
        
    if(eores_OK != eo_nvset_InitBRD(p, ownership, ipaddress, cfgofbrd->boardnum))
    {
        return(eores_NOK_generic);
    }
    
    nendpoints = eo_constvector_Size(cfgofbrd->epcfg_constvect);        
    for(i=0; i<nendpoints; i++)
    {
        eOprot_EPcfg_t* pepcfg = (eOprot_EPcfg_t*) eo_constvector_At(cfgofbrd->epcfg_constvect, i);
        if(eobool_true == eoprot_EPcfg_isvalid(pepcfg))
        {
            eo_nvset_LoadEP(p, pepcfg, initNVs);
        }
        else
        {
            //#warning TBD: put diagnostic message
        }
    }
    
    return(eores_OK); 
}


extern eOresult_t eo_nvset_DeinitBRD(EOnvSet* p)
{   
    if(NULL == p)
    {
        return(eores_NOK_nullpointer);
    }
    
    if(0 == p->theboard.ipaddress)
    {   // already deinitted or never initted
        return(eores_OK);
    }
    
    s_eo_nvset_DeinitEPs(p);        // all the EPs added one by one with eo_nvset_LoadEP()
    s_eo_nvset_DeinitDEV(p);        // deinit what done with eo_nvset_InitBRD()
    
    
    //#warning TBD: we should also call something to revert eoprot_config_board_reserve() ... but see comment
    // it is tricky to do it because i should also add a .used flag. then if i unreserve(brd) i should:
    // 1. set .used = 0 in entry brd
    // 2. verify from maxusedbrd down to 0 and find the first .used == 1. then call a realloc to that index ...
    // should i do it?
    
    // so that we dont get inside it again
    p->theboard.ipaddress = 0;
    
    return(eores_OK); 
}

static eOresult_t s_eo_nvset_NVsOfEP_Initialise(EOnvSet* p, eOnvset_ep_t* endpoint, eOnvEP8_t ep08)
{
    eOnvset_brd_t* theBoard = NULL;
    eOnvset_ep_t* theEndpoint = endpoint;

    eOvoid_fp_uint32_voidp_t initialise = NULL;
    
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    theBoard = &p->theboard;
    

    if(NULL == theEndpoint)
    {
        theEndpoint = s_eo_nvset_get_endpoint(p, ep08);   
        if(NULL == theEndpoint)
        {
            return(eores_NOK_generic);
        } 
        
//        if(ep08 != theEndpoint->epcfg.endpoint)
//        {
//            return(eores_NOK_generic);
//        }        
    }    
       

    if(eobool_true == (theEndpoint->initted))
    {   // already initted
        return(eores_OK);
    }            

    initialise = eoprot_endpoint_get_initialiser(ep08);
    
    if(NULL != initialise)
    {
        initialise(theBoard->ipaddress, theEndpoint->epram);
    }
    
    theEndpoint->initted = eobool_true;
    

#define EO_NVSET_INIT_EVERY_NV
#if defined(EO_NVSET_INIT_EVERY_NV)
    {   // put parenthesis to create a new scope and avoid errors in non c99 environments as windows
        EOnv thenv = {0};
        uint16_t k = 0;
        EOnv_rom_t* rom = NULL;
        uint8_t* ram = NULL;
        uint16_t nvars = theEndpoint->epnvsnumberof;
        eOipv4addr_t ip = theBoard->ipaddress;
        uint8_t brd =  theBoard->boardnum; // local or 0, 1, 2, 3
        eOnvID32_t id32 = EOK_uint32dummy;     
        uint32_t prog = 0;
        eObool_t proxied = eobool_false;
        eOvoid_fp_cnvp_cropdesp_t onsay = NULL;
        
        EOVmutexDerived* mtx2use = NULL;

        if(eo_nvset_protection_one_per_board == p->protection)
        {
            mtx2use = theBoard->mtx_board;
        }
        else if(eo_nvset_protection_one_per_endpoint == p->protection)
        {
            mtx2use = theEndpoint->mtx_endpoint;
        }
        // else if eo_nvset_protection_one_per_netvar ... eval foreach k

        for(k=0; k<nvars; k++)
        {
            if(eo_nvset_protection_one_per_netvar == p->protection)
            {
                //#warning -> think of uint32 and void* maybe use uint64
                uint32_t** addr = eo_vector_At(theEndpoint->themtxofthenvs, k);
                mtx2use = (EOVmutexDerived*) (*addr);
            }
            
            prog = k;
            // - 0. the id32 
            id32 = eoprot_endpoint_prognum2id(brd, ep08, prog);               
            if(EOK_uint32dummy == id32)
            {
                continue;
            }

            // - 0+. proxied?
            proxied = eoprot_variable_is_proxied(brd, id32);                
            // - 0++ onsay
            onsay = eoprot_onsay_endpoint_get(ep08); 
            // - 1. the rom
            rom = (EOnv_rom_t*) eoprot_variable_romof_get(brd, id32);
            // - 2. the ram
            ram = (uint8_t*) eoprot_variable_ramof_get(brd, id32);
            // - 3. the mtx
            mtx2use = mtx2use;
            // - load everything into the nv
            eo_nv_hid_Load(     &thenv,
                                ip, 
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

    
    return(eores_OK);    
}
  

extern eOresult_t eo_nvset_NVSinitialise(EOnvSet* p)
{
    eOnvset_brd_t* theBoard = NULL;   
    uint16_t j;
    uint16_t nendpoints;

 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}

    theBoard = &p->theboard;
    nendpoints = eo_vector_Size(theBoard->theendpoints);
    
    for(j=0; j<nendpoints; j++)
    {
        eOnvset_ep_t** theEndpoint = (eOnvset_ep_t**) eo_vector_At(theBoard->theendpoints, j);                        
        s_eo_nvset_NVsOfEP_Initialise(p, (*theEndpoint), (*theEndpoint)->epcfg.endpoint);
    }       

    return(eores_OK);
}


//extern eOresult_t eo_nvset_NVSdeinitialise(EOnvSet* p)
//{
//    //#warning -> add code in here for eo_nvset_NVSdeinitialise()
//    // so far it is not necessary to deinit    
//    return(eores_OK);    
//}


extern void* eo_nvset_RAMofEndpoint_Get(EOnvSet* p, eOnvEP8_t ep8)
{ 
    if((NULL == p)) 
    {
        return(NULL); 
    }
    
    // get directly the ram using the eoprot function.     
    return(eoprot_endpoint_ramof_get(p->theboard.boardnum, ep8));   
}


extern void* eo_nvset_RAMofEntity_Get(EOnvSet* p, eOnvEP8_t ep8, eOnvENT_t ent, uint8_t index)
{
    if((NULL == p)) 
    {
        return(NULL); 
    }
    
    // get directly the ram using the eoprot function.     
    return(eoprot_entity_ramof_get(p->theboard.boardnum, ep8, ent, index));
}


extern void* eo_nvset_RAMofVariable_Get(EOnvSet* p, eOnvID32_t id32)
{ 
    if((NULL == p)) 
    {
        return(NULL); 
    }

    return(eoprot_variable_ramof_get(p->theboard.boardnum, id32));
}


extern eOresult_t eo_nvset_BRD_Get(EOnvSet* p, eOnvBRD_t* brd)
{ 
    if((NULL == p) || (NULL == brd)) 
    {
        return(eores_NOK_nullpointer); 
    }

    *brd = p->theboard.boardnum;    

    return(eores_OK);    
}


extern eOresult_t eo_nvset_NV_Get(EOnvSet* p, eOnvID32_t id32, EOnv* thenv)
{
    eOnvEP8_t ep8 = eoprot_ID2endpoint(id32); 
    uint8_t brd = 0; // local, or 0, 1, 2, 3 ...
    eObool_t proxied = eobool_false;
    EOnv_rom_t* rom = NULL;
    uint8_t* ram = NULL;
    EOVmutexDerived* mtx2use = NULL;
    eOvoid_fp_cnvp_cropdesp_t onsay = NULL;
 
    if((NULL == p) || (NULL == thenv)) 
    {
        return(eores_NOK_nullpointer); 
    }

    brd = p->theboard.boardnum;
    
    // - verify that on the given endpoint there is a valid id32. if the id32 is not recognised, then ... eores_NOK_generic
    if(eobool_false == eoprot_id_isvalid(brd, id32))
    {
        return(eores_NOK_generic);       
    }
    
    // - retrieve from the device and endpoint what is required to form the netvar: con, ram, mtx, etc.   
    // - 0+. proxied? 
    proxied = eoprot_variable_is_proxied(brd, id32);
    // - 0++. the onsay function
    onsay = eoprot_onsay_endpoint_get(ep8);   
    // - 1. the rom
    rom = (EOnv_rom_t*) eoprot_variable_romof_get(brd, id32);
    // - 2. the ram
    ram = (uint8_t*) eoprot_variable_ramof_get(brd, id32);
    // - 3. the mtx
    mtx2use = s_eo_nvset_get_nvmutex(p, id32);
        
    // - final control about the validity of id32. it may be redundant but it is safer. for instance if the fptr_isepidsupported()
    //   does not take into account a removed tag and just checks that the tag-number is lower than the max allowed.
    
    if((NULL == rom) || (NULL == ram))  // mtx2use can be NULL
    {
        return(eores_NOK_generic); 
    }
    

    // - load everything into the nv
    eo_nv_hid_Load(     thenv,
                        p->theboard.ipaddress,
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



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static eOresult_t s_eo_nvset_InitBRD(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t brdnum)
{
    eOnvset_brd_t *theBoard = NULL;
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
       
    theBoard = &p->theboard;
    
    if(NULL != theBoard->theendpoints)
    {   // already initted
        return(eores_NOK_nullpointer);
    }
       
    theBoard->ipaddress             = ipaddress;
    theBoard->boardnum              = (eo_nvset_ownership_local == ownership) ? (eoprot_board_localboard) : (brdnum);
    theBoard->ownership             = ownership;
    theBoard->theendpoints          = eo_vector_New(sizeof(eOnvset_ep_t*), eo_vectorcapacity_dynamic, NULL, 0, NULL, NULL);    
    theBoard->mtx_board             = (eo_nvset_protection_one_per_board == p->protection) ? p->mtxderived_new() : NULL;
    // reset the ep2indexlut to have all values EOK_uint16dummy
    {
        uint8_t i = 0;
        const uint8_t lutsize = eonvset_max_endpoint_value+1;
        for(i=0; i<lutsize; i++)
        {
            theBoard->ep2indexlut[i] = EOK_uint16dummy;
        }
    }

    return(eores_OK);
}


static eOresult_t s_eo_nvset_DeinitDEV(EOnvSet* p)
{
    eOnvset_brd_t *theBoard = &p->theboard;
    
    // the array of endpoints
    eo_vector_Delete(theBoard->theendpoints);
    
    // the mtx of device, if non NULL
    if(NULL != theBoard->mtx_board)
    {
        eov_mutex_Delete(theBoard->mtx_board);
    }
    
    
    // so that we know that everything is deinitted
    p->theboard.ipaddress = 0;
     
    return(eores_OK);
}



extern eOresult_t eo_nvset_LoadEP(EOnvSet* p, eOprot_EPcfg_t *cfgofep, eObool_t initNVs)
{
    eOnvset_brd_t* theBoard = NULL;
    eOnvBRD_t brd = 0;      // local or 0, 1, 2, etc.
    eOnvset_ep_t *theEndpoint = NULL;
    uint16_t epnvsnumberof = 0;  
    uint16_t sizeofram =0;
 
    if((NULL == p) || (NULL == cfgofep)) 
    {
        return(eores_NOK_nullpointer); 
    }
        
        
    theBoard = &p->theboard;
    brd = p->theboard.boardnum;
        
    theEndpoint = eo_mempool_New(eo_mempool_GetHandle(), 1*sizeof(eOnvset_ep_t));
    
    memcpy(&theEndpoint->epcfg, cfgofep, sizeof(eOprot_EPcfg_t));   
    
    // ok, now in theEndpoint->epcfg.numberofsentities[] we have some ram. we use it to load the protocol.
    eoprot_config_endpoint_entities(brd, theEndpoint->epcfg.endpoint, theEndpoint->epcfg.numberofentities);
    // now it is ok to compute the size of the endpoint using the proper protocol function
    sizeofram = eoprot_endpoint_sizeof_get(brd, theEndpoint->epcfg.endpoint); 
    
    // now that i have loaded  the number of entities i verify if they are ok by checking the number of variables in the endpoint.
    epnvsnumberof = eoprot_endpoint_numberofvariables_get(brd, cfgofep->endpoint);   
    if(0 == epnvsnumberof)
    {
        //#warning TBD: see how we continue in here ....
        char str[64] = {0};
        snprintf(str, sizeof(str), "EOnvSet: ep %d has 0 nvs", cfgofep->endpoint);  
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_error, str, NULL, &eo_errman_DescrRuntimeErrorLocal); 
        
        eoprot_config_endpoint_entities(brd, theEndpoint->epcfg.endpoint, NULL);
        eo_mempool_Delete(eo_mempool_GetHandle(), theEndpoint); 
        
        return(eores_NOK_generic); 
    }  
    
    theEndpoint->epnvsnumberof      = epnvsnumberof;
    theEndpoint->initted            = eobool_false;    
    theEndpoint->epram              = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_auto, sizeofram, 1);
    theEndpoint->mtx_endpoint       = (eo_nvset_protection_one_per_endpoint == p->protection) ? p->mtxderived_new() : NULL;
        
    // now we must load the ram in the endpoint
    eoprot_config_endpoint_ram(brd, theEndpoint->epcfg.endpoint, theEndpoint->epram, sizeofram);
    
    // now add the vector of mtx if needed.
    if(eo_nvset_protection_one_per_netvar == p->protection)
    {
        uint16_t i;
        theEndpoint->themtxofthenvs = eo_vector_New(sizeof(EOVmutexDerived*), epnvsnumberof, NULL, 0, NULL, NULL);
        for(i=0; i<epnvsnumberof; i++)
        {
            EOVmutexDerived* mtx = p->mtxderived_new();
            eo_vector_PushBack(theEndpoint->themtxofthenvs, &mtx);           
        }
    }
    
    // now, i must update the mapping function from ep value to vector of endpoints  
    theBoard->ep2indexlut[theEndpoint->epcfg.endpoint] = eo_vector_Size(theBoard->theendpoints);
    // and only now i push back the endpoint
    eo_vector_PushBack(theBoard->theendpoints, &theEndpoint);
    
    
    if(eobool_true == initNVs)
    {
        s_eo_nvset_NVsOfEP_Initialise(p, theEndpoint, theEndpoint->epcfg.endpoint); 
    }

    return(eores_OK);
}



static eOresult_t s_eo_nvset_DeinitEPs(EOnvSet* p)
{
    eOnvset_brd_t* theBoard = &p->theboard;   
    uint16_t i = 0;  
    uint16_t endpointsnum = 0;
    
    if(NULL == theBoard->theendpoints)
    {   // already deinitted
        return(eores_OK);
    }
   
    endpointsnum = eo_vector_Size(theBoard->theendpoints);
 
    for(i=0; i<endpointsnum; i++)
    {
        eOnvset_ep_t **ppep = (eOnvset_ep_t **)eo_vector_At(theBoard->theendpoints, i);
        eOnvset_ep_t *theEndpoint = *ppep;
        
        // now i erase memory associated with this endpoint
        eo_mempool_Delete(eo_mempool_GetHandle(), theEndpoint->epram);
        // and i dissociates that from from the internals of the eoprot library
        eoprot_config_endpoint_ram(theBoard->boardnum, theEndpoint->epcfg.endpoint, NULL, 0);
        // i also de-init the number of entities for that endpoint
        eoprot_config_endpoint_entities(theBoard->boardnum, theEndpoint->epcfg.endpoint, NULL);
        
        // now i delete all data associated to the mutex protection
        
        if(NULL != theEndpoint->mtx_endpoint)
        {
            eov_mutex_Delete(theEndpoint->mtx_endpoint);
        }
        if(NULL != theEndpoint->themtxofthenvs)
        {
            uint16_t size = eo_vector_Size(theEndpoint->themtxofthenvs);
            int j = 0;
            for(j=0; j<size; j++)
            {
                EOVmutexDerived** pmtx =  (EOVmutexDerived**)eo_vector_At(theEndpoint->themtxofthenvs, j);
                eov_mutex_Delete(*pmtx);            
            } 
            eo_vector_Delete(theEndpoint->themtxofthenvs);
        }
   
        // now i erase the memory of the entire eOnvset_ep_t entry        
        eo_mempool_Delete(eo_mempool_GetHandle(), theEndpoint);       
    }
    
    // so that we dont get in here inside again
    theBoard->theendpoints = NULL;

    return(eores_OK);
}


static EOVmutexDerived* s_eo_nvset_get_nvmutex(EOnvSet* p, eOnvID32_t id32)
{
    EOVmutexDerived* mtx2use = NULL;
    
    if(eo_nvset_protection_none == p->protection)
    {   // the most common situation is no protection, thus ...
        mtx2use = NULL;
    }
    else
    {
        switch(p->protection)
        {            
            case eo_nvset_protection_one_per_board:
            {
                mtx2use = p->theboard.mtx_board;
            } break;            
       
            case eo_nvset_protection_one_per_endpoint:
            {
                // compute the endpoint and ....
                eOnvset_ep_t* theEndpoint = s_eo_nvset_get_endpoint(p, eoprot_ID2endpoint(id32));
                if(NULL != theEndpoint)
                {
                    mtx2use = theEndpoint->mtx_endpoint;
                }                    
            } break;  
            
            case eo_nvset_protection_one_per_netvar:
            {
                eOnvset_ep_t* theEndpoint = s_eo_nvset_get_endpoint(p, eoprot_ID2endpoint(id32));
                if(NULL != theEndpoint)
                {
                    //#warning .... i think of void* as a uint32_t* ///////////// think of it
                    uint32_t nvprognumber = eoprot_endpoint_id2prognum(p->theboard.boardnum, id32);
                    uint32_t** addr = eo_vector_At(theEndpoint->themtxofthenvs, nvprognumber);
                    mtx2use = (EOVmutexDerived*) (*addr);
                }
            } break;  
            
            default:
            {
                mtx2use = NULL;
            } break;
            
        }
    }
    
    return(mtx2use);  
}


static eOnvset_ep_t* s_eo_nvset_get_endpoint(EOnvSet* p, eOnvEP8_t ep8)
{
    eOnvset_brd_t* theBoard = &p->theboard;
    eOnvset_ep_t** ptheEndpoint = NULL;
    uint16_t onendpointindex = s_eonvset_EP2INDEX(p, ep8);
 
    if(EOK_uint16dummy == onendpointindex)
    {
        return(NULL);
    }   
    
    ptheEndpoint = (eOnvset_ep_t**) eo_vector_At(theBoard->theendpoints, onendpointindex);
    if(NULL == ptheEndpoint)
    {
        return(NULL);
    }    
   
    return(*ptheEndpoint);
}


uint16_t s_eonvset_EP2INDEX(EOnvSet* p, uint8_t ep08)
{
    eOnvset_brd_t* theBoard = &p->theboard;
    uint16_t index = EOK_uint16dummy;
    const uint8_t lutsize = eonvset_max_endpoint_value+1;
    if(ep08 < lutsize)
    {
        index = theBoard->ep2indexlut[ep08];
    }
    return(index);
}        

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





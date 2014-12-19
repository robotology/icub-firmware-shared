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

#include "EOnvsetDEVbuilder.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOnvsetDEVbuilder_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
    uint8_t         en;
    uint8_t         enmultiplicity;     // ie en is a joint, it tells how many joints there are
    uint8_t         tagsnumber;         // ie en is a joint, it tells how many tags there are in a joint
    uint8_t         filler01;
} ET_holder_t;

typedef struct
{
    uint8_t         ep;
    uint8_t         numofentities;  // if ep is motion control, it tells how many entities there are: 3 (joint, motor, controller) 
    uint16_t        filler02;
    EOvector*       entities; // contains ET_holder_t items
} ep_holder_t;


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_device_initialise(void *p, eObool_t islocal);

static uint16_t s_eoprot_ep2index(void* p, eOnvEP8_t ep);

static eOresult_t s_eo_nvsetdevbuilder_theendpoints_clear(EOnvsetDEVbuilder* p);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOnvsetDEVbuilder";


static const eOvoid_fp_uint32_voidp_t ram_init_ep[] =
{
    eoprot_fun_INITIALISE_mn,
    eoprot_fun_INITIALISE_mc,
    eoprot_fun_INITIALISE_as,
    eoprot_fun_INITIALISE_sk
};

extern const uint8_t* const eoprot_ep_tags_numberof[];

//#warning --> much better using ... proper include file 
//#include "EoProtocolEPs.h"

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOnvsetDEVbuilder* eo_nvsetdevbuilder_New(eOnvBRD_t board)
{
    EOnvsetDEVbuilder *p = NULL;  
    uint8_t i = 0;


    // i get the memory for the object
    p = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOnvsetDEVbuilder), 1);
    
    p->boardnum                 = board;
    //p->theendpoints_numberof    = 0;            // maxendpoints
    p->theepcfgs                = NULL;
    p->theendpoints             = eo_vector_New(sizeof(ep_holder_t), eo_vectorcapacity_dynamic, NULL, 0, NULL, NULL);  // or maxendpoints
    for(i=0; i<eoprot_endpoints_numberof; i++)
    {
        p->ep2indexhashtable[i] = EOK_uint16dummy; 
    }
    
    memset(p->eoprot_mn_entities_numberofeach, 0, sizeof(p->eoprot_mn_entities_numberofeach));
    memset(p->eoprot_mc_entities_numberofeach, 0, sizeof(p->eoprot_mc_entities_numberofeach));
    memset(p->eoprot_as_entities_numberofeach, 0, sizeof(p->eoprot_as_entities_numberofeach));
    memset(p->eoprot_sk_entities_numberofeach, 0, sizeof(p->eoprot_sk_entities_numberofeach));
    p->eoprot_ep_entities_numberofeach[eoprot_endpoint_management]      = NULL;
    p->eoprot_ep_entities_numberofeach[eoprot_endpoint_motioncontrol]   = NULL;
    p->eoprot_ep_entities_numberofeach[eoprot_endpoint_analogsensors]   = NULL;
    p->eoprot_ep_entities_numberofeach[eoprot_endpoint_skin]            = NULL;
    
    p->isvarproxied = NULL;
    
    p->devcfg = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(eOnvset_DEVcfg_t), 1);
    
    p->devcfg->boardnum                     = eo_prot_BRDdummy;
    p->devcfg->param                        = NULL;
    p->devcfg->fptr_device_initialise       = NULL;          
    p->devcfg->vectorof_epcfg               = NULL;
    p->devcfg->fptr_ep2indexofepcfg         = NULL;          
    
    return(p);
}


extern void eo_nvsetdevbuilder_Delete(EOnvsetDEVbuilder* p)
{
    if(NULL == p)
    {
        return;
    } 
    
    if(NULL == p->devcfg)
    {
        return;
    }
    
    eo_nvsetdevbuilder_UnPrepare(p);
    
    s_eo_nvsetdevbuilder_theendpoints_clear(p);
    
    eo_vector_Delete(p->theendpoints);
    
    memset(p->devcfg, 0, sizeof(eOnvset_DEVcfg_t)); 
    eo_mempool_Delete(eo_mempool_GetHandle(), p->devcfg);
   
       
    memset(p, 0, sizeof(EOnvsetDEVbuilder));    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;       
}

extern eOresult_t eo_nvsetdevbuilder_ENDPOINTpushback(EOnvsetDEVbuilder* p, eOnvEP8_t ep) 
{
    uint16_t size = 0;
    uint8_t *nn = NULL;
    ep_holder_t epholder = {0};

 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic); 
    }
    
    size = eo_vector_Size(p->theendpoints);
    
    if(EOK_uint16dummy != p->ep2indexhashtable[ep])
    {   // already inserted
        return(eores_OK); 
    }
    
    epholder.ep                 = ep;
    epholder.numofentities      = 0; // or maxentities in case u dont want using eo_vectorcapacity_dynamic
    epholder.entities           = eo_vector_New(sizeof(ET_holder_t), eo_vectorcapacity_dynamic, NULL, 0, NULL, NULL);  // or maxentities
    
    
    // i put the ep inside ... in next position. i put its size in the hash table
    p->ep2indexhashtable[ep] = size;
    eo_vector_PushBack(p->theendpoints, &epholder);   
    
    // now i assign the ..
    nn = NULL;
    switch(ep)
    {
        case eoprot_endpoint_management:
        {
            nn = p->eoprot_mn_entities_numberofeach;
        } break;
        case eoprot_endpoint_motioncontrol:
        {
            nn = p->eoprot_mc_entities_numberofeach;
        } break; 
        case eoprot_endpoint_analogsensors:
        {
            nn = p->eoprot_as_entities_numberofeach;
        } break;    
        case eoprot_endpoint_skin:
        {
            nn = p->eoprot_sk_entities_numberofeach;
        } break;  
        default:
        {
            nn = NULL;
        } break;
    }
    
    p->eoprot_ep_entities_numberofeach[ep] = nn;
        
    return(eores_OK);
}

extern eOresult_t eo_nvsetdevbuilder_ENTITYpushback(EOnvsetDEVbuilder* p, eOnvEP8_t ep, eOnvENT_t en, uint8_t howmanyofthisentity) //, uint8_t numberoftagsinentity)
{
    uint16_t epindex = 0;
    ep_holder_t* eph = NULL;
    ET_holder_t etholder = {0};

 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic); 
    }
    
    if(eores_OK != eo_nvsetdevbuilder_ENDPOINTpushback(p, ep))
    {
        return(eores_NOK_generic); 
    }
    
    epindex = p->ep2indexhashtable[ep];
    
    if(EOK_uint16dummy == epindex)
    {   // ep was never inserted.
        //#warning --> i may insert it now .... with: eo_nvsetdevbuilder_ENDPOINTpushback(p, ep); epindex = p->ep2indexhashtable[ep];
        return(eores_NOK_generic); 
//        eo_nvsetdevbuilder_ENDPOINTpushback(p, ep); 
//        epindex = p->ep2indexhashtable[ep];
    }
    
    // ok, i get the epholder an i modify it.
    eph = (ep_holder_t*) eo_vector_At(p->theendpoints, epindex);
    
    // i hope that eph is not NULL
    
    if(NULL == eph)
    {
        return(eores_NOK_generic); 
    }
    
    etholder.en                 = en;
    etholder.enmultiplicity     = howmanyofthisentity;
    etholder.tagsnumber         = eoprot_ep_tags_numberof[ep][en]; // or numberoftagsinentity

    // now i put the ethholde inside the eph

    eo_vector_PushBack(eph->entities, &etholder);
    eph->numofentities =  (uint8_t) eo_vector_Size(eph->entities); // numofentities is never too big
    
    // now change the number of entities in the ep. the default is zero.
    p->eoprot_ep_entities_numberofeach[ep][en] = howmanyofthisentity;
 
    return(eores_OK);
    
}

extern eOresult_t eo_nvsetdevbuilder_PROXIEDRULEset(EOnvsetDEVbuilder* p, eObool_fp_uint32_t isvarproxied_fn)
{
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    p->isvarproxied     = isvarproxied_fn;

    return(eores_OK);    
}

extern eOresult_t eo_nvsetdevbuilder_Prepare(EOnvsetDEVbuilder* p)
{
    uint16_t numofeps = 0;
    eOnvset_EPcfg_t epcfg = {0};
    eOnvset_EPcfg_t* data = NULL;
    uint8_t i = 0;
    uint8_t j = 0;
    
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    if(eo_prot_BRDdummy != p->devcfg->boardnum)
    {
        return(eores_OK);
    }
    
    
    p->devcfg->boardnum                     = p->boardnum;
    p->devcfg->param                        = p;
    p->devcfg->fptr_device_initialise       = s_device_initialise;  
    p->devcfg->vectorof_epcfg               = NULL;
    p->devcfg->fptr_ep2indexofepcfg         = s_eoprot_ep2index;           
    
    numofeps = eo_vector_Size(p->theendpoints);
    p->theepcfgs = eo_vector_New(sizeof(eOnvset_EPcfg_t), numofeps, NULL, 0, NULL, NULL);
    
    
    epcfg.dummy         = 0;
    epcfg.protif        = (eOnvset_protocol_Interface_t*)&eoprot_eonvset_Interface;
    
    for(i=0; i<numofeps; i++)
    {   // i is for endpoint
        ep_holder_t* eph = (ep_holder_t*) eo_vector_At(p->theendpoints, i);
        
        uint8_t numofentities = eph->numofentities;
        uint16_t epram_sizeof = 0;
        for(j=0; j<numofentities; j++)
        {   // j is for entity
            ET_holder_t* ETh = (ET_holder_t*) eo_vector_At(eph->entities, j);
            epram_sizeof += (ETh->enmultiplicity * eoprot_entity_sizeof_get(p->boardnum, eph->ep, ETh->en));
        }       
        
        epcfg.endpoint              = eph->ep;
        epcfg.epram_sizeof          = epram_sizeof;
        epcfg.fptr_ram_initialise   = ram_init_ep[eph->ep];   
        
        // ok, add it in the vector
        eo_vector_PushBack(p->theepcfgs, &epcfg);
        
        
//         // init the protocol ... with the number of entity for the endpoint
//         if(NULL != p->eoprot_ep_entities_numberofeach[eph->ep])
//         {
//             eoprot_config_endpoint_entities(p->boardnum, eph->ep, p->eoprot_ep_entities_numberofeach[eph->ep]);
//         }
               
    }
       
    // marco.accame on december 19, 2014: what is inside p cannot be deleted because everything is used for the
    // configuration of the nvset.
    data = (eOnvset_EPcfg_t*) eo_vector_storage_Get(p->theepcfgs);
    
    p->devcfg->vectorof_epcfg = eo_constvector_New(sizeof(eOnvset_EPcfg_t), numofeps, data);
    
    
    return(eores_OK);    
}

extern eOnvset_DEVcfg_t* eo_nvsetdevbuilder_DEVcfg_Get(EOnvsetDEVbuilder* p)
{
    eOresult_t res = eores_NOK_generic;
 	if(NULL == p) 
	{
		return(NULL); 
	}
    
    if(eo_prot_BRDdummy == p->devcfg->boardnum)
    {
        res = eo_nvsetdevbuilder_Prepare(p);
        if(eores_OK != res)
        {
            return(NULL);
        }
    }
    
    return(p->devcfg);   
}

extern eOresult_t eo_nvsetdevbuilder_UnPrepare(EOnvsetDEVbuilder* p)
{
    uint16_t numofeps = 0;
    eOnvset_EPcfg_t epcfg = {0};
    eOnvset_EPcfg_t* data = NULL;
    uint8_t i = 0;
    uint8_t j = 0;
    
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    if(eo_prot_BRDdummy == p->devcfg->boardnum)
    {
        return(eores_OK);
    }
    
    eo_constvector_Delete((EOconstvector*)p->devcfg->vectorof_epcfg);
    
    eo_vector_Delete(p->theepcfgs);
       
    p->devcfg->boardnum                     = eo_prot_BRDdummy;
    p->devcfg->param                        = NULL;
    p->devcfg->fptr_device_initialise       = NULL;  
    p->devcfg->vectorof_epcfg               = NULL;
    p->devcfg->fptr_ep2indexofepcfg         = NULL;           

    return(eores_OK);   
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_device_initialise(void* param, eObool_t islocal)
{
    EOnvsetDEVbuilder* p = (EOnvsetDEVbuilder*)param;
    
    uint8_t i = 0;
    
    uint16_t numofeps = eo_vector_Size(p->theendpoints);    
    
    // 1. configure protocol for teh given board so that it manages the correct number of entities
    
    for(i=0; i<numofeps; i++)
    {   // i is for endpoint
        ep_holder_t* eph = (ep_holder_t*) eo_vector_At(p->theendpoints, i);

        
        // init the protocol ... with the number of entity for the endpoint
        if(NULL != p->eoprot_ep_entities_numberofeach[eph->ep])
        {
            eoprot_config_endpoint_entities(p->boardnum, eph->ep, p->eoprot_ep_entities_numberofeach[eph->ep]);
        }
               
    }
    
    // 2. if the board is local, then set it
    
    if(eobool_true == islocal)
    {
        eoprot_config_board_local(p->boardnum);
    }    
    
    // 3. if board is local configure proxied variables in this board

    if(eobool_true == islocal)
    {    
        eoprot_config_proxied_variables(p->boardnum, p->isvarproxied);
    }   
        
    
    return(eores_OK);
}

static uint16_t s_eoprot_ep2index(void *param, eOnvEP8_t ep)
{
    EOnvsetDEVbuilder* p = (EOnvsetDEVbuilder*)param;
    uint16_t epindex = p->ep2indexhashtable[ep];
    return(epindex);
}


static eOresult_t s_eo_nvsetdevbuilder_theendpoints_clear(EOnvsetDEVbuilder* p) 
{
    uint16_t size = 0;
    uint8_t *nn = NULL;
    ep_holder_t epholder = {0};

 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    
    size = eo_vector_Size(p->theendpoints);
    
    if(0 == size)
    {
        return(eores_OK); 
    }
    
    uint16_t i =0;
    for(i=0; i<size; i++)
    {
        ep_holder_t *eph = (ep_holder_t*) eo_vector_Back(p->theendpoints);
        if(NULL == eph)
        {
            break;
        }
        eo_vector_Delete(eph->entities);
        eo_vector_PopBack(p->theendpoints);
    }
            
    return(eores_OK);
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------

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

/* @file       EoProtocol.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       06/05/2013
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"

#include "EoProtocolMC.h"
#include "EoProtocolMN.h"
#include "EoProtocolAS.h"
#include "EoProtocolSK.h"


#include "EoProtocolEPs.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocol.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

EO_VERIFYproposition(eoprot_check000, ((eoprot_entities_maxnumberofsupported) >= (eomn_entities_numberof)))
EO_VERIFYproposition(eoprot_check001, ((eoprot_entities_maxnumberofsupported) >= (eomc_entities_numberof)))
EO_VERIFYproposition(eoprot_check002, ((eoprot_entities_maxnumberofsupported) >= (eoas_entities_numberof)))
EO_VERIFYproposition(eoprot_check003, ((eoprot_entities_maxnumberofsupported) >= (eosk_entities_numberof)))

// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
    const uint8_t*      numberofeachentity[eoprot_endpoints_numberof];   
    void*               ramofeachendpoint[eoprot_endpoints_numberof];   
    eObool_fp_uint32_t  isvarproxied_fn[eoprot_endpoints_numberof];        
} eOprot_board_data_t;


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static uint16_t s_eoprot_endpoint_numberofvariables_get(eOprotBRD_t brd, eOprotEndpoint_t ep);
static uint16_t s_eoprot_brdentityindex2ramoffset(eOprotBRD_t brd, uint8_t epi, eOprotEntity_t entity, eOprotIndex_t index);
static uint16_t s_eoprot_brdid2ramoffset(eOprotBRD_t brd, uint8_t epi, eOprotID32_t id);
static eObool_t s_eoprot_entity_tag_is_valid(uint8_t epi, eOprotEntity_t entity, eOprotTag_t tag);

static uint16_t s_eoprot_rom_get_offset(uint8_t epi, eOprotEntity_t entity, eOprotTag_t tag);

static void* s_eoprot_rom_get_nvrom(eOprotID32_t id);
static uint16_t s_eoprot_rom_entity_offset_of_tag(uint8_t epi, uint8_t ent, eOprotTag_t tag);
static uint16_t s_eoprot_rom_get_sizeofvar(uint8_t epi, eOprotID32_t id);
static uint16_t s_eoprot_rom_get_prognum(eOprotID32_t id);

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
static eOresult_t s_eoprot_config_variable_callback(eOprotID32_t id, eOvoid_fp_cnvp_t init, eOvoid_fp_cnvp_cropdesp_t update);
#endif

static eOprot_board_data_t* s_eoprot_board_data_get(eOprotBRD_t brd);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static eOprotBRD_t s_eoprot_localboard = eo_prot_BRDdummy; // initted as 255. however, in runtime we assign a specific number to it.



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------


eOprot_board_data_t eoprot_loc_board_data = { NULL };

#if     defined(EOPROT_CFG_REMOTE_BOARDS_USE_DYNAMIC_MODE)
eOprot_board_data_t * eoprot_rem_board_data =  NULL ;
uint8_t eoprot_rem_board_data_size = 0;
#else
eOprot_board_data_t eoprot_rem_board_data[eoprot_board_remotes_maxnumberof] = { NULL };
uint8_t eoprot_rem_board_data_size = eoprot_board_remotes_maxnumberof;
#endif

const eOprot_EPcfg_t eoprot_arrayof_stdEPcfg[eoprot_endpoints_numberof] = 
{
    {
        EO_INIT(.endpoint)          eoprot_endpoint_management,
        EO_INIT(.numberofentities)  {1, 1, 1, 1, 0, 0, 0}
    },
  
    {
        EO_INIT(.endpoint)          eoprot_endpoint_motioncontrol,
        EO_INIT(.numberofentities)  {4, 4, 1, 0, 0, 0, 0}
    }, 
    {
        EO_INIT(.endpoint)          eoprot_endpoint_analogsensors,
        EO_INIT(.numberofentities)  {1, 1, 1, 1, 1, 1, 1}
    },
    {
        EO_INIT(.endpoint)          eoprot_endpoint_skin,
        EO_INIT(.numberofentities)  {2, 0, 0, 0, 0, 0, 0}
    }      
};

const eOprot_EPcfg_t eoprot_arrayof_maxEPcfg[eoprot_endpoints_numberof] =
{
    {
        EO_INIT(.endpoint)          eoprot_endpoint_management,
        EO_INIT(.numberofentities)  {1, 1, 1, 1, 0, 0, 0}
    },
  
    {
        EO_INIT(.endpoint)          eoprot_endpoint_motioncontrol,
        EO_INIT(.numberofentities)  {12, 12, 1, 0, 0, 0, 0}
    }, 
    {
        EO_INIT(.endpoint)          eoprot_endpoint_analogsensors,
        EO_INIT(.numberofentities)  {1, 1, 1, 1, 1, 1, 1}
    },
    {
        EO_INIT(.endpoint)          eoprot_endpoint_skin,
        EO_INIT(.numberofentities)  {2, 0, 0, 0, 0, 0, 0}
    }    
};  


const eOprot_EPcfg_t eoprot_arrayof_maxEPcfgOthers[eoprot_endpoints_numberof-1] =
{
    {
        EO_INIT(.endpoint)          eoprot_endpoint_motioncontrol,
        EO_INIT(.numberofentities)  {12, 12, 1, 0, 0, 0, 0}
    }, 
    {
        EO_INIT(.endpoint)          eoprot_endpoint_analogsensors,
        EO_INIT(.numberofentities)  {1, 1, 1, 1, 1, 1, 1}
    },
    {
        EO_INIT(.endpoint)          eoprot_endpoint_skin,
        EO_INIT(.numberofentities)  {2, 0, 0, 0, 0, 0, 0}
    }    
};  


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------



// current implementation depends on the fact taht we have the 32 bits of the id assigned to ep-entity-index-tag
// with 8-8-8-8 bits. thus we put a guard which fails compilation in case those values change.
EO_VERIFYproposition(eoprot_ep_1233, 256 == eoprot_endpoints_maxnumberof)
EO_VERIFYproposition(eoprot_ep_1234, 256 == eoprot_entities_maxnumberof)
EO_VERIFYproposition(eoprot_ep_1235, 256 == eoprot_indices_maxnumberof)
EO_VERIFYproposition(eoprot_ep_1236, 256 == eoprot_tags_maxnumberof)

extern eOprotID32_t eoprot_ID_get(eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index, eOprotTag_t tag)
{
    uint32_t id = (uint32_t)tag & 0xff;
    id |= (((uint32_t)index & 0xff)<<8);
    id |= (((uint32_t)entity & 0xff)<<16);
    id |= (((uint32_t)ep & 0xff)<<24);
    //uint32_t xx = EOPROT_ID_GET(ep, entity, index, tag);
    return((eOprotID32_t)id);    
}

extern eOprotEndpoint_t eoprot_ID2endpoint(eOprotID32_t id)
{
    uint32_t end = (((uint32_t)id>>24) & 0xff);
    return((eOprotEndpoint_t)end);    
}

extern eOprotEntity_t eoprot_ID2entity(eOprotID32_t id)
{
    uint32_t ent = (((uint32_t)id>>16) & 0xff);
    return((eOprotEntity_t)ent);    
}

extern eOprotIndex_t eoprot_ID2index(eOprotID32_t id)
{
    uint32_t index = (((uint32_t)id>>8) & 0xff);
    return((eOprotIndex_t)index);    
}


extern eOprotTag_t eoprot_ID2tag(eOprotID32_t id)
{
    uint32_t tag = ((uint32_t)id & 0xff);
    return((eOprotTag_t)tag);  
}


extern const char* eoprot_EP2string(eOprotEndpoint_t ep)
{
    if(ep < eoprot_endpoints_numberof)
    {
        return(eoprot_strings_endpoint[ep]);
    }
    else if(ep == eoprot_endpoint_all)
    {
        return(eoprot_strings_special[POSof_strings_special_endpoint_all]);
    }
    else if(ep == eoprot_endpoint_none)
    {
        return(eoprot_strings_special[POSof_strings_special_endpoint_none]);
    }
    else
    {
        return(eoprot_strings_special[POSof_strings_special_endpoint_unrecognised]);
    }   
}


extern const char* eoprot_EN2string(eOprotEndpoint_t ep, eOprotEntity_t en)
{
    if(ep < eoprot_endpoints_numberof)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        
        if(en < eoprot_ep_entities_numberof[epi])
        {
            return(eoprot_strings_entity[ep][en]);
        }
    } 
    
    return(eoprot_strings_special[POSof_strings_special_entity_unrecognised]);
}


extern const char* eoprot_TAG2string(eOprotEndpoint_t ep, eOprotEntity_t en, eOprotTag_t tag)
{
    if(ep < eoprot_endpoints_numberof)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);        
        if(en < eoprot_ep_entities_numberof[epi])
        {            
            if(tag < eoprot_ep_tags_numberof[epi][en])
            {
                return(eoprot_strings_tag[ep][en][tag]);
            }           
        }
    }
    
    return(eoprot_strings_special[POSof_strings_special_tag_unrecognised]);     
}


extern const char* eoprot_ID2stringOfEndpoint(eOprotID32_t id)
{
    return(eoprot_EP2string(eoprot_ID2endpoint(id)));   
}


extern const char* eoprot_ID2stringOfEntity(eOprotID32_t id)
{
    return(eoprot_EN2string(eoprot_ID2endpoint(id), eoprot_ID2entity(id)));
}

extern const char* eoprot_ID2stringOfTag(eOprotID32_t id)
{
    return(eoprot_TAG2string(eoprot_ID2endpoint(id), eoprot_ID2entity(id), eoprot_ID2tag(id)));
}


extern eOresult_t eoprot_ID2information(eOprotID32_t id, char* string, uint16_t size)
{
    if((NULL == string) || (0 == size))
    {
        return(eores_NOK_generic);
    }
    
    snprintf(string, size, "ID32 = 0x%08x -> IND = %d, TAG = %s", id, eoprot_ID2index(id), eoprot_ID2stringOfTag(id));
    
    return(eores_OK);    
}


extern eObool_t eoprot_EPcfg_isvalid(eOprot_EPcfg_t *cfgofep)
{

    uint8_t max = 0;
    uint16_t count = 0;
    uint8_t i = 0;
    
    if(NULL == cfgofep)
    {
        return(eobool_false);
    }
    
    if(0 == (max = eoprot_endpoint_get_numberofentities(cfgofep->endpoint)))
    {   // dont have entities in this endpoint (the endpoint is invalid)
        return(eobool_false);
    }
    
    if(max > eoprot_entities_maxnumberofsupported)
    {   // have too many entities in this endpoint, and my data structure dont manage them
        return(eobool_false);
    }
    
    for(i=0, count=0; i<max; i++)
    {
        count += cfgofep->numberofentities[i];
    }
    
    if(0 == count)
    {   // my config does not contain any entity .... it is a dummy configuration
        return(eobool_false);
    }
    
//    for(i=max, count=0; i<(eonvset_max_entity_value+1))
//    {   
//        count += cfgofep->numberofsentities[i];        
//    }
//    
//    if(count > 0)
//    {   // my configuration contains non-zero values beyond the limit of what the protocol manages for this endpoint
//        return(eobool_false);
//    }
    
    return(eobool_true);
}
    

extern const eoprot_version_t * eoprot_version_of_endpoint_get(eOprotEndpoint_t ep)
{
    uint8_t epi = 0;

    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }  

    epi = eoprot_ep_ep2index(ep);
    
    return(eoprot_endpoint_version[epi]);
}

extern eOresult_t eoprot_config_board_reserve(eOprotBRD_t brd)
{
    if(eoprot_board_localboard == brd)
    {
        return(eores_OK);
    }
    
#if     defined(EOPROT_CFG_REMOTE_BOARDS_USE_DYNAMIC_MODE)
    
    if(brd >= eoprot_board_remotes_maxnumberof)
    {
        return(eores_NOK_generic);
    }
    
    if(brd < eoprot_rem_board_data_size)
    {   // already allocated
        return(eores_OK);        
    }
    else
    {   
        // ok. just realloc   
        uint8_t oldsize = eoprot_rem_board_data_size;
        eOprot_board_data_t *p = (eOprot_board_data_t*)realloc(eoprot_rem_board_data, (brd+1)*sizeof(eOprot_board_data_t));  
        if(NULL == p)
        {
            return(eores_NOK_generic);
        }
        // if we can reallocate ...
        eoprot_rem_board_data_size = (brd+1);
        eoprot_rem_board_data = p;        
        memset(&eoprot_rem_board_data[oldsize], 0, (eoprot_rem_board_data_size-oldsize)*sizeof(eOprot_board_data_t));          
        return(eores_OK);
    }
    
#else

    if(brd >= eoprot_board_remotes_maxnumberof)
    {
        return(eores_NOK_generic);
    }
    return(eores_OK);
    
#endif       
}

extern eOresult_t eoprot_config_board_numberof(uint8_t numofboards)
{   
    return(eoprot_config_board_reserve(numofboards-1)); 
}

extern eObool_t eoprot_board_can_be_managed(eOprotBRD_t brd)
{
    if(eoprot_board_localboard == brd)
    {
        return(eobool_true);
    }    

    if(brd >= eoprot_rem_board_data_size)
    {
        return(eobool_false);
    }  
 
    return(eobool_true);    
}

extern eOresult_t eoprot_config_board_local(eOprotBRD_t brd)
{
    s_eoprot_localboard = brd;
    return(eores_OK);
}

extern eOresult_t eoprot_config_endpoint_entities(eOprotBRD_t brd, eOprotEndpoint_t ep, const uint8_t* numberofentities)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_OK;
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(eores_NOK_generic);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }
    
    epi = eoprot_ep_ep2index(ep);
            
    data->numberofeachentity[epi] = numberofentities;    
        
    return(res);
}

extern eOresult_t eoprot_config_proxied_variables(eOprotBRD_t brd, eOprotEndpoint_t ep, eObool_fp_uint32_t isvarproxied_fn)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_OK;
    uint8_t epi = 0;
    
    if(eoprot_board_localboard != brd)
    {
        return(eores_NOK_generic);
    }
    
    if(NULL == data)
    {
        return(eores_NOK_generic);
    }

    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    data->isvarproxied_fn[epi] = isvarproxied_fn;    
    
    return(res);        
}

extern eObool_t eoprot_endpoint_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t epi = 0;

    if(NULL == data)
    {
        return(eobool_false);
    }

    if(ep >= eoprot_endpoints_numberof)
    {
        return(eobool_false);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(eobool_false);
    }
    
    
    return(eobool_true);
}

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
extern eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_mn_fptr; 
extern eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_mc_fptr; 
extern eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_as_fptr; 
extern eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_sk_fptr; 
#endif

extern eOresult_t eoprot_config_callbacks_endpoint_set(const eOprot_callbacks_endpoint_descriptor_t* cbkdes)
{
#if     !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    return(eores_NOK_generic);
#else
    
    eOresult_t res = eores_OK;
    eOvoid_fp_uint32_voidp_t raminitialise = NULL;
    
    if(NULL == cbkdes)
    {
        return(eores_NOK_generic);
    }
    
    raminitialise = cbkdes->raminitialise;
    
    switch(cbkdes->endpoint)
    {
        case eoprot_endpoint_management:
            if(eoprot_fun_INITIALISE_mn != raminitialise) { eoprot_fun_INITIALISE_mn_fptr = raminitialise; } // to avoid infinite recursion
        break;
 
        case eoprot_endpoint_motioncontrol:
            if(eoprot_fun_INITIALISE_mc != raminitialise) { eoprot_fun_INITIALISE_mc_fptr = raminitialise; } // to avoid infinite recursion
        break;  

        case eoprot_endpoint_analogsensors:
            if(eoprot_fun_INITIALISE_as != raminitialise) { eoprot_fun_INITIALISE_as_fptr = raminitialise; } // to avoid infinite recursion
        break;

        case eoprot_endpoint_skin:
            if(eoprot_fun_INITIALISE_sk != raminitialise) { eoprot_fun_INITIALISE_sk_fptr = raminitialise; } // to avoid infinite recursion
        break;
        
        default:
            res = eores_NOK_generic;
        break;
    };
    
        
    return(res);   
    
#endif    
}

extern eOresult_t eoprot_config_onsay_endpoint_set(eOprotEndpoint_t ep, eOvoid_fp_cnvp_cropdesp_t onsay)
{
    uint8_t epindex = 0;
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }
    epindex = eoprot_ep_ep2index(ep);
    
    eoprot_ep_onsay[epindex] = onsay;

    return(eores_OK);      
}


extern eOvoid_fp_cnvp_cropdesp_t eoprot_onsay_endpoint_get(eOprotEndpoint_t ep)
{
    uint8_t epindex = 0;
    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }
    epindex = eoprot_ep_ep2index(ep);
    
    return(eoprot_ep_onsay[epindex]);    
}    
   

extern eOresult_t eoprot_config_callbacks_variable_set(const eOprot_callbacks_variable_descriptor_t *cbkdes)
{
#if     !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    return(eores_NOK_generic);
#else 
    eOprotID32_t id32 = 0;
    if(NULL == cbkdes)
    {
        return(eores_NOK_generic);
    }
    id32 = eoprot_ID_get(cbkdes->endpoint, cbkdes->entity, 0, cbkdes->tag);
    
    return(s_eoprot_config_variable_callback(id32, cbkdes->init, cbkdes->update));  
#endif    
}


extern eOprotBRD_t eoprot_board_local_get(void)
{
    return(s_eoprot_localboard);
}

extern uint8_t eoprot_endpoints_numberof_get(eOprotBRD_t brd)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t numberof = 0;
    uint8_t ep = 0;
    
    if(NULL == data)
    {
        return(0);
    }

    for(ep=0; ep<eoprot_endpoints_numberof; ep++)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        if(NULL != data->numberofeachentity[epi])
        {
            numberof++;
        }
    }
       
    return(numberof);
}


extern eOresult_t eoprot_endpoints_array_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
{  
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);    
    uint8_t ep = 0;
    uint8_t numberof = 0;

    if(NULL == data)
    {
        return(eores_NOK_generic);
    }
    
    if((NULL == array) || (sizeof(eOprotEndpoint_t) != eo_array_ItemSize(array)))
    {   // mild control: the array must be non NULL and with itemsize equal to 1
        return(eores_NOK_generic);
    }
    
    // we reset the array 
    eo_array_Reset(array);


    numberof = 0;
    //uint8_t capacity = eo_array_Capacity(array);
    for(ep=0; ep<eoprot_endpoints_numberof; ep++)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        if(NULL != data->numberofeachentity[epi])
        {
            numberof++;
            if(numberof>startfrom)
            {   // we want to retrieve the items starting from a given number
                eOresult_t res = eo_array_PushBack(array, &ep);
                if(eores_OK != res)
                {   // if array is valid, we have NOK when the array is full
                    break;
                }                    
            }
        }
    }
    
    return(eores_OK);   
}


extern eOresult_t eoprot_endpoints_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
{ 
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_NOK_generic;
    uint8_t ep = 0;
    uint8_t numberof = 0;

    if(NULL == data)
    {
        return(eores_NOK_generic);
    }  
    
    if((NULL == array) || (sizeof(eoprot_endpoint_descriptor_t) != eo_array_ItemSize(array)))
    {   // mild control: the array must be non NULL and with itemsize equal to 4
        return(eores_NOK_generic);
    }
    
    // we reset the array 
    eo_array_Reset(array);


    numberof = 0;
    //uint8_t capacity = eo_array_Capacity(array);
    for(ep=0; ep<eoprot_endpoints_numberof; ep++)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        if(NULL != data->numberofeachentity[epi])
        {
            numberof++;
            if(numberof>startfrom)
            {   // we want to retrieve the items starting from a given number
                uint8_t ent;
                uint16_t entitiesinside = 0;
                eoprot_endpoint_descriptor_t epdes = {0};
                epdes.endpoint          = ep;
                epdes.entitiesinside    = 255; // eoprot_ep_entities_numberof[epi];
                epdes.version.major     = eoprot_endpoint_version[epi]->major;
                epdes.version.minor     = eoprot_endpoint_version[epi]->minor;
                for(ent=0; ent<eoprot_ep_entities_numberof[epi]; ent++)
                {
                    if(0 != data->numberofeachentity[epi][ent])
                    {
                        entitiesinside++;
                    }     
                }
                epdes.entitiesinside    = (uint8_t) entitiesinside; // ok, epdes.entitiesinside does not contains many item                
                
                
                res = eo_array_PushBack(array, &epdes);
                if(eores_OK != res)
                {   // if array is valid, we have NOK when the array is full
                    break;
                }                    
            }
        }
    }
    
    return(eores_OK);   
}


extern uint16_t eoprot_entities_in_endpoint_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t numberof = 0;
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(0);
    }
     
    if(eoprot_endpoint_all == ep)
    {
        return(eoprot_entities_numberof_get(brd));
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }

    epi = eoprot_ep_ep2index(ep);
    if(NULL != data->numberofeachentity[epi])
    {
        uint8_t ent;
        for(ent=0; ent<eoprot_ep_entities_numberof[epi]; ent++)
        {
            if(0 != data->numberofeachentity[epi][ent])
            {
                numberof++;
            }
        }            
        
    }
      
    return(numberof);       
}


extern uint16_t eoprot_entities_numberof_get(eOprotBRD_t brd)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t numberof = 0;
    uint8_t ep = 0;
    
    if(NULL == data)
    {
        return(0);
    }
    
    for(ep=0; ep<eoprot_endpoints_numberof; ep++)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        if(NULL != data->numberofeachentity[epi])
        {
            uint8_t ent;
            for(ent=0; ent<eoprot_ep_entities_numberof[epi]; ent++)
            {
                if(0 != data->numberofeachentity[epi][ent])
                {
                    numberof++;
                }
            }            
            
        }
    }
       
    return(numberof);       
}


extern eOresult_t eoprot_entities_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_NOK_generic;
    uint16_t numberof = 0;
    uint8_t ep = 0;
    
    if(NULL == data)
    {
        return(eores_NOK_generic);
    }
    
    if((NULL == array) || (sizeof(eoprot_entity_descriptor_t) != eo_array_ItemSize(array)))
    {   // mild control: the array must be non NULL and with itemsize equal to sizeof(eoprot_entity_descriptor_t)
        return(eores_NOK_generic);
    }
    
    // we reset the array 
    eo_array_Reset(array);
    
    for(ep=0; ep<eoprot_endpoints_numberof; ep++)
    {
        uint8_t epi = eoprot_ep_ep2index(ep);
        if(NULL != data->numberofeachentity[epi])
        {
            uint8_t ent;
            for(ent=0; ent<eoprot_ep_entities_numberof[epi]; ent++)
            {
                if(0 != data->numberofeachentity[epi][ent])
                {
                    numberof++;
                    
                    if(numberof>startfrom)
                    {   // we want to retrieve the items starting from a given number
                        eoprot_entity_descriptor_t entdes = {0};
                        
                        entdes.endpoint         = ep;
                        entdes.entity           = ent;
                        entdes.multiplicity     = data->numberofeachentity[epi][ent];
                        entdes.numberoftags     = eoprot_ep_tags_numberof[epi][ent];
                        
                        res = eo_array_PushBack(array, &entdes);
                        if(eores_OK != res)
                        {   // if array is valid, we have NOK when the array is full
                            break;
                        }                    
                    }                    
                    
                }
            }            
            
        }
    }
       
    return(eores_OK);       
}

extern eOresult_t eoprot_entities_in_endpoint_arrayofdescriptors_get(eOprotBRD_t brd, eOprotEndpoint_t ep, EOarray* array, uint8_t startfrom)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_NOK_generic;
    uint16_t numberof = 0;
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(eores_NOK_generic);
    }
    
    if((NULL == array) || (sizeof(eoprot_entity_descriptor_t) != eo_array_ItemSize(array)))
    {   // mild control: the array must be non NULL and with itemsize equal to sizeof(eoprot_entity_descriptor_t)
        return(eores_NOK_generic);
    }
    
    if(eoprot_endpoint_all == ep)
    {
        return(eoprot_entities_arrayofdescriptors_get(brd, array, startfrom));
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }    
    
    // we reset the array 
    eo_array_Reset(array);
    
    epi = eoprot_ep_ep2index(ep);
    if(NULL != data->numberofeachentity[epi])
    {
        uint8_t ent;
        for(ent=0; ent<eoprot_ep_entities_numberof[epi]; ent++)
        {
            if(0 != data->numberofeachentity[epi][ent])
            {
                numberof++;
                
                if(numberof>startfrom)
                {   // we want to retrieve the items starting from a given number
                    eoprot_entity_descriptor_t entdes = {0};
                    
                    entdes.endpoint         = ep;
                    entdes.entity           = ent;
                    entdes.multiplicity     = data->numberofeachentity[epi][ent];
                    entdes.numberoftags     = eoprot_ep_tags_numberof[epi][ent];
                    
                    res = eo_array_PushBack(array, &entdes);
                    if(eores_OK != res)
                    {   // if array is valid, we have NOK when the array is full
                        break;
                    }                    
                }                    
                
            }
        }            
        
    }
       
    return(eores_OK);       
}

extern eOvoid_fp_uint32_voidp_t eoprot_endpoint_get_initialiser(eOprotEndpoint_t ep)
{
    eOvoid_fp_uint32_voidp_t fn = NULL;
    
    switch(ep)
    {
        case eoprot_endpoint_management:            fn = eoprot_fun_INITIALISE_mn;      break;
        case eoprot_endpoint_motioncontrol:         fn = eoprot_fun_INITIALISE_mc;      break;
        case eoprot_endpoint_analogsensors:         fn = eoprot_fun_INITIALISE_as;      break;
        case eoprot_endpoint_skin:                  fn = eoprot_fun_INITIALISE_sk;      break;
        
        default:                                    fn = NULL;                          break;
    }
    
    return(fn);
}

extern uint8_t eoprot_endpoint_get_numberofentities(eOprotEndpoint_t ep)
{
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    return(eoprot_ep_entities_numberof[eoprot_ep_ep2index(ep)]);   
}

extern uint16_t eoprot_endpoint_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t size = 0;
    uint8_t epi = 0;
    uint8_t i;
    
    if(NULL == data)
    {
        return(0);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(0);
    }
    
    
    for(i=0; i<eoprot_ep_entities_numberof[epi]; i++)
    {
        size += eoprot_ep_entities_sizeof[epi][i] * data->numberofeachentity[epi][i];
    }
    
    return(size);
}


extern eOresult_t eoprot_config_endpoint_ram(eOprotBRD_t brd, eOprotEndpoint_t ep, void* ram, uint16_t sizeofram)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOresult_t res = eores_OK;
    uint8_t epi = 0;
         
    if(NULL == data) 
    {
        return(eores_NOK_generic);
    }
    
//    if((0 == sizeofram) || (NULL == ram))
//    {
//        return(eores_NOK_generic);
//    }

    if(ep >= eoprot_endpoints_numberof)
    {
        return(eores_NOK_generic);
    }
    
    epi = eoprot_ep_ep2index(ep);    
        
    data->ramofeachendpoint[epi] = ram;    
        
    return(res);
}



extern void* eoprot_variable_ramof_get(eOprotBRD_t brd, eOprotID32_t id)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOprotEndpoint_t ep = eoprot_ID2endpoint(id);
    uint8_t epi = 0;
    uint8_t* startofdata = NULL;
    uint16_t offset = 0;
    
    if(NULL == data) 
    {
        return(NULL);
    }     

    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }
    
    epi = eoprot_ep_ep2index(ep); 
    
    
    startofdata = (uint8_t*)data->ramofeachendpoint[epi];
    
    if(NULL == startofdata)
    {
        return(NULL);
    }
    
    offset = s_eoprot_brdid2ramoffset(brd, epi, id);
    
    if(EOK_uint16dummy == offset)
    {
        return(NULL);
    }   

    return(&startofdata[offset]);     
}

extern uint16_t eoprot_variable_sizeof_get(eOprotBRD_t brd, eOprotID32_t id)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t size = 0;
    uint8_t epi = 0;
    
    eOprotEndpoint_t ep = eoprot_ID2endpoint(id);
      
    if(NULL == data)
    {   // because we dont have such a board. however, the size of a variable is independent by the board
        return(0);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    size = s_eoprot_rom_get_sizeofvar(epi, id);
        
    return(size);
}

extern eObool_t eoprot_variable_is_proxied(eOprotBRD_t brd, eOprotID32_t id)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eObool_t res = eobool_false;
    eObool_fp_uint32_t fptr = NULL;
    eOprotEndpoint_t ep = eoprot_ID2endpoint(id);
    uint8_t epi = 0;
    
//    if(eoprot_board_localboard == brd)
//    {
//        return(eobool_false);
//    }

    if(NULL == data)
    {
        return(eobool_false);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eobool_false);
    }
    
    epi = eoprot_ep_ep2index(ep);
    fptr = data->isvarproxied_fn[epi];
    
    if(NULL != fptr)
    {
        res = fptr(id);
    }
    
    return(res);
}

extern void* eoprot_variable_romof_get(eOprotBRD_t brd, eOprotID32_t id)
{
    // we dont verify brd validity because the eonvrom is common to every board
    return(s_eoprot_rom_get_nvrom(id));
}

extern eObool_t eoprot_entity_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t epi = 0;
      
    if(NULL == data)
    {
        return(eobool_false);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(eobool_false);
    }  

    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(eobool_false);
    }
   
    if(entity < eoprot_ep_entities_numberof[epi])
    {   // is supported if we have a valid entity number
        return(eobool_true);
    }

    return(eobool_false);    
}

extern void* eoprot_entity_ramof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t epi = 0;
    uint16_t offset = 0;
    uint8_t* startofdata = NULL;
          
    if(NULL == data)
    {
        return(NULL);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }
    
    epi = eoprot_ep_ep2index(ep);

    startofdata = (uint8_t*)data->ramofeachendpoint[epi];
    if(NULL == startofdata)
    {
        return(NULL);
    }
    
    offset = s_eoprot_brdentityindex2ramoffset(brd, epi, entity, index);
    
    if(EOK_uint16dummy == offset)
    {
        return(NULL);
    }   

    return(&startofdata[offset]); 
}


extern uint16_t eoprot_entity_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t size = 0;
    uint8_t epi = 0;
    
    if(NULL == data)
    {   // because we dont have such a board. however, the size of an entity is independent by the board
        return(0);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(entity < eoprot_ep_entities_numberof[epi])
    {
        size = eoprot_ep_entities_sizeof[epi][entity];
    }
    
    return(size);
}


extern uint8_t eoprot_entity_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t numberof = 0;
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(0);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }  

    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(0);
    }

    if(entity < eoprot_ep_entities_numberof[epi])
    {
        numberof = data->numberofeachentity[ep][entity];
    }
    
    return(numberof);
}


extern eObool_t eoprot_id_isvalid(eOprotBRD_t brd, eOprotID32_t id)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOprotEntity_t ent = eoprot_ID2entity(id);
    eOprotIndex_t  ind = eoprot_ID2index(id);
    eOprotTag_t    tag = eoprot_ID2tag(id);
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(eobool_false);
    }    

    // just verifies that the entity, index, and tag have numbers which are consistent with their maximum number
    
    if(id == eo_prot_ID32dummy)
    {
        return(eobool_false);
    }      
    
    epi = eoprot_ep_ep2index(eoprot_ID2endpoint(id));

    
    if(ent >= eoprot_ep_entities_numberof[epi])
    {
        return(eobool_false);
    }
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(eobool_false);
    }      
    
    if(ind >= data->numberofeachentity[epi][ent])
    {
        return(eobool_false);
    }   
    
    return(s_eoprot_entity_tag_is_valid(epi, ent, tag));
}

extern uint16_t eoprot_endpoint_numberofvariables_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    return(s_eoprot_endpoint_numberofvariables_get(brd, ep));
}

extern eOprotID32_t eoprot_endpoint_prognum2id(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotProgNumber_t prog)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOprotTag_t tag = 0xff;
    eOprotIndex_t index = 0xff;
    eOprotEntity_t entity = 0xff;
    uint8_t epi = 0;
    uint8_t i;
     
    if(NULL == data)
    {
        return(EOK_uint32dummy);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(EOK_uint32dummy);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(EOK_uint32dummy);
    }
    
       
    for(i=0; i<eoprot_ep_entities_numberof[epi]; i++)
    {
        // starting from the first entity (if it present in the board) we progressively check if the signedprog is in its relevant range.
        uint8_t tags_number_ith = eoprot_ep_tags_numberof[epi][i];
        eOprotProgNumber_t progs_ith = tags_number_ith * data->numberofeachentity[epi][i]; // num of progs in all the entities i-th
        if((0 != progs_ith) && (prog < (progs_ith)))
        {   // entity is the i-th 
            entity  = i;
            index   = prog / tags_number_ith;       // tags_number_ith cannot be zero if progs_ith is non-zero
            tag     = prog % tags_number_ith;  

            return(eoprot_ID_get(ep, entity, index, tag));
        }
        prog -= progs_ith; // entity may be the next one. 
    }

    return(EOK_uint32dummy);   
}


extern eOprotProgNumber_t eoprot_endpoint_id2prognum(eOprotBRD_t brd, eOprotID32_t id)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOprotProgNumber_t prog = 0;
    uint8_t epi = 0;
    eOprotEntity_t entity = eoprot_ID2entity(id);
    eOprotIndex_t  index  = eoprot_ID2index(id);
    uint8_t i;
    eOprotEndpoint_t ep;

    if(NULL == data)
    {
        return(EOK_uint32dummy);
    }    
    
    ep = eoprot_ID2endpoint(id);
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(EOK_uint32dummy);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(EOK_uint32dummy);
    }
    
    
    if(entity >= eoprot_ep_entities_numberof[epi])
    {
        return(EOK_uint32dummy);
    }
    
    for(i=0; i<entity; i++)
    {   // we add all the tags in the entities below
        prog += (eoprot_ep_tags_numberof[epi][i] * data->numberofeachentity[epi][i]);
    }
    // then we add only the tags of the entities equal to the current one + the progressive number of the tag
    prog += (index*eoprot_ep_tags_numberof[epi][entity] + s_eoprot_rom_get_prognum(id));

    return(prog);
}    

extern void* eoprot_endpoint_ramof_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint8_t epi = 0;
    
    if(NULL == data)
    {
        return(NULL);
    }
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    return(data->ramofeachendpoint[epi]); 
}

// extern eOprotID32_t eoprot_prognum2id(eOprotBRD_t brd, eOprotProgNumber_t prog)
// {
//     return(EOK_uint32dummy);   
// }


// extern eOprotProgNumber_t eoprot_id2prognum(eOprotBRD_t brd, eOprotID32_t id)
// {
//     return(EOK_uint32dummy);
// }    

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static uint16_t s_eoprot_endpoint_numberofvariables_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t num = 0;
    uint8_t epi = 0;
    uint8_t i = 0;

    if(NULL == data)
    {
        return(0);
    }    
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(0);
    }
    
    epi = eoprot_ep_ep2index(ep);
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(0);
    }
    
    for(i=0; i<eoprot_ep_entities_numberof[epi]; i++)
    {   
        // simply the sum for each entity of the number of tags multiplied the number of each entity. 
        // it also works if an entity is not present in the board
        num += (eoprot_ep_tags_numberof[epi][i] * data->numberofeachentity[epi][i]);
    }

    return(num);
}

static uint16_t s_eoprot_brdentityindex2ramoffset(eOprotBRD_t brd, uint8_t epi, eOprotEntity_t entity, eOprotIndex_t index)
{
    eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    uint16_t offset = 0;
    uint8_t i = 0;
    
    if(NULL == data)
    {
        return(EOK_uint16dummy);
    }
    
    if(entity >= eoprot_ep_entities_numberof[epi])
    {
        return(EOK_uint16dummy);
    }
    
    if(NULL == data->numberofeachentity[epi])
    {
        return(EOK_uint16dummy);
    }
    
    if(index >= data->numberofeachentity[epi][entity])
    {
        return(EOK_uint16dummy);
    }
        
    for(i=0; i<entity; i++)
    {   // we sum the size of all the entities before the current one
        offset += (data->numberofeachentity[epi][i] * eoprot_ep_entities_sizeof[epi][i]);
    }
    // then we add the offset of the current entity
    offset += (index*eoprot_ep_entities_sizeof[epi][entity]);

    return(offset);
}    


static uint16_t s_eoprot_brdid2ramoffset(eOprotBRD_t brd, uint8_t epi, eOprotID32_t id)
{
    //eOprot_board_data_t *data = s_eoprot_board_data_get(brd);
    eOprotEntity_t entity = eoprot_ID2entity(id);
    eOprotIndex_t index = eoprot_ID2index(id);
    eOprotTag_t tag = eoprot_ID2tag(id);
    
    // we compute the offset of the entity. the validity of the brd is verified inside the following function
    uint16_t offset = s_eoprot_brdentityindex2ramoffset(brd, epi, entity, index);
       
    if(EOK_uint16dummy == offset)
    {
        return(EOK_uint16dummy);
    }
    
    // then we add the offset of the tag
    //offset += (eoprot_ep_romif[epi]->get_offset(entity, tag)); 
    offset += s_eoprot_rom_get_offset(epi, entity, tag); 

    return(offset);
} 


static eObool_t s_eoprot_entity_tag_is_valid(uint8_t epi, eOprotEntity_t entity, eOprotTag_t tag)
{
    return((tag < eoprot_ep_tags_numberof[epi][entity])?(eobool_true):(eobool_false));
}


// returns the offset of the variable with a given tag from the start of the entity
static uint16_t s_eoprot_rom_entity_offset_of_tag(uint8_t epi, uint8_t ent, eOprotTag_t tag)
{
    uint32_t i, tag_aux = 0;
    uint8_t *one = NULL;
    uint8_t *two = NULL;
    int res = 0;

    tag_aux = 0;
    for(i=0; i<ent; i++)
    {
        tag_aux += eoprot_ep_tags_numberof[epi][i];
    }
    // one contains the address of the default value of the entire entity (eg: &MYdefentity = 0x08001200).
    one = (uint8_t*) eoprot_ep_entities_defval[epi][ent];
    // two contains the address of the default value of the variable, but inside the default value of the entire entity (eg: &MYdefentity.var = 0x08001220)  
    two = (uint8_t*) eoprot_ep_descriptors[epi][ent][tag]->resetval;
    // if we want to know how many bytes they are fare aways we simply cast them to pointers of 1 byte and make the difference.
    // the C standard assures that the result is an integer containing the distance between the pointers expressed in object pointed. in our case 1 byte.
    res = two - one;
    // and now we cast result to 16 bits because we dont have big entities.
    return((uint16_t)res); 
}

static uint16_t s_eoprot_rom_get_offset(uint8_t epi, eOprotEntity_t entity, eOprotTag_t tag)
{
    if(entity >= eoprot_ep_entities_numberof[epi])
    {
        return(EOK_uint16dummy);
    }
    
    return(s_eoprot_rom_entity_offset_of_tag(epi, entity, tag));
}


static void* s_eoprot_rom_get_nvrom(eOprotID32_t id)
{
    uint8_t epindex = 0;
    eOprotEndpoint_t ep = eoprot_ID2endpoint(id);
    eOprotEntity_t entity = eoprot_ID2entity(id);
    eOprotTag_t tag = eoprot_ID2tag(id);
    
    if(ep >= eoprot_endpoints_numberof)
    {
        return(NULL);
    }
    
    epindex = eoprot_ep_ep2index(ep);    
    
    if(entity >= eoprot_ep_entities_numberof[epindex])
    {
        return(NULL);
    }  

    if(tag >= eoprot_ep_tags_numberof[epindex][entity])
    {
        return(NULL);
    }        
    
    return((void*)eoprot_ep_descriptors[epindex][entity][tag]);  
}


static uint16_t s_eoprot_rom_get_sizeofvar(uint8_t epi, eOprotID32_t id)
{     
    EOnv_rom_t* rom = (EOnv_rom_t*) s_eoprot_rom_get_nvrom(id);  
    if(NULL == rom)
    {
        return(0);
    }    
    return(rom->capacity); 
}


static uint16_t s_eoprot_rom_get_prognum(eOprotID32_t id)
{   // we assume that the variables are inserted in a progressive way without holes. and even if there are a few holes never mind.
    return(eoprot_ID2tag(id));
}


#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
static eOresult_t s_eoprot_config_variable_callback(eOprotID32_t id, eOvoid_fp_cnvp_t init, eOvoid_fp_cnvp_cropdesp_t update)
{
#if     !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    return(eores_NOK_generic);
#else
    EOnv_rom_t* nvrom = (EOnv_rom_t*)eoprot_variable_romof_get(0, id); // eoprot_variable_romof_get() does not use brd param
    
    if(NULL == nvrom)
    {
        return(eores_NOK_generic);
    }
    
    if(NULL != init)
    {
        nvrom->init = init;
    }
    
    if(NULL != update)
    {
        nvrom->update = update;
    }    
    
    return(eores_OK);
    
#endif    
}
#endif



static eOprot_board_data_t* s_eoprot_board_data_get(eOprotBRD_t brd)
{
    if(eoprot_board_localboard == brd)
    {
        return(&eoprot_loc_board_data);
    } 
    else if((brd >= eoprot_rem_board_data_size))
    {
        return(NULL);
    } 
    else
    {
        return(&eoprot_rem_board_data[brd]);        
    }    
}

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




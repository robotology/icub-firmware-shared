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

#include "EOvector.h" 


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOnvsetBRDbuilder.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOnvsetBRDbuilder_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOnvsetBRDbuilder";




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOnvsetBRDbuilder* eo_nvsetbrdbuilder_New(eOnvBRD_t board)
{
    EOnvsetBRDbuilder *p = NULL;  
    uint8_t i = 0;


    // i get the memory for the object
    p = eo_mempool_New(eo_mempool_GetHandle(), 1*sizeof(EOnvsetBRDbuilder));
    
    p->epcfg_vector = eo_vector_New(sizeof(eOprot_EPcfg_t), eo_vectorcapacity_dynamic, NULL, 0, NULL, NULL);
    
    p->brdcfg = eo_mempool_New(eo_mempool_GetHandle(), 1*sizeof(eOnvset_BRDcfg_t));
    
    p->brdcfg->boardnum = board;
    p->brdcfg->epcfg_constvect = eo_constvector_New(0, 0, NULL);
    
    
    return(p);
}


extern void eo_nvsetbrdbuilder_Delete(EOnvsetBRDbuilder* p)
{
    if(NULL == p)
    {
        return;
    } 
    
    if(NULL == p->epcfg_vector)
    {
        return;
    }
    
       
    eo_constvector_Delete(p->brdcfg->epcfg_constvect); 
    p->brdcfg->epcfg_constvect = NULL;
    
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p->brdcfg);
    memset(p->brdcfg, 0, sizeof(eOnvset_BRDcfg_t));
    
    eo_vector_Delete(p->epcfg_vector);
    p->epcfg_vector = NULL;
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
 
    return;       
}


extern eOresult_t eo_nvsetbrdbuilder_EP_Load(EOnvsetBRDbuilder* p, eOprot_EPcfg_t* epcfg) 
{
    EOvector *v = NULL;
 	if((NULL == p) || (NULL == epcfg)) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    if(eobool_false == eoprot_EPcfg_isvalid(epcfg))
    {
        return(eores_NOK_generic); 
    }
    
    // ok. now i put the item inside p->brdcfg->epcfg_constvect
    v = p->epcfg_vector;
    eo_vector_PushBack(v, epcfg); 
    // and make sure that the constvector is udapted
    eo_constvector_Load(p->brdcfg->epcfg_constvect, sizeof(eOprot_EPcfg_t), eo_vector_Size(v), eo_vector_storage_Get(v)); 
        
    return(eores_OK);
}


extern eOresult_t eo_nvsetbrdbuilder_EPs_Clear(EOnvsetBRDbuilder* p)
{
    EOvector *v = NULL;
 	if(NULL == p) 
	{
		return(eores_NOK_nullpointer); 
	}
    
    // ok. now i put the item inside p->brdcfg->epcfg_constvect
    v = p->epcfg_vector;
    eo_vector_Clear(v); 
    // and make sure that teh constvector is updated
    eo_constvector_Load(p->brdcfg->epcfg_constvect, 0, 0, NULL);
        
    return(eores_OK);        
}


extern eOnvset_BRDcfg_t* eo_nvsetbrdbuilder_BRDcfg_Get(EOnvsetBRDbuilder* p)
{
    eOresult_t res = eores_NOK_generic;
 	if(NULL == p) 
	{
		return(NULL); 
	}
    
    return(p->brdcfg);   
}




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
//  empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------

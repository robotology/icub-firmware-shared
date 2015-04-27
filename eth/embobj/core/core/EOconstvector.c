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
#include "EOtheErrorManager.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOconstvector.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOconstvector_hid.h" 


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
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOconstvector";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOconstvector * eo_constvector_New(eOsizeitem_t item_size, eOsizecntnr_t size, const void *data)
{
    EOconstvector *retptr = NULL;
    
    if(NULL == data)
    {
        item_size = 0;
        size = 0;
    }
    else
    {
        if((0 == size) || (0 == item_size))
        {
            return(NULL);
        }        
    }
 
    // i get the memory for the object. no need to check versus NULL because the memory pool already does it
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOconstvector), 1);

    retptr->item_size           = item_size;
    retptr->size                = size;
    retptr->item_array_data     = data;     
    
    return(retptr);   
}

extern EOconstvector * eo_constvector_Load(EOconstvector *p, eOsizeitem_t item_size, eOsizecntnr_t size, const void *data)
{
    if(NULL == p)
    {
        return(NULL);
    }
    
    if(NULL == data)
    {
        item_size = 0;
        size = 0;
    }
    else
    {
        if((0 == size) || (0 == item_size))
        {
            return(NULL);
        }        
    }
    
    p->item_size            = item_size;
    p->size                 = size;
    p->item_array_data      = data;   
    
    return(p);
}


extern void eo_constvector_Delete(EOconstvector *p)
{
    if(NULL == p)
    {
        return;
    }
    if(NULL == p->item_array_data)
    {
        return;
    }
    
    memset(p, 0, sizeof(EOconstvector));   
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;
}

extern eOsizecntnr_t eo_constvector_Size(const EOconstvector *p) 
{
    if(NULL == p) 
    {   // invalid p
        return(0);    
    }
    
    return(p->size);        
}


extern const void * eo_constvector_At(const EOconstvector *p, eOsizecntnr_t pos) 
{
    // here we require uint8_t to access item_array_data because we work with bytes.
    uint8_t *start = NULL;
    
    if(NULL == p) 
    {
        return(NULL);    
    }
    
    if(pos >= p->size) 
    {    // p does not have any element in pos
        return(NULL);     
    }
    
   
    start = (uint8_t*) (p->item_array_data);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    start = &start[(uint32_t)pos * p->item_size];
    
    return((const void*) start);         
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section




// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




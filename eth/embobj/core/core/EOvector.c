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

#include "EOvector.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOvector_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define EOVECTOR_DEFAULTCLEAR_DOES_NOTHING


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


EO_static_inline void s_eo_vector_default_clear(void *item, EOvector* vector)
{
#if defined(EOVECTOR_DEFAULTCLEAR_DOES_NOTHING)
#else
    memset(item, 0, vector->item_size);
#endif
}

EO_static_inline void s_eo_vector_default_copy(void* item, void* p, EOvector* vector)
{
    memcpy(item, p, vector->item_size);
}

EO_static_inline void s_eo_vector_default_init(void* item,  EOvector* vector)
{
    memset(item, 0, vector->item_size);
}

//EO_static_inline void s_eo_vector_default_initall(EOvector* vector)
//{
//    memset(vector->stored_items, 0, vector->capacity*vector->item_size);
//}

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOvector";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOvector * eo_vector_New(eOsizeitem_t item_size, eOsizecntnr_t capacity,
                              eOres_fp_voidp_uint32_t item_init, uint32_t init_par,  
                              eOres_fp_voidp_voidp_t item_copy, eOres_fp_voidp_t item_clear)
{
    EOvector *retptr = NULL;
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    eOsizecntnr_t i = 0; 


    // i get the memory for the object. no need to check versus NULL because the memory pool already does it
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOvector), 1);


    // now the object has valid memory. i need to initialise it with user-defined data,
    
    retptr->size                = 0;

    eo_errman_Assert(eo_errman_GetHandle(), (0 != item_size), s_eobj_ownname, "item_size is zero");
    eo_errman_Assert(eo_errman_GetHandle(), (0 != capacity), s_eobj_ownname, "capacity is zero");

    retptr->item_size           = item_size;
    retptr->sizeofstoreditem    = item_size;
    retptr->capacity            = capacity;
    retptr->item_init_fn        = item_init;
    retptr->item_init_par       = init_par;
    retptr->item_copy_fn        = item_copy;
    retptr->item_clear_fn       = item_clear;
    
    
    if(eo_vectorcapacity_dynamic == retptr->capacity)
    {
        eo_errman_Assert(eo_errman_GetHandle(), (eo_mempool_alloc_dynamic == eo_mempool_alloc_mode_Get(eo_mempool_GetHandle())), s_eobj_ownname, "can use eo_vectorcapacity_dynamic only w/ eo_mempool_alloc_dynamic");
        retptr->stored_items = NULL;
    }
    else
    {

        eOmempool_alignment_t align = eo_mempool_align_08bit;
        
        // now we get memory for copying objects inside
        if(1 == item_size)
        {
            align = eo_mempool_align_08bit;
        }
        else if(2 == item_size)
        {
            align = eo_mempool_align_16bit;
        }
        else if (item_size <= 4)
        {
            align = eo_mempool_align_32bit;
            retptr->sizeofstoreditem = 4;
        }
        else
        {   // use 8-bytes alignment for everything else
            align = eo_mempool_align_64bit;
            retptr->sizeofstoreditem = (item_size+7)/8;
            retptr->sizeofstoreditem *= 8;
        }

        // here is the memory from the correct memory pool (or the heap)
        retptr->stored_items  = eo_mempool_GetMemory(eo_mempool_GetHandle(), align, item_size, capacity);     
              

        start = (uint8_t*) (retptr->stored_items);
        for(i=0; i<capacity; i++) 
        {
            // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
            item = &start[(uint32_t)i * item_size];
            if(NULL != item_init)
            {
                // construct each item 
                item_init(item, init_par);
            }
            else
            {   // default init
                s_eo_vector_default_init(item, retptr);
            }
        }

    }

    return(retptr);   
}


extern eOsizecntnr_t eo_vector_Capacity(EOvector * vector) 
{
    if(NULL == vector) 
    {
        return(0);    
    }
    
    return(vector->capacity);    
}


extern void eo_vector_PushBack(EOvector * vector, void *p) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
        
    if((NULL == vector) || (NULL == p)) 
    {   // invalid data
        return;    
    }
    
    if(vector->capacity == vector->size) 
    {   // vector is full
        return;
    }
    
    if(eo_vectorcapacity_dynamic == vector->capacity)
    {   // in here i dont make any control because in _New() we have already verified that mempool is dynamic 
        vector->stored_items = eo_mempool_Realloc(eo_mempool_GetHandle(), vector->stored_items, (uint32_t)(vector->size+1) * vector->item_size);
    }
            
    start = (uint8_t*) (vector->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)vector->size * vector->item_size]; 
    
    if(NULL != vector->item_copy_fn) 
    {
        vector->item_copy_fn(item, p);
    }
    else
    {
        s_eo_vector_default_copy(item, p, vector);
    }
    
    vector->size ++;
    
    return; 
}


extern void * eo_vector_Back(EOvector * vector) 
{    
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == vector) 
    {   // invalid vector. return NULL
        return(NULL);    
    }
    
    if(0 == vector->size) 
    {   // vector is empty. return NULL
        return(start);     
    }
     
    start = (uint8_t*) (vector->stored_items);
    item = &start[(uint32_t)(vector->size-1) * vector->item_size];
    
    return((void*) item);         
}


extern void eo_vector_PopBack(EOvector * vector) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == vector) 
    {   // invalid vector
        return;    
    }
    
    if(0 == vector->size) 
    {   // vector is empty
        return;     
    }

    start = (uint8_t*) (vector->stored_items);
    item = &start[(uint32_t)(vector->size - 1) * vector->item_size];            
    
    if(NULL != vector->item_clear_fn) 
    {
        vector->item_clear_fn(item);
    } 
    else 
    { 
        // clean the removed item
        s_eo_vector_default_clear(item, vector);
    }
    
    vector->size --;
    
    //if size is zero, eo_mempool_Realloc() calls eo_mempool_Free() and returns NULL. that is correct.
    if(eo_vectorcapacity_dynamic == vector->capacity)
    {   // in here i dont make any control because in _New() we have already verified that mempool is dynamic 
        vector->stored_items = eo_mempool_Realloc(eo_mempool_GetHandle(), vector->stored_items, (uint32_t)(vector->size) * vector->item_size);
    }
}


extern eOsizecntnr_t eo_vector_Size(EOvector * vector) 
{
    if(NULL == vector) 
    {    // invalid vector
        return(0);    
    }
    
    return(vector->size);        
}


extern void eo_vector_Clear(EOvector * vector) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    eOsizecntnr_t i = 0;        
    
    
    if(NULL == vector) 
    {   // invalid vector
        return;    
    }

    if(0 == vector->size) 
    {   // vector is empty
        return;     
    }
    

    start = (uint8_t*) (vector->stored_items);
    for(i=0; i<vector->size; i++) 
    {   // i use i as index because the items in a vector are always stored from pos 0 to size-1
        // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
        item = &start[(uint32_t)i * vector->item_size];
        if(NULL != vector->item_clear_fn)
        {
            vector->item_clear_fn(item);
        }
        else
        {
            s_eo_vector_default_clear(item, vector);
        }
    }
        
    
    vector->size = 0;
    
    //its ok to use realloc when size is zero because eo_mempool_Realloc() calls eo_mempool_Free() and returns NULL.
    if(eo_vectorcapacity_dynamic == vector->capacity)
    {   // in here i dont make any control because in _New() we have already verified that mempool is dynamic 
        vector->stored_items = eo_mempool_Realloc(eo_mempool_GetHandle(), vector->stored_items, (uint32_t)(vector->size) * vector->item_size);
    }
}


extern void * eo_vector_At(EOvector * vector, eOsizecntnr_t pos) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == vector) 
    {
        return(NULL);    
    }
    
    if(pos >= vector->size) 
    {   // vector does not have any element in pos
        return(NULL);     
    }
    
   
    start = (uint8_t*) (vector->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * vector->item_size];
    
    return((void*) item);         
}

extern void eo_vector_Assign(EOvector * vector, eOsizecntnr_t pos, void *items, eOsizecntnr_t nitems)
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *p = NULL;          // external item
    uint8_t *item = NULL;       // internal item
    uint16_t i;
        
    if((NULL == vector) || (NULL == items) || (0 == nitems)) 
    {   // invalid data
        return;    
    }
    
    if((pos+nitems-1) >= vector->capacity) 
    {   // beyond the capacity of the vector
        return;
    }
 
    
    if((pos+nitems-1) >= vector->size)
    {
        eo_vector_Resize(vector, pos+nitems); 
    }
    
    // now fill from pos-th position until (pos+nitems-1)-th position w/ objects pointed by items
    
    
    start = (uint8_t*) (vector->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * vector->item_size]; 
    p = (uint8_t*) items;    // first ext item of items[]
    
    for(i=0; i<nitems; i++)
    {
        if(NULL != vector->item_copy_fn) 
        {
            vector->item_copy_fn(item, p);
        }
        else
        {
            s_eo_vector_default_copy(item, p, vector);
        } 
        
        item += vector->item_size;
        p += vector->item_size;
    }
    
    return;     
}

extern void* eo_vector_storage_Get(EOvector * vector)
{
    if(NULL == vector) 
    {   // invalid data
        return (NULL);    
    }    
    
    return(vector->stored_items);   
}


extern void eo_vector_AssignOne(EOvector * vector, eOsizecntnr_t pos, void *p) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
        
    if((NULL == vector) || (NULL == p)) 
    {   // invalid data
        return;    
    }
    
    if(pos >= vector->capacity) 
    {   // beyond the capacity of the vector
        return;
    }
 
    
    if(pos >= vector->size)
    {
        eo_vector_Resize(vector, pos+1); 
    }
    
    // now fill the pos-th position w/ object p
    
    
    start = (uint8_t*) (vector->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * vector->item_size]; 
    
    if(NULL != vector->item_copy_fn) 
    {
        vector->item_copy_fn(item, p);
    }
    else
    {
        s_eo_vector_default_copy(item, p, vector);
    }
    
    return; 
}


extern void eo_vector_Resize(EOvector * vector, eOsizecntnr_t size) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    eOsizecntnr_t first;
    eOsizecntnr_t last;
    uint8_t added = 0;
    
    eOsizecntnr_t i = 0;        
        
    if(NULL == vector) 
    {   // invalid vector
        return;    
    }
        
    if(size == vector->size)
    {   // nothing to do
        return; 
    }
    
    if(0 == size)
    {   // if dymanic mode _Clear() also deletes the items
        eo_vector_Clear(vector);
        return;
    }

    if(size > vector->capacity)
    {   // nothing to do
        return; 
    }
    
    if(size < vector->size)
    {   // must remove some items
        first   = size;
        last    = vector->size;
        added = 0;
    }
    else
    {   // must add some items
        first   = vector->size;
        last    = size;
        added = 1;      
    }
    
    // new size
    vector->size = size;
    
    
    if(1 == added)
    {   // must create the new items
    
        // add items if dynamic mode
        if(eo_vectorcapacity_dynamic == vector->capacity)
        {   // i must allocate new memory
            // in here i dont make any control because in _New() we have already verified that mempool is dynamic 
            vector->stored_items = eo_mempool_Realloc(eo_mempool_GetHandle(), vector->stored_items, (uint32_t)(vector->size) * vector->item_size);
        }
        
        // ok, now i init the new memory as if it was just created. we do it for the new items.
        start = (uint8_t*) (vector->stored_items);
        for(i=first; i<last; i++) 
        {
            // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
            item = &start[(uint32_t)i * vector->item_size];
            if(NULL != vector->item_init_fn) 
            {
                vector->item_init_fn(item, vector->item_init_par);
            }
            else
            {
                s_eo_vector_default_init(item, vector);
            }
        }        
                
    } 
    else
    {   // must destroy the items
    
        start = (uint8_t*) (vector->stored_items);
        for(i=first; i<last; i++) 
        {
            // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
            item = &start[(uint32_t)i * vector->item_size];
            if(NULL != vector->item_clear_fn) 
            {
                vector->item_clear_fn(item);
            }
            else
            {
                s_eo_vector_default_clear(item, vector);
            }
        }  

        // remove items if dynamic mode
        if(eo_vectorcapacity_dynamic == vector->capacity)
        {   // in here i dont make any control because in _New() we have already verified that mempool is dynamic 
            vector->stored_items = eo_mempool_Realloc(eo_mempool_GetHandle(), vector->stored_items, (uint32_t)(vector->size) * vector->item_size);
        }  
        
    }
    
}


extern eObool_t eo_vector_Full(EOvector * vector) 
{
    if(NULL == vector) 
    {   // invalid vector
        return(eobool_true);    
    }
    
    return((vector->size == vector->capacity) ? (eobool_true) : (eobool_false));        
}

extern eObool_t eo_vector_Empty(EOvector * vector) 
{
    if(NULL == vector) 
    {   // invalid vector
        return(eobool_true);    
    }
    
    return((vector->size == 0) ? (eobool_true) : (eobool_false));        
}


extern eObool_t eo_vector_Find(EOvector * vector, void *p, eOsizecntnr_t *index)
{
    eOsizecntnr_t i = 0;
    uint8_t *item;

    if((NULL == vector) || (p == NULL) || (0 == vector->size)) 
    {   // invalid vector or invalid data to search
        return(eobool_false);    
    }

    // loop over all items to see is any matches with external data
    for(i=0, item = (uint8_t*) (vector->stored_items); i<vector->size; i++, item += vector->item_size)
    {
        if(0 == memcmp(p, item, vector->item_size))
        {
            if(NULL != index)
            {
                *index = i;
            }
            return(eobool_true);
        }
    }

    return(eobool_false);
}


extern void eo_vector_Delete(EOvector * vector)
{  
    if(NULL == vector) 
    {   // invalid vector
        return;    
    }   
    
    eo_errman_Assert(eo_errman_GetHandle(), (eo_mempool_alloc_dynamic == eo_mempool_alloc_mode_Get(eo_mempool_GetHandle())), s_eobj_ownname, "can use eo_vector_Delete() only w/ eo_mempool_alloc_dynamic");
  
    // destroy every item. in case of eo_vectorcapacity_dynamic, the vector->stored_items is also deleted
    eo_vector_Clear(vector);
    
    // destroy dataarray
    eo_mempool_Delete(eo_mempool_GetHandle(), vector->stored_items);
    
    // reset all things inside vector
    memset(vector, 0, sizeof(EOvector));
    
    // destroy object
    eo_mempool_Delete(eo_mempool_GetHandle(), vector);
       
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




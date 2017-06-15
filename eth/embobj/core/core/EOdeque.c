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

#include "EOdeque.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOdeque_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define EODEQUE_DEFAULTCLEAR_DOES_NOTHING


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


EO_static_inline void s_eo_deque_default_clear(void *item, EOdeque* deque)
{
#if defined(EODEQUE_DEFAULTCLEAR_DOES_NOTHING)
#else
    memset(item, 0, deque->item_size);
#endif
}

EO_static_inline void s_eo_deque_default_copy(void* item, void* p, EOdeque* deque)
{
    memcpy(item, p, deque->item_size);
}

EO_static_inline void s_eo_deque_default_init(void* item,  EOdeque* deque)
{
    memset(item, 0, deque->item_size);
}

//EO_static_inline void s_eo_deque_default_initall(EOdeque* deque)
//{
//    memset(deque->stored_items, 0, deque->capacity*deque->item_size);
//}

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOdeque";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOdeque * eo_deque_New(eOsizeitem_t item_size, eOsizecntnr_t capacity,
                              eOres_fp_voidp_uint32_t item_init, uint32_t init_par,  
                              eOres_fp_voidp_voidp_t item_copy, eOres_fp_voidp_t item_clear)
{
    EOdeque *retptr = NULL;
    uint8_t *start = NULL;
    uint8_t *item = NULL;
//    uint32_t pos = 0;
    eOsizecntnr_t i = 0; 
    eOmempool_alignment_t align = eo_mempool_align_08bit;


    // i get the memory for the object. no need to check versus NULL because the memory pool already does it
    retptr = (EOdeque*) eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOdeque), 1);



    // now the object has valid memory. i need to initialise it with user-defined data,
    
    retptr->size                = 0;
    retptr->first               = 0;
    retptr->next                = 0;

    eo_errman_Assert(eo_errman_GetHandle(), (0 != item_size), "eo_deque_New(): 0 item_size", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    eo_errman_Assert(eo_errman_GetHandle(), (0 != capacity), "eo_deque_New(): 0 capacity", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);

    retptr->item_size           = item_size;
    retptr->sizeofstoreditem    = item_size;
    retptr->capacity            = capacity;
    retptr->item_init_fn        = item_init;
    retptr->item_init_par       = init_par;    
    retptr->item_copy_fn        = item_copy;
    retptr->item_clear_fn       = item_clear;

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
    
    //#warning --> se alloco memoria per n oggetti (eg. di dimensione 6) in modo che ci sia allineamento ad 8, come indicizzo gli oggetti? a 6 oppure a 8?
    // da codice si evince a 6 ... quindi perche' spreco memoria nell'allocazione?
    // se uso dynamic, il heap viene gestito con allineamento a 8. ma quetso vuol dire che per 4 oggetti: 6*4 = 24 che sono tre uint64_t.
    // se uso align_64, anche qui ho array da uint64_t ma per 4 oggetti uso 4*(6+7)/8 = 4*1 = quattro uint64_t .... si spreca perche arrotondo sul item size e non sul totale. 

    // here is the memory from the correct memory pool (or the heap)
    retptr->stored_items  = (void*) eo_mempool_GetMemory(eo_mempool_GetHandle(), align, item_size, capacity);     

    start = (uint8_t*) (retptr->stored_items);
    for(i=0; i<capacity; i++) 
    {
        // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
        item = &start[(uint32_t)i * item_size];        
        if(NULL != item_init)
        {   // construct each item
            item_init(item, init_par);
        }
        else
        {   // default init
            s_eo_deque_default_init(item, retptr);
        }
    }

    return(retptr);  
}


extern void eo_deque_Delete(EOdeque * deque)
{
    if(NULL == deque) 
    {   // invalid deque
        return;    
    }   
    
    if(NULL == deque->stored_items)
    {
        return;
    }

    eo_deque_Clear(deque);

    memset(deque, 0, sizeof(EOdeque));    
    eo_mempool_Delete(eo_mempool_GetHandle(), deque);
    return;    
}


extern eOsizecntnr_t eo_deque_Capacity(EOdeque * deque) 
{
    if(NULL == deque) 
    {   // invalid deque
        return(0);    
    }
    
    return(deque->capacity);    
}


extern eObool_t eo_deque_Full(EOdeque * deque) 
{
    if(NULL == deque) 
    {   // invalid deque
        return(eobool_true);    
    }
    
    return((deque->size == deque->capacity) ? (eobool_true) : (eobool_false));        
}


extern eObool_t eo_deque_Empty(EOdeque * deque) 
{
    if(NULL == deque) 
    {   // invalid deque
        return(eobool_true);    
    }
    
    return((0 == deque->size) ? (eobool_true) : (eobool_false));        
}
 

extern void eo_deque_PushBack(EOdeque * deque, void *p) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
        
    if((NULL == deque) || (NULL == p)) 
    {   // invalid data
        return;    
    }
    
    if(deque->capacity == deque->size) 
    {   // deque is full
        return;
    }
       
    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)deque->next * deque->item_size]; 
    
    if(NULL != deque->item_copy_fn) 
    {
        deque->item_copy_fn(item, p);
    }
    else
    {
        s_eo_deque_default_copy(item, p, deque);
    }
    
    deque->next = (deque->next + 1) % (deque->capacity);
    deque->size ++;
    
    return; 
}


extern void eo_deque_PushFront(EOdeque * deque, void *p) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    // we use uint32_t because .... see note xxx.
    uint32_t pos = 0;
            
    if((NULL == deque) || (NULL == p)) 
    {   // invalid data
        return;    
    }
    
    if(deque->capacity == deque->size) 
    {   // deque is full
        return;
    }
    
    // note xxx.
    // cast to uint32_t is used to avoid problems when (capacity + first - 1) goes beyond max value of 
    // eOsizecntnr_t.
    // example for eOsizecntnr_t equal to uint8_t, capacity = 255, and first = 10: 
    // - we want that 265 is correctly represented before it is extracted the remainder of division by 255, 
    // - thus we must use a larger variable for pos .... uint32_t is large enough. 
    // SIMILARLY IF WE USE uint16_t for eOsizecntnr_t, but capacity = 65535 is ... hard to manage
    pos = (uint32_t)(deque->capacity + deque->first - 1) % deque->capacity;  
    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * deque->item_size];
    
    if(NULL != deque->item_copy_fn) 
    {
        deque->item_copy_fn(item, p);
    }
    else
    {
        s_eo_deque_default_copy(item, p, deque);
    }
    
    // cast is used to avoid possible compiler complains ... the cast is safe because ... see note xxx
    deque->first = (eOsizecntnr_t)pos;
    deque->size ++;
    
    return; 
}


extern void * eo_deque_Front(EOdeque * deque) 
{
    // here uint8_t is required to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == deque) 
    {   // invalid data
        return(NULL);    
    }
    
    if(0 == deque->size) 
    {   // deque is empty. returns NULL
        return(NULL);     
    }
    
    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)deque->first * deque->item_size];
    
    return((void*) item);         
}


extern void eo_deque_PopFront(EOdeque * deque) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == deque) 
    {   // invalid data
        return;    
    }
    
    if(0 == deque->size) 
    {   // deque is empty
        return;     
    }

    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)deque->first * deque->item_size];

    if(NULL != deque->item_clear_fn) 
    {
        deque->item_clear_fn(item);
    } 
    else 
    { 
        // clean the removed item
        s_eo_deque_default_clear(item, deque);
    }
    
    // in here there is no need to cast to a bigger integer. 
    // suppose uint8_t: even if capacity is 255, deque->first can reach at most 254. 
    // thus 254+1 = 255 can still be managed. 
    // SIMILARLY IF WE USE uint16_t for eOsizecntnr_t, but capacity = 65535 is ... hard to manage
    deque->first = (deque->first + 1) % (deque->capacity);
    deque->size --;        
}


extern void * eo_deque_Back(EOdeque * deque) 
{    
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    // we use uint32_t because .... see note xxx.
    uint32_t pos = 0;
    
    if(NULL == deque) 
    {   // invalid deque. return NULL
        return(NULL);    
    }
    
    if(0 == deque->size) 
    {   // deque is empty. return NULL
        return(start);     
    }
    
    // cast to uint32_t ... see note xxx
    pos = (uint32_t)(deque->next + deque->capacity - 1) % deque->capacity; 
    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * deque->item_size];
    
    return((void*) item);         
}


extern void eo_deque_PopBack(EOdeque * deque) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    // we use uint32_t because .... see note xxx.
    uint32_t pos = 0;
    
    if(NULL == deque) 
    {   // invalid deque
        return;    
    }
    
    if(0 == deque->size) 
    {   // deque is empty
        return;     
    }

    // cast to uint32_t. see note xxx.
    pos = (uint32_t)(deque->next + deque->capacity - 1) % deque->capacity; 

    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * deque->item_size];            
    
    if(NULL != deque->item_clear_fn) 
    {
        deque->item_clear_fn(item);
    } 
    else 
    {  // clear the removed item
       s_eo_deque_default_clear(item, deque);
    }

                
    // cast is used to avoid possible compiler complains ... the cast is safe because ... see note xxx
    deque->next = (eOsizecntnr_t)pos;
    deque->size --;       
}


extern eOsizecntnr_t eo_deque_Size(EOdeque * deque) 
{
    if(NULL == deque) 
    {   // invalid deque
        return(0);    
    }
    
    return(deque->size);        
}


extern void eo_deque_Clear(EOdeque * deque) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    // we use uint32_t because .... see note xxx.
    uint32_t pos = 0;
    eOsizecntnr_t i = 0;        
    
    
    if(NULL == deque) 
    {   // invalid deque
        return;    
    }

    if(0 == deque->size) 
    {   // deque is empty
        return;     
    }
    

    start = (uint8_t*) (deque->stored_items);
    pos = deque->first;     // i clear only deque->size items, thus i must start from deque->first position. I CANNOT loop from 0 to capacity because we would destroy also not-existing objects
    for(i=0; i<deque->size; i++) 
    {
        // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
        item = &start[(uint32_t)pos * deque->item_size];
        if(NULL != deque->item_clear_fn)
        {
            deque->item_clear_fn(item);
        }
        else
        {
            s_eo_deque_default_clear(item, deque);
        }
        pos = (pos + 1) % (deque->capacity);
    }
        
    deque->size     = 0;
    deque->first     = 0;
    deque->next     = 0;   
}


extern void * eo_deque_At(EOdeque * deque, eOsizecntnr_t pos) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;
    
    if(NULL == deque) 
    {
        return(NULL);    
    }
    
    if(pos >= deque->size) 
    {   // deque does not have any element in pos
        return(start);     
    }
    
    // we use uint32_t because .... see note xxx.
    pos = (uint32_t)(pos + deque->first) % (deque->capacity);
    
    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)pos * deque->item_size];
    
    return((void*) item);         
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------

extern void eo_deque_hid_QuickPopFront(EOdeque * deque) 
{
    // here we require uint8_t to access stored_items because we work with bytes.
    uint8_t *start = NULL;
    uint8_t *item = NULL;

// remove controls in order to speed-up things    
//    if(NULL == deque) 
//    {
//        // invalid data
//        return;    
//    }
//    
//    if(0 == deque->size) 
//    { 
//        // deque is empty
//        return;     
//    }

    start = (uint8_t*) (deque->stored_items);
    // cast to uint32_t to tell the reader that index of array start[] can be bigger than max eOsizecntnr_t
    item = &start[(uint32_t)deque->first * deque->item_size];

    if(NULL != deque->item_clear_fn) 
    {   // destroy
        deque->item_clear_fn(item);
    } 
    else 
    {   // default clear
        s_eo_deque_default_clear(item, deque);
    }
    
    // in here there is no need to cast to a bigger integer. 
    // suppose uint8_t: even if capacity is 255, deque->first can reach at most 254. 
    // thus 254+1 = 255 can still be managed. 
    deque->first = (deque->first + 1) % (deque->capacity);
    deque->size --;          
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section




// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




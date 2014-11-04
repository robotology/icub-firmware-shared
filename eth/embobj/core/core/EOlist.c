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

#include "EOlist.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOlist_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define EOLIST_DEFAULTCLEAR_DOES_NOTHING


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


EO_static_inline void s_eo_list_default_clear(void *item, EOlist* list)
{
#if defined(EOLIST_DEFAULTCLEAR_DOES_NOTHING)
#else
    memset(item, 0, list->item_size);
#endif
}

EO_static_inline void s_eo_list_default_copy(void* item, void* p, EOlist* list)
{
    memcpy(item, p, list->item_size);
}

EO_static_inline void s_eo_list_default_init(void* item,  EOlist* list)
{
    memset(item, 0, list->item_size);
}



EO_static_inline void* s_eo_list_get_data(EOlist *list, EOlistIter *li)
{
    if(list->item_size > sizeof(void*))
    {
        return(li->data);
    }
    else
    {
        return(&(li->data));
    }
}

static EOlistIter * s_eo_list_push_front(EOlistIter *head, EOlistIter *li);
static EOlistIter * s_eo_list_push_back(EOlistIter *tail, EOlistIter *li);
static EOlistIter * s_eo_list_insert_before(EOlistIter *head, EOlistIter *iter, EOlistIter *li);
static EOlistIter * s_eo_list_rem_front(EOlistIter *head);
static EOlistIter * s_eo_list_rem_back(EOlistIter *tail);
//static EOlistIter * s_eo_list_rem_iter(EOlistIter *head, EOlistIter *li);
static void s_eo_list_rem_any(EOlist *list, EOlistIter *li);
static EOlistIter * s_eo_list_front(EOlist *list);
static EOlistIter * s_eo_list_back(EOlist *list);
static EOlistIter * s_eo_list_iter_next(EOlistIter *li);
static EOlistIter * s_eo_list_iter_prev(EOlistIter *li);

static void s_eo_list_copy_item_into_iterator(EOlist *list, EOlistIter *li, void *p);
static void s_eo_list_clean_iterator(EOlist *list, EOlistIter *li);

static EOlistIter* s_eo_list_iterator_create(EOlist* list);

static void s_eo_list_iterator_destroy(EOlist* list, EOlistIter* li);

static EOlistIter* s_eo_list_iterator_get(EOlist* list);
static void s_eo_list_iterator_release(EOlist* list, EOlistIter* li);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOlist";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOlist* eo_list_New(eOsizeitem_t item_size, eOsizecntnr_t capacity, 
                           eOres_fp_voidp_uint32_t item_init, uint32_t init_par,
                           eOres_fp_voidp_voidp_t item_copy, eOres_fp_voidp_t item_clear)
{
    EOlist *retptr = NULL;
    eOsizecntnr_t i = 0; 


    // i get the memory for the object
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOlist), 1);

    
    // now the obj has valid memory. i need to initialise it with user-defined data,
    retptr->head            = NULL;
    retptr->tail            = NULL;
    retptr->size            = 0;

    eo_errman_Assert(eo_errman_GetHandle(), (0 != item_size), s_eobj_ownname, "item_size is zero");
    eo_errman_Assert(eo_errman_GetHandle(), (0 != capacity), s_eobj_ownname, "capacity is zero");

    retptr->capacity            = capacity;
    retptr->item_size           = item_size;
    retptr->item_init_fn        = item_init;
    retptr->item_init_par       = init_par;    
    retptr->item_copy_fn        = item_copy;
    retptr->item_clear_fn       = item_clear;
    
    if(eo_listcapacity_dynamic == retptr->capacity)
    {
        eo_errman_Assert(eo_errman_GetHandle(), (eo_mempool_alloc_dynamic == eo_mempool_alloc_mode_Get(eo_mempool_GetHandle())), s_eobj_ownname, "can use eo_vectorcapacity_dynamic only w/ eo_mempool_alloc_dynamic");
        retptr->freeiters = NULL;        
    }
    else
    {   
        for(i=0; i<capacity; i++) 
        {
            EOlistIter *li = s_eo_list_iterator_create(retptr);    
            retptr->freeiters = s_eo_list_push_front(retptr->freeiters, li);
        } 

    }    

    return(retptr);
}



extern eOsizecntnr_t eo_list_Size(EOlist *list) 
{
    if(NULL == list) 
    {
        return(0);    
    }
    
    return(list->size);        
}


extern eOsizecntnr_t eo_list_Capacity(EOlist *list) 
{
    if(NULL == list)
    {
        return(0);    
    }
    
    return(list->capacity);    
}


extern eObool_t eo_list_Empty(EOlist *list) 
{
    if(NULL == list) 
    {
        return(eobool_true);    
    }
    
    return((0 == list->size) ? (eobool_true) : (eobool_false));        
}


extern eObool_t eo_list_Full(EOlist *list) 
{
    if(NULL == list) 
    {
        return(eobool_true);    
    }
    
    return((list->capacity == list->size) ? (eobool_true) : (eobool_false));        
}


extern void eo_list_PushFront(EOlist *list, void *p) 
{
    EOlistIter *tmpiter = NULL;
    
    if((NULL == list) || (NULL == p)) 
    {
        return;    
    }
    
    if(list->capacity == list->size) 
    { 
        // list is full
        return;
    }

    tmpiter = s_eo_list_iterator_get(list);

    if(NULL != tmpiter) 
    {
        // copy the passed obj inside the iter or store it directly if size is small
        s_eo_list_copy_item_into_iterator(list, tmpiter, p);
        
        // if it is the first element in the list, set the tail.
        if(0 == list->size) 
        {
            list->tail = tmpiter;
        }

        // insert the iter in front of the head
        list->head = s_eo_list_push_front(list->head, tmpiter);

        // increment size of the list    
        list->size ++;
    }
    
    return; 
}


extern void * eo_list_Front(EOlist *list) 
{
    EOlistIter *li = NULL;
    void *ret = NULL;
    
    if(NULL == list) 
    {
        return(NULL);
    }
    
    li = s_eo_list_front(list);
    
    if(NULL == li) 
    {
        return(NULL);    
    }

    ret = s_eo_list_get_data(list, li);
   
    return(ret);         
}


extern void eo_list_PushBack(EOlist *list, void *p) 
{
    EOlistIter *tmpiter = NULL;
    
    if((NULL == list) || (NULL == p)) 
    {
        return;    
    }
    
    if(list->capacity == list->size) 
    { 
        // list is full
        return;
    }

    tmpiter = s_eo_list_iterator_get(list);    

    if(NULL != tmpiter) 
    {
        // copy the passed obj inside the iter or store it directly if size is small
        s_eo_list_copy_item_into_iterator(list, tmpiter, p);
        
        // if it is the first element in the list, set the head.
        if(0 == list->size) 
        {
             list->head = tmpiter;
        }

        // insert the iter after the tail
        list->tail = s_eo_list_push_back(list->tail, tmpiter);

        // increment size of the list    
        list->size ++;
    }
    
    return; 
}


extern void * eo_list_Back(EOlist *list) 
{
    EOlistIter *tmpiter = NULL;
    void *ret = NULL;
    
    if(NULL == list) 
    {
        return(NULL);
    }
    
    tmpiter = s_eo_list_back(list);
    
    if(NULL == tmpiter) 
    {
        return(NULL);    
    }

    ret = s_eo_list_get_data(list, tmpiter);
    
    return(ret);         
}


extern void eo_list_Insert(EOlist *list, EOlistIter *li, void *p)
{
    EOlistIter *tmpiter = NULL;
    
    if((NULL == list) || (NULL == li)) 
    {
        return;    
    }

    if(list->capacity == list->size) 
    {   // list is full
        return;
    }

    tmpiter = s_eo_list_iterator_get(list);
    
    if(NULL != tmpiter) 
    {
        // copy the passed obj inside the iter tmpiter or store it directly if size is small
        s_eo_list_copy_item_into_iterator(list, tmpiter, p);
        
        // if it is the first element in the list, set the tail.
        if(0 == list->size) 
        {
             list->tail = tmpiter;
        }
        // insert the element tmpiter in front of the iter li
        list->head = s_eo_list_insert_before(list->head, li, tmpiter);
        // increment size of the list    
        list->size ++;
    }
   
    return;
}


extern void * eo_list_At(EOlist *list, EOlistIter *li) 
{
    void *ret = NULL;
    
    if((NULL == list) || (NULL == li)) 
    {
        return(NULL);    
    }

    ret = s_eo_list_get_data(list, li);

    return(ret);
}


extern EOlistIter* eo_list_Begin(EOlist *list) 
{
    return((NULL == list) ? (NULL) :(s_eo_list_front(list)));         
}
 

extern EOlistIter* eo_list_Next(EOlist *list, EOlistIter *li) 
{
    // next in list is simple. if li belongs to list, it is enough to get next of li.
    // we dont do check that li belong to list, because it can be heavy to do.
    if(NULL == list)
    {
        return(NULL);
    }
    return(s_eo_list_iter_next(li));         
    
}


extern EOlistIter* eo_list_Prev(EOlist *list, EOlistIter *li) 
{
    // previous in list can be dangerous also if li belongs to list.
    // if li is head, .... what is prev ?? NULL.
    // we dont do check that li belong to list, because it can be heavy to do. 
    if(NULL == list)
    {
        return(NULL);
    }
    return(s_eo_list_iter_prev(li));         
}


extern EOlistIter* eo_list_FindItem(EOlist *list, void *p) 
{
    EOlistIter *tmpiter = NULL;
    const void* target = (const void*)p;
    void* data = NULL;

    if((NULL == list) || (NULL == p)) 
    {
         return(NULL);
    }
    
    // i navigate from beginning to end until i find a NULL pointer or i break
    for(tmpiter = s_eo_list_front(list); NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
    {
        data = s_eo_list_get_data(list, tmpiter);
        // data is a pointer to what is contained inside the list.

        if(0 == memcmp(data, target, list->item_size))
        {
            break;
        }

    }
    
    return(tmpiter);    
}


/* @fn         extern EOlistIter* eo_list_FindInside(EOlist *list, uint32_t target, uint32_t (get_value_from_item)(void *item))
    @brief      Finds the iterator which contains the object which matches the target by means of function get_target(). 
    @param      list            Pointer to the EOlist object.
    @param      target          The target.
    @param      get_value_from_item   The function which gets the value to be compared with @e target
    @return     The iterator (or NULL if list is NULL or empty / @e p is not in the list / @e p 
                is NULL).
 **/
//extern EOlistIter* eo_list_FindInside(EOlist *list, uint32_t target, uint32_t (get_value_from_item)(void *item));

//extern EOlistIter* eo_list_FindInside(EOlist *list, uint32_t target, uint32_t (get_value_from_item)(void *item)) 
//{
//    EOlistIter *tmpiter = NULL;
//    void* data = NULL;
//
//    if((NULL == list) || (NULL == get_value_from_item)) 
//    {
//         return(NULL);
//    }
//    
//    
//    // i navigate from beginning to end until i find a NULL pointer or i break
//    for(tmpiter = s_eo_list_front(list); NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
//    {
//        data = s_eo_list_get_data(list, tmpiter);
//
//        if(target == get_value_from_item(data)) 
//        {
//            break;
//        }
//
//    }
//    
//    return(tmpiter);    
////}

static eOresult_t s_eo_list_default_matching_rule(EOlist * list, void *item, void *param)
{
    if(0 == memcmp(item, param, list->item_size))
    {
        return(eores_OK);
    }
    else
    {
        return(eores_NOK_generic);
    }
}

extern EOlistIter* eo_list_Find(EOlist *list, eOresult_t (matching_rule)(void *item, void *param), void *param)
{
    EOlistIter *tmpiter = NULL;
    void* data = NULL;
    eOresult_t res = eores_NOK_generic;

    if((NULL == list) || (NULL == param)) 
    {
         return(NULL);
    }
    
    // i navigate from beginning to end until i find a NULL pointer or i break
    for(tmpiter = s_eo_list_front(list); NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
    {
        data = s_eo_list_get_data(list, tmpiter);

        if(NULL != matching_rule)
        {
            res = matching_rule(data, param);
        }
        else
        {
            res = s_eo_list_default_matching_rule(list, data, param);
        }
        
        if(eores_OK == res)      
        //if(eores_OK == matching_rule(data, param)) 
        {
            break;
        }

    }
    
    return(tmpiter);   

}

extern void eo_list_Execute(EOlist *list, void (execute)(void *item, void *param), void *param)
{
    EOlistIter *tmpiter = NULL;
    void* data = NULL;

    if((NULL == list) || (NULL == execute)) 
    {
         return;
    }
    
    // i navigate from beginning to end until i find a NULL pointer
    for(tmpiter = s_eo_list_front(list); NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
    {
        data = s_eo_list_get_data(list, tmpiter);
        execute(data, param);
    }
    
    return; 
}


extern void eo_list_ExecuteFromIter(EOlist *list, void (execute)(void *item, void *param), void *param, EOlistIter *li)
{
    EOlistIter *tmpiter = NULL;
    void* data = NULL;

    if((NULL == list) || (NULL == li) || (NULL == execute)) 
    {
         return;
    }
    
    // i navigate from li to end until i find a NULL pointer
    for(tmpiter = li; NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
    {
        data = s_eo_list_get_data(list, tmpiter);
        execute(data, param);
    }
    
    return; 
}


extern eObool_t eo_list_IsIterInside(EOlist *list, EOlistIter *li)
{
    EOlistIter *tmpiter = NULL;

    if((NULL == list) || (NULL == li)) 
    {
         return(eobool_false);
    }
    
    // i navigate from beginning to end until we find li pointer or we return
    for(tmpiter = s_eo_list_front(list); NULL != tmpiter; tmpiter = s_eo_list_iter_next(tmpiter)) 
    {
        if(li == tmpiter) 
        {
            return(eobool_true);
        }
    }
    
    return(eobool_false);    
}


extern void eo_list_PopFront(EOlist *list) 
{
    EOlistIter *tmpiter = NULL;
            
    if(NULL == list) 
    {
        return;    
    }
 
    // get the first iter of list
    tmpiter = s_eo_list_front(list);
    
    if(NULL != tmpiter) 
    {
        // i remove it from front of the list
        list->head = s_eo_list_rem_front(list->head);

        // release it
        s_eo_list_iterator_release(list, tmpiter);
        
        // finally, i decrement size of list
        list->size --;

        // if there are no more elements in the list, set the tail to NULL
        if(0 == list->size) 
        {
             list->tail = NULL;
        }
    }
    
    return; 
}


extern void eo_list_PopBack(EOlist *list) 
{
    EOlistIter *tmpiter = NULL;

            
    if(NULL == list) 
    {
        return;    
    }
    
    // get the last iter of list
    tmpiter = s_eo_list_back(list);
    
    if(NULL != tmpiter) 
    {
        // i remove it from end of the list
        list->tail = s_eo_list_rem_back(list->tail);

        // release it
        s_eo_list_iterator_release(list, tmpiter);

        // finally, i decrement size of list
        list->size --;

        // if there are no more elements in the list, set the head to NULL
        if(0 == list->size) 
        {
             list->head = NULL;
        }
    }
    
    return; 
}


extern void eo_list_Erase(EOlist *list, EOlistIter *li) 
{
    EOlistIter *tmpiter = NULL;
    
    if((NULL == list) || (NULL == li)) 
    {
        return;    
    }

    // extra safety
    if(eobool_false == eo_list_IsIterInside(list, li)) 
    {
         return;
    }
    
    // ok, the iter li exists in the list, thus i can safely remove it.

    // get the first iter of list, the head
    tmpiter = s_eo_list_front(list);

    if(NULL != tmpiter) 
    {
        // i have a valid head and a valid list iter li, thus i can i remove it
        //list->head = s_eo_list_rem_iter(list->head, li);
        s_eo_list_rem_any(list, li);

        // release it
        s_eo_list_iterator_release(list, li);

        // finally, i decrement size of list
        list->size --;

        // if there are no more elements in the list, set the tail to NULL
        if(0 == list->size) 
        {
             list->tail = NULL;
        }
    }
    
    return;
}


extern void eo_list_Clear(EOlist *list)
{
    if(NULL == list) 
    {
        return;    
    }

    while(NULL != s_eo_list_front(list))
    {
        eo_list_PopFront(list);
    }
}


extern void eo_list_Delete(EOlist *list)
{  
    if(NULL == list) 
    {   // invalid list
        return;    
    }   
    
    eo_errman_Assert(eo_errman_GetHandle(), (eo_mempool_alloc_dynamic == eo_mempool_alloc_mode_Get(eo_mempool_GetHandle())), s_eobj_ownname, "can use eo_list_Delete() only w/ eo_mempool_alloc_dynamic");
  
    // destroy every item. in case of eo_listcapacity_dynamic, each internal listiter is properly deleted and freeiters is NULL
    eo_list_Clear(list);
    
    // destroy freeiters list and its items (not needed)
    //eo_mempool_Delete(eo_mempool_GetHandle(), list->freeiters);
    if(NULL != list->freeiters) 
    {   // aka we dont use dymanic mode: we must destroy each element inside freeiters
        uint16_t i;
        for(i=0; i<list->capacity; i++) 
        {
            EOlistIter *li =  list->freeiters;
            if(NULL == li)
            {
                break;
            }
            list->freeiters = s_eo_list_rem_front(list->freeiters);
            s_eo_list_iterator_destroy(list, li);             
        }         
    }
    
    // reset all things inside vector
    memset(list, 0, sizeof(EOlist));
    
    // destroy object
    eo_mempool_Delete(eo_mempool_GetHandle(), list);
       
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


// returns the head
static EOlistIter * s_eo_list_push_front(EOlistIter *head, EOlistIter *li) 
{
    EOlistIter *oldhead = head;
    
    li->prev = NULL;
    li->next = head;
    
    head = li;
    
    if(NULL != oldhead) 
    {
        oldhead->prev = head;
    }
    
    return(head);
}


// returns teh tail
static EOlistIter * s_eo_list_push_back(EOlistIter *tail, EOlistIter *li) 
{
    EOlistIter *oldtail = tail;
    
    li->prev = tail;
    li->next = NULL;
    
    tail = li;
    
    if(NULL != oldtail) 
    {
        oldtail->next = tail;
    }
    
    return(tail);
}


// returns the head
static EOlistIter * s_eo_list_insert_before(EOlistIter *head, EOlistIter *iter, EOlistIter *li) 
{
    
    if(iter == head)
    {
        return(s_eo_list_push_front(head, li));
    }
    else
    {
        // iter is not null, if iter is not the head, then ... also iter->prev is not NULL
        // pre is teh node before iter
        EOlistIter *pre = iter->prev;

        // fix the links of li
        li->prev = pre;
        li->next = iter;

        // fix the link of pre
        pre->next = li;

        // fix the link of iter
        iter->prev = li;

        // the head remains the same
        return(head);
    }

}

 
// returns the head
static EOlistIter * s_eo_list_rem_front(EOlistIter *head) 
{
    EOlistIter *oldhead = head;
    
    if(NULL == oldhead) 
    {
        return(NULL);    
    }
    
    if(NULL == oldhead->next) 
    {
        // only one element in current list
        return(NULL);
    } 
    else 
    {
        // at least two elements
        head = oldhead->next;
        head->prev = NULL;
    }
    

    return(head);
}


// returns the tail
static EOlistIter * s_eo_list_rem_back(EOlistIter *tail) 
{
    EOlistIter *oldtail = tail;
    
    if(NULL == oldtail) 
    {
        return(NULL);    
    }
    
    if(NULL == oldtail->prev) 
    {
        // only one element in current list
        return(NULL);
    } 
    else 
    {
        // at least two elements
        tail = oldtail->prev;
        tail->next = NULL;
    }
    

    return(tail);
}



// returns the head
//static EOlistIter * s_eo_list_rem_iter(EOlistIter *head, EOlistIter *li) 
//{
//    EOlistIter *newhead = head;
//    
//    if(NULL == head) 
//    {
//        return(NULL);    
//    }
//
//    if(NULL == li) 
//    {
//         return(newhead);
//    }
//
//    // head and li are not NULL
//    if(li == head) 
//    {
//         // li is the head, thus remove li from the front.
//        newhead = s_eo_list_rem_front(head);
//    }
//    else if(NULL == li->next) 
//    {
//        // li is the tail, thus ...
//        li->prev->next = NULL;
//    }
//    else 
//    {
//         // li is in the middle
//        li->prev->next = li->next;
//        li->next->prev = li->prev;
//    }
//
//
//    return(newhead);
//}


static void s_eo_list_rem_any(EOlist *list, EOlistIter *li) 
{
    EOlistIter *head = list->head;
//    EOlistIter *newhead = list->head;
    
    if(NULL == list->head) 
    {
        return;    
    }

    if(NULL == li) 
    {
         return;
    }

    // head and li are not NULL
    if(li == list->head) 
    {
         // li is the head, thus remove li from the front.
        list->head = s_eo_list_rem_front(head);
    }
    else if(li == list->tail) 
    {
        // li is the tail, thus ...
        list->tail = s_eo_list_rem_back(list->tail);
    }
    else 
    {
         // li is in the middle
        li->prev->next = li->next;
        li->next->prev = li->prev;
    }

}


static EOlistIter * s_eo_list_front(EOlist *list) 
{
    return(list->head);
}


static EOlistIter * s_eo_list_back(EOlist *list) 
{
    return(list->tail);
}


static EOlistIter * s_eo_list_iter_next(EOlistIter *li) 
{
    if(NULL == li) 
    {
         return(NULL);
    }

    return(li->next);
}  


static EOlistIter * s_eo_list_iter_prev(EOlistIter *li) 
{
    if(NULL == li) 
    {
         return(NULL);
    }

    return(li->prev);
}  


static void s_eo_list_copy_item_into_iterator(EOlist *list, EOlistIter *li, void *p)
{
    void* data = s_eo_list_get_data(list, li);

    if(NULL != list->item_copy_fn)
    {
        list->item_copy_fn(data, p);
    }
    else
    {
        s_eo_list_default_copy(data, p, list);
    }

}


static void s_eo_list_clean_iterator(EOlist *list, EOlistIter *li)
{
    void* data = s_eo_list_get_data(list, li);

    // call its destructor
    if(NULL != list->item_clear_fn)
    {
        list->item_clear_fn(data);
    }
    else
    {
        s_eo_list_default_clear(data, list);
    }

}


static EOlistIter* s_eo_list_iterator_create(EOlist* list)
{
//    #warning --> prima allocavo errato e troppo: una sizeof(EOlist)
    EOlistIter *li = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOlistIter), 1);
    //eo_mempool_New(eo_mempool_GetHandle(), sizeof(EOlist));
    
    // now we allocate memory for storing the items with size item_size. 
    // however, if item_size is smaller/equal to the size of a void* (<=4 in 32-bit arch), then we use the value of data to 
    // store the item directly instead of allocating extra memory .... 
    if(list->item_size > sizeof(void*))
    {   // normal mode: the .data field contains a pointer to the actual data
        li->data = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, list->item_size, 1);
        //eo_mempool_New(eo_mempool_GetHandle(), list->item_size);
        
        if(NULL != list->item_init_fn)
        {
            list->item_init_fn(li->data, list->item_init_par);
        }
        else
        {
//            #warning --> verifica
            s_eo_list_default_init(li->data, list);
        }
    }
    else
    {   // compact mode: the bytes of the .data field contain the data itself. 
        if(NULL != list->item_init_fn)
        {   // thus, item_init() accepts the pointer to .data
            list->item_init_fn(&(li->data), list->item_init_par);
        }
        else
        {
//            #warning --> verifica
            s_eo_list_default_init(&(li->data), list);
        }
    }
    
    return(li);
}

static void s_eo_list_iterator_destroy(EOlist* list, EOlistIter* li)
{
    if(list->item_size > sizeof(void*))
    {   // normal mode: the .data field contains a pointer to the actual data
        
//        if(NULL != list->item_clear_fn)
//        {
//            list->item_clear_fn(li->data);
//        }
//        else
//        {
//            #warning --> verifica
//            s_eo_list_default_clear(li->data, list);
//        }
        
        eo_mempool_Delete(eo_mempool_GetHandle(), li->data);   
    }
    else
    {   // compact mode: the bytes of the .data field contain the data itself. 
//        if(NULL != list->item_clear_fn)
//        {   // thus, item_init() accepts the pointer to .data
//            list->item_clear_fn(&(li->data));
//        }
//        else
//        {
//            #warning --> verifica
//            s_eo_list_default_clear(&(li->data), list);
//        }
    }
    
    memset(li, 0, sizeof(EOlistIter));
    eo_mempool_Delete(eo_mempool_GetHandle(), li);  
} 


static EOlistIter* s_eo_list_iterator_get(EOlist* list)
{
    EOlistIter* li = NULL;
    
    if(eo_listcapacity_dynamic == list->capacity)
    {   // create it
        li = s_eo_list_iterator_create(list);
    }
    else
    {   // get the first free iter
        li = list->freeiters;
        // and i remove it from front of the free list. the following s_eo_list_rem_front() does nothing is argument is NULL.
        list->freeiters = s_eo_list_rem_front(list->freeiters);
    }
    
    return(li);
}

static void s_eo_list_iterator_release(EOlist* list, EOlistIter* li)
{
    // i clean it 
    s_eo_list_clean_iterator(list, li);
   
    if(eo_listcapacity_dynamic == list->capacity)
    {   // destroy it
        s_eo_list_iterator_destroy(list, li);
    }
    else
    {   // i put li back into the free iters
        list->freeiters = s_eo_list_push_front(list->freeiters, li);
    }
}

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




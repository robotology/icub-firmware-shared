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
#include "stdio.h"
#include "string.h"
#include "EoCommon.h"
#include "EOtheErrorManager.h"
#include "EOVmutex.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheMemoryPool.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheMemoryPool_hid.h"


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section

 // --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

const eOmempool_cfg_t eom_mempool_DefaultCfg = 
{
    EO_INIT(.mode)          eo_mempool_alloc_dynamic,
    EO_INIT(.conf)          NULL
};



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static void * s_eo_mempool_get_static(eOmempool_alignment_t alignmode, uint16_t size, uint16_t number, uint32_t* usedbytes);

static void * s_memallocator(uint32_t s);

static void s_memfree(void *p);

static void * s_memrealloc(void *p, uint32_t s);

//static size_t s_eo_mempool_heap_sizeof_allocated_pointer(void* p);

//static uint16_t s_align_size(eOmempool_alignment_t alignmode, uint16_t size);

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOtheMemoryPool";


static EOtheMemoryPool s_the_mempool = 
{ 
    EO_INIT(.initted)       0, 
    EO_INIT(.config)        
    {
        EO_INIT(.mode)      eo_mempool_alloc_dynamic,
        EO_INIT(.conf)      NULL
    }, 
    EO_INIT(.theheap)  
    {
        EO_INIT(.allocate)      s_memallocator,
        EO_INIT(.reallocate)    s_memrealloc,
        EO_INIT(.release)       s_memfree           
    },
    EO_INIT(.thepool)
    {
        EO_INIT(.config)
        {
            EO_INIT(.size08)        0, 
            EO_INIT(.data08)        NULL, 
            EO_INIT(.size16)        0, 
            EO_INIT(.data16)        NULL, 
            EO_INIT(.size32)        0, 
            EO_INIT(.data32)        NULL, 
            EO_INIT(.size64)        0, 
            EO_INIT(.data64)        NULL   
        },
        EO_INIT(.status)
        {
            EO_INIT(.poolsmask)     0, 
            EO_INIT(.uint08index)   0, 
            EO_INIT(.uint16index)   0, 
            EO_INIT(.uint32index)   0, 
            EO_INIT(.uint64index)   0      
        }
    },
    EO_INIT(.mutex)         NULL, 
    EO_INIT(.tout)          0, 
    EO_INIT(.stats)     
    {
        EO_INIT(.usedbytesheap)     0,
        EO_INIT(.usedbytespool)     0
    }
};


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOtheMemoryPool * eo_mempool_Initialise(const eOmempool_cfg_t *cfg)
{   
    if(1 == s_the_mempool.initted)
    {
        return(&s_the_mempool);
    } 

    
    s_the_mempool.initted = 1;
    s_the_mempool.stats.usedbytesheap = 0;
    s_the_mempool.stats.usedbytespool = 0;

    // allow initialisation with null cfg .....
    if(NULL == cfg)
    {
        // if we use the default, then ... use dynamic mode with calloc, realloc, free
        cfg = &eom_mempool_DefaultCfg;            
    }
    

    
    // for every mode i copy cfg into config
    
    memcpy(&s_the_mempool.config, cfg, sizeof(eOmempool_cfg_t));
    
    
    // evaluate the mode. 
    // for dynamic just prepare the heap, for static or mixed prepare the heap and the pool   
    
    switch(cfg->mode)
    {
        case eo_mempool_alloc_dynamic:
        {
            if(NULL != cfg->conf)
            {
                // override heap w/ user-provided functions. however at end of function we verify that they are non-null
                s_the_mempool.theheap.allocate      =   cfg->conf->heap.allocate;
                s_the_mempool.theheap.reallocate    =   cfg->conf->heap.reallocate;
                s_the_mempool.theheap.release       =   cfg->conf->heap.release;
            }
            
        } break;
        
        case eo_mempool_alloc_static:
        case eo_mempool_alloc_mixed:
        {               
            if(NULL != cfg->conf)
            {   
                eOmempool_the_pool_t* pool = &s_the_mempool.thepool;
                memcpy(&s_the_mempool.thepool.config, cfg->conf, sizeof(eOmempool_pool_config_t)); 
                
                
                if(0 != pool->config.size08)
                {
                    pool->status.poolsmask |= 1;
                    pool->status.uint08index = 0;
                }
                
                if(0 != pool->config.size16)
                {
                    pool->status.poolsmask |= 2;
                    pool->status.uint16index = 0;
                }                
                
                if(0 != pool->config.size32)
                {
                    pool->status.poolsmask |= 4;
                    pool->status.uint32index = 0;
                }      

                if(0 != pool->config.size64)
                {
                    pool->status.poolsmask |= 8;
                    pool->status.uint64index = 0;
                }                   

            }
            else
            {
                eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_mempool_Initialise(): pools needed", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
            }    
                       
        } break;
        
        default:
        {
            
        } break;
 
    }

    // manage NULL heap functions
   
    if(NULL == s_the_mempool.theheap.allocate)
    {
        s_the_mempool.theheap.allocate      =   s_memallocator;
    }
    
    if(NULL == s_the_mempool.theheap.reallocate)
    {
        s_the_mempool.theheap.reallocate   =   s_memrealloc;
    }
    
    if(NULL == s_the_mempool.theheap.release)
    {
        s_the_mempool.theheap.release      =   s_memfree;
    }    
    
    return(&s_the_mempool);
}
 
    
extern EOtheMemoryPool* eo_mempool_GetHandle(void) 
{
    return((1==s_the_mempool.initted) ? (&s_the_mempool) : (NULL));
}


extern eOresult_t eo_mempool_SetMutex(EOtheMemoryPool *p, EOVmutexDerived *mutex, eOreltime_t tout) 
{ 
    // avoid assigning more than one mutex to the mempool
    if(NULL != s_the_mempool.mutex) 
    {
        return(eores_NOK_generic);
    }
   
    s_the_mempool.mutex = mutex;
    s_the_mempool.tout = tout;
    
    return(eores_OK);
}    


extern void * eo_mempool_GetMemory(EOtheMemoryPool *p, eOmempool_alignment_t alignmode, uint16_t size, uint16_t number)
{
    void *ret = NULL;
    uint32_t usedbytespool = 0;
    uint32_t usedbytesheap = 0;
    eOmempool_alloc_mode_t mode = s_the_mempool.config.mode;
          
    if((0 == size) || (0 == number)) 
    {
        // manage the ... warning 
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_zerorequested;
        errdes.par16            = 0;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_warning, "eo_mempool_GetMemory() is asked 0 bytes", s_eobj_ownname, &errdes);        
        return(NULL);
    }
   

    if(NULL == p)
    {
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_notinitialised;
        errdes.par16            = 0;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;    
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_info, "eo_mempool_GetMemory() not initted uses dyn alloc", s_eobj_ownname, &errdes); 
        mode = eo_mempool_alloc_dynamic;       
    }    
      
    
    // ok, using the singleton
        
    switch(mode)
    {
        case eo_mempool_alloc_dynamic:
        {
            ret = s_the_mempool.theheap.allocate(number*size);
            usedbytesheap = eo_common_msize(ret);
        } break;
        
        case eo_mempool_alloc_mixed:
        {        
            if(0 == (p->thepool.status.poolsmask & (uint8_t)alignmode))
                {   // dont have a pool for the alignmode: use heap
                ret = s_the_mempool.theheap.allocate(number*size);
                usedbytesheap = eo_common_msize(ret);             
            }
            else
            {   // i have a proper pool 
                //size = s_align_size(alignmode, size);  // alignment is internal to s_eo_mempool_get_static()
                ret = s_eo_mempool_get_static(alignmode, size, number, &usedbytespool);   
            }

        } break;
        
        case eo_mempool_alloc_static:
        {   // dont care if i dont have a proper pool for teh requested align mode. if not found ... error
            //size = s_align_size(alignmode, size);  // alignment is internal to s_eo_mempool_get_static()
            ret = s_eo_mempool_get_static(alignmode, size, number, &usedbytespool);
        } break;
    
    }
    
    if(NULL == ret)
    {   // manage the fatal error in case memory could not achieved
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_missing;
        errdes.par16            = number*size;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;         
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_mempool_GetMemory() no more memory", s_eobj_ownname, &errdes);
    }
    
    s_the_mempool.stats.usedbytespool += usedbytespool; 
    s_the_mempool.stats.usedbytesheap += usedbytesheap;
    
    return(ret);   
}


extern uint32_t eo_mempool_SizeOfAllocated(EOtheMemoryPool *p)
{
    return(s_the_mempool.stats.usedbytespool+s_the_mempool.stats.usedbytesheap);
}

extern eOmempool_alloc_mode_t eo_mempool_alloc_mode_Get(EOtheMemoryPool *p)
{   
    return(s_the_mempool.config.mode);
}


extern void * eo_mempool_New(EOtheMemoryPool *p, uint32_t size)
{
    void *ret = s_the_mempool.theheap.allocate(size);

    if(NULL == ret)
    {   // manage the fatal error in case memory could not be achieved
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_missing;
        errdes.par16            = size;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0; 
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_mempool_New() no more memory", s_eobj_ownname, &errdes);
    }
    
    s_the_mempool.stats.usedbytesheap += eo_common_msize(ret);      

    return(ret);   
}


extern void * eo_mempool_Realloc(EOtheMemoryPool *p, void *m, uint32_t size)
{  
    void *ret = NULL;
    if(0 == size)
    {
        eo_mempool_Delete(p, m);
        return(NULL);
    }
    
    
    if(eo_mempool_alloc_dynamic != s_the_mempool.config.mode)
    {
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_wrongusage;
        errdes.par16            = 0;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;         
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_mempool_Realloc() used when non dyn", s_eobj_ownname, &errdes);
        
        return(NULL);
    }            
    
    if(NULL != m)
    {
        s_the_mempool.stats.usedbytesheap -= eo_common_msize(m); 
    }    
    
    ret = s_the_mempool.theheap.reallocate(m, size);
    
    if(NULL == ret)
    {   // manage the fatal error in case memory could not be achieved
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_missing;
        errdes.par16            = size;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;         
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "eo_mempool_Realloc() no more memory", s_eobj_ownname, &errdes);
    }
    
    s_the_mempool.stats.usedbytesheap += eo_common_msize(ret);  
    
    return(ret);   
}


extern void eo_mempool_Delete(EOtheMemoryPool *p, void *m)
{
    if(NULL == m)
    {
        return;
    }
    
    if(eo_mempool_alloc_dynamic != s_the_mempool.config.mode)
    {        
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_warning, "eo_mempool_Delete(): only w/ eo_mempool_alloc_dynamic", s_eobj_ownname, &eo_errman_DescrWrongUsageLocal);       
        return;
    }        
        
    s_the_mempool.stats.usedbytesheap -= eo_common_msize(m); 

    s_the_mempool.theheap.release(m);          
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static void * s_eo_mempool_get_static(eOmempool_alignment_t alignmode, uint16_t size, uint16_t number, uint32_t* usedbytes)
{
    uint32_t numentries = 0;
    void *ret = NULL;
    
    eOmempool_the_pool_t* pool = &s_the_mempool.thepool;

    // attempt to lock mutex if it is null it will return nok_nullpointer, else ok or nok_timeout.
    if(eores_NOK_timeout == eov_mutex_Take(s_the_mempool.mutex, s_the_mempool.tout))
    {
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_mutex_timeout;
        errdes.par16            = s_the_mempool.tout / 1000;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "s_eo_mempool_get_static(): mutex_take() tout", s_eobj_ownname, &errdes);
    }
     
    
    switch(alignmode) 
    {    
        case eo_mempool_align_08bit:
        {  
            numentries = number * size;
            if((uint32_t)(pool->status.uint08index + numentries) <= (pool->config.size08)) 
            {
                ret = (void*) &pool->config.data08[pool->status.uint08index];
                pool->status.uint08index += numentries;
                *usedbytes = numentries;
            }

        } break;

        case eo_mempool_align_16bit:
        {
            numentries = number * ((size+1)/2);
            if((uint32_t)(pool->status.uint16index + numentries) <= (pool->config.size16/2)) 
            {
                ret = (void*) &pool->config.data16[pool->status.uint16index];
                pool->status.uint16index += numentries;
                *usedbytes = numentries*2;
            }            

        } break;
        
        case eo_mempool_align_32bit:
        {
            numentries = number * ((size+3)/4);
            if((uint32_t)(pool->status.uint32index + numentries) <= (pool->config.size32/4)) 
            {
                ret = (void*) &pool->config.data32[pool->status.uint32index];
                pool->status.uint32index += numentries;
                *usedbytes = numentries*4;
            }   
        } break;

        case eo_mempool_align_auto:
        case eo_mempool_align_64bit:
        {
            numentries = number * ((size+7)/8);
            if((uint32_t)(pool->status.uint64index + numentries) <= (pool->config.size64/8)) 
            {
                ret = (void*) &pool->config.data64[pool->status.uint64index];
                pool->status.uint64index += numentries;
                *usedbytes = numentries*8;
            }  
        } break;
       
        default:
        {
            ret = NULL;
        } break;    
        
    }
    
    // unlock mutex. it is safe even if mutex is null.
    eov_mutex_Release(s_the_mempool.mutex);
        
    if(NULL == ret) 
    {   // manage error
        eOerrmanDescriptor_t errdes = {0};
        errdes.code             = eo_errman_code_sys_memory_missing;
        errdes.par16            = size*number;
        errdes.sourcedevice     = eo_errman_sourcedevice_localboard;
        errdes.sourceaddress    = 0;
        eo_errman_Error(eo_errman_GetHandle(), eo_errortype_fatal, "s_eo_mempool_get_static(): no more mem", s_eobj_ownname, &errdes);
    }
    
    return(ret);
}

static void * s_memallocator(uint32_t s)
{
    return(calloc(s, 1));
}


static void s_memfree(void *p)
{
    free(p);
}


static void * s_memrealloc(void *p, uint32_t s)
{
    return(realloc(p, s));
}

//static size_t s_eo_mempool_heap_sizeof_allocated_pointer(void* p)
//{   // not sure it is portable on 64 bit architectures.
//    size_t* xx = (size_t*)p;
//    xx --;
//    size_t usedbytes = *xx; 
//    return(usedbytes);    
//}

//static uint16_t s_align_size(eOmempool_alignment_t alignmode, uint16_t size)
//{
//    switch(alignmode)
//    {   
//        case eo_mempool_align_08bit: {} break;
//        case eo_mempool_align_16bit: { size++;  size>>=1; size<<=1;} break;    
//        case eo_mempool_align_32bit: { size+=3; size>>=2; size<<=2;} break;      
//        case eo_mempool_align_64bit: { size+=7; size>>=3; size<<=3;} break;
//        default:
//        case eo_mempool_align_auto:  {} break;            
//    } 
//
//    return(size);
//}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------






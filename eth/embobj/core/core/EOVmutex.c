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

#include "EOVmutex.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOVmutex_hid.h" 


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

static const char s_eobj_ownname[] = "EOVmutex";



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------



extern eOresult_t eov_mutex_Take(EOVmutexDerived *d, eOreltime_t tout) 
{   
    EOVmutex *mutex;
    eOres_fp_voidp_uint32_t fptr;

    
    mutex = (EOVmutex*) eo_common_getbaseobject(d);
	
	if(NULL == mutex) 
	{
		return(eores_NOK_nullpointer); 
	}

    // get take function
    fptr = (eOres_fp_voidp_uint32_t)mutex->vtable[VF00_take]; 
	
//  if(NULL == fptr)
//	{
//		return(eores_NOK_nullpointer);     
//	}

    // call funtion of derived object. it cant be NULL
    return(fptr(d, tout));
}


extern eOresult_t eov_mutex_Release(EOVmutexDerived *d) 
{
	EOVmutex *mutex;
    eOres_fp_voidp_t fptr;
    
    mutex = (EOVmutex*) eo_common_getbaseobject(d);

	if(NULL == mutex) 
	{
		return(eores_NOK_nullpointer); 
	}

    // get release function
    fptr = (eOres_fp_voidp_t)mutex->vtable[VF01_release]; 
	
//  if(NULL == fptr)
//	{
//		return(eores_NOK_nullpointer);     
//	}

    // call funtion of derived object. it cant be NULL
    return(fptr(d));
	
}

extern void eov_mutex_Delete(EOVmutexDerived *d) 
{
	EOVmutex *mutex;
    eOres_fp_voidp_t fptr;
    
    mutex = (EOVmutex*) eo_common_getbaseobject(d);

	if(NULL == mutex) 
	{
		return; 
	}

    // get release function
    fptr = (eOres_fp_voidp_t)mutex->vtable[VF02_delete]; 
	
//  if(NULL == fptr)
//	{
//		return;     
//	}

    // call funtion of derived object. it cant be NULL
    fptr(d);
    return;	
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


extern EOVmutex* eov_mutex_hid_New(void) 
{
	EOVmutex *retptr = NULL;	

	// i get the memory for the object
	retptr = (EOVmutex*) eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOVmutex), 1);

	// now the obj has valid memory. i need to initialise it with user-defined data
    
    // vtable
    retptr->vtable[VF00_take]           = NULL;
    retptr->vtable[VF01_release]        = NULL;
    retptr->vtable[VF02_delete]         = NULL;
    // other stuff


	return(retptr);	
}


extern void eov_mutex_hid_Delete(EOVmutex *p) 
{
	if(NULL == p)
    {
        return;
    }

    memset(p, 0, sizeof(EOVmutex));
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;
}


extern eOresult_t eov_mutex_hid_SetVTABLE(EOVmutex *p, eOres_fp_voidp_uint32_t v_take, eOres_fp_voidp_t v_release, eOres_fp_voidp_t v_delete)
{
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != v_take), "eov_mutex_hid_SetVTABLE(): NULL v_take", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != v_release), "eov_mutex_hid_SetVTABLE(): NULL v_release", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);
    eo_errman_Assert(eo_errman_GetHandle(), (NULL != v_delete), "eov_mutex_hid_SetVTABLE(): NULL v_delete", s_eobj_ownname, &eo_errman_DescrWrongParamLocal);

    p->vtable[VF00_take]            = (void*) v_take;
    p->vtable[VF01_release]         = (void*) v_release;
    p->vtable[VF02_delete]          = (void*) v_delete;

    return(eores_OK);
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty section





// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





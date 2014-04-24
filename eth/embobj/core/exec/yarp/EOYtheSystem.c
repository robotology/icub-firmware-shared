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

#define EOY_SYS_USE_FEATURE_INTERFACE
#undef EOY_SYS_USE_FEATURE_INTERFACE

#include "stdlib.h"
#include "EoCommon.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

#if     defined(EOY_SYS_USE_FEATURE_INTERFACE)
#include "FeatureInterface.h"
#endif


#include "EOtheErrorManager.h"
#include "EOVtheSystem_hid.h" 

#if     !defined(EOY_SYS_USE_FEATURE_INTERFACE)
    #warning  marco.accame on 24 april 2014: remember to test w/ EOY_SYS_USE_FEATURE_INTERFACE defined so that te code is portable w/ YARP
#endif



#if     !defined(EOY_SYS_USE_FEATURE_INTERFACE)
  #if   defined(EO_TAILOR_CODE_FOR_LINUX)
  #include <time.h>
  #include <sys/time.h>
  #endif
#endif


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOYtheSystem.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOYtheSystem_hid.h" 


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

static eOresult_t s_eoy_sys_start(void (*init_fn)(void));

static void s_eoy_thecreation(void);

static EOVtaskDerived* s_eoy_sys_gettask(void);

static eOabstime_t s_eoy_sys_abstime_get(void);
static void s_eoy_sys_abstime_set(eOabstime_t time);
static eOnanotime_t s_eoy_sys_nanotime_get(void);
static void s_eoy_sys_stop(void);

#if     !defined(EOY_SYS_USE_FEATURE_INTERFACE)
#if   defined(EO_TAILOR_CODE_FOR_LINUX)
static int s_timeval_subtract(struct timespec *_result, struct timespec *_x, struct timespec *_y);
#endif
#endif

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------


static const char s_eobj_ownname[] = "EOYtheSystem";

static const eOysystem_cfg_t s_eoy_sys_defaultconfig = 
{
    EO_INIT(nothing)        0
};

static EOYtheSystem s_eoy_system = 
{
    EO_INIT(thevsys)        NULL,               
    EO_INIT(user_init_fn)   NULL,
    EO_INIT(.start)         0
};

#if     !defined(EOY_SYS_USE_FEATURE_INTERFACE)
#if   defined(EO_TAILOR_CODE_FOR_LINUX)
static struct timespec s_eoy_sys_linux_start_time;
#endif
#endif

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

extern EOYtheSystem * eoy_sys_Initialise(const eOysystem_cfg_t *syscfg, 
                                         const eOmempool_cfg_t *mpoolcfg, 
                                         const eOerrman_cfg_t *errmancfg)
{

    if(NULL != s_eoy_system.thevsys) 
    {
        // already initialised
        return(&s_eoy_system);
    }

    if(NULL == syscfg)
    {
        syscfg = &s_eoy_sys_defaultconfig;
    }
   
    // mempool and error manager initialised inside here.
    s_eoy_system.thevsys = eov_sys_hid_Initialise(mpoolcfg,
                                                  errmancfg,        // error man 
                                                  (eOres_fp_voidfpvoid_t)s_eoy_sys_start, s_eoy_sys_gettask, 
                                                  s_eoy_sys_abstime_get, s_eoy_sys_abstime_set, 
                                                  (eOuint64_fp_void_t)s_eoy_sys_nanotime_get,
                                                  s_eoy_sys_stop);


    // initialise y-environment
    
#if     defined(EOY_SYS_USE_FEATURE_INTERFACE) 
    s_eoy_system.start = feat_yarp_time_now();
#else

#if   defined(EO_TAILOR_CODE_FOR_LINUX)
  clock_gettime(CLOCK_REALTIME, &s_eoy_sys_linux_start_time);
#endif

#endif

    return(&s_eoy_system);  
}


extern EOYtheSystem* eoy_sys_GetHandle(void)
{
    if(NULL == s_eoy_system.thevsys)
    {
        return(NULL);
    }

    return(&s_eoy_system);
}    


extern void eoy_sys_Start(EOYtheSystem *p, eOvoid_fp_void_t userinit_fn)
{
    s_eoy_sys_start(userinit_fn);
}

extern eOabstime_t eoy_sys_abstime_get(EOYtheSystem *p)
{
	if(p==NULL)
	{
		return(0);
	}

	return(s_eoy_sys_abstime_get());
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

static eOresult_t s_eoy_sys_start(eOvoid_fp_void_t userinit_fn)
{

    s_eoy_system.user_init_fn = userinit_fn;
    s_eoy_thecreation();
 
    return(eores_OK);
}


static void s_eoy_thecreation(void)
{
    // run user defined initialisation ...
    if(NULL != s_eoy_system.user_init_fn)
    {
        s_eoy_system.user_init_fn();
    }
}


static EOVtaskDerived* s_eoy_sys_gettask(void)
{
    return(NULL);
}

static eOabstime_t s_eoy_sys_abstime_get(void)
{
    eOabstime_t time = 0xEABABABABABABABF;

#if     defined(EOY_SYS_USE_FEATURE_INTERFACE)

    double delta = feat_yarp_time_now() - s_eoy_system.start;
    delta *= (1e6);
    time = (eOabstime_t)floor(delta);

#else//defined(EOY_SYS_USE_FEATURE_INTERFACE)

    eOabstime_t time = 0xEABABABABABABABF;
#if   defined(EO_TAILOR_CODE_FOR_LINUX)
    struct timespec curr_time, diff;
    clock_gettime(CLOCK_REALTIME, &curr_time);
    s_timeval_subtract(&diff, &curr_time, &s_eoy_sys_linux_start_time);
    time = ((diff.tv_sec * 1000 * 1000) + (diff.tv_nsec/1000));
#endif

#endif//defined(EOY_SYS_USE_FEATURE_INTERFACE)

    return(time);
}


static void s_eoy_sys_abstime_set(eOabstime_t time)
{
#if     defined(EOY_SYS_USE_FEATURE_INTERFACE)
    s_eoy_system.start = ((double) time)/ 1e6;
#else
    // do nothing ...
#endif
}


static eOnanotime_t s_eoy_sys_nanotime_get(void)
{
    eOnanotime_t nanotime = 0;

#if     defined(EOY_SYS_USE_FEATURE_INTERFACE)
    double delta = feat_yarp_time_now() - s_eoy_system.start;
    delta *= 1e9;
    nanotime = (eOnanotime_t)floor(delta);
#endif

    return(nanotime);
}

static void s_eoy_sys_stop(void)
{
    // do nothing
}



#if     !defined(EOY_SYS_USE_FEATURE_INTERFACE)

#if   defined(EO_TAILOR_CODE_FOR_LINUX)
// risultato= stop - start, cioè _result = _x - _y
static int s_timeval_subtract(struct timespec *_result, struct timespec *_x, struct timespec *_y)
{
	//difftime();
	/* Perform the carry for the later subtraction by updating y. */
	static const long int MULTIPLIER = 1000 * 1000 * 1000;

	if(_x->tv_nsec < _y->tv_nsec)
	{
		int nsec = (_y->tv_nsec - _x->tv_nsec) / MULTIPLIER + 1;

		_y->tv_nsec -= MULTIPLIER * nsec;
		_y->tv_sec += nsec;
	}

	if(_x->tv_nsec - _y->tv_nsec > MULTIPLIER)
	{
		int nsec = (_x->tv_nsec - _y->tv_nsec) / MULTIPLIER;

		_y->tv_nsec += MULTIPLIER * nsec;
		_y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait. tv_usec is certainly positive. */

	_result->tv_sec = _x->tv_sec - _y->tv_sec;
	_result->tv_nsec = _x->tv_nsec - _y->tv_nsec;

	/* Return 1 if result is negative. */

	return _x->tv_sec < _y->tv_sec;
}
#endif

#endif//!defined(EOY_SYS_USE_FEATURE_INTERFACE)


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------





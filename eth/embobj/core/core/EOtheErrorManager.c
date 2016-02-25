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
#include "EOVtheSystem.h"
#include "EOVtask.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheErrorManager.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOtheErrorManager_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

//#warning marco.accame: think of removing eOerrmanCode_t and use only eoerror_code_get(). OR ... be sure its values are the same as those in EoError.h 

const eOerrman_cfg_t eo_errman_DefaultCfg = 
{
    EO_INIT(.extfn)
    { 
        EO_INIT(.usr_on_error)      NULL
    }
};


const eOerrmanDescriptor_t eo_errman_DescrUnspecified = 
{
    EO_INIT(.code)          eo_errman_code_sys_unspecified,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrTobedecided = 
{
    EO_INIT(.code)          eo_errman_code_sys_tobedecided,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrWrongParamLocal = 
{
    EO_INIT(.code)          eo_errman_code_sys_wrongparam,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrWrongUsageLocal = 
{
    EO_INIT(.code)          eo_errman_code_sys_wrongusage,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrRuntimeErrorLocal = 
{
    EO_INIT(.code)          eo_errman_code_sys_runtimeerror,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrRunningHappily = 
{
    EO_INIT(.code)          eo_errman_code_sys_runninghappily,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};

const eOerrmanDescriptor_t eo_errman_DescrRuntimeFatalErrorState = 
{
    EO_INIT(.code)          eo_errman_code_sys_runninginfatalerrorstate,
    EO_INIT(.sourcedevice)  eo_errman_sourcedevice_localboard,
    EO_INIT(.sourceaddress) 0,
    EO_INIT(.par16)         0,
    EO_INIT(.par64)         0
};


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static void s_eo_errman_OnError(eOerrmanErrorType_t errtype, const char *info, eOerrmanCaller_t *caller, const eOerrmanDescriptor_t *des);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOtheErrorManager";

 
static EOtheErrorManager s_errman_singleton = 
{
	EO_INIT(.cfg) 
    {   
        EO_INIT(.extfn.usr_on_error)    NULL
    },
    EO_INIT(.errorstrings)
    {
        "INFO", 
        "DEBUG", 
        "WARNING", 
        "ERROR", 
        "FATAL"
    }
};


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern EOtheErrorManager * eo_errman_Initialise(const eOerrman_cfg_t *errmancfg)
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    if(NULL == errmancfg)
    {
        errmancfg = &eo_errman_DefaultCfg;    
    }

    memcpy(&s_errman_singleton.cfg, errmancfg, sizeof(eOerrman_cfg_t));

    return(&s_errman_singleton);
#else
    return(NULL);
#endif
}


extern EOtheErrorManager* eo_errman_GetHandle(void) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN	
    return(&s_errman_singleton);
#else
    return(NULL);
#endif
}


extern const char* eo_errman_ErrorStringGet(EOtheErrorManager *p, eOerrmanErrorType_t errtype) 
{
    return(s_errman_singleton.errorstrings[(uint8_t)errtype]);
}


extern eObool_t eo_errman_IsErrorHandlerConfigured(EOtheErrorManager *p) 
{
    return((NULL == s_errman_singleton.cfg.extfn.usr_on_error) ? (eobool_false) : (eobool_true));
}

extern void eo_errman_SetOnErrorHandler(EOtheErrorManager *p, eOerrman_fp_onerror_t onerrorhandler)
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN    
    s_errman_singleton.cfg.extfn.usr_on_error = onerrorhandler;
#else
    ;
#endif
}

 
extern void eo_errman_Assert(EOtheErrorManager *p, uint32_t cond, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN    
    if(0 == cond) 
    {
        eo_errman_Error(p, eo_errortype_fatal, info, eobjstr, des);
    }

#else
    ;
#endif
}


extern void eo_errman_Error(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    EOVtaskDerived *task = NULL;
    
    eOerrmanCaller_t caller = {0};

    caller.eobjstr = eobjstr;
    
    task = eov_sys_GetRunningTask(eov_sys_GetHandle());   
    
    if(NULL == task)
    {
        // we are in init task ...
        caller.taskid = 2;
    }
    else
    {
        caller.taskid = eov_task_GetID(task);
    }
    
    

    s_eo_errman_OnError(errtype, info, &caller, des);
#else
    ;
#endif
}


extern void eo_errman_Info(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    eo_errman_Error(p, eo_errortype_info, info, eobjstr, des);
#else
    ;
#endif
}


extern void eo_errman_Debug(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    eo_errman_Error(p, eo_errortype_debug, info, eobjstr, des);
#else
    ;
#endif
}


extern void eo_errman_Warning(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    eo_errman_Error(p, eo_errortype_warning, info, eobjstr, des);
#else
    ;
#endif
}


extern void eo_errman_Trace(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *info, const char *eobjstr) 
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    EOVtaskDerived *task = NULL;
    
    eOerrmanCaller_t caller = {0};

    caller.eobjstr = eobjstr;
    
    task = eov_sys_GetRunningTask(eov_sys_GetHandle());   
    
    if(NULL == task)
    {
        // we are in init task ...
        caller.taskid = 2;
    }
    else
    {
        caller.taskid = eov_task_GetID(task);
    }
    
    

    s_eo_errman_OnError(errtype, info, &caller, NULL);
#else
    ;
#endif
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

static void s_eo_errman_OnError(eOerrmanErrorType_t errtype, const char *info, eOerrmanCaller_t *caller, const eOerrmanDescriptor_t *des)
{
#ifndef EODEF_DONT_USE_THE_ERRORMAN
    if(NULL != s_errman_singleton.cfg.extfn.usr_on_error)
    {
        s_errman_singleton.cfg.extfn.usr_on_error(errtype, info, caller, des);
    }
    else
    {
        if(errtype <= eo_errortype_error)
        {
            return;
        }

        eov_sys_Stop(eov_sys_GetHandle());

        for(;;);
    }
#else
#endif
}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------






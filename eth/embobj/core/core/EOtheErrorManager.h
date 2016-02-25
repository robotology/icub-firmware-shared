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

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EOTHEERRORMANAGER_H_
#define _EOTHEERRORMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOtheErrorManager.h
	@brief      This header file implements public interface to the error manager singleton.
 	@author     marco.accame@iit.it
	@date       08/03/2011
 **/

/** @defgroup eo_theerrormanager Singleton EOtheErrorManager 
    
    The error manager singleton is used by the embOBJ to report errors and to enter in the appropriate error mode.
    This singleton can work in the SEE or MEE by means of some virtual objects: the EOVtheSystem and the EOVtask.  
  
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"


// - public #define  --------------------------------------------------------------------------------------------------

#define EO_ERRMAN_VERSION   2
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 

/** @typedef    typedef struct EOtheErrorManager_hid EOtheErrorManager
    @brief      EOtheErrorManager is an opaque struct. It is used to implement data abstraction for the error manager 
                object so that the user cannot see its private fields so that he/she is forced to manipulate the
                object only with the proper public functions
 **/  
typedef struct EOtheErrorManager_hid EOtheErrorManager;


/**	@typedef    typedef enum eOerrmanErrorType_t 
 	@brief      Contains the error types managed by the EOtheErrorManager 
 **/  
typedef enum  
{
    eo_errortype_info    = 0,       /**< used to communicate some innocent situation */
    eo_errortype_debug   = 1,       /**< used to communicate debugging situation */
    eo_errortype_warning = 2,       /**< used to communicate some strange situation */
    eo_errortype_error   = 3,       /**< used to communicate an error */
    eo_errortype_fatal   = 4        /**< used to communicate a fatal error which requires stopping the system */
} eOerrmanErrorType_t;

enum { eo_errortype_numberof = 5 };


/** @typedef    typedef struct eOerrmanCaller_t
    @brief      Contains information about the caller of the error handler
 **/ 
typedef struct
{
    eOid08_t        taskid;         /**< the id of the caller task */
    const char*     eobjstr;        /** the name of the caller object */
} eOerrmanCaller_t;


typedef enum 
{ 
    eo_errman_code_sys_unspecified              = 0,
    eo_errman_code_sys_tobedecided              = 1,
    eo_errman_code_sys_memory_zerorequested     = 2,    
    eo_errman_code_sys_memory_notinitialised    = 3,
    eo_errman_code_sys_memory_missing           = 4,    /**< param contains the requested bytes */
    eo_errman_code_sys_mutex_timeout            = 5,    /**< param contains the timeout expressed in milli-seconds */
    eo_errman_code_sys_wrongparam               = 6,    /**< param MAY ?? contain the param position */
    eo_errman_code_sys_wrongusage               = 7,
    eo_errman_code_sys_runtimeerror             = 8,
    eo_errman_code_sys_runninghappily           = 9,
    eo_errman_code_sys_runninginfatalerrorstate = 10,
    eo_errman_code_sys_ctrloop_execoverflowRX   = 11,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_execoverflowDO   = 12,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_execoverflowTX   = 13,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_udptxfailure     = 14, 
    eo_errman_code_sys_ropparsingerror          = 15,   /**< param contains the specific parsing error */
    eo_errman_code_sys_halerror                 = 16,   /**< param contains the specific ipal error */
    eo_errman_code_sys_osalerror                = 17,   /**< param contains the specific ipal error */
    eo_errman_code_sys_ipalerror                = 18,   /**< param contains the specific ipal error */     
} eOerrmanCode_t;

typedef enum 
{ 
    eo_errman_sourcedevice_localboard   = 0,
    eo_errman_sourcedevice_canbus1      = 1,
    eo_errman_sourcedevice_canbus2      = 2    
} eOerrmanSourceDevice_t;


/** @typedef    typedef struct eOerrmanDescriptor_t
    @brief      Contains description of the error which will be used by a particular user-defined error handler.
                This type is built so that it completes the other parameters passed to the user-defined error handler
                so that it can call the object EOtheInfoDispatcher which uses the eOmn_info_status_t type.
 **/ 
typedef struct
{
    uint32_t        code;           /* the system uses eOerrmanCode_t, other modules may use values defined in a particular table ... see eoerror_code_get() */
    uint8_t         sourcedevice;   /* use values in eOerrmanSourceDevice_t */
    uint8_t         sourceaddress;  /* the system uses 0, other modules may use other values ...  e.g., CAN addresses */   
    uint16_t        par16;          /* it must be 0 unless the code needs one particular param */
    uint64_t        par64;          /* it must be 0 unless the code needs one particular param */
} eOerrmanDescriptor_t;


typedef     void (*eOerrman_fp_onerror_t)(eOerrmanErrorType_t errtype, const char *info, eOerrmanCaller_t *caller, const eOerrmanDescriptor_t *des);


/** @typedef    typedef struct eOerrman_fn_cfg_t
    @brief      eOerrman_fn_cfg_t keeps pointers to functions of EOtheErrorManager whcih can be redefined by the user.
 **/
typedef struct
{
    /** When an error is detected, the error manager attempt to call this function.
        If not defined: if errtype is eo_errortype_weak or lower it returns control to the environment. Otherwise if 
        error is eo_errortype_fatal it stops the environment and it enters in a forever loop.
        If defined: it just calls the function and if the function returns it returns control to the caller. 
        Parameters are: the error type, a string with a more detailed info, optional pointer to the caller info, pointer to
        data which is specific of the  user-defined function 
    */ 
    void (*usr_on_error)(eOerrmanErrorType_t errtype, const char *info, eOerrmanCaller_t *caller, const eOerrmanDescriptor_t *des);
} eOerrman_fn_cfg_t;


/**	@typedef    typedef struct eOerrman_cfg_t 
 	@brief      Contains the configuration for the EOtheErrorManager. 
 **/
typedef struct
{
    eOerrman_fn_cfg_t    extfn;
} eOerrman_cfg_t;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------


extern const eOerrman_cfg_t eo_errman_DefaultCfg; // = {.extfn = { .usr_on_error = NULL}};


extern const eOerrmanDescriptor_t eo_errman_DescrUnspecified;

extern const eOerrmanDescriptor_t eo_errman_DescrTobedecided;

extern const eOerrmanDescriptor_t eo_errman_DescrWrongParamLocal;

extern const eOerrmanDescriptor_t eo_errman_DescrWrongUsageLocal;

extern const eOerrmanDescriptor_t eo_errman_DescrRuntimeErrorLocal;

extern const eOerrmanDescriptor_t eo_errman_DescrRunningHappily;

extern const eOerrmanDescriptor_t eo_errman_DescrRuntimeFatalErrorState;

//extern const eOerrmanDescriptor_t eo_errman_DescrConfigParamLocal;


// - declaration of extern public functions ---------------------------------------------------------------------------



/** @fn         extern EOtheErrorManager * eo_errman_Initialise(const eOerrman_cfg_t *errmancfg)
    @brief      Initialise the EOtheErrorManager singleton 
    @arg        errmancfg       The configuration. NULL is OK.
    @return     The handle to the EOtheErrorManager
 **/

extern EOtheErrorManager * eo_errman_Initialise(const eOerrman_cfg_t *errmancfg);


/** @fn         extern EOtheErrorManager * eo_errman_GetHandle(void)
    @brief      Retrieve the EOtheErrorManager singleton 
    @return     The handle to the EOtheErrorManager
 **/
extern EOtheErrorManager * eo_errman_GetHandle(void);


/** @fn         extern const char* eo_errman_ErrorStringGet(EOtheErrorManager *p, eOerrmanErrorType_t errtype) 
    @brief      It returns a string describing the error type in argument. 
    @param      p               The singleton
    @param      errtype         The error type.
    @return     The error string: INFO, DEBUG, WARNING, ERROR, FATAL 
 **/
extern const char* eo_errman_ErrorStringGet(EOtheErrorManager *p, eOerrmanErrorType_t errtype);


extern eObool_t eo_errman_IsErrorHandlerConfigured(EOtheErrorManager *p);

/** @fn         extern void eo_errman_SetOnErrorHandler(EOtheErrorManager *p, eOerrman_fp_onerror_t onerrorhandler) 
    @brief      It changes the error handler function. 
    @param      p               The singleton
    @param      onerrorhandler  the error handler. If NULL, then the default error handler is used which does nothing if severity 
                                is lower or equal to eo_errortype_error and it stops the system inside only if severity is eo_errortype_fatal.
 **/
extern void eo_errman_SetOnErrorHandler(EOtheErrorManager *p, eOerrman_fp_onerror_t onerrorhandler);
 
 
/** @fn         extern void eo_errman_Assert(EOtheErrorManager *p, uint32_t cond, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des)
    @brief      If the value of @e cond is 0, it calls the configured usr_on_error() with eo_errortype_fatal, 
                then stops the system using the eov_sys_Stop() function and finally enters in a forever loop.
    @param      p               The singleton
    @param      cond            The condition to be tested
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Assert(EOtheErrorManager *p, uint32_t cond, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


/** @fn         extern void eo_errman_Error(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
    @brief      It calls the configured usr_on_error() with the passed errtype, and if it is a fatal error it also
                stops the system using the eov_sys_Stop() function and finally enters in a forever loop. 
    @param      p               The singleton
    @param      errtype         The error type.
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Error(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


/** @fn         extern void eo_errman_Info(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
    @brief      It calls eo_errman_Error(p, eo_errortype_info, info, eobjstr, par). 
    @param      p               The singleton
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Info(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


/** @fn         extern void eo_errman_Debug(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
    @brief      It calls eo_errman_Error(p, eo_errortype_debug, eobjstr, info). 
    @param      p               The singleton
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Debug(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


/** @fn         extern void eo_errman_Warning(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
    @brief      It calls eo_errman_Error(p, eo_errortype_warning, eobjstr, info). 
    @param      p               The singleton
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Warning(EOtheErrorManager *p, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


extern void eo_errman_Trace(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *info, const char *eobjstr);

/** @}            
    end of group eo_theerrormanager  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


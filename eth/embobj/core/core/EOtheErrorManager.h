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
// empty-section
  

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
    eo_errortype_warning = 1,       /**< used to communicate some strange situation */
    eo_errortype_weak    = 2,       /**< used to communicate a weak error which could be recovered by user intervention */
    eo_errortype_fatal   = 3        /**< used to communicate a fatal error which requires stopping the system */
} eOerrmanErrorType_t;


typedef     void (*eOerrman_fp_onerror_t)(eOerrmanErrorType_t errtype, eOid08_t taskid, const char *eobjstr, const char *info);


/** @typedef    typedef struct eOerrman_fn_cfg_t
    @brief      eOerrman_fn_cfg_t keeps pointers to functions of EOtheErrorManager whcih can be redefined by the user.
 **/
typedef struct
{
    /** When an error is detected, the error manager attempt to call this function.
        If not defined: if errtype is eo_errortype_weak or lower it returns control to the environment. Otherwise if 
        error is eo_errortype_fatal it stops the environment and it enters in a forever loop.
        If defined: it just calls the function and if the function returns it returns control to teh caller. 
        Parameters are: the error type, the id of the calling task, the name of the calling embOBJ, and a string with a
        more detailed info */ 
    void            (*usr_on_error)(eOerrmanErrorType_t errtype, eOid08_t taskid, const char *eobjstr, const char *info);
} eOerrman_fn_cfg_t;

/**	@typedef    typedef struct eOerrman_cfg_t 
 	@brief      Contains the configuration for the EOtheErrorManager. 
 **/
typedef struct
{
    eOerrman_fn_cfg_t    extfn;
} eOerrman_cfg_t;


// marco.accame: in here are some temporary types that can be used to write code compatible with the new error manager 
// begin
    
/**	@typedef    typedef enum eOerrmanErrorType2_t 
 	@brief      Contains the error types managed by the EOtheErrorManager 
 **/  
typedef enum  
{
    eo_errortype2_info    = 0,       /**< used to communicate some innocent situation */
    eo_errortype2_debug   = 1,       /**< used to communicate debugging situation */
    eo_errortype2_warning = 2,       /**< used to communicate some strange situation */
    eo_errortype2_error   = 3,       /**< used to communicate an error */
    eo_errortype2_fatal   = 4        /**< used to communicate a fatal error which requires stopping the system */
} eOerrmanErrorType2_t;

enum { eo_errortype2_numberof = 5 };


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
    eo_errman_code_sys_ctrloop_execoverflowRX   = 10,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_execoverflowDO   = 11,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_execoverflowTX   = 12,   /**< param contains the duration ??? */ // meglio non tx stringa
    eo_errman_code_sys_ctrloop_udptxfailure     = 13, 
    eo_errman_code_sys_ropparsingerror          = 14,   /**< param contains the specific parsing error */
    eo_errman_code_sys_halerror                 = 15,   /**< param contains the specific ipal error */
    eo_errman_code_sys_osalerror                = 16,   /**< param contains the specific ipal error */
    eo_errman_code_sys_ipalerror                = 17,   /**< param contains the specific ipal error */    
} eOerrmanCode_t;

typedef enum 
{ 
    eo_errman_sourcedevice_localboard = 0 
} eOerrmanSourceDevice_t;


/** @typedef    typedef struct eOerrmanDescriptor_t
    @brief      Contains description of the error which will be used by a particular user-defined error handler.
                This type is built so that it completes the other parameters passed to the user-defined error handler
                so that it can call the object EOtheInfoDispatcher which uses the eOmn_info_status_t type.
 **/ 
typedef struct
{
    uint32_t        code;           /* the system uses eOerrmanCode_t, other modules may use values defined in a particular table ... see eoerror_code_get() */
    uint16_t        param;          /* it must be 0 unless the code needs one particular param */
    uint8_t         sourcedevice;   /* the system uses eo_errman_sourcedevice_localboard, others modules may use values defined in a particular enum ... see eOmn_info_source_t */
    uint8_t         sourceaddress;  /* the system uses 0, other modules may use other values ...  e.g., CAN addresses */   
} eOerrmanDescriptor_t;


/** @fn         extern void eo_errman_Error2(EOtheErrorManager *p, eOerrmanErrorType2_t errtype, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des) 
    @brief      It calls the configured usr_on_error() with the passed errtype, and if it is a fatal error it also
                stops the system using the eov_sys_Stop() function and finally enters in a forever loop. 
    @param      p               The singleton
    @param      errtype         The error type.
    @param      info            A string containing a specific message from the calling object.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      par             An optional parameter used by the user-defined usr_on_error()
 **/
extern void eo_errman_Error2(EOtheErrorManager *p, eOerrmanErrorType2_t errtype, const char *info, const char *eobjstr, const eOerrmanDescriptor_t *des);


// end   

 
    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOerrman_cfg_t eom_errman_DefaultCfg; // = {.extfn = { .usr_on_error = NULL}};


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


extern void eo_errman_SetOnErrorHandler(EOtheErrorManager *p, eOerrman_fp_onerror_t onerrorhandler);
 
 
/** @fn         extern void eo_errman_Assert(EOtheErrorManager *p, uint32_t cond, const char *eobjstr,
                                            const char *info)
    @brief      If the value of @e cond is 0, it calls the configured usr_on_error() with eo_errortype_fatal, 
                then stops the system using the eov_sys_Stop() function and finally enters in a forever loop.
    @param      p               The singleton
    @param      condition       The condition to be tested
    @param      eobjstr         A string containing the name of the calling object. 
    @param      info            A string containing a specific message from the calling object.
 **/
extern void eo_errman_Assert(EOtheErrorManager *p, uint32_t cond, const char *eobjstr, const char *info);


/** @fn         extern void eo_errman_Error(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *eobjstr, 
                                            const char *info) 
    @brief      It calls the configured usr_on_error() with the passed errtype, and if it is a fatal error it also
                 stops the system using the eov_sys_Stop() function and finally enters in a forever loop. 
    @param      p               The singleton
    @param      errtype         The error type.
    @param      eobjstr         A string containing the name of the calling object. 
    @param      info            A string containing a specific message from the calling object.
 **/

extern void eo_errman_Error(EOtheErrorManager *p, eOerrmanErrorType_t errtype, const char *eobjstr, const char *info);


/** @fn         extern void eo_errman_Info(EOtheErrorManager *p, const char *eobjstr, const char *info) 
    @brief      It calls eo_errman_Error(p, eo_errortype_info, eobjstr, info). 
    @param      p               The singleton
    @param      eobjstr         A string containing the name of the calling object. 
    @param      info            A string containing a specific message from the calling object.
 **/
extern void eo_errman_Info(EOtheErrorManager *p, const char *eobjstr, const char *info);







/** @}            
    end of group eo_theerrormanager  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


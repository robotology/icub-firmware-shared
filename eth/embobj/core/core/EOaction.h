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
// -
// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EOACTION_H_
#define _EOACTION_H_

#ifdef __cplusplus
extern "C" {
#endif
 
 
/** @file       EOaction.h
    @brief      This header file implements public interface to a generic action.
    @author     marco.accame@iit.it
    @date       08/03/2011
**/

/** @defgroup eo_action Object EOaction
    The EOaction object is used to contain an action and its parameters. The action can be the sending of
    a message or of an event to a task, or even the request to a task to execute a callback.
     
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOVtask.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 

/** @typedef    typedef enum eOactiontype_t
    @brief      eOactiontype_t contains the types of an action
 **/
typedef enum  
{
    eo_actypeNONE = 0,                  /**< No action */
    eo_actypeEvent,                     /**< The action is an event eOevent_t to be sent to a task-derived object */
    eo_actypeMessage,                   /**< The action is a message eOmessage_t be sent to a task-derived object */
    eo_actypeCallback,                  /**< The action is a callback function eOcallback_t to be executed by a proper task */
} eOactiontype_t;


/** @typedef    typedef struct EOaction_hid EOaction
    @brief      EOaction is an opaque struct. It is used to implement data abstraction for the action 
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOaction_hid EOaction;


enum { EOaction_sizeof = 32 };
typedef uint64_t EOaction_strg[EOaction_sizeof/sizeof(uint64_t)];

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
 
 
/** @fn         extern EOaction* eo_action_New(void)
    @brief      Creates a new action object. 
    @return     The pointer to the required object.
 **/
extern EOaction* eo_action_New(void);


/** @fn         externvoid eo_action_Delete(EOaction *p)
    @brief      deletes a EOaction object. 
    @param      p               The target object
 **/
extern void eo_action_Delete(EOaction *p);


/** @fn         extern eOresult_t eo_action_Clear(EOaction *p)
    @brief      Clears a given EOaction object. 
    @param      p               The target object
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_OK otherwise.
 **/
extern eOresult_t eo_action_Clear(EOaction *p);

extern eObool_t eo_action_Isvalid(EOaction *p);


/** @fn         extern eOresult_t eo_action_Copy(EOaction *p, const EOaction *src)
    @brief      Copies the action @e src into a given EOaction object. 
    @param      p               The target object
    @param      src             The source object
    @return     The value eores_NOK_nullpointer if any argument is NULL, eores_OK otherwise.
 **/
extern eOresult_t eo_action_Copy(EOaction *p, const EOaction *src);


/** @fn         extern eOresult_t eo_action_SetEvent(EOaction *p, eOevent_t event, EOVtaskDerived *totask)
    @brief      Configure the EOaction object to contain an event. 
    @param      p               The target object
    @param      event           The event to be sent
    @param      totask          The task-derived object to which the event is to be sent
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_OK otherwise.
 **/
extern eOresult_t eo_action_SetEvent(EOaction *p, eOevent_t event, EOVtaskDerived *totask);


/** @fn         extern eOresult_t eo_action_SetMessage(EOaction *p, eOmessage_t message, EOVtaskDerived *totask)
    @brief      Configure the EOaction object to contain a message. 
    @param      p               The target object
    @param      message         The message to be sent
    @param      totask          The task-derived object to which the message is to be sent
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_OK otherwise.
 **/
extern eOresult_t eo_action_SetMessage(EOaction *p, eOmessage_t message, EOVtaskDerived *totask);


/** @fn         extern eOresult_t eo_action_SetCallback(EOaction *p, eOcallback_t callback, EOVtaskDerived *exectask)
    @brief      Configure the EOaction object to contain a callback. The execution of the callback is responsibility
                of a proper callback manager (such as EOMtheCallbackManager or EOStheCallbackManager)
    @param      p               The target object
    @param      callback        The callback to execute
    @param      arg             The param of the callback
    @param      exectask        The task which will execute the callback = {NULL, eov_callbackman_GetTask(eov_callbackman_GetHandle()), pointer to a task of type eom_mtask_CallbackDriven}
                                If NULL, the callback will be executed by the task which calls eo_action_Execute(). If used inside a EOtimer the task will be the EOtheTimerManager.
                                Otherwise: the caller of eo_action_Execute() will send a request to the default callback manager or to any other task of type eom_mtask_CallbackDriven.
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_OK otherwise.
 **/
extern eOresult_t eo_action_SetCallback(EOaction *p, eOcallback_t callback, void *arg, EOVtaskDerived *exectask);


/** @fn         extern eOactiontype_t eo_action_GetType(EOaction *p)
    @brief      Get the type of action
    @param      p               The target object
    @return     The type.
 **/
extern eOactiontype_t eo_action_GetType(EOaction *p);


/** @fn         extern eOresult_t eo_action_GetEvent(EOaction *p, eOevent_t *event, EOVtaskDerived **totask)
    @brief      Retrieve the event in the the EOaction object. 
    @param      p               The target object
    @param      event           The event to be sent
    @param      totask          The task-derived object to which the event is to be sent
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_NOK_generic if the object does not contain 
                an event, eores_OK upon success.
 **/
extern eOresult_t eo_action_GetEvent(EOaction *p, eOevent_t *event, EOVtaskDerived **totask);


/** @fn         extern eOresult_t eo_action_GetMessage(EOaction *p, eOmessage_t *message, EOVtaskDerived **totask)
    @brief      Retrieve the message in the the EOaction object. 
    @param      p               The target object
    @param      message         The message to be sent
    @param      totask          The task-derived object to which the event is to be sent
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_NOK_generic if the object does not contain 
                a message, eores_OK upon success.
 **/
extern eOresult_t eo_action_GetMessage(EOaction *p, eOmessage_t *message, EOVtaskDerived **totask);


/** @fn         extern eOresult_t eo_action_GetCallback(EOaction *p, eOcallback_t *callback, EOVtaskDerived **exectask)
    @brief      Retrieve the callback in the the EOaction object. 
    @param      p               The target object
    @param      callback        The callback
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_NOK_generic if the object does not contain 
                a callback, eores_OK upon success.
 **/
extern eOresult_t eo_action_GetCallback(EOaction *p, eOcallback_t *callback, void **arg, EOVtaskDerived **exectask);


/** @fn         extern eOresult_t eo_action_Execute(EOaction *act, eOreltime_t tout)
    @brief      Executes the action in @e act. For ist execution are given proper execution functions 
    @param      p               The target object
    @param      tout            The timeout used for sending a message or request a callback. 
    @return     The value eores_NOK_nullpointer if @e p is NULL, eores_NOK_generic if the object does not contain 
                the resource, eores_OK upon success.
 **/
extern eOresult_t eo_action_Execute(EOaction *act, eOreltime_t tout);


/** @}            
    end of group eo_action  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


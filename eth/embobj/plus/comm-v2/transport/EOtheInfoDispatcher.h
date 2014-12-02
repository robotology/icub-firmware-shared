/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
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
#ifndef _EOTHEINFODISPATCHER_H_
#define _EOTHEINFODISPATCHER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOtheInfoDispatcher.h
    @brief      This header file implements public interface to info dispatcher
    @author     marco.accame@iit.it
    @date       Nov 04 2014
**/

/** @defgroup eo_ecvrevrebvtr2342rd Object EOtheInfoDispatcher
    The EOtheInfoDispatcher is a singleton .....
      
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoManagement.h"
#include "EOtransmitter.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section

// - declaration of public user-defined types ------------------------------------------------------------------------- 



/** @typedef    typedef struct EOtheInfoDispatcher_hid EOtheInfoDispatcher
    @brief      EOtheInfoDispatcher is an opaque struct. It is used to implement data abstraction for the dispatcher  
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOtheInfoDispatcher_hid EOtheInfoDispatcher;


typedef struct
{
    eOsizecntnr_t       capacity;
    EOtransmitter*      transmitter;
} eOinfodispatcher_cfg_t;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOinfodispatcher_cfg_t eo_infodispatcher_cfg_default; 


// - declaration of extern public functions ---------------------------------------------------------------------------
 
 
 
/** @fn         extern EOtheInfoDispatcher * eo_infodispatcher_Initialise(void)
    @brief      Initialise the singleton EOtheInfoDispatcher. 
    @param      cfg         Contains configuration for the object.
                            If NULL, then the eo_infodispatcher_cfg_default is used.
    @return     A valid and not-NULL pointer to the EOtheInfoDispatcher singleton.
 **/
extern EOtheInfoDispatcher * eo_infodispatcher_Initialise(const eOinfodispatcher_cfg_t *cfg);


/** @fn         extern EOtheInfoDispatcher * eo_infodispatcher_GetHandle(void)
    @brief      Gets the handle of the EOtheInfoDispatcher singleton 
    @return     Constant pointer to the singleton.
 **/
extern EOtheInfoDispatcher * eo_infodispatcher_GetHandle(void);



extern eOresult_t eo_infodispatcher_Put(EOtheInfoDispatcher* p, eOmn_info_properties_t* props, const char* extra);

// use  .... if you want try sending them all
enum { eoinfodispatcher_sendnumber_all = EOK_uint16dummy };

extern eOresult_t eo_infodispatcher_Send(EOtheInfoDispatcher* p, uint16_t number, uint16_t* numberofsent);



/** @}            
    end of group eo_ecvrevrebvtr2342rd  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




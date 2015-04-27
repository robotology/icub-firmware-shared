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
#ifndef _EODEVICETRANSCEIVER_H_
#define _EODEVICETRANSCEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOdeviceTransceiver.h
    @brief      This header file implements public interface to device transceiver
    @author     marco.accame@iit.it
    @date       09/06/2011
**/

/** @defgroup eo_ecvrevrebvtr2342r455 Object EOdeviceTransceiver
    The EOdeviceTransceiver is a singleton .....
      
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOtransceiver.h"
#include "EOproxy.h"
#include "EOnvSet.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section

// - declaration of public user-defined types ------------------------------------------------------------------------- 


typedef struct
{
    const eOnvset_BRDcfg_t*         nvsetbrdcfg;          
    eOipv4addr_t                    remotehostipv4addr;
    eOipv4port_t                    remotehostipv4port;
    eOtransceiver_sizes_t           sizes; 
    eov_mutex_fn_mutexderived_new   mutex_fn_new;    
    eOtransceiver_protection_t      transprotection;
    eOnvset_protection_t            nvsetprotection;
    eOproxy_cfg_t*                  proxycfg;
    eOtransceiver_extfn_t           extfn;
} eOdevicetransceiver_cfg_t;


/** @typedef    typedef struct EOdeviceTransceiver_hid EOdeviceTransceiver
    @brief      EOdeviceTransceiver is an opaque struct. It is used to implement data abstraction for the Parser  
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOdeviceTransceiver_hid EOdeviceTransceiver;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOdevicetransceiver_cfg_t eo_devicetransceiver_cfg_default; 


// - declaration of extern public functions ---------------------------------------------------------------------------
 
 
 
/** @fn         extern EOdeviceTransceiver * eo_devicetransceiver_Initialise(void)
    @brief      Initialise the singleton EOdeviceTransceiver. 
    @param      cfg         Contains actions to be done on reception or transmission which are specific to the application.
                            If NULL, then  is is issued a info by the EOtheErrorManager.
    @return     A valid and not-NULL pointer to the EOdeviceTransceiver singleton.
 **/
extern EOdeviceTransceiver * eo_devicetransceiver_New(const eOdevicetransceiver_cfg_t *cfg);


extern void eo_devicetransceiver_Delete(EOdeviceTransceiver* p);


extern EOtransceiver * eo_devicetransceiver_GetTransceiver(EOdeviceTransceiver* p);

extern EOnvSet * eo_devicetransceiver_GetNVset(EOdeviceTransceiver* p);

extern eOnvBRD_t eo_devicetransceiver_GetBoardNumber(EOdeviceTransceiver* p);


/** @}            
    end of group eo_ecvrevrebvtr2342r455  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




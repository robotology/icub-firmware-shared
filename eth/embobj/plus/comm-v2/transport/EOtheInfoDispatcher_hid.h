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
#ifndef _EOTHEINFODISPATCHER_HID_H_
#define _EOTHEINFODISPATCHER_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOtheInfoDispatcher_hid.h
    @brief      This header file implements hidden interface to the EOtheInfoDispatcher singleton.
    @author     marco.accame@iit.it
    @date       Nov 04 2014
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOvector.h"
#include "EOnv.h"
#include "EoManagement.h"


// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOtheInfoDispatcher.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------
// empty-section



// - definition of the hidden struct implementing the object ----------------------------------------------------------

/** @struct     EOtheInfoDispatcher_hid
    @brief      Hidden definition. Implements private data used only internally by the 
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/  
 
struct EOtheInfoDispatcher_hid 
{
    EOvector*               vectorOfinfostatus;
    eOmn_info_status_t*     overflow;  
    eOmn_info_status_t*     infostatus;
    EOtransmitter*          transmitter;
    // marco.accame: for now the function eo_transmitter_occasional_rops_Load() use only a eOropdescriptor_t argument and 
    //               computes the EOnv internally. thus we dont need the following two EOnv. however, we may speed up 
    //               by passing a EOnv* as argument which overrides the internal computation 
    EOnv*                   nvinfostatus;
    EOnv*                   nvinfostatusbasic;    
}; 


// - declaration of extern hidden functions ---------------------------------------------------------------------------



#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------








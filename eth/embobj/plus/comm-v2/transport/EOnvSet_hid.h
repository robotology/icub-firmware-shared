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

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EONVSET_HID_H_
#define _EONVSET_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOnvSet_hid.h
    @brief      This header file implements hidden interface to a to a collector of EOnv objects.
    @author     marco.accame@iit.it
    @date       06/07/2013
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOnv_hid.h"
#include "EOvector.h"
#include "EOconstvector.h"
#include "EOVmutex.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOnvSet.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------


// - definition of the hidden struct implementing the object ----------------------------------------------------------

typedef struct
{
    eOprot_EPcfg_t                      epcfg;
    uint16_t                            epnvsnumberof;
    eObool_t                            initted;
    uint8_t                             dummy; 
    void*                               epram;    
    EOVmutexDerived*                    mtx_endpoint;    
    EOvector*                           themtxofthenvs;    
} eOnvset_ep_t;


typedef struct
{
    eOipv4addr_t                    ipaddress;
    eOnvBRD_t                       boardnum;
    eOnvsetOwnership_t              ownership;
    EOvector*                       theendpoints;       // of eOnvset_ep_t items
    EOVmutexDerived*                mtx_board;    
    uint16_t                        ep2indexlut[eonvset_max_endpoint_value+1];    
} eOnvset_brd_t;




/** @struct     EOnvSet_hid
    @brief      Hidden definition. Implements private data used only internally by the 
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/
struct EOnvSet_hid 
{
    eOnvset_brd_t                   theboard;
    eOnvset_protection_t            protection;
    eov_mutex_fn_mutexderived_new   mtxderived_new;
};   
 



// - declaration of extern hidden functions ---------------------------------------------------------------------------
// empty-section
 

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





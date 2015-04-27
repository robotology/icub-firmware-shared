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
#ifndef _EONVSETDEVBUILDER_HID_H_
#define _EONVSETDEVBUILDER_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOnvsetBRDbuilder_hid.h
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
#include "EoProtocol.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOnvsetBRDbuilder.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------


// - definition of the hidden struct implementing the object ----------------------------------------------------------




/** @struct     EOnvsetBRDbuilder_hid
    @brief      Hidden definition. Implements private data used only internally by the 
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/
struct EOnvsetBRDbuilder_hid 
{
    EOvector*           epcfg_vector;
    eOnvset_BRDcfg_t*   brdcfg;
};   
 




// - declaration of extern hidden functions ---------------------------------------------------------------------------





 

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



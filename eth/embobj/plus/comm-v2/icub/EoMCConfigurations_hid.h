/*
 * Copyright (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Davide Pollarolo
 * email:   davide.pollarolo@iit.it
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
#ifndef _EOMCCONFIGURATIONS_HID_H
#define _EOMCCONFIGURATIONS_HID_H

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoMCConfigurations_hid.h
    @brief      This header file implements hidden interface to MCConfigurations.
    @author     davide.pollarolo@iit.it
    @date       06/05/2015
**/

// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoMCConfigurations.h"

// - definition of the hidden struct implementing the object ----------------------------------------------------------

struct EoMCConfigurations_hid 
{
    //1+2+1 = 4B
    eObool_t initted;
    eOmcconfig_code_t active_code;
    uint8_t dummy;
    //something missing?
}; 



#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





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
#ifndef _EONVSETDEVBUILDER_H_
#define _EONVSETDEVBUILDER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOnvsetBRDbuilder.h
    @brief      This header file implements public interface to a collector of EOnv objects.
    @author     marco.accame@iit.it
    @date       04/22/2014
**/

/** @defgroup eo_nvsetbrdbuilder EOnvsetBRDbuilder
    cefcewqfcew
      
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOnvSet.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types -------------------------------------------------------------------------    


/** @typedef    typedef const struct EOnvsetBRDbuilder_hid EOnvsetBRDbuilder
    @brief      EOaction is an opaque struct. It is used to implement data abstraction for the  
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOnvsetBRDbuilder_hid EOnvsetBRDbuilder;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------

extern EOnvsetBRDbuilder* eo_nvsetbrdbuilder_New(eOnvBRD_t board);

extern void eo_nvsetbrdbuilder_Delete(EOnvsetBRDbuilder* p);

extern eOresult_t eo_nvsetbrdbuilder_EP_Load(EOnvsetBRDbuilder* p, eOprot_EPcfg_t* epcfg);

extern eOresult_t eo_nvsetbrdbuilder_EPs_Clear(EOnvsetBRDbuilder* p);

extern eOnvset_BRDcfg_t* eo_nvsetbrdbuilder_BRDcfg_Get(EOnvsetBRDbuilder* p);





/** @}            
    end of group eo_nvsetbrdbuilder 
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




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

/** @file       EOnvsetDEVbuilder.h
    @brief      This header file implements public interface to a collector of EOnv objects.
    @author     marco.accame@iit.it
    @date       04/22/2014
**/

/** @defgroup eo_nvsetdevbuilder EOnvsetDEVbuilder
    cefcewqfcew
      
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOnvSet.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types -------------------------------------------------------------------------    


/** @typedef    typedef const struct EOnvsetDEVbuilder_hid EOnvsetDEVbuilder
    @brief      EOaction is an opaque struct. It is used to implement data abstraction for the  
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOnvsetDEVbuilder_hid EOnvsetDEVbuilder;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------

extern EOnvsetDEVbuilder* eo_nvsetdevbuilder_New(eOnvBRD_t board);


extern eOnvset_DEVcfg_t* eo_nvsetdevbuilder_DEVcfg_Get(EOnvsetDEVbuilder* p);

// by default all entities have zero number.
extern eOresult_t eo_nvsetdevbuilder_ENDPOINTpushback(EOnvsetDEVbuilder* p, eOnvEP8_t ep);

extern eOresult_t eo_nvsetdevbuilder_ENTITYpushback(EOnvsetDEVbuilder* p, eOnvEP8_t ep, eOnvENT_t en, uint8_t howmanyofthisentity);

extern eOresult_t eo_nvsetdevbuilder_PROXIEDRULEset(EOnvsetDEVbuilder* p, eObool_fp_uint32_t isvarproxied_fn);

extern eOresult_t eo_nvsetdevbuilder_Prepare(EOnvsetDEVbuilder* p);

//extern eOresult_t eo_nvsetdevbuilder_Delete(EOnvsetDEVbuilder* p);

//extern eOresult_t eo_nvsetdevbuilder_Clean(EOnvsetDEVbuilder* p);



/** @}            
    end of group eo_nvsetdevbuilder 
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




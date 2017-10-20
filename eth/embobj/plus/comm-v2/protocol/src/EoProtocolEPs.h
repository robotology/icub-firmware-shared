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

#ifndef _EOPROTOCOLEPS_H_
#define _EOPROTOCOLEPS_H_




/** @file       EoProtocolEPs.h
	@brief      This header file gives ...
	@author     marco.accame@iit.it
	@date       05/29/2013
**/

/** @defgroup eo_EoProtocolEPs Ccewcwe
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoProtocol.h"
#include "EOnv_hid.h"


// - public #define  --------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    #define EOPROT_ROMmap
#else    
    #define EOPROT_ROMmap const
#endif 


// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------


extern EOPROT_ROMmap EOnv_rom_t * const * const * const eoprot_ep_descriptors[];
    
extern eOvoid_fp_cnvp_cropdesp_t eoprot_ep_onsay[];                 // eoprot_endpoints_numberof
extern const eoprot_version_t * const eoprot_endpoint_version[];    // eoprot_endpoints_numberof
extern const uint8_t eoprot_ep_entities_numberof[];                 // eoprot_endpoints_numberof
extern const uint16_t* const eoprot_ep_entities_sizeof[];           // eoprot_endpoints_numberof
extern const void** const eoprot_ep_entities_defval[];              // eoprot_endpoints_numberof
extern const uint8_t* const eoprot_ep_tags_numberof[];              // eoprot_endpoints_numberof
extern const char * const eoprot_strings_endpoint[];                // eoprot_endpoints_numberof
extern const char ** const eoprot_strings_entity[];                 // eoprot_endpoints_numberof
extern const char *** const eoprot_strings_tag[];                   // eoprot_endpoints_numberof

#define POSof_strings_special_endpoint_all                  0
#define POSof_strings_special_endpoint_none                 1
#define POSof_strings_special_endpoint_unrecognised         2
#define POSof_strings_special_entity_unrecognised           3
#define POSof_strings_special_tag_unrecognised              4
extern const char * eoprot_strings_special[];


// - declaration of extern public functions ---------------------------------------------------------------------------

extern uint8_t eoprot_ep_ep2index(eOprotEndpoint_t ep);

extern eOprotEndpoint_t eoprot_ep_index2ep(uint8_t index);


/** @}            
    end of group eo_EoProtocolEPs  
 **/

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------











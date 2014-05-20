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

/* @file       EoProtocolMN_fun.c
    @brief      This file keeps constant configuration for ...
    @author     marco.accame@iit.it
    @date       06/06/2013
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolMN.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#if !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    #define EOPROT_EP_MN_FUN_OVERRIDE

    #if defined(EOPROT_EP_MN_FUN_OVERRIDE)
    #include "EoProtocolMN_overridden_fun.h"
    #endif
#endif



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section
 
// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_mn_fptr = NULL; 
#endif




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
extern void eoprot_fun_INITIALISE_mn(eOprotIP_t ip, void *ram) 
{
    if(NULL != eoprot_fun_INITIALISE_mn_fptr)
    {
        eoprot_fun_INITIALISE_mn_fptr(ip, ram);
    }
}
#else
#if !defined(OVERRIDE_eoprot_mn_fun_INITIALISE)
EO_weak extern void eoprot_fun_INITIALISE_mn(eOprotIP_t ip, void *ram) {}
#endif
#endif


#if    !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)

// -- comm
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mn_comm_wholeitem)
EO_weak extern void eoprot_fun_INIT_mn_comm_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_comm_wholeitem)
EO_weak extern void eoprot_fun_UPDT_mn_comm_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mn_comm_cmmnds_ropsigcfg)
EO_weak extern void eoprot_fun_INIT_mn_comm_cmmnds_ropsigcfg(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_comm_cmmnds_ropsigcfg)
EO_weak extern void eoprot_fun_UPDT_mn_comm_cmmnds_ropsigcfg(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif
  
// -- appl
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mn_appl_wholeitem)
EO_weak extern void eoprot_fun_INIT_mn_appl_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_appl_wholeitem)
EO_weak extern void eoprot_fun_UPDT_mn_appl_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mn_appl_config)
EO_weak extern void eoprot_fun_INIT_mn_appl_config(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_appl_config)
EO_weak extern void eoprot_fun_UPDT_mn_appl_config(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mn_appl_status)
EO_weak extern void eoprot_fun_INIT_mn_appl_status(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_appl_status)
EO_weak extern void eoprot_fun_UPDT_mn_appl_status(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mn_appl_cmmnds_go2state)
EO_weak extern void eoprot_fun_INIT_mn_appl_cmmnds_go2state(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mn_appl_cmmnds_go2state)
EO_weak extern void eoprot_fun_UPDT_mn_appl_cmmnds_go2state(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#endif//!defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section




// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


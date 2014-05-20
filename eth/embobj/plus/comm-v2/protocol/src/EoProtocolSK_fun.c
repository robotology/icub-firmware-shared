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

/* @file       EoProtocolSK_fun.c
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

#include "EoProtocolSK.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#if !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
    #define EOPROT_EP_SK_FUN_OVERRIDE

    #if defined(EOPROT_EP_SK_FUN_OVERRIDE)
    #include "EoProtocolSK_overridden_fun.h"
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
eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_sk_fptr = NULL; 
#endif




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
extern void eoprot_fun_INITIALISE_sk(eOprotIP_t ip, void *ram) 
{
    if(NULL != eoprot_fun_INITIALISE_sk_fptr)
    {
        eoprot_fun_INITIALISE_sk_fptr(ip, ram);
    }
}
#else
#if !defined(OVERRIDE_eoprot_sk_fun_INITIALISE)
EO_weak extern void eoprot_fun_INITIALISE_sk(eOprotIP_t ip, void *ram) {}
#endif
#endif


#if    !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)

// -- skin

#if !defined(OVERRIDE_eoprot_fun_INIT_sk_skin_wholeitem)
EO_weak extern void eoprot_fun_INIT_sk_skin_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_sk_skin_wholeitem)
EO_weak extern void eoprot_fun_UPDT_sk_skin_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_sk_skin_config_sigmode)
EO_weak extern void eoprot_fun_INIT_sk_skin_config_sigmode(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_sk_skin_config_sigmode)
EO_weak extern void eoprot_fun_UPDT_sk_skin_config_sigmode(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_sk_skin_status_arrayof10canframes)
EO_weak extern void eoprot_fun_INIT_sk_skin_status_arrayof10canframes(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_sk_skin_status_arrayof10canframes)
EO_weak extern void eoprot_fun_UPDT_sk_skin_status_arrayof10canframes(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_sk_skin_commands_boardscfg)
EO_weak extern void eoprot_fun_INIT_sk_skin_commands_boardscfg(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_sk_skin_commands_trianglescfg)
EO_weak extern void eoprot_fun_UPDT_sk_skin_commands_boardscfg(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_sk_skin_commands_trianglescfg)
EO_weak extern void eoprot_fun_INIT_sk_skin_commands_trianglescfg(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_sk_skin_commands_trianglescfg)
EO_weak extern void eoprot_fun_UPDT_sk_skin_commands_trianglescfg(const EOnv* nv, const eOropdescriptor_t* rd) {}
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


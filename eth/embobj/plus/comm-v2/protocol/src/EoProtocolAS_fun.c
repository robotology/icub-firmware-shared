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

/* @file       EoProtocolAS_fun.c
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

#include "EoProtocolAS.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section




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
eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_as_fptr = NULL; 
#endif




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
extern void eoprot_fun_INITIALISE_as(eOprotIP_t ip, void *ram) 
{
    if(NULL != eoprot_fun_INITIALISE_as_fptr)
    {
        eoprot_fun_INITIALISE_as_fptr(ip, ram);
    }
}
#else
EO_weak extern void eoprot_fun_INITIALISE_as(eOprotIP_t ip, void *ram) {}
#endif


#if     !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
// if not defined, then we remove them

// -- strain 

EO_weak extern void eoprot_fun_INIT_as_strain_wholeitem(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_strain_config(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_config(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_strain_status(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_status(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_strain_status_fullscale(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_status_fullscale(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_strain_status_calibratedvalues(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_status_calibratedvalues(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_strain_status_uncalibratedvalues(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_strain_status_uncalibratedvalues(const EOnv* nv, const eOropdescriptor_t* rd) {}



// -- mais

EO_weak extern void eoprot_fun_INIT_as_mais_wholeitem(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_config(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_config(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_config_mode(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_config_mode(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_config_datarate(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_config_datarate(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_config_resolution(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_config_resolution(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_status(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_status(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_mais_status_the15values(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_mais_status_the15values(const EOnv* nv, const eOropdescriptor_t* rd) {}



// -- extorque

EO_weak extern void eoprot_fun_INIT_as_extorque_wholeitem(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_extorque_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_extorque_config(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_extorque_config(const EOnv* nv, const eOropdescriptor_t* rd) {}

EO_weak extern void eoprot_fun_INIT_as_extorque_inputs(const EOnv* nv) {}
EO_weak extern void eoprot_fun_UPDT_as_extorque_inputs(const EOnv* nv, const eOropdescriptor_t* rd) {}



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


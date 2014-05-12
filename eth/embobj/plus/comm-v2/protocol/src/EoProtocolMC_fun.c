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

/* @file       EoProtocolMC_fun.c
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

#include "EOnv_hid.h"
#include "EoProtocolMC_rom.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolMC.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define EOPROT_EP_MC_FUN_OVERRIDE

#if defined(EOPROT_EP_MC_FUN_OVERRIDE)
#include "EoProtocolMC_overridden_fun.h"
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
eOvoid_fp_uint32_voidp_t eoprot_fun_INITIALISE_mc_fptr = NULL; 
#endif




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)
extern void eoprot_fun_INITIALISE_mc(eOprotIP_t ip, void *ram) 
{
    if(NULL != eoprot_fun_INITIALISE_mc_fptr)
    {
        eoprot_fun_INITIALISE_mc_fptr(ip, ram);
    }
}
#else
#if !defined(OVERRIDE_eoprot_mc_fun_INITIALISE)
EO_weak extern void eoprot_fun_INITIALISE_mc(eOprotIP_t ip, void *ram) {}
#endif
#endif


#if    !defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)

// -- joint
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_wholeitem)
EO_weak extern void eoprot_fun_INIT_mc_joint_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_wholeitem)
EO_weak extern void eoprot_fun_UPDT_mc_joint_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd)  {}
#endif
      
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config)
EO_weak extern void eoprot_fun_INIT_mc_joint_config(const EOnv* nv)  {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config(const EOnv* nv, const eOropdescriptor_t* rd)  {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_pidposition)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_pidposition(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_pidposition)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_pidposition(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_pidvelocity)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_pidvelocity(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_pidvelocity)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_pidvelocity(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_pidtorque)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_pidtorque(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_pidtorque)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_pidtorque(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif
   
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_limitsofjoint)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_limitsofjoint(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_limitsofjoint)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_limitsofjoint(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_impedance)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_impedance(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_impedance)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_impedance(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_config_motionmonitormode)
EO_weak extern void eoprot_fun_INIT_mc_joint_config_motionmonitormode(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_config_motionmonitormode)
EO_weak extern void eoprot_fun_UPDT_mc_joint_config_motionmonitormode(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_status)
EO_weak extern void eoprot_fun_INIT_mc_joint_status(const EOnv* nv)  {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_status)
EO_weak extern void eoprot_fun_UPDT_mc_joint_status(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_status_basic)
EO_weak extern void eoprot_fun_INIT_mc_joint_status_basic(const EOnv* nv) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_status_basic)
EO_weak extern void eoprot_fun_UPDT_mc_joint_status_basic(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_status_ofpid)
EO_weak extern void eoprot_fun_INIT_mc_joint_status_ofpid(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_status_ofpid)
EO_weak extern void eoprot_fun_UPDT_mc_joint_status_ofpid(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_inputs)
EO_weak extern void eoprot_fun_INIT_mc_joint_inputs(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_inputs)
EO_weak extern void eoprot_fun_UPDT_mc_joint_inputs(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_inputs_externallymeasuredtorque)
EO_weak extern void eoprot_fun_INIT_mc_joint_inputs_externallymeasuredtorque(const EOnv* nv) {}
#endif   
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_inputs_externallymeasuredtorque)
EO_weak extern void eoprot_fun_UPDT_mc_joint_inputs_externallymeasuredtorque(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_cmmnds_calibration)
EO_weak extern void eoprot_fun_INIT_mc_joint_cmmnds_calibration(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_cmmnds_calibration_UDPT)
EO_weak extern void eoprot_fun_UPDT_mc_joint_cmmnds_calibration(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_cmmnds_setpoint)
EO_weak extern void eoprot_fun_INIT_mc_joint_cmmnds_setpoint(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_cmmnds_setpoint)
EO_weak extern void eoprot_fun_UPDT_mc_joint_cmmnds_setpoint(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_cmmnds_stoptrajectory)
EO_weak extern void eoprot_fun_INIT_mc_joint_cmmnds_stoptrajectory(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_cmmnds_stoptrajectory)
EO_weak extern void eoprot_fun_UPDT_mc_joint_cmmnds_stoptrajectory(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_cmmnds_controlmode)
EO_weak extern void eoprot_fun_INIT_mc_joint_cmmnds_controlmode(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_cmmnds_controlmode)
EO_weak extern void eoprot_fun_UPDT_mc_joint_cmmnds_controlmode(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_joint_cmmnds_interactionmode)
EO_weak extern void eoprot_fun_INIT_mc_joint_cmmnds_interactionmode(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_joint_cmmnds_interactionmode)
EO_weak extern void eoprot_fun_UPDT_mc_joint_cmmnds_interactionmode(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif
 
// -- motor    

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_motor_wholeitem)
EO_weak extern void eoprot_fun_INIT_mc_motor_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_motor_wholeitem)
EO_weak extern void eoprot_fun_UPDT_mc_motor_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif  
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_motor_config)
EO_weak extern void eoprot_fun_INIT_mc_motor_config(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_motor_config)
EO_weak extern void eoprot_fun_UPDT_mc_motor_config(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_motor_config_maxcurrentofmotor)
EO_weak extern void eoprot_fun_INIT_mc_motor_config_maxcurrentofmotor(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_motor_config_maxcurrentofmotor)
EO_weak extern void eoprot_fun_UPDT_mc_motor_config_maxcurrentofmotor(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_motor_status)
EO_weak extern void eoprot_fun_INIT_mc_motor_status(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_motor_status)
EO_weak extern void eoprot_fun_UPDT_mc_motor_status(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_motor_status_basic)
EO_weak extern void eoprot_fun_INIT_mc_motor_status_basic(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_motor_status_basic)
EO_weak extern void eoprot_fun_UPDT_mc_motor_status_basic(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

// -- controller
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_controller_wholeitem)
EO_weak extern void eoprot_fun_INIT_mc_controller_wholeitem(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_controller_wholeitem)
EO_weak extern void eoprot_fun_UPDT_mc_controller_wholeitem(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif    
    
#if !defined(OVERRIDE_eoprot_fun_INIT_mc_controller_config)
EO_weak extern void eoprot_fun_INIT_mc_controller_config(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_controller_config)
EO_weak extern void eoprot_fun_UPDT_mc_controller_config(const EOnv* nv, const eOropdescriptor_t* rd) {}
#endif

#if !defined(OVERRIDE_eoprot_fun_INIT_mc_controller_status)
EO_weak extern void eoprot_fun_INIT_mc_controller_status(const EOnv* nv) {}
#endif
#if !defined(OVERRIDE_eoprot_fun_UPDT_mc_controller_status)
EO_weak extern void eoprot_fun_UPDT_mc_controller_status(const EOnv* nv, const eOropdescriptor_t* rd) {}
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


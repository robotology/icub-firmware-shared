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

/* @file       EoProtocolMC_rom.c
    @brief      This file keeps ....
    @author     marco.accame@iit.it
    @date       06/06/2013
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"

#include "EoCommon.h"
#include "EOnv_hid.h"
//#include "EOconstvector_hid.h"

#include "EoProtocolMC.h"
#include "EoMotionControl.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolMC_rom.h"


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

// - guard on max number of entities
EO_VERIFYproposition(eoprot_mc_dew4, eoprot_entities_mc_numberof <= eoprot_entities_maxnumberof);

// - guard on tags ...
EO_VERIFYproposition(eoprot_mc_tagsnum_jo, eoprot_tags_mc_joint_numberof == eoprot_rwms_mc_joint_numberof);
EO_VERIFYproposition(eoprot_mc_tagsmax_jo, eoprot_tags_mc_joint_numberof <= eoprot_tags_maxnumberof);

EO_VERIFYproposition(eoprot_mc_tagsnum_mo, eoprot_tags_mc_motor_numberof == eoprot_rwms_mc_motor_numberof);
EO_VERIFYproposition(eoprot_mc_tagsmax_mo, eoprot_tags_mc_motor_numberof <= eoprot_tags_maxnumberof);

EO_VERIFYproposition(eoprot_mc_tagsnum_co, eoprot_tags_mc_controller_numberof == eoprot_rwms_mc_controller_numberof);
EO_VERIFYproposition(eoprot_mc_tagsmax_co, eoprot_tags_mc_controller_numberof <= eoprot_tags_maxnumberof);



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

// - default value of a joint
static const eOmc_joint_t eoprot_mc_rom_joint_defaultvalue = { 0 };

// - default value of a motor
static const eOmc_motor_t eoprot_mc_rom_motor_defaultvalue = { 0 };

// - default value of a controller
static const eOmc_controller_t eoprot_mc_rom_controller_defaultvalue = { 0 };


// - descriptors for the variables of a joint

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_wholeitem
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_pidposition =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.pidposition),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_pidposition,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.pidposition,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_pidposition,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_pidposition
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_pidvelocity =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.pidvelocity),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_pidvelocity,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.pidvelocity,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_pidvelocity,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_pidvelocity
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_pidtorque =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.pidtorque),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_pidtorque,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.pidtorque,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_pidtorque,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_pidtorque
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_limitsofjoint =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.limitsofjoint),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_limitsofjoint,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.limitsofjoint,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_limitsofjoint,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_limitsofjoint
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_impedance =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.impedance),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_impedance,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.impedance,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_impedance,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_impedance
#endif
}; 



static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_motor_params =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.motor_params),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_motor_params,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.motor_params,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_motor_params,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_motor_params
#endif
}; 

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_config_tcfiltertype =
{
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.config.tcfiltertype),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_config_tcfiltertype,
    EO_INIT(.dummy)     0,
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.config.tcfiltertype,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_config_tcfiltertype,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_config_tcfiltertype
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status_basic =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status.basic),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status_basic,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status.basic,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status_basic,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status_basic
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status_ofpid =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status.ofpid),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status_ofpid,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status.ofpid,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status_ofpid,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status_ofpid
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status_controlmodestatus =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status.controlmodestatus),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status_controlmodestatus,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status.controlmodestatus,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status_controlmodestatus,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status_controlmodestatus
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status_interactionmodestatus =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status.interactionmodestatus),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status_interactionmodestatus,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status.interactionmodestatus,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status_interactionmodestatus,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status_interactionmodestatus
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_status_ismotiondone =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.status.ismotiondone),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_status_ismotiondone,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.status.ismotiondone,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_status_ismotiondone,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_status_ismotiondone
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_inputs =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.inputs),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_inputs,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.inputs,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_inputs,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_inputs
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_inputs_externallymeasuredtorque =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.inputs.externallymeasuredtorque),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_inputs_externallymeasuredtorque,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.inputs.externallymeasuredtorque,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_inputs_externallymeasuredtorque,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_inputs_externallymeasuredtorque
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_cmmnds_calibration =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.cmmnds.calibration),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_cmmnds_calibration,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.cmmnds.calibration,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_cmmnds_calibration,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_cmmnds_calibration
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_cmmnds_setpoint =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.cmmnds.setpoint),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_cmmnds_setpoint,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.cmmnds.setpoint,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_cmmnds_setpoint,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_cmmnds_setpoint
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_cmmnds_stoptrajectory =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.cmmnds.stoptrajectory),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_cmmnds_stoptrajectory,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.cmmnds.stoptrajectory,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_cmmnds_stoptrajectory,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_cmmnds_stoptrajectory
#endif
}; 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_cmmnds_controlmode =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.cmmnds.controlmode),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_cmmnds_controlmode,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.cmmnds.controlmode,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_cmmnds_controlmode,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_cmmnds_controlmode
#endif
}; 

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_joint_cmmnds_interactionmode =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_joint_defaultvalue.cmmnds.interactionmode),
    EO_INIT(.rwmode)    eoprot_rwm_mc_joint_cmmnds_interactionmode,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_joint_defaultvalue.cmmnds.interactionmode,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_joint_cmmnds_interactionmode,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_joint_cmmnds_interactionmode
#endif
}; 



// - descriptors for the variables of a motor 


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_wholeitem
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_config_maxcurrentofmotor =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.config.maxcurrentofmotor),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_config_maxcurrentofmotor,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.config.maxcurrentofmotor,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_config_maxcurrentofmotor,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_config_maxcurrentofmotor
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_config_gearboxratio =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.config.gearboxratio),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_config_gearboxratio,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.config.gearboxratio,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_config_gearboxratio,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_config_gearboxratio
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_config_rotorencoder =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.config.rotorEncoderResolution),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_config_rotorencoder,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.config.rotorEncoderResolution,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_config_rotorencoder,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_config_rotorencoder
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_status,
    EO_INIT(.dummy)     0,   
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_motor_status_basic =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_motor_defaultvalue.status.basic),
    EO_INIT(.rwmode)    eoprot_rwm_mc_motor_status_basic,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_motor_defaultvalue.status.basic,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_motor_status_basic,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_motor_status_basic
#endif
};



// - descriptors for the variables of a controller

static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_controller_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_controller_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mc_controller_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_controller_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_controller_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_controller_wholeitem
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_controller_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_controller_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_mc_controller_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_controller_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_controller_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_controller_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_controller_config_jointcoupling =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_controller_defaultvalue.config.jointcoupling),
    EO_INIT(.rwmode)    eoprot_rwm_mc_controller_config_jointcoupling,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_controller_defaultvalue.config.jointcoupling,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_controller_config_jointcoupling,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_controller_config_jointcoupling
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mc_rom_descriptor_controller_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mc_rom_controller_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mc_controller_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mc_rom_controller_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mc_controller_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mc_controller_status
#endif
};



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eoprot_version_t eoprot_mc_version =
{
    EO_INIT(.major)     eoprot_version_mc_major,
    EO_INIT(.minor)     eoprot_version_mc_minor
};


// -- collector of eonv descriptors for the endpoint

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mc_rom_joint_descriptors[] =
{   // here are eoprot_tags_mc_joint_numberof descriptors for the joint entity
    &eoprot_mc_rom_descriptor_joint_wholeitem,
    &eoprot_mc_rom_descriptor_joint_config,
    &eoprot_mc_rom_descriptor_joint_config_pidposition,
    &eoprot_mc_rom_descriptor_joint_config_pidvelocity,
    &eoprot_mc_rom_descriptor_joint_config_pidtorque,
    &eoprot_mc_rom_descriptor_joint_config_limitsofjoint,
    &eoprot_mc_rom_descriptor_joint_config_impedance,
    &eoprot_mc_rom_descriptor_joint_config_motor_params,
    &eoprot_mc_rom_descriptor_joint_config_tcfiltertype,
    &eoprot_mc_rom_descriptor_joint_status,
    &eoprot_mc_rom_descriptor_joint_status_basic,
    &eoprot_mc_rom_descriptor_joint_status_ofpid,
    &eoprot_mc_rom_descriptor_joint_status_controlmodestatus,
    &eoprot_mc_rom_descriptor_joint_status_interactionmodestatus,
    &eoprot_mc_rom_descriptor_joint_status_ismotiondone,
    &eoprot_mc_rom_descriptor_joint_inputs,
    &eoprot_mc_rom_descriptor_joint_inputs_externallymeasuredtorque,
    &eoprot_mc_rom_descriptor_joint_cmmnds_calibration,
    &eoprot_mc_rom_descriptor_joint_cmmnds_setpoint,
    &eoprot_mc_rom_descriptor_joint_cmmnds_stoptrajectory,
    &eoprot_mc_rom_descriptor_joint_cmmnds_controlmode,
    &eoprot_mc_rom_descriptor_joint_cmmnds_interactionmode  
};  EO_VERIFYsizeof(s_eoprot_mc_rom_joint_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mc_joint_numberof));

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mc_rom_motor_descriptors[] =
{   // here are eoprot_tags_mc_motor_numberof descriptors for the motor entity
    &eoprot_mc_rom_descriptor_motor_wholeitem,
    &eoprot_mc_rom_descriptor_motor_config,
    &eoprot_mc_rom_descriptor_motor_config_maxcurrentofmotor,
    &eoprot_mc_rom_descriptor_motor_config_gearboxratio,
    &eoprot_mc_rom_descriptor_motor_config_rotorencoder,
    &eoprot_mc_rom_descriptor_motor_status,
    &eoprot_mc_rom_descriptor_motor_status_basic   
};  EO_VERIFYsizeof(s_eoprot_mc_rom_motor_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mc_motor_numberof));


static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mc_rom_controller_descriptors[] =
{   // here are eoprot_tags_mc_controller_numberof descriptors for the controller entity
    &eoprot_mc_rom_descriptor_controller_wholeitem,
    &eoprot_mc_rom_descriptor_controller_config,
    &eoprot_mc_rom_descriptor_controller_config_jointcoupling,
    &eoprot_mc_rom_descriptor_controller_status
};  EO_VERIFYsizeof(s_eoprot_mc_rom_controller_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mc_controller_numberof));


EOPROT_ROMmap EOnv_rom_t * const * const eoprot_mc_rom_descriptors[] = 
{
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mc_rom_joint_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mc_rom_motor_descriptors,   
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mc_rom_controller_descriptors, 
};  EO_VERIFYsizeof(eoprot_mc_rom_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t** const)*(eoprot_entities_mc_numberof));



// the other constants: to be changed when a new entity is added

const uint8_t eoprot_mc_rom_tags_numberof[] = 
{
    eoprot_tags_mc_joint_numberof, 
    eoprot_tags_mc_motor_numberof, 
    eoprot_tags_mc_controller_numberof
};  EO_VERIFYsizeof(eoprot_mc_rom_tags_numberof, eoprot_entities_mc_numberof*sizeof(uint8_t)); 


const uint16_t eoprot_mc_rom_entities_sizeof[] = 
{
    sizeof(eOmc_joint_t), 
    sizeof(eOmc_motor_t), 
    sizeof(eOmc_controller_t)
};  EO_VERIFYsizeof(eoprot_mc_rom_entities_sizeof, eoprot_entities_mc_numberof*sizeof(uint16_t)); 


const void* const eoprot_mc_rom_entities_defval[] = 
{
    (const void*)&eoprot_mc_rom_joint_defaultvalue, 
    (const void*)&eoprot_mc_rom_motor_defaultvalue,
	(const void*)&eoprot_mc_rom_controller_defaultvalue
};  EO_VERIFYsizeof(eoprot_mc_rom_entities_defval, eoprot_entities_mc_numberof*sizeof(const void*)); 


// the strings of the endpoint

const char * const eoprot_mc_strings_entity[] =
{
    "eoprot_entity_mc_joint",   
    "eoprot_entity_mc_motor",
    "eoprot_entity_mc_controller"
};  EO_VERIFYsizeof(eoprot_mc_strings_entity, eoprot_entities_mc_numberof*sizeof(const char*)); 


static const char * const s_eoprot_mc_strings_tags_joint[] =
{
    "eoprot_tag_mc_joint_wholeitem",
    "eoprot_tag_mc_joint_config",
    "eoprot_tag_mc_joint_config_pidposition",
    "eoprot_tag_mc_joint_config_pidvelocity",
    "eoprot_tag_mc_joint_config_pidtorque",
    "eoprot_tag_mc_joint_config_limitsofjoint",
    "eoprot_tag_mc_joint_config_impedance",
    "eoprot_tag_mc_joint_config_motor_params",
    "eoprot_tag_mc_joint_config_tcfiltertype",
    "eoprot_tag_mc_joint_status",
    "eoprot_tag_mc_joint_status_basic",
    "eoprot_tag_mc_joint_status_ofpid",
    "eoprot_tag_mc_joint_status_controlmodestatus",
    "eoprot_tag_mc_joint_status_interactionmodestatus",
    "eoprot_tag_mc_joint_status_ismotiondone",
    "eoprot_tag_mc_joint_inputs",
    "eoprot_tag_mc_joint_inputs_externallymeasuredtorque",
    "eoprot_tag_mc_joint_cmmnds_calibration",
    "eoprot_tag_mc_joint_cmmnds_setpoint",
    "eoprot_tag_mc_joint_cmmnds_stoptrajectory",
    "eoprot_tag_mc_joint_cmmnds_controlmode",
    "eoprot_tag_mc_joint_cmmnds_interactionmode"
};  EO_VERIFYsizeof(s_eoprot_mc_strings_tags_joint, eoprot_tags_mc_joint_numberof*sizeof(const char*)); 

static const char * const s_eoprot_mc_strings_tags_motor[] =
{
    "eoprot_tag_mc_motor_wholeitem",
    "eoprot_tag_mc_motor_config",
    "eoprot_tag_mc_motor_config_maxcurrentofmotor",
    "eoprot_tag_mc_motor_config_gearboxratio", 
    "eoprot_tag_mc_motor_config_rotorencoder", 
    "eoprot_tag_mc_motor_status",
    "eoprot_tag_mc_motor_status_basic"
};  EO_VERIFYsizeof(s_eoprot_mc_strings_tags_motor, eoprot_tags_mc_motor_numberof*sizeof(const char*)); 


static const char * const s_eoprot_mc_strings_tags_controller[] =
{
     "eoprot_tag_mc_controller_wholeitem",
     "eoprot_tag_mc_controller_config",
     "eoprot_tag_mc_controller_config_jointcoupling",
     "eoprot_tag_mc_controller_status"
};  EO_VERIFYsizeof(s_eoprot_mc_strings_tags_controller, eoprot_tags_mc_controller_numberof*sizeof(const char*)); 

const char ** const eoprot_mc_strings_tags[] =
{
    (const char**)&s_eoprot_mc_strings_tags_joint,   
    (const char**)&s_eoprot_mc_strings_tags_motor,
    (const char**)&s_eoprot_mc_strings_tags_controller
};  EO_VERIFYsizeof(eoprot_mc_strings_tags, eoprot_entities_mc_numberof*sizeof(const char**)); 

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section

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




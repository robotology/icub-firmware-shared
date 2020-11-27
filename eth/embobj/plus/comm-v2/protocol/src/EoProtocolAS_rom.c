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

/* @file       EoProtocolAS_rom.c
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

#include "EoProtocolAS.h"
#include "EoAnalogSensors.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolAS_rom.h"


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
EO_VERIFYproposition(eoprot_as_ded34, eoprot_entities_as_numberof <= eoprot_entities_maxnumberof)

// - guard on tags ...
EO_VERIFYproposition(eoprot_as_tagsnum_st, eoprot_tags_as_strain_numberof == eoprot_rwms_as_strain_numberof)
EO_VERIFYproposition(eoprot_as_tagsmax_st, eoprot_tags_as_strain_numberof <= eoprot_tags_maxnumberof)

EO_VERIFYproposition(eoprot_as_tagsnum_ma, eoprot_tags_as_mais_numberof == eoprot_rwms_as_mais_numberof)
EO_VERIFYproposition(eoprot_as_tagsmax_ma, eoprot_tags_as_mais_numberof <= eoprot_tags_maxnumberof)

EO_VERIFYproposition(eoprot_as_tagsnum_et, eoprot_tags_as_temperature_numberof == eoprot_rwms_as_temperature_numberof)
EO_VERIFYproposition(eoprot_as_tagsmax_et, eoprot_tags_as_temperature_numberof <= eoprot_tags_maxnumberof)

EO_VERIFYproposition(eoprot_as_tagsnum_in, eoprot_tags_as_inertial_numberof == eoprot_rwms_as_inertial_numberof)
EO_VERIFYproposition(eoprot_as_tagsmax_in, eoprot_tags_as_inertial_numberof <= eoprot_tags_maxnumberof)

// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

// - default value of a strain
static const eOas_strain_t eoprot_as_rom_strain_defaultvalue = { 0 };

// - default value of a mais
static const eOas_mais_t eoprot_as_rom_mais_defaultvalue = { 0 };

// - default value of a temperature sensor
static const eOas_temperature_t eoprot_as_rom_temperature_defaultvalue = { 0 };

// - default value of a inertial
static const eOas_inertial_t eoprot_as_rom_inertial_defaultvalue = { 0 };

// - default value of a inertial3
static const eOas_inertial3_t eoprot_as_rom_inertial3_defaultvalue = { 0 };

// - default value of a psc
static const eOas_psc_t eoprot_as_rom_psc_defaultvalue = { 0 };

// - default value of a pos
static const eOas_psc_t eoprot_as_rom_pos_defaultvalue = { 0 };

// - descriptors for the variables of a strain

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else    
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_config
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_status
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_status_fullscale =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue.status.fullscale),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_status_fullscale,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue.status.fullscale,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_status_fullscale,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_status_fullscale
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_status_calibratedvalues =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue.status.calibratedvalues),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_status_calibratedvalues,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue.status.calibratedvalues,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_status_calibratedvalues,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_status_calibratedvalues
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_strain_status_uncalibratedvalues =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_strain_defaultvalue.status.uncalibratedvalues),
    EO_INIT(.rwmode)    eoprot_rwm_as_strain_status_uncalibratedvalues,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_strain_defaultvalue.status.uncalibratedvalues,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_strain_status_uncalibratedvalues,
    EO_INIT(.update)    eoprot_fun_UPDT_as_strain_status_uncalibratedvalues
#endif
};

// - descriptors for the variables of a mais

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_config
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_config_mode =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.config.mode),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_config_mode,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.config.mode,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_config_mode,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_config_mode
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_config_datarate =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.config.datarate),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_config_datarate,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.config.datarate,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_config_datarate,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_config_datarate
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_config_resolution =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.config.resolution),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_config_resolution,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.config.resolution,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_config_resolution,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_config_resolution
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_status
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_mais_status_the15values =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_mais_defaultvalue.status.the15values),
    EO_INIT(.rwmode)    eoprot_rwm_as_mais_status_the15values,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_mais_defaultvalue.status.the15values,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_mais_status_the15values,
    EO_INIT(.update)    eoprot_fun_UPDT_as_mais_status_the15values
#endif
};

// - descriptors for the variables of a temperature sensor

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_temperature_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_temperature_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_temperature_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_temperature_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_temperature_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_temperature_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_temperature_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_temperature_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_temperature_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_temperature_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_temperature_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_temperature_config
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_temperature_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_temperature_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_temperature_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_temperature_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_temperature_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_temperature_status
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_temperature_cmmnds_enable =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_temperature_defaultvalue.cmmnds.enable),
    EO_INIT(.rwmode)    eoprot_rwm_as_temperature_cmmnds_enable,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_temperature_defaultvalue.cmmnds.enable,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_temperature_cmmnds_enable,
    EO_INIT(.update)    eoprot_fun_UPDT_as_temperature_cmmnds_enable
#endif
};


// - descriptors for the variables of an inertial

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_config_datarate =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue.config.datarate),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_config_datarate,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue.config.datarate,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_config_datarate,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_config_datarate
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_config_enabled =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue.config.enabled),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_config_enabled,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue.config.enabled,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_config_enabled,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_config_enabled
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial_cmmnds_enable =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial_defaultvalue.cmmnds.enable),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial_cmmnds_enable,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial_defaultvalue.cmmnds.enable,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial_cmmnds_enable,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial_cmmnds_enable
#endif
};


// - descriptors for the variables of an inertial3

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial3_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial3_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial3_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial3_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial3_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial3_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial3_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial3_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial3_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial3_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial3_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial3_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial3_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial3_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial3_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial3_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial3_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial3_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_inertial3_cmmnds_enable =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_inertial3_defaultvalue.cmmnds.enable),
    EO_INIT(.rwmode)    eoprot_rwm_as_inertial3_cmmnds_enable,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_inertial3_defaultvalue.cmmnds.enable,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_inertial3_cmmnds_enable,
    EO_INIT(.update)    eoprot_fun_UPDT_as_inertial3_cmmnds_enable
#endif
};


// - descriptors for the variables of a psc

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_psc_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_psc_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_psc_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_psc_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_psc_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_psc_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_psc_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_psc_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_psc_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_psc_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_psc_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_psc_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_psc_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_psc_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_psc_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_psc_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_psc_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_psc_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_psc_cmmnds_enable =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_psc_defaultvalue.cmmnds.enable),
    EO_INIT(.rwmode)    eoprot_rwm_as_psc_cmmnds_enable,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_psc_defaultvalue.cmmnds.enable,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_psc_cmmnds_enable,
    EO_INIT(.update)    eoprot_fun_UPDT_as_psc_cmmnds_enable
#endif
};

// - descriptors for the variables of a pos

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_pos_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_pos_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_as_pos_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_pos_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_pos_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_as_pos_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_pos_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_pos_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_as_pos_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_pos_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_pos_config,
    EO_INIT(.update)    eoprot_fun_UPDT_as_pos_config
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_pos_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_pos_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_as_pos_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_pos_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_pos_status,
    EO_INIT(.update)    eoprot_fun_UPDT_as_pos_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_as_rom_descriptor_pos_cmmnds_enable =
{   
    EO_INIT(.capacity)  sizeof(eoprot_as_rom_pos_defaultvalue.cmmnds.enable),
    EO_INIT(.rwmode)    eoprot_rwm_as_pos_cmmnds_enable,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_as_rom_pos_defaultvalue.cmmnds.enable,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_as_pos_cmmnds_enable,
    EO_INIT(.update)    eoprot_fun_UPDT_as_pos_cmmnds_enable
#endif
};

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eoprot_version_t eoprot_as_version =
{
    EO_INIT(.major)     eoprot_version_as_major,
    EO_INIT(.minor)     eoprot_version_as_minor
};


// -- collector of eonv descriptors for the endpoint

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_strain_descriptors[] =
{   // here are eoprot_tags_as_strain_numberof descriptors for the strain entity
    &eoprot_as_rom_descriptor_strain_wholeitem,
    &eoprot_as_rom_descriptor_strain_config,
    &eoprot_as_rom_descriptor_strain_status,
    &eoprot_as_rom_descriptor_strain_status_fullscale,
    &eoprot_as_rom_descriptor_strain_status_calibratedvalues,
    &eoprot_as_rom_descriptor_strain_status_uncalibratedvalues
};  EO_VERIFYsizeof(s_eoprot_as_rom_strain_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_strain_numberof))

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_mais_descriptors[] =
{   // here are eoprot_tags_as_mais_numberof descriptors for the strain entity
    &eoprot_as_rom_descriptor_mais_wholeitem,
    &eoprot_as_rom_descriptor_mais_config,
    &eoprot_as_rom_descriptor_mais_config_mode,
    &eoprot_as_rom_descriptor_mais_config_datarate,
    &eoprot_as_rom_descriptor_mais_config_resolution,
    &eoprot_as_rom_descriptor_mais_status,
    &eoprot_as_rom_descriptor_mais_status_the15values
};  EO_VERIFYsizeof(s_eoprot_as_rom_mais_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_mais_numberof))

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_temperature_descriptors[] =
{   // here are eoprot_tags_as_temperature_numberof descriptors for the strain entity
    &eoprot_as_rom_descriptor_temperature_wholeitem,
    &eoprot_as_rom_descriptor_temperature_config,
    &eoprot_as_rom_descriptor_temperature_status,
    &eoprot_as_rom_descriptor_temperature_cmmnds_enable
};  EO_VERIFYsizeof(s_eoprot_as_rom_temperature_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_temperature_numberof))

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_inertial_descriptors[] =
{   // here are eoprot_tags_as_inertial_numberof descriptors for the inertial entity
    &eoprot_as_rom_descriptor_inertial_wholeitem,
    &eoprot_as_rom_descriptor_inertial_config,
    &eoprot_as_rom_descriptor_inertial_config_datarate,
    &eoprot_as_rom_descriptor_inertial_config_enabled,
    &eoprot_as_rom_descriptor_inertial_status,
    &eoprot_as_rom_descriptor_inertial_cmmnds_enable
};  EO_VERIFYsizeof(s_eoprot_as_rom_inertial_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_inertial_numberof))

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_inertial3_descriptors[] =
{   // here are eoprot_tags_as_inertial_numberof descriptors for the inertial entity
    &eoprot_as_rom_descriptor_inertial3_wholeitem,
    &eoprot_as_rom_descriptor_inertial3_config,
    &eoprot_as_rom_descriptor_inertial3_status,
    &eoprot_as_rom_descriptor_inertial3_cmmnds_enable
};  EO_VERIFYsizeof(s_eoprot_as_rom_inertial3_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_inertial3_numberof))


static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_psc_descriptors[] =
{   // here are eoprot_tags_as_inertial_numberof descriptors for the inertial entity
    &eoprot_as_rom_descriptor_psc_wholeitem,
    &eoprot_as_rom_descriptor_psc_config,
    &eoprot_as_rom_descriptor_psc_status,
    &eoprot_as_rom_descriptor_psc_cmmnds_enable
};  EO_VERIFYsizeof(s_eoprot_as_rom_psc_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_psc_numberof))

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_as_rom_pos_descriptors[] =
{   // here are eoprot_tags_as_inertial_numberof descriptors for the inertial entity
    &eoprot_as_rom_descriptor_pos_wholeitem,
    &eoprot_as_rom_descriptor_pos_config,
    &eoprot_as_rom_descriptor_pos_status,
    &eoprot_as_rom_descriptor_pos_cmmnds_enable
};  EO_VERIFYsizeof(s_eoprot_as_rom_pos_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_as_pos_numberof))

EOPROT_ROMmap EOnv_rom_t * const * const eoprot_as_rom_descriptors[] = 
{
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_strain_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_mais_descriptors,   
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_temperature_descriptors, 
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_inertial_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_inertial3_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_psc_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_as_rom_pos_descriptors
};  EO_VERIFYsizeof(eoprot_as_rom_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t** const)*(eoprot_entities_as_numberof))


// the other constants: to be changed when a new entity is added

const uint8_t eoprot_as_rom_tags_numberof[] = 
{
    eoprot_tags_as_strain_numberof, 
    eoprot_tags_as_mais_numberof, 
    eoprot_tags_as_temperature_numberof,
    eoprot_tags_as_inertial_numberof,
    eoprot_tags_as_inertial3_numberof,
    eoprot_tags_as_psc_numberof,
    eoprot_tags_as_pos_numberof
};  EO_VERIFYsizeof(eoprot_as_rom_tags_numberof, eoprot_entities_as_numberof*sizeof(uint8_t)) 

const uint16_t eoprot_as_rom_entities_sizeof[] = 
{
    sizeof(eOas_strain_t), 
    sizeof(eOas_mais_t), 
    sizeof(eOas_temperature_t),
    sizeof(eOas_inertial_t),
    sizeof(eOas_inertial3_t),
    sizeof(eOas_psc_t),
    sizeof(eOas_pos_t)
};  EO_VERIFYsizeof(eoprot_as_rom_entities_sizeof, eoprot_entities_as_numberof*sizeof(uint16_t)) 

const void* const eoprot_as_rom_entities_defval[] = 
{
    (const void*)&eoprot_as_rom_strain_defaultvalue, 
    (const void*)&eoprot_as_rom_mais_defaultvalue,
    (const void*)&eoprot_as_rom_temperature_defaultvalue,
    (const void*)&eoprot_as_rom_inertial_defaultvalue,
    (const void*)&eoprot_as_rom_inertial3_defaultvalue,
    (const void*)&eoprot_as_rom_psc_defaultvalue,
    (const void*)&eoprot_as_rom_pos_defaultvalue
};  EO_VERIFYsizeof(eoprot_as_rom_entities_defval, eoprot_entities_as_numberof*sizeof(const void*)) 


// the strings of the endpoint

const char * const eoprot_as_strings_entity[] =
{
    "eoprot_entity_as_strain",   
    "eoprot_entity_as_mais",
    "eoprot_entity_as_temperature",
    "eoprot_entity_as_inertial",
    "eoprot_entity_as_inertial3",
    "eoprot_entity_as_psc",
    "eoprot_entity_as_pos"
};  EO_VERIFYsizeof(eoprot_as_strings_entity, eoprot_entities_as_numberof*sizeof(const char*)) 


static const char * const s_eoprot_as_strings_tags_strain[] =
{
    "eoprot_tag_as_strain_wholeitem",
    "eoprot_tag_as_strain_config",
    "eoprot_tag_as_strain_status",
    "eoprot_tag_as_strain_status_fullscale",
    "eoprot_tag_as_strain_status_calibratedvalues",
    "eoprot_tag_as_strain_status_uncalibratedvalues"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_strain, eoprot_tags_as_strain_numberof*sizeof(const char*)) 

static const char * const s_eoprot_as_strings_tags_mais[] =
{
    "eoprot_tag_as_mais_wholeitem",
    "eoprot_tag_as_mais_config",
    "eoprot_tag_as_mais_config_mode",
    "eoprot_tag_as_mais_config_datarate",
    "eoprot_tag_as_mais_config_resolution",
    "eoprot_tag_as_mais_status",
    "eoprot_tag_as_mais_status_the15values"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_mais, eoprot_tags_as_mais_numberof*sizeof(const char*)) 

static const char * const s_eoprot_as_strings_tags_temperature[] =
{
    "eoprot_tag_as_temperature_wholeitem",
    "eoprot_tag_as_temperature_config",
    "eoprot_tag_as_temperature_status",
    "eoprot_tag_as_temperature_cmmnds_enable"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_temperature, eoprot_tags_as_temperature_numberof*sizeof(const char*)) 

static const char * const s_eoprot_as_strings_tags_inertial[] =
{
    "eoprot_tag_as_inertial_wholeitem",
    "eoprot_tag_as_inertial_config",
    "eoprot_tag_as_inertial_config_datarate",
    "eoprot_tag_as_inertial_config_enabled",
    "eoprot_tag_as_inertial_status",
    "eoprot_tag_as_inertial_cmmnds_enable"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_inertial, eoprot_tags_as_inertial_numberof*sizeof(const char*))

static const char * const s_eoprot_as_strings_tags_inertial3[] =
{
    "eoprot_tag_as_inertial3_wholeitem",
    "eoprot_tag_as_inertial3_config",
    "eoprot_tag_as_inertial3_status",
    "eoprot_tag_as_inertial3_cmmnds_enable"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_inertial3, eoprot_tags_as_inertial3_numberof*sizeof(const char*))

static const char * const s_eoprot_as_strings_tags_psc[] =
{
    "eoprot_tag_as_psc_wholeitem",
    "eoprot_tag_as_psc_config",
    "eoprot_tag_as_psc_status",
    "eoprot_tag_as_psc_cmmnds_enable"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_psc, eoprot_tags_as_psc_numberof*sizeof(const char*))

static const char * const s_eoprot_as_strings_tags_pos[] =
{
    "eoprot_tag_as_pos_wholeitem",
    "eoprot_tag_as_pos_config",
    "eoprot_tag_as_pos_status",
    "eoprot_tag_as_pos_cmmnds_enable"
};  EO_VERIFYsizeof(s_eoprot_as_strings_tags_pos, eoprot_tags_as_pos_numberof*sizeof(const char*))

const char ** const eoprot_as_strings_tags[] =
{
    (const char**)&s_eoprot_as_strings_tags_strain,   
    (const char**)&s_eoprot_as_strings_tags_mais,
    (const char**)&s_eoprot_as_strings_tags_temperature,
    (const char**)&s_eoprot_as_strings_tags_inertial,
    (const char**)&s_eoprot_as_strings_tags_inertial3,
    (const char**)&s_eoprot_as_strings_tags_psc,
    (const char**)&s_eoprot_as_strings_tags_pos
};  EO_VERIFYsizeof(eoprot_as_strings_tags, eoprot_entities_as_numberof*sizeof(const char**)) 


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




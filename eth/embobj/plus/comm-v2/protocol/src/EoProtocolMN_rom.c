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

/* @file       EoProtocolMN_rom.c
    @brief      This file keeps ....
    @author     marco.accame@iit.it
    @date       06/06/2013
**/


#if !defined(EOPROT_USE_MN_VERSION_1_0) & !defined(EOPROT_USE_MN_VERSION_2_0)
    #define EOPROT_USE_MN_VERSION_1_0
#endif

#if     !defined(EOPROT_USE_MN_VERSION_1_0)
        // marco.accame:    keep the double mode just for some days, until the blue robot is done.
        //                  forgive me for the double code block in the file 
        

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"

#include "EoCommon.h"
#include "EOnv_hid.h"
#include "EOconstvector_hid.h"

#include "EoProtocolMN.h"
#include "EoMotionControl.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolMN_rom.h"


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
EO_VERIFYproposition(eoprot_mn_ded34, eoprot_entities_mn_numberof <= eoprot_entities_maxnumberof);

// - guard on tags ...
EO_VERIFYproposition(eoprot_mn_tagsnum_co, eoprot_tags_mn_comm_numberof == eoprot_rwms_mn_comm_numberof);
EO_VERIFYproposition(eoprot_mn_tagsmax_co, eoprot_tags_mn_comm_numberof <= eoprot_tags_maxnumberof);

EO_VERIFYproposition(eoprot_mn_tagsnum_ap, eoprot_tags_mn_appl_numberof == eoprot_rwms_mn_appl_numberof);
EO_VERIFYproposition(eoprot_mn_tagsmax_ap, eoprot_tags_mn_appl_numberof <= eoprot_tags_maxnumberof);



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

// - default value of a comm
static const eOmn_comm_t eoprot_mn_rom_comm_defaultvalue = { 0 };

// - default value of a appl
static const eOmn_appl_t eoprot_mn_rom_appl_defaultvalue = { 0 };


// - descriptors for the variables of a comm

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_status
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_command_querynumof =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.querynumof),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_command_querynumof,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.querynumof,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_command_querynumof,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_command_querynumof
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_command_queryarray =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.queryarray),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_command_queryarray,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.queryarray,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_command_queryarray,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_command_queryarray
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_command_replynumof =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.replynumof),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_command_replynumof,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.replynumof,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_command_replynumof,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_command_replynumof
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_command_replyarray =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.replyarray),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_command_replyarray,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.replyarray,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_command_replyarray,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_command_replyarray
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_command_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.config),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_command_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.command.cmd.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_command_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_command_config
#endif
};


// - descriptors for the variables of a appl

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_config
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_status
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_cmmnds_go2state =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.cmmnds.go2state),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_cmmnds_go2state,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.cmmnds.go2state,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_cmmnds_go2state,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_cmmnds_go2state
#endif
};



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eoprot_version_t eoprot_mn_version =
{
    EO_INIT(.major)     eoprot_version_mn_major,
    EO_INIT(.minor)     eoprot_version_mn_minor
};

// -- collector of eonv descriptors for the endpoint

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mn_rom_comm_descriptors[] =
{   // here are eoprot_mn_tags_comm_numberof descriptors for the comm entity
    &eoprot_mn_rom_descriptor_comm_wholeitem,
    &eoprot_mn_rom_descriptor_comm_status,
    &eoprot_mn_rom_descriptor_comm_cmmnds_command_querynumof,
    &eoprot_mn_rom_descriptor_comm_cmmnds_command_queryarray,
    &eoprot_mn_rom_descriptor_comm_cmmnds_command_replynumof,
    &eoprot_mn_rom_descriptor_comm_cmmnds_command_replyarray,
    &eoprot_mn_rom_descriptor_comm_cmmnds_command_config         
};  EO_VERIFYsizeof(s_eoprot_mn_rom_comm_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mn_comm_numberof));

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mn_rom_appl_descriptors[] =
{   // here are eoprot_mn_tags_appl_numberof descriptors for the appl entity
    &eoprot_mn_rom_descriptor_appl_wholeitem,
    &eoprot_mn_rom_descriptor_appl_config,
    &eoprot_mn_rom_descriptor_appl_status,
    &eoprot_mn_rom_descriptor_appl_cmmnds_go2state     
};  EO_VERIFYsizeof(s_eoprot_mn_rom_appl_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mn_appl_numberof));


EOPROT_ROMmap EOnv_rom_t * const * const eoprot_mn_rom_descriptors[] = 
{
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mn_rom_comm_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mn_rom_appl_descriptors   
};  EO_VERIFYsizeof(eoprot_mn_rom_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t** const)*(eoprot_entities_mn_numberof));

    

// the other constants: to be changed when a new entity is added

const uint8_t eoprot_mn_rom_tags_numberof[] = 
{
    eoprot_tags_mn_comm_numberof, 
    eoprot_tags_mn_appl_numberof
};  EO_VERIFYsizeof(eoprot_mn_rom_tags_numberof, eoprot_entities_mn_numberof*sizeof(uint8_t));

const uint16_t eoprot_mn_rom_entities_sizeof[] = 
{
    sizeof(eOmn_comm_t), 
    sizeof(eOmn_appl_t)
};  EO_VERIFYsizeof(eoprot_mn_rom_entities_sizeof, eoprot_entities_mn_numberof*sizeof(uint16_t));


const void* const eoprot_mn_rom_entities_defval[] = 
{
    (const void*)&eoprot_mn_rom_comm_defaultvalue, 
    (const void*)&eoprot_mn_rom_appl_defaultvalue
};  EO_VERIFYsizeof(eoprot_mn_rom_entities_defval, eoprot_entities_mn_numberof*sizeof(const void*)); 


// the strings of the endpoint

const char * const eoprot_mn_strings_entity[] =
{
    "eoprot_entity_mn_communication",   
    "eoprot_entity_mn_application",
};  EO_VERIFYsizeof(eoprot_mn_strings_entity, eoprot_entities_mn_numberof*sizeof(const char*)); 


static const char * const s_eoprot_mn_strings_tags_comm[] =
{
    "eoprot_tag_mn_comm_wholeitem", 
    "eoprot_tag_mn_comm_status",
    "eoprot_tag_mn_comm_cmmnds_command_querynumof",
    "eoprot_tag_mn_comm_cmmnds_command_queryarray",
    "eoprot_tag_mn_comm_cmmnds_command_replynumof",
    "eoprot_tag_mn_comm_cmmnds_command_replyarray",
    "eoprot_tag_mn_comm_cmmnds_command_config"
};  EO_VERIFYsizeof(s_eoprot_mn_strings_tags_comm, eoprot_tags_mn_comm_numberof*sizeof(const char*)); 

static const char * const s_eoprot_mn_strings_tags_appl[] =
{
    "eoprot_tag_mn_appl_wholeitem",
    "eoprot_tag_mn_appl_config",
    "eoprot_tag_mn_appl_status",
    "eoprot_tag_mn_appl_cmmnds_go2state"
};  EO_VERIFYsizeof(s_eoprot_mn_strings_tags_appl, eoprot_tags_mn_appl_numberof*sizeof(const char*)); 


const char ** const eoprot_mn_strings_tags[] =
{
    (const char**)&s_eoprot_mn_strings_tags_comm,   
    (const char**)&s_eoprot_mn_strings_tags_appl
};  EO_VERIFYsizeof(eoprot_mn_strings_tags, eoprot_entities_mn_numberof*sizeof(const char**)); 


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
        
        
#else//!defined(EOPROT_USE_MN_VERSION_1_0)

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"

#include "EoCommon.h"
#include "EOnv_hid.h"
#include "EOconstvector_hid.h"

#include "EoProtocolMN.h"
#include "EoMotionControl.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolMN_rom.h"


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
EO_VERIFYproposition(eoprot_mn_ded34, eoprot_entities_mn_numberof <= eoprot_entities_maxnumberof);

// - guard on tags ...
EO_VERIFYproposition(eoprot_mn_tagsnum_co, eoprot_tags_mn_comm_numberof == eoprot_rwms_mn_comm_numberof);
EO_VERIFYproposition(eoprot_mn_tagsmax_co, eoprot_tags_mn_comm_numberof <= eoprot_tags_maxnumberof);

EO_VERIFYproposition(eoprot_mn_tagsnum_ap, eoprot_tags_mn_appl_numberof == eoprot_rwms_mn_appl_numberof);
EO_VERIFYproposition(eoprot_mn_tagsmax_ap, eoprot_tags_mn_appl_numberof <= eoprot_tags_maxnumberof);



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

// - default value of a comm
static const eOmn_comm_t eoprot_mn_rom_comm_defaultvalue = { 0 };

// - default value of a appl
static const eOmn_appl_t eoprot_mn_rom_appl_defaultvalue = { 0 };


// - descriptors for the variables of a comm

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_comm_cmmnds_ropsigcfg =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_comm_defaultvalue.cmmnds.ropsigcfg),
    EO_INIT(.rwmode)    eoprot_rwm_mn_comm_cmmnds_ropsigcfg,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_comm_defaultvalue.cmmnds.ropsigcfg,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_comm_cmmnds_ropsigcfg,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_comm_cmmnds_ropsigcfg
#endif
};

// - descriptors for the variables of a appl

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_wholeitem
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_config =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.config),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_config,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.config,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_config,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_config
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_status =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.status),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_status,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.status,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_status,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_status
#endif
};

static EOPROT_ROMmap EOnv_rom_t eoprot_mn_rom_descriptor_appl_cmmnds_go2state =
{   
    EO_INIT(.capacity)  sizeof(eoprot_mn_rom_appl_defaultvalue.cmmnds.go2state),
    EO_INIT(.rwmode)    eoprot_rwm_mn_appl_cmmnds_go2state,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_mn_rom_appl_defaultvalue.cmmnds.go2state,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_mn_appl_cmmnds_go2state,
    EO_INIT(.update)    eoprot_fun_UPDT_mn_appl_cmmnds_go2state
#endif
};



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eoprot_version_t eoprot_mn_version =
{
    EO_INIT(.major)     eoprot_version_mn_major,
    EO_INIT(.minor)     eoprot_version_mn_minor
};

// -- collector of eonv descriptors for the endpoint

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mn_rom_comm_descriptors[] =
{   // here are eoprot_mn_tags_comm_numberof descriptors for the comm entity
    &eoprot_mn_rom_descriptor_comm_wholeitem,
    &eoprot_mn_rom_descriptor_comm_cmmnds_ropsigcfg        
};  EO_VERIFYsizeof(s_eoprot_mn_rom_comm_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mn_comm_numberof));

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_mn_rom_appl_descriptors[] =
{   // here are eoprot_mn_tags_appl_numberof descriptors for the appl entity
    &eoprot_mn_rom_descriptor_appl_wholeitem,
    &eoprot_mn_rom_descriptor_appl_config,
    &eoprot_mn_rom_descriptor_appl_status,
    &eoprot_mn_rom_descriptor_appl_cmmnds_go2state   
};  EO_VERIFYsizeof(s_eoprot_mn_rom_appl_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_mn_appl_numberof));


EOPROT_ROMmap EOnv_rom_t * const * const eoprot_mn_rom_descriptors[] = 
{
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mn_rom_comm_descriptors,
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_mn_rom_appl_descriptors   
};  EO_VERIFYsizeof(eoprot_mn_rom_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t** const)*(eoprot_entities_mn_numberof));
    

// the other constants: to be changed when a new entity is added

const uint8_t eoprot_mn_rom_tags_numberof[] = 
{
    eoprot_tags_mn_comm_numberof, 
    eoprot_tags_mn_appl_numberof
};  EO_VERIFYsizeof(eoprot_mn_rom_tags_numberof, eoprot_entities_mn_numberof*sizeof(uint8_t));

const uint16_t eoprot_mn_rom_entities_sizeof[] = 
{
    sizeof(eOmn_comm_t), 
    sizeof(eOmn_appl_t)
};  EO_VERIFYsizeof(eoprot_mn_rom_entities_sizeof, eoprot_entities_mn_numberof*sizeof(uint16_t));


const void* const eoprot_mn_rom_entities_defval[] = 
{
    (const void*)&eoprot_mn_rom_comm_defaultvalue, 
    (const void*)&eoprot_mn_rom_appl_defaultvalue
};  EO_VERIFYsizeof(eoprot_mn_rom_entities_defval, eoprot_entities_mn_numberof*sizeof(const void*)); 

// the strings of the endpoint

const char * const eoprot_mn_strings_entity[] =
{
    "eoprot_entity_mn_communication",   
    "eoprot_entity_mn_application",
};  EO_VERIFYsizeof(eoprot_mn_strings_entity, eoprot_entities_mn_numberof*sizeof(const char*)); 


static const char * const s_eoprot_mn_strings_tags_comm[] =
{
    "eoprot_tag_mn_comm_wholeitem", 
    "eoprot_tag_mn_comm_cmmnds_ropsigcfg"
};  EO_VERIFYsizeof(s_eoprot_mn_strings_tags_comm, eoprot_tags_mn_comm_numberof*sizeof(const char*)); 

static const char * const s_eoprot_mn_strings_tags_appl[] =
{
    "eoprot_tag_mn_appl_wholeitem",
    "eoprot_tag_mn_appl_config",
    "eoprot_tag_mn_appl_status",
    "eoprot_tag_mn_appl_cmmnds_go2state"
};  EO_VERIFYsizeof(s_eoprot_mn_strings_tags_appl, eoprot_tags_mn_appl_numberof*sizeof(const char*)); 

const char ** const eoprot_mn_strings_tags[] =
{
    (const char**)&s_eoprot_mn_strings_tags_comm,   
    (const char**)&s_eoprot_mn_strings_tags_appl
};  EO_VERIFYsizeof(eoprot_mn_strings_tags, eoprot_entities_mn_numberof*sizeof(const char**)); 

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

#endif//!defined(EOPROT_USE_MN_VERSION_1_0)

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




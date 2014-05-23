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

/* @file       EoProtocolSK_rom.c
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
#include "EOconstvector_hid.h"

#include "EoProtocolSK.h"
#include "EoMotionControl.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoProtocolSK_rom.h"


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
EO_VERIFYproposition(eoprot_sk_ded34, eoprot_entities_sk_numberof <= eoprot_entities_maxnumberof);

// - guard on tags ...
EO_VERIFYproposition(eoprot_sk_tagsnum_sk, eoprot_tags_sk_skin_numberof == eoprot_rwms_sk_skin_numberof);
EO_VERIFYproposition(eoprot_sk_tagsmax_sk, eoprot_tags_sk_skin_numberof <= eoprot_tags_maxnumberof);




// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

// - default value of a skin
static const eOsk_skin_t eoprot_sk_rom_skin_defaultvalue = { 0 };



// - descriptors for the variables of a skin

static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_wholeitem =
{   
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_wholeitem,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_wholeitem,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_wholeitem
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_config_sigmode =
{   
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.config.sigmode),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_config_sigmode,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.config.sigmode,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_config_sigmode,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_config_sigmode
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_status_arrayof10canframes =
{   
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.status.arrayof10canframes),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_status_arrayof10canframes,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.status.arrayof10canframes,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_status_arrayof10canframes,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_status_arrayof10canframes
#endif
};



static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_commands_cmd_boardscfg =
{
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.commands.boardscfg),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_cmd_boardscfg,
    EO_INIT(.dummy)     0,
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.commands.boardscfg,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_commands_boardscfg,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_commands_boardscfg
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_commands_cmd_trianglescfg =
{
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.commands.trianglescfg),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_cmd_trianglescfg,
    EO_INIT(.dummy)     0,
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.commands.trianglescfg,
    #ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_commands_trianglescfg,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_commands_trianglescfg
#endif
};


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eoprot_version_t eoprot_sk_version =
{
    EO_INIT(.major)     eoprot_version_sk_major,
    EO_INIT(.minor)     eoprot_version_sk_minor
};
  
    
// -- the folded array of descriptors: to be changed if any new tag is added

// eoprot_sk_rom_folded_descriptors is an array whcih keeps const pointers (its values in rom) to objects which are
// in rom or not depending on EOPROT_ROMmap
EOPROT_ROMmap EOnv_rom_t * const eoprot_sk_rom_folded_descriptors[] =
{
    // here are eoprot_tags_sk_skin_numberof descriptors for the skins (equal for every skin)
    &eoprot_sk_rom_descriptor_skin_wholeitem,
    &eoprot_sk_rom_descriptor_skin_config_sigmode,
    &eoprot_sk_rom_descriptor_skin_status_arrayof10canframes,
    &eoprot_sk_rom_descriptor_skin_commands_cmd_boardscfg,
    &eoprot_sk_rom_descriptor_skin_commands_cmd_trianglescfg
};  EO_VERIFYsizeof(eoprot_sk_rom_folded_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_sk_skin_numberof));


// the other constants: to be changed when a new entity is added

const uint8_t eoprot_sk_rom_tags_numberof[] = 
{
    eoprot_tags_sk_skin_numberof
};  EO_VERIFYsizeof(eoprot_sk_rom_tags_numberof, eoprot_entities_sk_numberof*sizeof(uint8_t)); 

const uint16_t eoprot_sk_rom_entities_sizeof[] = 
{
    sizeof(eOsk_skin_t)
};  EO_VERIFYsizeof(eoprot_sk_rom_entities_sizeof, eoprot_entities_sk_numberof*sizeof(uint16_t));

const void* const eoprot_sk_rom_entities_defval[] = 
{
    (const void*)&eoprot_sk_rom_skin_defaultvalue
};  EO_VERIFYsizeof(eoprot_sk_rom_entities_defval, eoprot_entities_sk_numberof*sizeof(const void*)); 



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




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


static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_status_arrayofcandata =
{   
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.status.arrayofcandata),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_status_arrayofcandata,
    EO_INIT(.dummy)     0,    
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.status.arrayofcandata,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_status_arrayofcandata,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_status_arrayofcandata
#endif
};



static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_cmmnds_boardscfg =
{
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.cmmnds.boardscfg),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_cmmnds_boardscfg,
    EO_INIT(.dummy)     0,
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.cmmnds.boardscfg,
#ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_cmmnds_boardscfg,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_cmmnds_boardscfg
#endif
};


static EOPROT_ROMmap EOnv_rom_t eoprot_sk_rom_descriptor_skin_cmmnds_trianglescfg =
{
    EO_INIT(.capacity)  sizeof(eoprot_sk_rom_skin_defaultvalue.cmmnds.trianglescfg),
    EO_INIT(.rwmode)    eoprot_rwm_sk_skin_cmmnds_trianglescfg,
    EO_INIT(.dummy)     0,
    EO_INIT(.resetval)  (const void*)&eoprot_sk_rom_skin_defaultvalue.cmmnds.trianglescfg,
    #ifdef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
    EO_INIT(.init)      NULL,
    EO_INIT(.update)    NULL
#else       
    EO_INIT(.init)      eoprot_fun_INIT_sk_skin_cmmnds_trianglescfg,
    EO_INIT(.update)    eoprot_fun_UPDT_sk_skin_cmmnds_trianglescfg
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
  

// -- collector of eonv descriptors for the endpoint

static EOPROT_ROMmap EOnv_rom_t * const s_eoprot_sk_rom_skin_descriptors[] =
{   // here are eoprot_tags_sk_skin_numberof descriptors for the skin entity
    &eoprot_sk_rom_descriptor_skin_wholeitem,
    &eoprot_sk_rom_descriptor_skin_config_sigmode,
    &eoprot_sk_rom_descriptor_skin_status_arrayofcandata,
    &eoprot_sk_rom_descriptor_skin_cmmnds_boardscfg,
    &eoprot_sk_rom_descriptor_skin_cmmnds_trianglescfg
};  EO_VERIFYsizeof(s_eoprot_sk_rom_skin_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t* const)*(eoprot_tags_sk_skin_numberof));



EOPROT_ROMmap EOnv_rom_t * const * const eoprot_sk_rom_descriptors[] = 
{
    (EOPROT_ROMmap EOnv_rom_t **)&s_eoprot_sk_rom_skin_descriptors 
};  EO_VERIFYsizeof(eoprot_sk_rom_descriptors, sizeof(EOPROT_ROMmap EOnv_rom_t** const)*(eoprot_entities_sk_numberof));

    

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

// the strings of the endpoint

const char * const eoprot_sk_strings_entity[] =
{
    "eoprot_entity_sk_skin"
};  EO_VERIFYsizeof(eoprot_sk_strings_entity, eoprot_entities_sk_numberof*sizeof(const char*)); 


static const char * const s_eoprot_sk_strings_tags_skin[] =
{
    "eoprot_tag_sk_skin_wholeitem",
    "eoprot_tag_sk_skin_config_sigmode",
    "eoprot_tag_sk_skin_status_arrayofcandata",
    "eoprot_tag_sk_skin_cmmnds_boardscfg",
    "eoprot_tag_sk_skin_cmmnds_trianglescfg"
};  EO_VERIFYsizeof(s_eoprot_sk_strings_tags_skin, eoprot_tags_sk_skin_numberof*sizeof(const char*)); 

const char ** const eoprot_sk_strings_tags[] =
{
    (const char**)&s_eoprot_sk_strings_tags_skin
};  EO_VERIFYsizeof(eoprot_sk_strings_tags, eoprot_entities_sk_numberof*sizeof(const char**)); 

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




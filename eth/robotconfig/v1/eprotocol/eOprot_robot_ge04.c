/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
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

/* @file       eOprot_robot_ge04.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       04/14/2014
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"

#include "EOconstvector.h"
#include "EOnvSet.h"
#include "EOconstvector_hid.h"


#include "eOprot_b01.h"
#include "eOprot_b02.h"
#include "eOprot_b03.h"
#include "eOprot_b04.h"
#include "eOprot_b05.h"
#include "eOprot_b06.h"
#include "eOprot_b07.h"
#include "eOprot_b08.h"
#include "eOprot_b09.h"





// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "eOprot_robot.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "eOprot_robot_hid.h"


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------


enum { eoprot_robot_ge04_numberofboards = 9 };
enum { eoprot_robot_ge04_id = 0x00000004 };

EO_VERIFYproposition(eoprot_robot_gasdfe, eoprot_boards_maxnumberof >= eoprot_robot_ge04_numberofboards);



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)  
static void s_eoprot_robot_override_mn(void);
static void s_eoprot_robot_override_mc(void);
static void s_eoprot_robot_override_as(void);
static void s_eoprot_robot_override_sk(void);
#endif//defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)  



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static uint8_t s_eoprot_robot_initted = 0;

static const eOnvset_DEVcfg_t * const s_eoprot_robot_ge04_theDEVcfgptrs[] =
{  
    &eoprot_b01_nvsetDEVcfg,
    &eoprot_b02_nvsetDEVcfg,
    &eoprot_b03_nvsetDEVcfg,
    &eoprot_b04_nvsetDEVcfg,
    &eoprot_b05_nvsetDEVcfg,
    &eoprot_b06_nvsetDEVcfg,
    &eoprot_b07_nvsetDEVcfg,
    &eoprot_b08_nvsetDEVcfg,
    &eoprot_b09_nvsetDEVcfg   
};  EO_VERIFYsizeof(s_eoprot_robot_ge04_theDEVcfgptrs, sizeof(eOnvset_DEVcfg_t*)*(eoprot_robot_ge04_numberofboards));



static const EOconstvector s_eoprot_robot_ge04_constvectofDEVcfgptr = 
{
    EO_INIT(.size)                  sizeof(s_eoprot_robot_ge04_theDEVcfgptrs)/sizeof(eOnvset_DEVcfg_t*),
    EO_INIT(.item_size)             sizeof(eOnvset_DEVcfg_t*),
    EO_INIT(.item_array_data)       s_eoprot_robot_ge04_theDEVcfgptrs
};



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

#if 0 
    // use one for each board ... so that it is tuned with the board-transceiver
    EO_INIT(.sizes)
    {
        EO_INIT(.capacityoftxpacket)                EOK_HOSTTRANSCEIVER_capacityoftxpacket,
        EO_INIT(.capacityofrop)                     EOK_HOSTTRANSCEIVER_capacityofrop,
        EO_INIT(.capacityofropframeregulars)        EOK_HOSTTRANSCEIVER_capacityofropframeregulars,
        EO_INIT(.capacityofropframeoccasionals)     EOK_HOSTTRANSCEIVER_capacityofropframeoccasionals,
        EO_INIT(.capacityofropframereplies)         EOK_HOSTTRANSCEIVER_capacityofropframereplies,
        EO_INIT(.maxnumberofregularrops)            EOK_HOSTTRANSCEIVER_maxnumberofregularrops       
    },    
#endif

// the configuration of the eth protocol for the robot
//extern const eOprot_ROBOTcfg_t eoprot_robot_ROBOTcfg;
const eOprot_ROBOTcfg_t eoprot_robot_ROBOTcfg =
{
    EO_INIT(.id)                        eoprot_robot_ge04_id,
    EO_INIT(.family)                    0x00000001,
    EO_INIT(.name)                      "icub-ge04",
    EO_INIT(.vectorof_devcfgptr)        &s_eoprot_robot_ge04_constvectofDEVcfgptr
//    EO_INIT(.vectorof_transizesptr)     &s_eoprot_robot_ge04_constvectoftranssizesptr .... eOtransceiver_sizes_t
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern eOresult_t eoprot_robot_Initialise(void)
{
    // at first: initialise the number of boards in protocol.
    if(eores_OK != eoprot_config_board_numberof(eoprot_robot_ge04_numberofboards))
    {
        return(eores_NOK_generic);
    }
    
    // then: override what is needed
    
#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)    
  
    s_eoprot_robot_override_mn();
    
    s_eoprot_robot_override_mc();
    
    s_eoprot_robot_override_as();
    
    s_eoprot_robot_override_sk();

#endif//defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)   
   
    s_eoprot_robot_initted = 1;
       
    return(eores_OK);
}

extern uint32_t eoprot_robot_id_get(void)
{
    return(eoprot_robot_ROBOTcfg.id);
}

extern uint32_t eoprot_robot_family_get(void)
{
    return(eoprot_robot_ROBOTcfg.family);
}

extern const char * eoprot_robot_name_get(void)
{
    return(eoprot_robot_ROBOTcfg.name);
}


extern uint8_t eoprot_robot_DEVcfg_numberof(void)
{
    return(eo_constvector_Size(eoprot_robot_ROBOTcfg.vectorof_devcfgptr));
}

extern const eOnvset_DEVcfg_t* eoprot_robot_DEVcfg_get(uint8_t n)
{
    if(n > eoprot_robot_DEVcfg_numberof())
    {
        return(NULL);
    }
    
    if(0 == s_eoprot_robot_initted)
    {
        eoprot_robot_Initialise();
    }

    const eOnvset_DEVcfg_t **p = (const eOnvset_DEVcfg_t**) eo_constvector_At(eoprot_robot_ROBOTcfg.vectorof_devcfgptr, n);
    return(*p);
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

#if     defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)  

// nothing to override in endpoint management.
// the robot just send rops set<>. it never sends rops ask<> to variables of this endpoint, nor it receives and sig<> 
static void s_eoprot_robot_override_mn(void)
{  

}


static void s_eoprot_robot_override_mc(void)
{
    eOprotID32_t id32 = eo_prot_ID32dummy;
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- general ram initialise of mc endpoint called by every board.
    
    // we dont do any general initialisation, even if we could do it with a xxeoprot_fun_INITIALISE_mc() function
//    eoprot_config_endpoint_callback(eoprot_endpoint_motioncontrol, xxeoprot_fun_INITIALISE_mc);
    
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- override of the callbacks of variables of mc. common to every board. we use the id, even if the eoprot_config_variable_callback()
    //    operates on any index.
    
    // - joint entities
    
    // eoprot_tag_mc_joint_config: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_config);
    
    // eoprot_tag_mc_joint_config_pidposition: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config_pidposition);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_config_pidposition);
 
    // eoprot_tag_mc_joint_config_impedance: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config_impedance);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_config_impedance);

    // eoprot_tag_mc_joint_config_pidtorque: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config_pidtorque);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_config_pidtorque);  
    
    // eoprot_tag_mc_joint_status: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_status);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_status);
        
    // eoprot_tag_mc_joint_status_basic: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_status_basic);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_status_basic);
 
    // eoprot_tag_mc_joint_cmmnds_setpoint: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_cmmnds_setpoint);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_cmmnds_setpoint);

    // eoprot_tag_mc_joint_config_limitsofjoint: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config_limitsofjoint);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_joint_config_limitsofjoint);   
    
    // - motor entities
    
    // eoprot_tag_mc_motor_config: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_motor, 0, eoprot_tag_mc_motor_config);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_motor_config);  
    
    // eoprot_tag_mc_motor_config_maxcurrentofmotor: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_motor, 0, eoprot_tag_mc_motor_config_maxcurrentofmotor);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_motor_config_maxcurrentofmotor);
 
    // eoprot_tag_mc_motor_status_basic: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_motor, 0, eoprot_tag_mc_motor_status_basic);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_mc_motor_status_basic);
        
}


static void s_eoprot_robot_override_as(void)
{
    eOprotID32_t id32 = eo_prot_ID32dummy;
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- general ram initialise of as endpoint called by every board.
    
    // we dont do any general initialisation, even if we could do it with a xxeoprot_fun_INITIALISE_as() function
//    eoprot_config_endpoint_callback(eoprot_endpoint_analogsensors, xxeoprot_fun_INITIALISE_as);
    
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- override of the callbacks of variables of as. common to every board. we use the id, even if the eoprot_config_variable_callback()
    //    operates on any index.
    
    // - strain entities

    // eoprot_tag_as_strain_status_calibratedvalues: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_analogsensors, eoprot_entity_as_strain, 0, eoprot_tag_as_strain_status_calibratedvalues);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_as_strain_status_calibratedvalues); 

    // eoprot_tag_as_strain_status_uncalibratedvalues: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_analogsensors, eoprot_entity_as_strain, 0, eoprot_tag_as_strain_status_uncalibratedvalues);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_as_strain_status_uncalibratedvalues); 


    // - mais entities

    // eoprot_tag_as_mais_status_the15values: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_analogsensors, eoprot_entity_as_mais, 0, eoprot_tag_as_mais_status_the15values);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_as_mais_status_the15values); 
    
}

static void s_eoprot_robot_override_sk(void)
{
    eOprotID32_t id32 = eo_prot_ID32dummy;
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- general ram initialise of sk endpoint called by every board.
    
    // we dont do any general initialisation, even if we could do it with a xxeoprot_fun_INITIALISE_sk() function
//    eoprot_config_endpoint_callback(eoprot_endpoint_skin, xxeoprot_fun_INITIALISE_sk);
    
    
    // ------------------------------------------------------------------------------------------------------------------------------------
    // -- override of the callbacks of variables of sk. common to every board. we use the id, even if the eoprot_config_variable_callback()
    //    operates on any index.
    
    // - skin entities

    // eoprot_tag_sk_skin_status_arrayof10canframes: use 0 index, but operation is on every indices, only UPDT
    id32 = eoprot_ID_get(eoprot_endpoint_skin, eoprot_entity_sk_skin, 0, eoprot_tag_sk_skin_status_arrayof10canframes);
    eoprot_config_variable_callback(id32, NULL, eoprot_fun_UPDT_sk_skin_status_arrayof10canframes); 
}

#endif//defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)  


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





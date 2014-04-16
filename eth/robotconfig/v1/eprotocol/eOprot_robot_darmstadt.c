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

/* @file       eOprot_robot_darmstadt.c
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
#include "eOprot_b10.h"
#include "eOprot_b11.h"




// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "eOprot_robot.h"


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



typedef struct
{
    uint16_t                id;                     /*< a unique id of the robot */
    uint16_t                family;                 /*< family to which the robot belongs */
    char                    name[12];               /*< a descriptive name of the robot, e.g, "icub-ge04" */
    const EOconstvector*    vectorof_devcfgptr;     /*< a const vector of eOnvset_DEVcfg_t* of size equal to the number of managed boards. */
    // put a const-array of eOtransceiver_sizes_t for the host-transceiver, each one linked to the sizes of the ems-transceiver  
} eOprot_ROBOTcfg_t;


enum { eoprot_robot_darmstadt_numberofboards = 11 };
enum { eoprot_robot_darmstadt_id = 0x0005 };

EO_VERIFYproposition(eoprot_robot_gasdfe, eoprot_boards_maxnumberof >= eoprot_robot_darmstadt_numberofboards);



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

static const eOnvset_DEVcfg_t * const s_eoprot_robot_darmstadt_theDEVcfgptrs[] =
{  
    &eoprot_b01_nvsetDEVcfg,
    &eoprot_b02_nvsetDEVcfg,
    &eoprot_b03_nvsetDEVcfg,
    &eoprot_b04_nvsetDEVcfg,
    &eoprot_b05_nvsetDEVcfg,
    &eoprot_b06_nvsetDEVcfg,
    &eoprot_b07_nvsetDEVcfg,
    &eoprot_b08_nvsetDEVcfg,
    &eoprot_b08_nvsetDEVcfg   
};  EO_VERIFYsizeof(s_eoprot_robot_darmstadt_theDEVcfgptrs, sizeof(eOnvset_DEVcfg_t*)*(eoprot_robot_darmstadt_numberofboards));



static const EOconstvector s_eoprot_robot_darmstadt_constvectofDEVcfgptr = 
{
    EO_INIT(.size)                  sizeof(s_eoprot_robot_darmstadt_theDEVcfgptrs)/sizeof(eOnvset_DEVcfg_t*),
    EO_INIT(.item_size)             sizeof(eOnvset_DEVcfg_t*),
    EO_INIT(.item_array_data)       s_eoprot_robot_darmstadt_theDEVcfgptrs
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
    EO_INIT(.id)                        eoprot_robot_darmstadt_id,
    EO_INIT(.family)                    0x0002,
    EO_INIT(.name)                      "icub-darmstadt",
    EO_INIT(.vectorof_devcfgptr)        &s_eoprot_robot_darmstadt_constvectofDEVcfgptr
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern eOresult_t eoprot_robot_Initialise(void)
{
    // at first: initialise the number of boards in protocol.
    if(eores_OK != eoprot_config_board_numberof(eoprot_robot_darmstadt_numberofboards))
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

extern uint16_t eoprot_robot_id_get(void)
{
    return(eoprot_robot_ROBOTcfg.id);
}

extern uint16_t eoprot_robot_family_get(void)
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

static void s_eoprot_robot_override_mn(void)
{    

}

//static void xxeoprot_fun_INITIALISE_mc(uint32_t u, void* p)
//{
//    u = u;
//}

static void s_eoprot_robot_override_mc(void)
{
    // general ram initialise of mc endpoint called by every board.
    
    //eoprot_config_endpoint_callback(eoprot_endpoint_motioncontrol, xxeoprot_fun_INITIALISE_mc);
    
    
    // override of the callbacks of variables of mc. common to every board
    eOprotID32_t id = eoprot_ID_get(eoprot_endpoint_motioncontrol, eoprot_entity_mc_joint, 0, eoprot_tag_mc_joint_config_pidvelocity);
    eoprot_config_variable_callback(id, NULL, eoprot_fun_UPDT_mc_joint_config_pidvelocity);
    
    #warning --> mettere le callback appropriate
}

static void s_eoprot_robot_override_as(void)
{

}

static void s_eoprot_robot_override_sk(void)
{

}

#endif//defined(EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME)  

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





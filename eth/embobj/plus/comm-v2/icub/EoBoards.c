/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
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

/* @file       EoBoards.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       Apr 18 2016
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

#include "EoBoards.h"


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
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const uint64_t s_eoboards_is_eth_mask =  (0x1LL << eobrd_ems4) | 
                                                (0x1LL << eobrd_mc4plus) | 
                                                (0x1LL << eobrd_mc2plus) |
                                                (0x1LL << eobrd_amc) |
                                                (0x1LL << eobrd_amcfoc);

static const uint64_t s_eoboards_is_can_mask =  (0x1LL << eobrd_mc4) | 
                                                (0x1LL << eobrd_mtb) | 
                                                (0x1LL << eobrd_strain) |                                                
                                                (0x1LL << eobrd_mais) |
                                                (0x1LL << eobrd_foc) |
                                                (0x1LL << eobrd_dsp) |
                                                (0x1LL << eobrd_pic) |
                                                (0x1LL << eobrd_2dc) |
                                                (0x1LL << eobrd_bll) |
                                                (0x1LL << eobrd_6sg) |
                                                (0x1LL << eobrd_jog) |
                                                (0x1LL << eobrd_mtb4) |
                                                (0x1LL << eobrd_strain2) |
                                                (0x1LL << eobrd_rfe) |
                                                (0x1LL << eobrd_sg3) |
                                                (0x1LL << eobrd_psc) |
                                                (0x1LL << eobrd_mtb4w) |
                                                (0x1LL << eobrd_pmc)   |
                                                (0x1LL << eobrd_amcbldc)|
                                                (0x1LL << eobrd_bms)|
                                                (0x1LL << eobrd_mtb4c) |
                                                (0x1LL << eobrd_amc2c)|
                                                (0x1LL << eobrd_strain2c) | 
                                                (0x1LL << eobrd_bat) |
                                                (0x1LL << eobrd_amcfoc2c); 
       
   
static const eOmap_str_str_u08_t s_eoboards_map_of_boards[] =
{    
    {"ems4", "eobrd_ems4", eobrd_ems4},
    {"mc4plus", "eobrd_mc4plus", eobrd_mc4plus},
    {"mc2plus", "eobrd_mc2plus", eobrd_mc2plus},
    {"amc", "eobrd_amc", eobrd_amc},
    {"amcfoc", "eobrd_amcfoc", eobrd_amcfoc},
    
    {"mc4", "eobrd_mc4", eobrd_mc4},
    {"mtb", "eobrd_mtb", eobrd_mtb},
    {"strain", "eobrd_strain", eobrd_strain},
    {"mais", "eobrd_mais", eobrd_mais},
    {"foc", "eobrd_foc", eobrd_foc},
    
    {"dsp", "eobrd_dsp", eobrd_dsp},
    {"pic", "eobrd_pic", eobrd_pic},
    {"2dc", "eobrd_2dc", eobrd_2dc},
    {"bll", "eobrd_bll", eobrd_bll},    
    {"6sg", "eobrd_6sg", eobrd_6sg},
    {"jog", "eobrd_jog", eobrd_jog},
    {"mtb4", "eobrd_mtb4", eobrd_mtb4},
    {"strain2", "eobrd_strain2", eobrd_strain2},
    {"rfe", "eobrd_rfe", eobrd_rfe},
    {"sg3", "eobrd_sg3", eobrd_sg3},
    {"psc", "eobrd_psc", eobrd_psc},
    {"mtb4w", "eobrd_mtb4w", eobrd_mtb4w},
    {"pmc", "eobrd_pmc", eobrd_pmc},
    {"amcbldc", "eobrd_amcbldc", eobrd_amcbldc},
    {"bms", "eobrd_bms", eobrd_bms},
    {"mtb4c", "eobrd_mtb4c", eobrd_mtb4c},
    {"amc2c", "eobrd_amc2c", eobrd_amc2c},
    {"strain2c", "eobrd_strain2c", eobrd_strain2c},
    {"bat", "eobrd_bat", eobrd_bat},
    {"amcfoc2c", "eobrd_amcfoc2c", eobrd_amcfoc2c},
    
    {"none", "eobrd_none", eobrd_none},
    {"unknown", "eobrd_unknown", eobrd_unknown}
};  EO_VERIFYsizeof(s_eoboards_map_of_boards, (eobrd_type_numberof+2)*sizeof(eOmap_str_str_u08_t))

// just to check that the number of can type board is lower than 32 otherwise we go up to the ems.
// if that happens, the can boards will have a hole between [32, 63] ...
static const uint8_t xxx[eobrd_ems4-eobrd_cantype_numberof+1] = {0};
    


static const eOmap_str_str_u08_t s_eoboards_map_of_connectors[] =
{    
    {"P1", "eobrd_conn_P1",eobrd_conn_P1},
    {"P2", "eobrd_conn_P2", eobrd_conn_P2},
    {"P3", "eobrd_conn_P3", eobrd_conn_P3},
    {"P4", "eobrd_conn_P4", eobrd_conn_P4},
    {"P5", "eobrd_conn_P5", eobrd_conn_P5},
    {"P6", "eobrd_conn_P6", eobrd_conn_P6},
    {"P7", "eobrd_conn_P7", eobrd_conn_P7},
    {"P8", "eobrd_conn_P8", eobrd_conn_P8},
    {"P9", "eobrd_conn_P9", eobrd_conn_P9},
    {"P10", "eobrd_conn_P10", eobrd_conn_P10},
    {"P11", "eobrd_conn_P11", eobrd_conn_P11},
    {"P12", "eobrd_conn_P12", eobrd_conn_P12},
    {"P13", "eobrd_conn_P13", eobrd_conn_P13},
    {"P14", "eobrd_conn_P14", eobrd_conn_P14},
    {"P15", "eobrd_conn_P15", eobrd_conn_P15},
    {"J5_X1", "eobrd_conn_J5_X1", eobrd_conn_J5_X1},
    {"J5_X2", "eobrd_conn_J5_X2", eobrd_conn_J5_X2},
    {"J5_X3", "eobrd_conn_J5_X3", eobrd_conn_J5_X3},
    {"J3_SDA0", "eobrd_conn_J3_SDA0", eobrd_conn_J3_SDA0},
    {"J3_SDA1", "eobrd_conn_J3_SDA1", eobrd_conn_J3_SDA1},
    {"J3_SDA2", "eobrd_conn_J3_SDA2", eobrd_conn_J3_SDA2},
    {"J3_SDA3", "eobrd_conn_J3_SDA3", eobrd_conn_J3_SDA3},
    {"J4", "eobrd_conn_J4", eobrd_conn_J4},
    {"J5", "eobrd_conn_J5", eobrd_conn_J5},
    {"J6", "eobrd_conn_J6", eobrd_conn_J6},
    {"J7", "eobrd_conn_J7", eobrd_conn_J7},

    {"J20", "eobrd_conn_J20", eobrd_conn_J20},
    {"J21", "eobrd_conn_J21", eobrd_conn_J21},
    {"J22", "eobrd_conn_J22", eobrd_conn_J22},
    {"J23", "eobrd_conn_J23", eobrd_conn_J23},
    
    {"J30", "eobrd_conn_J30", eobrd_conn_J30},
    {"J31", "eobrd_conn_J31", eobrd_conn_J31},
    {"J32", "eobrd_conn_J32", eobrd_conn_J32},
    {"J33", "eobrd_conn_J33", eobrd_conn_J33},
    {"P3P", "eobrd_conn_P3P", eobrd_conn_P3P},
    {"P4P", "eobrd_conn_P4P", eobrd_conn_P4P},

    {"J11", "eobrd_conn_J11", eobrd_conn_J11},
    {"J12", "eobrd_conn_J12", eobrd_conn_J12},

    {"none", "eobrd_conn_none", eobrd_conn_none},
    {"unknown", "eobrd_conn_unknown", eobrd_conn_unknown}
};  EO_VERIFYsizeof(s_eoboards_map_of_connectors, (eobrd_connectors_numberof+2)*sizeof(eOmap_str_str_u08_t))


static const eOmap_str_str_u08_u08_u08_t s_eoboards_map_of_ports[] =
{//  shortname  longname          port value        boardtype   connector  
    {"emsP6", "eobrd_port_emsP6", eobrd_port_emsP6, eobrd_ems4, eobrd_conn_P6},
    {"emsP7", "eobrd_port_emsP7", eobrd_port_emsP7, eobrd_ems4, eobrd_conn_P7},
    {"emsP8", "eobrd_port_emsP8", eobrd_port_emsP8, eobrd_ems4, eobrd_conn_P8},
    {"emsP9", "eobrd_port_emsP9", eobrd_port_emsP9, eobrd_ems4, eobrd_conn_P9},
    {"emsP10", "eobrd_port_emsP10", eobrd_port_emsP10, eobrd_ems4, eobrd_conn_P10},
    {"emsP11", "eobrd_port_emsP11", eobrd_port_emsP11, eobrd_ems4, eobrd_conn_P11},
    
    {"mc4plusP2", "eobrd_port_mc4plusP2", eobrd_port_mc4plusP2, eobrd_mc4plus, eobrd_conn_P2},
    {"mc4plusP3", "eobrd_port_mc4plusP3", eobrd_port_mc4plusP3, eobrd_mc4plus, eobrd_conn_P3},
    {"mc4plusP4", "eobrd_port_mc4plusP4", eobrd_port_mc4plusP4, eobrd_mc4plus, eobrd_conn_P4},
    {"mc4plusP5", "eobrd_port_mc4plusP5", eobrd_port_mc4plusP5, eobrd_mc4plus, eobrd_conn_P5},
    {"mc4plusP3P", "eobrd_port_mc4plusP3P", eobrd_port_mc4plusP3P, eobrd_mc4plus, eobrd_conn_P3P},
    {"mc4plusP4P", "eobrd_port_mc4plusP4P", eobrd_port_mc4plusP4P, eobrd_mc4plus, eobrd_conn_P4P},    
    {"mc4plusP10", "eobrd_port_mc4plusP10", eobrd_port_mc4plusP10, eobrd_mc4plus, eobrd_conn_P10},
    {"mc4plusP11", "eobrd_port_mc4plusP11", eobrd_port_mc4plusP11, eobrd_mc4plus, eobrd_conn_P11},
    
    {"mc2plusP2", "eobrd_port_mc2plusP2", eobrd_port_mc2plusP2, eobrd_mc2plus, eobrd_conn_P2},
    {"mc2plusP3", "eobrd_port_mc2plusP3", eobrd_port_mc2plusP3, eobrd_mc2plus, eobrd_conn_P3},
    {"mc2plusP10", "eobrd_port_mc2plusP10", eobrd_port_mc2plusP10, eobrd_mc2plus, eobrd_conn_P10},
    {"mc2plusP11", "eobrd_port_mc2plusP11", eobrd_port_mc2plusP11, eobrd_mc2plus, eobrd_conn_P11},

    {"amcJ5_X1", "eobrd_port_amc_J5_X1", eobrd_port_amc_J5_X1, eobrd_amc, eobrd_conn_J5_X1},
    {"amcJ5_X2", "eobrd_port_amc_J5_X2", eobrd_port_amc_J5_X2, eobrd_amc, eobrd_conn_J5_X2},
    {"amcJ5_X3", "eobrd_port_amc_J5_X3", eobrd_port_amc_J5_X3, eobrd_amc, eobrd_conn_J5_X3},
    
    {"mtb4_J3_SDA0", "eobrd_port_mtb4_J3_SDA0", eobrd_port_mtb4_J3_SDA0, eobrd_mtb4, eobrd_conn_J3_SDA0},
    {"mtb4_J3_SDA1", "eobrd_port_mtb4_J3_SDA1", eobrd_port_mtb4_J3_SDA1, eobrd_mtb4, eobrd_conn_J3_SDA1},
    {"mtb4_J3_SDA2", "eobrd_port_mtb4_J3_SDA2", eobrd_port_mtb4_J3_SDA2, eobrd_mtb4, eobrd_conn_J3_SDA2},
    {"mtb4_J3_SDA3", "eobrd_port_mtb4_J3_SDA3", eobrd_port_mtb4_J3_SDA3, eobrd_mtb4, eobrd_conn_J3_SDA3},
    
    {"mtb4c_J3_SDA0", "eobrd_port_mtb4c_J3_SDA0", eobrd_port_mtb4c_J3_SDA0, eobrd_mtb4c, eobrd_conn_J3_SDA0},
    {"mtb4c_J3_SDA1", "eobrd_port_mtb4c_J3_SDA1", eobrd_port_mtb4c_J3_SDA1, eobrd_mtb4c, eobrd_conn_J3_SDA1},
    {"mtb4c_J3_SDA2", "eobrd_port_mtb4c_J3_SDA2", eobrd_port_mtb4c_J3_SDA2, eobrd_mtb4c, eobrd_conn_J3_SDA2},
    {"mtb4c_J3_SDA3", "eobrd_port_mtb4c_J3_SDA3", eobrd_port_mtb4c_J3_SDA3, eobrd_mtb4c, eobrd_conn_J3_SDA3},

    {"pmc_J4", "eobrd_port_pmc_J4", eobrd_port_pmc_J4, eobrd_pmc, eobrd_conn_J4},
    {"pmc_J5", "eobrd_port_pmc_J5", eobrd_port_pmc_J5, eobrd_pmc, eobrd_conn_J5},
    {"pmc_J6", "eobrd_port_pmc_J6", eobrd_port_pmc_J6, eobrd_pmc, eobrd_conn_J6},
    {"pmc_J7", "eobrd_port_pmc_J7", eobrd_port_pmc_J7, eobrd_pmc, eobrd_conn_J7},
        
    {"mtb4_mmaJ20", "eobrd_port_mtb4_mmaJ20", eobrd_port_mtb4_mmaJ20, eobrd_mtb4, eobrd_conn_J20},
    {"mtb4_mmaJ21", "eobrd_port_mtb4_mmaJ21", eobrd_port_mtb4_mmaJ21, eobrd_mtb4, eobrd_conn_J21},
    {"mtb4_mmaJ22", "eobrd_port_mtb4_mmaJ22", eobrd_port_mtb4_mmaJ22, eobrd_mtb4, eobrd_conn_J22},
    {"mtb4_mmaJ23", "eobrd_port_mtb4_mmaJ23", eobrd_port_mtb4_mmaJ23, eobrd_mtb4, eobrd_conn_J23},

    {"mtb4c_mmaJ20", "eobrd_port_mtb4c_mmaJ20", eobrd_port_mtb4c_mmaJ20, eobrd_mtb4c, eobrd_conn_J20},
    {"mtb4c_mmaJ21", "eobrd_port_mtb4c_mmaJ21", eobrd_port_mtb4c_mmaJ21, eobrd_mtb4c, eobrd_conn_J21},
    {"mtb4c_mmaJ22", "eobrd_port_mtb4c_mmaJ22", eobrd_port_mtb4c_mmaJ22, eobrd_mtb4c, eobrd_conn_J22},
    {"mtb4c_mmaJ23", "eobrd_port_mtb4c_mmaJ23", eobrd_port_mtb4c_mmaJ23, eobrd_mtb4c, eobrd_conn_J23},
    
    {"mtb4_mmaJ30", "eobrd_port_mtb4_mmaJ30", eobrd_port_mtb4_mmaJ30, eobrd_mtb4, eobrd_conn_J30},
    {"mtb4_mmaJ31", "eobrd_port_mtb4_mmaJ31", eobrd_port_mtb4_mmaJ31, eobrd_mtb4, eobrd_conn_J31},
    {"mtb4_mmaJ32", "eobrd_port_mtb4_mmaJ32", eobrd_port_mtb4_mmaJ32, eobrd_mtb4, eobrd_conn_J32},
    {"mtb4_mmaJ33", "eobrd_port_mtb4_mmaJ33", eobrd_port_mtb4_mmaJ33, eobrd_mtb4, eobrd_conn_J33},

    {"mtb4c_mmaJ30", "eobrd_port_mtb4c_mmaJ30", eobrd_port_mtb4c_mmaJ30, eobrd_mtb4c, eobrd_conn_J30},
    {"mtb4c_mmaJ31", "eobrd_port_mtb4c_mmaJ31", eobrd_port_mtb4c_mmaJ31, eobrd_mtb4c, eobrd_conn_J31},
    {"mtb4c_mmaJ32", "eobrd_port_mtb4c_mmaJ32", eobrd_port_mtb4c_mmaJ32, eobrd_mtb4c, eobrd_conn_J32},
    {"mtb4c_mmaJ33", "eobrd_port_mtb4c_mmaJ33", eobrd_port_mtb4c_mmaJ33, eobrd_mtb4c, eobrd_conn_J33},

    {"amcfocJ11", "eobrd_port_amcfoc_J11", eobrd_port_amcfoc_J11, eobrd_amcfoc, eobrd_conn_J11},
    {"amcfocJ12", "eobrd_port_amcfoc_J12", eobrd_port_amcfoc_J12, eobrd_amcfoc, eobrd_conn_J12},
    
    {"none", "eobrd_port_none", eobrd_port_none, eobrd_none, eobrd_conn_none},
    {"unknown", "eobrd_port_unknown", eobrd_port_unknown, eobrd_unknown, eobrd_conn_unknown}
};  EO_VERIFYsizeof(s_eoboards_map_of_ports, (eobrd_ports_numberof+2)*sizeof(eOmap_str_str_u08_u08_u08_t))


static const eOmap_str_str_u08_t s_boards_map_of_portmaiss[] =
{
    {"thumbproximal", "eobrd_portmais_thumbproximal", eobrd_portmais_thumbproximal},
    {"thumbdistal", "eobrd_portmais_thumbdistal", eobrd_portmais_thumbdistal},
    {"indexproximal", "eobrd_portmais_indexproximal", eobrd_portmais_indexproximal},
    {"indexdistal", "eobrd_portmais_indexdistal", eobrd_portmais_indexdistal},
    {"mediumproximal", "eobrd_portmais_mediumproximal", eobrd_portmais_mediumproximal},
    {"mediumdistal", "eobrd_portmais_mediumdistal", eobrd_portmais_mediumdistal},
    {"littlefingers", "eobrd_portmais_littlefingers", eobrd_portmais_littlefingers},
    
    {"none", "eobrd_portmais_none", eobrd_portmais_none},
    {"unknown", "eobrd_portmais_unknown", eobrd_portmais_unknown}    
};  EO_VERIFYsizeof(s_boards_map_of_portmaiss, (eobrd_portmaiss_numberof+2)*sizeof(eOmap_str_str_u08_t))


static const eOmap_str_str_u08_t s_boards_map_of_portpscs[] =
{
    {"finger0", "eobrd_portpsc_finger0", eobrd_portpsc_finger0},
    {"finger1", "eobrd_portpsc_finger1", eobrd_portpsc_finger1},
    
    {"none", "eobrd_portpsc_none", eobrd_portpsc_none},
    {"unknown", "eobrd_portpsc_unknown", eobrd_portpsc_unknown}    
};  EO_VERIFYsizeof(s_boards_map_of_portpscs, (eobrd_portpscs_numberof+2)*sizeof(eOmap_str_str_u08_t))


static const eOmap_str_str_u08_t s_boards_map_of_portposs[] =
{
    {"hand_thumb_oc", "eobrd_portpos_hand_thumb_oc", eobrd_portpos_hand_thumb_oc},
    {"hand_index_oc", "eobrd_portpos_hand_index_oc", eobrd_portpos_hand_index_oc},
    {"hand_middle_oc", "eobrd_portpos_hand_middle_oc", eobrd_portpos_hand_middle_oc},
    {"hand_ring_pinky_oc", "eobrd_portpos_hand_ring_pinky_oc", eobrd_portpos_hand_ring_pinky_oc},
    {"hand_thumb_add", "eobrd_portpos_hand_thumb_add", eobrd_portpos_hand_thumb_add},
    {"hand_tbd", "eobrd_portpos_hand_tbd", eobrd_portpos_hand_tbd},
    {"hand_index_add", "eobrd_portpos_hand_index_add", eobrd_portpos_hand_index_add},
    
    {"none", "eobrd_portpos_none", eobrd_portpos_none},
    {"unknown", "eobrd_portpos_unknown", eobrd_portpos_unknown}    
};  EO_VERIFYsizeof(s_boards_map_of_portposs, (eobrd_portposs_numberof+2)*sizeof(eOmap_str_str_u08_t))


static const eOmap_str_str_u08_t s_boards_map_of_reportmodes[] =
{
    {"NEVER", "eobrd_canmonitor_reportmode_NEVER", eobrd_canmonitor_reportmode_NEVER},
    {"justLOSTjustFOUND", "eobrd_canmonitor_reportmode_justLOSTjustFOUND", eobrd_canmonitor_reportmode_justLOSTjustFOUND},
    {"justLOSTjustFOUNDstillLOST", "eobrd_canmonitor_reportmode_justLOSTjustFOUNDstillLOST", eobrd_canmonitor_reportmode_justLOSTjustFOUNDstillLOST},
    {"ALL", "eobrd_canmonitor_reportmode_ALL", eobrd_canmonitor_reportmode_ALL},

    {"none", "eobrd_canmonitor_reportmode_none", eobrd_canmonitor_reportmode_none},
    {"unknown", "eobrd_canmonitor_reportmode_unknown", eobrd_canmonitor_reportmode_unknown}    
};  EO_VERIFYsizeof(s_boards_map_of_reportmodes, (eobrd_reportmodes_numberof+2)*sizeof(eOmap_str_str_u08_t))


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern eObool_t eoboards_is_can(eObrd_type_t type)
{
    if(type > 63)
    {
        return(eobool_false);
    }
 
    return(eo_common_dword_bitcheck(s_eoboards_is_can_mask, type));      
}


extern eObool_t eoboards_is_eth(eObrd_type_t type)
{
    if(type > 63)
    {
        return(eobool_false);
    }
 
    return(eo_common_dword_bitcheck(s_eoboards_is_eth_mask, type));       
}


extern eObrd_cantype_t eoboards_type2cantype(eObrd_type_t type)
{
    if(eobool_true == eoboards_is_can(type))
    {
        return((eObrd_cantype_t)type);
    }  

    if(eobrd_none == type)
    {
        return(eobrd_cantype_none);
    }
    
    return(eobrd_cantype_unknown);
}


extern eObrd_ethtype_t eoboards_type2ethtype(eObrd_type_t type)
{
    if(eobool_true == eoboards_is_eth(type))
    {
        return((eObrd_ethtype_t)type);
    }  

    if(eobrd_none == type)
    {
        return(eobrd_ethtype_none);
    }
    
    return(eobrd_ethtype_unknown);
}

extern eObrd_type_t eoboards_cantype2type(eObrd_cantype_t type)
{
    return((eObrd_type_t)type);
}

extern eObrd_type_t eoboards_ethtype2type(eObrd_ethtype_t type)
{
    return((eObrd_type_t)type);
}


extern eObrd_type_t eoboards_string2type(const char * name)
{    
    return(eoboards_string2type2(name, eobool_false));
}


extern const char * eoboards_type2string(eObrd_type_t type)
{
    return(eoboards_type2string2(type, eobool_false));
}


extern eObrd_type_t eoboards_string2type2(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eoboards_map_of_boards;
    const uint8_t size = eobrd_type_numberof+2;
    const uint8_t defvalue = eobrd_unknown;
    
    return((eObrd_type_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));    
}


extern const char * eoboards_type2string2(eObrd_type_t type, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eoboards_map_of_boards;
    const uint8_t size = eobrd_type_numberof+2;
    const uint8_t value = type;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str); 
}


extern eObrd_connector_t eoboards_string2connector(const char * string, eObool_t usecompactstring)
{    
    const eOmap_str_str_u08_t * map = s_eoboards_map_of_connectors;
    const uint8_t size = eobrd_connectors_numberof+2;
    const uint8_t defvalue = eobrd_conn_unknown;
    
    return((eObrd_connector_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));
}


extern const char * eoboards_connector2string(eObrd_connector_t connector, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eoboards_map_of_connectors;
    const uint8_t size = eobrd_connectors_numberof+2;
    const uint8_t value = connector;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str); 
}


extern eObrd_port_t eoboards_string2port(const char * string, eObool_t usecompactstring)
{    
    const eOmap_str_str_u08_u08_u08_t * map = s_eoboards_map_of_ports;
    const uint8_t size = eobrd_ports_numberof+2;
    const uint8_t defvalue = eobrd_port_unknown;    
    
    uint8_t i = 0;    
    if(NULL == string)
    {
        return((eObrd_port_t)defvalue);
    }
      
    for(i=0; i<size; i++)
    {
        const char * str = (eobool_true == usecompactstring) ? map[i].str0 : map[i].str1;
        if(0 == strcmp(string, str))
        {
            return((eObrd_port_t)map[i].val0);
        }
    }
    
    return((eObrd_port_t)defvalue);       
}


extern const char * eoboards_port2string(eObrd_port_t port, eObrd_type_t board, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_u08_u08_t * map = s_eoboards_map_of_ports;
    const uint8_t size = eobrd_ports_numberof+2;
    const uint8_t value0 = port;
    const uint8_t value1 = board;
    uint8_t i = 0;

    if((eobrd_port_none == port) || (eobrd_none == board))
    {   // force the be equal _none
        port = eobrd_port_none;
        board = eobrd_none;
    }

    if((eobrd_port_unknown == port) || (eobrd_unknown == board))
    {   // force to be equal _unknown
        port = eobrd_port_unknown;
        board = eobrd_unknown;
    }
    
    for(i=0; i<size; i++)
    {
        if((value0 == map[i].val0) && (value1 == map[i].val1))
        {
            const char * str = (eobool_true == usecompactstring) ? map[i].str0 : map[i].str1;
            return(str);
        }
    }    
    
    return((eobool_true == usecompactstring) ? map[size-1].str0 : map[size-1].str1);   
}


extern eObrd_port_t eoboards_connector2port(eObrd_connector_t connector, eObrd_type_t board)
{
    const eOmap_str_str_u08_u08_u08_t * map = s_eoboards_map_of_ports;
    const uint8_t size = eobrd_ports_numberof+2;
    const uint8_t defvalue = eobrd_port_unknown;
    uint8_t i = 0;

    if((eobrd_conn_none == connector) || (eobrd_none == board))
    {   // force to be equal to _none
        connector = eobrd_conn_none;
        board = eobrd_none;
    }

    if((eobrd_conn_unknown == connector) || (eobrd_unknown == board))
    {   // force to be equal to _unknown
        connector = eobrd_conn_unknown;
        board = eobrd_unknown;
    }
    
    for(i=0; i<size; i++)
    {
        if((board == map[i].val1) && (connector == map[i].val2))
        {
            return((eObrd_port_t)map[i].val0);
        }
    }
    
    return((eObrd_port_t)defvalue);               
}


extern eObrd_connector_t eoboards_port2connector(eObrd_port_t port, eObrd_type_t board)
{
    const eOmap_str_str_u08_u08_u08_t * map = s_eoboards_map_of_ports;
    const uint8_t size = eobrd_ports_numberof+2;
    const uint8_t defvalue = eobrd_conn_unknown;
    uint8_t i = 0;

    if((eobrd_port_none == port) || (eobrd_none == board))
    {   // force to be equal to _none
        port = eobrd_port_none;
        board = eobrd_none;
    }

    if((eobrd_port_unknown == port) || (eobrd_unknown == board))
    {   // force to be equal to _unknown
        port = eobrd_port_unknown;
        board = eobrd_unknown;
    }
    
        
    for(i=0; i<size; i++)
    {
        if((board == map[i].val1) && (port == map[i].val0))
        {
            return((eObrd_connector_t)map[i].val2);
        }
    }
    
    return((eObrd_connector_t)defvalue);           
}


extern const char * eoboards_portmais2string(eObrd_portmais_t portmais, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portmaiss;
    const uint8_t size = eobrd_portmaiss_numberof+2;
    const uint8_t value = portmais;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   
}


extern eObrd_portmais_t eoboards_string2portmais(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portmaiss;
    const uint8_t size = eobrd_portmaiss_numberof+2;
    const uint8_t defvalue = eobrd_portmais_unknown;
    
    return((eObrd_portmais_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));        
}


extern const char * eoboards_portpsc2string(eObrd_portpsc_t portpsc, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portpscs;
    const uint8_t size = eobrd_portpscs_numberof+2;
    const uint8_t value = portpsc;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   
}


extern eObrd_portpsc_t eoboards_string2portpsc(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portpscs;
    const uint8_t size = eobrd_portpscs_numberof+2;
    const uint8_t defvalue = eobrd_portpsc_unknown;
    
    return((eObrd_portpsc_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));        
}


extern const char * eoboards_portpos2string(eObrd_portpos_t portpos, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portposs;
    const uint8_t size = eobrd_portposs_numberof+2;
    const uint8_t value = portpos;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   
}


extern eObrd_portpos_t eoboards_string2portpos(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_portposs;
    const uint8_t size = eobrd_portposs_numberof+2;
    const uint8_t defvalue = eobrd_portpos_unknown;
    
    return((eObrd_portpos_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));        
}

extern const char * eoboards_reportmode2string(eObrd_canmonitor_reportmode_t mode, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_reportmodes;
    const uint8_t size = eobrd_reportmodes_numberof+2;
    const uint8_t value = mode;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   
}


extern eObrd_canmonitor_reportmode_t eoboards_string2reportmode(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_reportmodes;
    const uint8_t size = eobrd_reportmodes_numberof+2;
    const uint8_t defvalue = eobrd_canmonitor_reportmode_unknown;
    
    return((eObrd_canmonitor_reportmode_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));        
}

extern uint8_t eoboards_type2numberofcores(eObrd_type_t type)
{
    return ((eobrd_amc == type) || (eobrd_amcfoc == type)) ? 2 : 1;
}


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




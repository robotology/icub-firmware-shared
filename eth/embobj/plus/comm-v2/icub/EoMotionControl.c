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

/* @file       EoMotionControl.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       Jul 19 2016
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

#include "EoMotionControl.h"


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



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const eOmap_str_str_u08_t s_eomc_map_of_actuators[] =
{    
    {"foc", "eomc_act_foc", eomc_act_foc},
    {"mc4", "eomc_act_mc4", eomc_act_mc4},
    {"pwm", "eomc_act_pwm", eomc_act_pwm},

    {"none", "eomc_act_none", eomc_act_none},
    {"unknown", "eomc_act_unknown", eomc_act_unknown}
};  EO_VERIFYsizeof(s_eomc_map_of_actuators, (eomc_actuators_numberof+2)*sizeof(eOmap_str_str_u08_t));


static const eOmap_str_str_u08_t s_eomc_map_of_encoders[] =
{    
    {"aea", "eomc_enc_aea",eomc_enc_aea},
    {"amo", "eomc_enc_amo", eomc_enc_amo},
    {"qenc", "eomc_enc_qenc", eomc_enc_qenc},
    {"spichainof2", "eomc_enc_spichainof2", eomc_enc_spichainof2},
    {"absanalog", "eomc_enc_absanalog", eomc_enc_absanalog},
    {"mais", "eomc_enc_mais", eomc_enc_mais},
    {"spichainof3", "eomc_enc_spichainof3", eomc_enc_spichainof3},
    {"onfoc", "eomc_enc_onfoc", eomc_enc_onfoc},

    {"none", "eomc_enc_none", eomc_enc_none},
    {"unknown", "eomc_enc_unknown", eomc_enc_unknown}
};  EO_VERIFYsizeof(s_eomc_map_of_encoders, (eomc_encoders_numberof+2)*sizeof(eOmap_str_str_u08_t));


static const eOmap_str_str_u08_t s_eomc_map_of_positions[] =
{    
    {"atjoint", "eomc_pos_atjoint", eomc_pos_atjoint},
    {"atmotor", "eomc_pos_atmotor", eomc_pos_atmotor},
    
    {"none", "eomc_pos_none", eomc_pos_none},
    {"unknown", "eomc_pos_unknown", eomc_pos_unknown}
};  EO_VERIFYsizeof(s_eomc_map_of_positions, (eomc_positions_numberof+2)*sizeof(eOmap_str_str_u08_t));


static const eOmap_str_str_u08_t s_eomc_map_of_maisvalues[] =
{
    {"thumbproximal", "eomc_maisvalue_thumbproximal", eomc_maisvalue_thumbproximal},
    {"thumbdistal", "eomc_maisvalue_thumbdistal", eomc_maisvalue_thumbdistal},
    {"indexproximal", "eomc_maisvalue_indexproximal", eomc_maisvalue_indexproximal},
    {"indexdistal", "eomc_maisvalue_indexdistal", eomc_maisvalue_indexdistal},
    {"mediumproximal", "eomc_maisvalue_mediumproximal", eomc_maisvalue_mediumproximal},
    {"mediumdistal", "eomc_maisvalue_mediumdistal", eomc_maisvalue_mediumdistal},
    {"littlefingers", "eomc_maisvalue_littlefingers", eomc_maisvalue_littlefingers},
    
    {"none", "eomc_maisvalue_none", eomc_maisvalue_none},
    {"unknown", "eomc_maisvalue_unknown", eomc_maisvalue_unknown}    
};  EO_VERIFYsizeof(s_eomc_map_of_maisvalues, (eomc_maisvalues_numberof+2)*sizeof(eOmap_str_str_u08_t));
    


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern const char * eomc_actuator2string(eOmc_actuator_t actuator, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_actuators;
    const uint8_t size = eomc_actuators_numberof+2;
    const uint8_t value = actuator;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);    
}


extern eOmc_actuator_t eomc_string2actuator(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_actuators;
    const uint8_t size = eomc_actuators_numberof+2;
    const uint8_t defvalue = eomc_act_unknown;
    
    return((eOmc_actuator_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));
}


extern const char * eomc_encoder2string(eOmc_encoder_t encoder, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_encoders;
    const uint8_t size = eomc_encoders_numberof+2;
    const uint8_t value = encoder;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);    
}


extern eOmc_encoder_t eomc_string2encoder(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_encoders;
    const uint8_t size = eomc_encoders_numberof+2;
    const uint8_t defvalue = eomc_enc_unknown;
    
    return((eOmc_encoder_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));
}


extern const char * eomc_position2string(eOmc_position_t position, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_positions;
    const uint8_t size = eomc_positions_numberof+2;
    const uint8_t value = position;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   
}


extern eOmc_position_t eomc_string2position(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_positions;
    const uint8_t size = eomc_positions_numberof+2;
    const uint8_t defvalue = eomc_pos_unknown; 
    
    return((eOmc_position_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));    
}


extern const char * eomc_maisvalue2string(eOmc_maisvalue_t maisvalue, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_maisvalues;
    const uint8_t size = eomc_maisvalues_numberof+2;
    const uint8_t value = maisvalue;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);   

}


extern eOmc_maisvalue_t eomc_string2maisvalue(const char * string, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_eomc_map_of_maisvalues;
    const uint8_t size = eomc_maisvalues_numberof+2;
    const uint8_t defvalue = eomc_maisvalue_unknown;
    
    return((eOmc_maisvalue_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));        
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




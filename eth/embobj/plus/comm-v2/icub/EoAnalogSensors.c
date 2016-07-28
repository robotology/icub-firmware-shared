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

/* @file       EoAnalogSensors.c
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

#include "EoAnalogSensors.h"


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

enum { eoas_others_numberof = 2 };

// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char * s_eoas_sensors_strings[] =
{
    "eoas_strain",
    "eoas_mais",
    "eoas_accel_mtb_int",
    "eoas_accel_mtb_ext",
    "eoas_gyros_mtb_ext",   
    "eoas_accel_st_lis3x",    
    "eoas_gyros_st_l3g4200d"
};  EO_VERIFYsizeof(s_eoas_sensors_strings, eoas_sensors_numberof*sizeof(const char *));    



static const char * s_eoas_sensors_string_unknown = "eoas_unknown";

static const char * s_eoas_sensors_string_none = "eoas_none";


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------



extern const char * eoas_sensor2string(eOas_sensor_t sensor)
{
    const char * ret = s_eoas_sensors_string_unknown;
    
    if(sensor < eoas_sensors_numberof)
    {
        return(s_eoas_sensors_strings[sensor]);
    }
    else if(eoas_none == sensor)
    {
        return(s_eoas_sensors_string_none);
    }


    return(ret);
}


extern eOas_sensor_t eoas_string2sensor(const char * string)
{    
    uint8_t i = 0;
    if(NULL == string)
    {
        return(eoas_unknown);
    }
    
    for(i=0; i<eoas_sensors_numberof; i++)
    {
        if(0 == strcmp(string, s_eoas_sensors_strings[i]))
        {
            return((eOas_sensor_t)(i+0));
        }
    }
    
    if(0 == strcmp(string, s_eoas_sensors_string_none))
    {
        return(eoas_none);
    }        
    
    return(eoas_unknown);    
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




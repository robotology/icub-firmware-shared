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

/* @file       EoManagement.c
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

#include "EoManagement.h"


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

static const char * s_mn_servicetype_strings[] =
{
    "eomn_serv_MC_generic",
    "eomn_serv_MC_foc",
    "eomn_serv_MC_mc4",
    "eomn_serv_MC_mc4plus",
    "eomn_serv_MC_mc4plusmais",   
    "eomn_serv_AS_mais",    
    "eomn_serv_AS_strain",
    "eomn_serv_AS_inertials",
    "eomn_serv_SK_skin",
    "eomn_serv_AS_inertials3",
    "eomn_serv_AS_temperature",
    "eomn_serv_MC_mc2plus", 
    "eomn_serv_MC_mc2pluspsc",
    "eomn_serv_AS_psc",
    "eomn_serv_AS_pos",
    "eomn_serv_MC_mc4plusfaps",
    "eomn_serv_MC_mc4pluspmc"
};  EO_VERIFYsizeof(s_mn_servicetype_strings, eomn_serv_types_numberof*sizeof(const char *))   
 
static const char * s_mn_servicetype_string_unknown = "eomn_serv_UNKNOWN";
static const char * s_mn_servicetype_string_none = "eomn_serv_NONE";


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern const char * eomn_servicetype2string(eOmn_serv_type_t service)
{
    const char * ret = s_mn_servicetype_string_unknown;
    
    if(service < eomn_serv_types_numberof)
    {
        return(s_mn_servicetype_strings[service]);
    }
    else if(eomn_serv_NONE == service)
    {
        return(s_mn_servicetype_string_none);
    }

    return(ret);
}

extern eOmn_serv_type_t eomn_string2servicetype(const char * string)
{     
    uint8_t i = 0;   
    
    if(NULL == string)
    {
        return(eomn_serv_UNKNOWN);
    }
      
    for(i=0; i<eomn_serv_types_numberof; i++)
    {
        if(0 == strcmp(string, s_mn_servicetype_strings[i]))
        {
            return((eOmn_serv_type_t)(i+0));
        }
    }
    
    if(0 == strcmp(string, s_mn_servicetype_string_none))
    {
        return(eomn_serv_NONE);
    }        
    
    return(eomn_serv_UNKNOWN);    
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




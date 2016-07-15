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
    "eomn_serv_SK_skin"
};  EO_VERIFYsizeof(s_mn_servicetype_strings, eomn_serv_types_numberof*sizeof(const char *));    


static const char * s_mn_controllertype_strings[] =
{
    "eomn_servMC_controller_NO_CONTROL", //0
    "eomn_servMC_controller_ANKLE",      //1
    "eomn_servMC_controller_UPPERLEG",   //2
    "eomn_servMC_controller_WAIST",      //3
    "eomn_servMC_controller_SHOULDER",   //4
    "eomn_servMC_controller_HEAD_neckpitch_neckroll", //5
    "eomn_servMC_controller_HEAD_neckyaw_eyes",      //6
    "eomn_servMC_controller_FACE_eyelids_jaw",       //7
    "eomn_servMC_controller_4jointsNotCoupled",      //8
    "eomn_servMC_controller_HAND_thumb",             //9
    "eomn_servMC_controller_HAND_2",                 //10
    "eomn_servMC_controller_FOREARM",                //11
    //CER
    "eomn_servMC_controller_CER_LOWER_ARM",         //12
    "",                                            //13
    "eomn_servMC_controller_CER_HAND",              //14
    "eomn_servMC_controller_CER_WAIST",             //15
    "eomn_servMC_controller_DONTCARE",              //16
    "eomn_servMC_controller_CER_UPPER_ARM",         //17
    "", //18
    "", //19
    "", //20
    "eomn_servMC_controller_CER_BASE",   //21
    "eomn_servMC_controller_CER_NECK"    //22

};  EO_VERIFYsizeof(s_mn_controllertype_strings, eomn_controller_type_numberof*sizeof(const char *));    




static const char * s_mn_mcsensor_strings[] =
{   // values can be from 1 to 255. 0 is none
    "NONE", // eomn_serv_mc_sensor_none                = eomn_serv_item_none,
    "AEA",  //eomn_serv_mc_sensor_encoder_aea         = 1,
    "AMO",  //eomn_serv_mc_sensor_encoder_amo         = 2,
    "ÏNC", //eomn_serv_mc_sensor_encoder_inc         = 3,
    "SPICHAINOF2", //eomn_serv_mc_sensor_encoder_spichainof2 = 4,
    "ABSANALOG", //eomn_serv_mc_sensor_encoder_absanalog   = 5,
    "MAIS", //eomn_serv_mc_sensor_mais                = 6,
    "SPICHAINOF3", //eomn_serv_mc_sensor_encoder_spichainof3 = 7,
    "ONFOC", //eomn_serv_mc_sensor_encoder_onfoc       = 8,
}; EO_VERIFYsizeof(s_mn_mcsensor_strings, eOmn_serv_mc_sensor_type_numberof*sizeof(const char *));    

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
    if(NULL == string)
    {
        return(eomn_serv_UNKNOWN);
    }
    
    uint8_t i = 0;
    
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



extern eOmn_serv_mc_sensor_type_t eomn_string2mcsensortype(const char * string)
{    
    if(NULL == string)
    {
        return(eomn_serv_mc_sensor_unknown);
    }
    
    uint8_t i = 0;
    
    for(i=0; i<eOmn_serv_mc_sensor_type_numberof; i++)
    {
        if(0 == strcmp(string, s_mn_mcsensor_strings[i]))
        {
            return((eOmn_serv_mc_sensor_type_t)(i+0));
        }
    }
      
    return(eomn_serv_mc_sensor_unknown);    
}



extern eOmn_servMC_controller_type_t eomn_string2MCcontrollertype(const char * string)
{    
    if(NULL == string)
    {
        return(eomn_servMC_controller_UNKNOWN);
    }
    
    uint8_t i = 0;
    
    for(i=0; i<eomn_controller_type_numberof; i++)
    {
        if(0 == strcmp(string, s_mn_controllertype_strings[i]))
        {
            return((eOmn_serv_type_t)(i+0));
        }
    }     
    
    return(eomn_servMC_controller_UNKNOWN);    
}


extern eOmn_serv_mc_sensor_nuomofcomponents_t eomn_mc_sensor_getnumofcomponets(eOmn_serv_mc_sensor_type_t type)
{

    switch(type)
    {
        case eomn_serv_mc_sensor_none:
        case eomn_serv_mc_sensor_maxvalidvalue:
            return eomn_serv_mc_sensor_nuomofcomp_zero;
        
        
        case eomn_serv_mc_sensor_encoder_aea:
        case eomn_serv_mc_sensor_encoder_amo:
        case eomn_serv_mc_sensor_encoder_inc: 
        case eomn_serv_mc_sensor_encoder_absanalog:
        case eomn_serv_mc_sensor_mais:    
        case eomn_serv_mc_sensor_encoder_onfoc:
            return eomn_serv_mc_sensor_nuomofcomp_one;
    
        case eomn_serv_mc_sensor_encoder_spichainof2:
            return eomn_serv_mc_sensor_nuomofcomp_two;
  
        case eomn_serv_mc_sensor_encoder_spichainof3:
            return eomn_serv_mc_sensor_nuomofcomp_three;

        default:
            return eomn_serv_mc_sensor_nuomofcomp_zero;
    } 
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




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

enum { brdFirstEthBoard = eobrd_ethtype_ems4, brdLastEthBoard = eobrd_ethtype_ems4 + eobrd_ethtype_numberof - 1 };
enum { eobrd_otherstype_numberof = 2 };


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char * s_eoboards_can_strings[] =
{
    "eobrd_dsp",
    "eobrd_pic",
    "eobrd_2dc",
    "eobrd_mc4",
    "eobrd_bll",   
    "eobrd_mtb",    
    "eobrd_strain",
    "eobrd_mais",
    "eobrd_foc",
    "eobrd_6sg",
    "eobrd_jog"    
};  EO_VERIFYsizeof(s_eoboards_can_strings, eobrd_cantype_numberof*sizeof(const char *));    


static const char * s_eoboards_eth_strings[] =
{
    "eobrd_ems4",
    "eobrd_mc4plus",
    "eobrd_mc2plus"
};  EO_VERIFYsizeof(s_eoboards_eth_strings, eobrd_ethtype_numberof*sizeof(const char *));    



static const char * s_eoboards_string_none = "brdNONE";

static const char * s_eoboards_string_unknown = "brdUNKNOWN";


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern eObool_t eoboards_is_can(eObrd_type_t type)
{
    if(type < eobrd_cantype_numberof)
    {
        return(eobool_true);     
    }   
    return(eobool_false);   
}


extern eObool_t eoboards_is_eth(eObrd_type_t type)
{
   if((type >= brdFirstEthBoard) && (type <= brdLastEthBoard))
    {
        return(eobool_true);     
    }   
    return(eobool_false);       
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

extern const char * eoboards_type2string(eObrd_type_t type)
{
//    const char * ret = s_eoboards_string_unknown;
    
    if(eobool_true == eoboards_is_can(type))
    {
        return(s_eoboards_can_strings[type]);
    }
    else if(eobool_true == eoboards_is_eth(type))
    {
        return(s_eoboards_eth_strings[type - brdFirstEthBoard]);
    }
    else if(eobrd_ethtype_none == type)
    {
        return(s_eoboards_string_none);
    }
    
    return(s_eoboards_string_unknown);
}


extern eObrd_type_t eoboards_string2type(const char * name)
{    
    if(NULL == name)
    {
        return(eobrd_unknown);
    }
    
    uint8_t i = 0;
    
    for(i=0; i<eobrd_cantype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_can_strings[i]))
        {
            return((eObrd_type_t)(i+0));
        }
    }
    
    for(i=0; i<eobrd_ethtype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_eth_strings[i]))
        {
            return((eObrd_type_t)(i+brdFirstEthBoard));
        }
    } 

    if(0 == strcmp(name, s_eoboards_string_none))
    {
        return(eobrd_none);
    }        
    
    return(eobrd_unknown);    
}



//extern eObrd_cantype_t eoboards_string2cantype(const char * name)
//{
//    if(NULL == name)
//    {
//        return(eobrd_cantype_unknown);
//    }
//    
//    uint8_t i = 0;
//    
//    for(i=0; i<eobrd_cantype_numberof; i++)
//    {
//        if(0 == strcmp(name, s_eoboards_can_strings[i]))
//        {
//            return((eObrd_cantype_t)(i+0));
//        }
//    }

//    if(0 == strcmp(name, s_eoboards_string_none))
//    {
//        return(eobrd_cantype_none);
//    }   
//    
//    return(eobrd_cantype_unknown);            
//}


//extern eObrd_ethtype_t eoboards_string2ethtype(const char * name)
//{
//    if(NULL == name)
//    {
//        return(eobrd_ethtype_unknown);
//    }
//    
//    uint8_t i = 0;
//        
//    for(i=0; i<eobrd_ethtype_numberof; i++)
//    {
//        if(0 == strcmp(name, s_eoboards_eth_strings[i]))
//        {
//            return((eObrd_ethtype_t)(i+brdFirstEthBoard));
//        }
//    } 
// 
//    if(0 == strcmp(name, s_eoboards_string_none))
//    {
//        return(eobrd_ethtype_none);
//    }   
//    
//    return(eobrd_ethtype_unknown);    
//}


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




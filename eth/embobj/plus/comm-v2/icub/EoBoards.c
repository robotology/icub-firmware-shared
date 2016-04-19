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

static const char * s_eoboards_can_names[] =
{
    "brdDSPcan",
    "brdPICcan",
    "brd2DCcan",
    "brdMC4can",
    "brdBLLcan",   
    "brdMTB",    
    "brdSTRAIN",
    "brdMAIS",
    "brdFOC",
    "brd6SGcan",
    "brdJOG"    
};  EO_VERIFYsizeof(s_eoboards_can_names, eobrd_cantype_numberof*sizeof(const char *));    


static const char * s_eoboards_eth_names[] =
{
    "brdEMS4",
    "brdMC4PLUS",
    "brdMC2PLUS"
};  EO_VERIFYsizeof(s_eoboards_eth_names, eobrd_ethtype_numberof*sizeof(const char *));    


static const char * s_eoboards_other_names[] =
{
    "brdNONE",
    "brdUNKNOWN"
};  EO_VERIFYsizeof(s_eoboards_other_names, eobrd_otherstype_numberof*sizeof(const char *));   


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern uint8_t eoboards_maxsizeofnames(void)
{
    return(24);
}


extern const char * eoboards_type2name(eObrd_type_t boardtype)
{
    const char * ret = s_eoboards_other_names[1];
    
    if(boardtype < eobrd_cantype_numberof)
    {
        return(s_eoboards_can_names[boardtype]);
    }
    else if(eobrd_ethtype_none == boardtype)
    {
        return(s_eoboards_other_names[0]);
    }
    else if((boardtype >= brdFirstEthBoard) && (boardtype <= brdLastEthBoard))
    {
        return(s_eoboards_eth_names[boardtype - brdFirstEthBoard]);
    }

    return(ret);
}


extern eObrd_type_t eoboards_name2type(const char * name)
{    
    if(NULL == name)
    {
        return(eobrd_type_unknown);
    }
    
    uint8_t i = 0;
    
    for(i=0; i<eobrd_cantype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_can_names[i]))
        {
            return((eObrd_type_t)(i+0));
        }
    }
    
    for(i=0; i<eobrd_ethtype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_eth_names[i]))
        {
            return((eObrd_type_t)(i+brdFirstEthBoard));
        }
    } 

    if(0 == strcmp(name, s_eoboards_eth_names[0]))
    {
        return(eobrd_type_none);
    }        
    
    return(eobrd_type_unknown);    
}


extern eObool_t eoboards_is_ethboardtype(eObrd_type_t boardtype)
{
    if((boardtype >= eobrd_ethtype_ems4) && (boardtype <= eobrd_ethtype_ems4))
    {
        return(eobool_true);     
    }   
    return(eobool_false);       
}


extern eObool_t eoboards_is_canboardtype(eObrd_type_t boardtype)
{
    if(boardtype < eobrd_cantype_numberof)
    {
        return(eobool_true);     
    }   
    return(eobool_false);   
}


extern eObrd_cantype_t eoboards_name2cantype(const char * name)
{
    if(NULL == name)
    {
        return(eobrd_cantype_unknown);
    }
    
    uint8_t i = 0;
    
    for(i=0; i<eobrd_cantype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_can_names[i]))
        {
            return((eObrd_cantype_t)(i+0));
        }
    }
    
    return(eobrd_cantype_unknown);            
}


extern eObrd_ethtype_t eoboards_name2ethtype(const char * name)
{
    if(NULL == name)
    {
        return(eobrd_ethtype_unknown);
    }
    
    uint8_t i = 0;
        
    for(i=0; i<eobrd_ethtype_numberof; i++)
    {
        if(0 == strcmp(name, s_eoboards_eth_names[i]))
        {
            return((eObrd_ethtype_t)(i+brdFirstEthBoard));
        }
    } 
         
    return(eobrd_ethtype_unknown);    
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




/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author:  Valentina Gaggero
 * email:   valentina.gaggero@iit.it
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

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------
#include "stdio.h"
#include "OPCprotocolManager_Cfg.h" 
#include "OPCprotocolManager.h"

#include "EoDiagnostics.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

static opcprotman_var_map_t s_myarray[] = 
{
    {
        .var        = eodgn_nvidbdoor_cmds,
        .size       = sizeof(eOdgn_commands_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_errorlog,
        .size       = sizeof(eOdgn_errorlog_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_emsperiph,
        .size       = sizeof(eOdgn_emsperipheralstatus_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_emsapplcommon,
        .size       = sizeof(eOdgn_emsapplication_common_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_emsapplmc,
        .size       = sizeof(eOdgn_emsapplication_emswithmc_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_motorstatus,
        .size       = sizeof(eOdgn_motorstatusflags_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_canQueueStatistics,
        .size       = sizeof(eOdgn_can_statistics_t),
        .ptr        = NULL,
        .onrec      = NULL
    },
    {
        .var        = eodgn_nvidbdoor_rxcheckSetpoints,
        .size       = sizeof(eOdgn_rxCheckSetpoints_t),
        .ptr        = NULL,
        .onrec      = NULL
    }
};

opcprotman_cfg_t opcprotmanCFGv0x1234 =
{
    .databaseversion        = 0x1236,
    .numberofvariables      = eodgn_nvidbdoor_max,
    .arrayofvariablemap     = s_myarray
};

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
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


// use this in case you dont want use the backdoortransceiver
extern opcprotman_cfg_t* opcprotman_getconfiguration(void)
{
    return(NULL);
}


// this one is used only in the ems board. use it if you dont want to use the backdoortransceiver.
// otehrwise use calls to opcprotman_personalize_var() function 
extern opcprotman_res_t opcprotman_personalize_database(OPCprotocolManager *p)
{
    return(opcprotman_NOK_generic);
}



extern opcprotman_cfg_t* opcprotman_getconfiguration(void)
{
    return(&opcprotmanCFGv0x1234);
}

extern opcprotman_res_t opcprotman_personalize_database(OPCprotocolManager *p)
{
    opcprotman_res_t res = opcprotman_OK;
    

/* personalize eodgn_nvidbdoor_cmds var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_cmds,
                                        (uint8_t*) &eo_dgn_cmds, 
                                        on_rec_dgn_cmds);

    if(opcprotman_OK != res)
    {
        return(res);
    }
    
/* personalize eodgn_nvidbdoor_errorlog var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_errorlog,
                                        (uint8_t*) &eo_dgn_errorlog, 
                                        NULL);

    if(opcprotman_OK != res)
    {
        return(res);
    }
    
/* personalize eodgn_nvidbdoor_emsperiph var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_emsperiph,
                                        (uint8_t*)&eo_dgn_emsperiph, 
                                        NULL); //on ems i don't receive this data

    if(opcprotman_OK != res)
    {
        return(res);
    }
    
/* personalize eodgn_nvidbdoor_emsapplcore var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_emsapplcommon,
                                        (uint8_t*)&eo_dgn_emsapplcore, 
                                        NULL); //on ems i don't receive this data

    if(opcprotman_OK != res)
    {
        return(res);
    }
    
/* personalize eodgn_nvidbdoor_emsapplmc var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_emsapplmc,
                                        (uint8_t*)&eo_dgn_emsappmc, 
                                        NULL); //on ems i don't receive this data

    if(opcprotman_OK != res)
    {
        return(res);
    }
  

/* personalize eodgn_nvidbdoor_motorstatus var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_motorstatus,
                                        (uint8_t*)&eo_dgn_motorstflag, 
                                        NULL); //on ems i don't receive this data

    if(opcprotman_OK != res)
    {
        return(res);
    }

    
    /* personalize eodgn_nvidbdoor_motorstatus var*/
	res = opcprotman_personalize_var(   p, 
                                        eodgn_nvidbdoor_canQueueStatistics,
                                        (uint8_t*)&eo_dgn_canstatistics, 
                                        NULL); //on ems i don't receive this data

    if(opcprotman_OK != res)
    {
        return(res);
    }
    
    
//#if defined(VERIFY_ROP_SETIMPEDANCE) | defined(VERIFY_ROP_SETPOSITIONRAW)
//
//    /* personalize eodgn_nvidbdoor_rxcheckSetpoints var*/
//	res = opcprotman_personalize_var(   p, 
//                                        eodgn_nvidbdoor_rxcheckSetpoints,
//                                        (uint8_t*)&eo_dgn_rxchecksepoints, 
//                                         NULL); //on ems i don't receive this data
//
//    if(opcprotman_OK != res)
//    {
//        return(res);
//    }
//    
//#endif   
    return(res);
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





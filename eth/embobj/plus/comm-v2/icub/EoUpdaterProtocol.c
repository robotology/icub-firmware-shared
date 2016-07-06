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

/* @file       EoUpdaterProtocol.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       June 16 2016
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

#include "EoUpdaterProtocol.h"


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

static const char * s_eouprot_procnames[uprot_proc_numberofthem+1] = {"eLoader", "eUpdater", "eApplication", "eApplPROGupdater", "uprot_proc_None"};


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern const char * eouprot_process2string(eOuprot_process_t proc)
{
    if(proc >= uprot_proc_numberofthem)
    {
        return(s_eouprot_procnames[uprot_proc_numberofthem]);
    }

    return(s_eouprot_procnames[proc]);    
}

extern eOuprot_process_t eouprot_string2process(const char * name)
{
    uint8_t i=0;
    for(i=0; i<uprot_proc_numberofthem; i++)
    {
        if(0 == strcmp(name, s_eouprot_procnames[i]))
        {
            return((eOuprot_process_t)i);
        }        
    }
    return(uprot_proc_None);
}


extern uint32_t eouprot_get_capabilities(eOuprot_process_t proc, uint8_t protocolversion)
{
    static const uint32_t s_capabilities_procs[2][uprot_proc_numberofthem] =
    {
        {   // version 0: legacy    
            
            (   // eLoader
                uprot_canDO_nothing
            ),
            
            (   // eUpdater
                uprot_canDO_PROG_loader | 
                uprot_canDO_PROG_application |
                uprot_canDO_restart | 
                uprot_canDO_DEF2RUN_set |
                uprot_canDO_LEGACY_MAC_set |
                uprot_canDO_LEGACY_cangateway |
                uprot_canDO_LEGACY_scan |
                uprot_canDO_LEGACY_IPaddr_set |
                uprot_canDO_LEGACY_IPmask_set |
                uprot_canDO_LEGACY_EEPROM_erase |
                uprot_canDO_LEGACY_procs |
                uprot_canDO_blink            
            ),
            
            (   // eApplication
                uprot_canDO_restart |
                uprot_canDO_LEGACY_scan |
                uprot_canDO_LEGACY_procs            
            ),
            
            (   // eApplPROGupdater
                uprot_canDO_PROG_loader | 
                uprot_canDO_PROG_updater |
                uprot_canDO_restart | 
                uprot_canDO_DEF2RUN_set |

                uprot_canDO_LEGACY_MAC_set |
                uprot_canDO_LEGACY_scan |
                uprot_canDO_LEGACY_IPaddr_set |
                uprot_canDO_LEGACY_IPmask_set |
                uprot_canDO_LEGACY_procs |
                uprot_canDO_blink |
                uprot_canDO_JUMP2UPDATER                
            )
        },
        {   // version 1: revised in june 2016
            
            (   // eLoader
                uprot_canDO_nothing
            ),
            
            (   // eUpdater
                uprot_canDO_reply2discover |
                uprot_canDO_reply2moreinfo |
                uprot_canDO_PROG_loader | 
                uprot_canDO_PROG_application |
                uprot_canDO_restart | 
                uprot_canDO_cangateway |
                uprot_canDO_IPaddr_set |
                uprot_canDO_EEPROM_erase |
                uprot_canDO_EEPROM_read |
                uprot_canDO_DEF2RUN_set |
                uprot_canDO_PAGE_get |
                uprot_canDO_PAGE_set |
                uprot_canDO_PAGE_clr |
                uprot_canDO_LEGACY_IPmask_set |
                uprot_canDO_LEGACY_MAC_set |
                uprot_canDO_LEGACY_cangateway |
                uprot_canDO_LEGACY_scan |
                uprot_canDO_LEGACY_IPaddr_set |
                uprot_canDO_LEGACY_EEPROM_erase |
                uprot_canDO_LEGACY_procs |
                uprot_canDO_blink |
                uprot_canDO_JUMP2ADDRESS                
            ),
            
            (   // eApplication
                uprot_canDO_reply2discover |
                uprot_canDO_LEGACY_scan |
                uprot_canDO_restart |
                uprot_canDO_cangateway |
                uprot_canDO_LEGACY_cangateway
            ),
            
            (   // eApplPROGupdater            
                uprot_canDO_reply2discover |
                uprot_canDO_reply2moreinfo |
                uprot_canDO_PROG_updater |                 
                uprot_canDO_restart | 
                uprot_canDO_IPaddr_set |
                uprot_canDO_DEF2RUN_set |
                uprot_canDO_JUMP2UPDATER |
                uprot_canDO_LEGACY_IPmask_set |
                uprot_canDO_LEGACY_MAC_set |                
                uprot_canDO_LEGACY_scan |
                uprot_canDO_LEGACY_IPaddr_set |
                uprot_canDO_LEGACY_procs |
                uprot_canDO_blink |
                uprot_canDO_PAGE_get |
                uprot_canDO_JUMP2ADDRESS
            )
        }    
    };
    
    if((uprot_proc_None == proc) || (proc > uprot_proc_numberofthem))
    {
        return uprot_canDO_nothing;
    }
    
    if(protocolversion > 1)
    {
        return uprot_canDO_nothing;
    }
    
    return s_capabilities_procs[protocolversion][proc];
    
}



extern eObool_t eouprot_can_process_opcode(eOuprot_process_t proc, uint8_t protocolversion, eOuprot_opcodes_t opcode, uint32_t param)
{
    eObool_t yeswecan = eobool_false;
    
    uint32_t maskofcapabilities = eouprot_get_capabilities(proc, protocolversion);    
    eOuprot_proc_capabilities_t targetcapability = uprot_canDO_nothing;
    
    switch(opcode)
    {
        default:
        case uprot_OPC_NONE:
        {
            targetcapability = uprot_canDO_nothing;
        } break;
    
        case uprot_OPC_LEGACY_SCAN:
        {
            targetcapability = uprot_canDO_LEGACY_scan;
        } break;
        
        case uprot_OPC_LEGACY_PROCS:
        {
            targetcapability = uprot_canDO_LEGACY_procs;
        } break;

        case uprot_OPC_LEGACY_CANGATEWAY:
        {
            targetcapability = uprot_canDO_LEGACY_cangateway;
        } break;

        case uprot_OPC_LEGACY_IP_ADDR_SET:
        {
            targetcapability = uprot_canDO_LEGACY_IPaddr_set;
        } break;

        case uprot_OPC_LEGACY_EEPROM_ERASE:
        {
            targetcapability = uprot_canDO_LEGACY_EEPROM_erase;
        } break;

        case uprot_OPC_DISCOVER:
        {
            targetcapability = uprot_canDO_reply2discover;
        } break;
   
        case uprot_OPC_MOREINFO:
        {
            targetcapability = uprot_canDO_reply2moreinfo;
        } break;
        
        case uprot_OPC_PROG_DATA:
        case uprot_OPC_PROG_START: 
        case uprot_OPC_PROG_END:
        {
            if(uprot_partitionLOADER == param)
            {
                targetcapability = uprot_canDO_PROG_loader; 
            }
            if(uprot_partitionUPDATER == param)
            {
                targetcapability = uprot_canDO_PROG_updater;                
            }
            else if(uprot_partitionAPPLICATION == param)
            {
                targetcapability = uprot_canDO_PROG_application;
            }       
        } break;

        case uprot_OPC_DEF2RUN:
        {
            targetcapability = uprot_canDO_DEF2RUN_set;
        } break;

        case uprot_OPC_RESTART:
        {
            targetcapability = uprot_canDO_restart;
        } break;

        case uprot_OPC_IP_ADDR_SET:
        {
            targetcapability = uprot_canDO_LEGACY_IPaddr_set;
        } break;

        case uprot_OPC_EEPROM_ERASE:
        {
            targetcapability = uprot_canDO_EEPROM_erase;
        } break;

        case uprot_OPC_EEPROM_READ:
        {
            targetcapability = uprot_canDO_EEPROM_erase;
        } break;
   
        case uprot_OPC_BLINK:
        {
            targetcapability = uprot_canDO_blink;
        } break;

        case uprot_OPC_JUMP2UPDATER:
        {
            targetcapability = uprot_canDO_JUMP2UPDATER;
        } break;

        case uprot_OPC_JUMP2ADDRESS:
        {
            targetcapability = uprot_canDO_JUMP2ADDRESS;
        } break;
        
        case uprot_OPC_PAGE_CLR:
        {
            targetcapability = uprot_canDO_PAGE_clr;
        } break;

        case uprot_OPC_PAGE_SET:
        {
            targetcapability = uprot_canDO_PAGE_set;
        } break;

        case uprot_OPC_PAGE_GET:
        {
            targetcapability = uprot_canDO_PAGE_get;
        } break;
    
        case uprot_OPC_CANGATEWAY:
        {
            targetcapability = uprot_canDO_cangateway;
        } break;

        case uprot_OPC_LEGACY_MAC_SET:
        {
            targetcapability = uprot_canDO_LEGACY_MAC_set;
        } break;

        case uprot_OPC_LEGACY_IP_MASK_SET:
        {
            targetcapability = uprot_canDO_LEGACY_IPmask_set;
        } break;

    }
    
    yeswecan = (targetcapability == (targetcapability & maskofcapabilities)) ? (eobool_true) : (eobool_false);    
    
    return(yeswecan);
}


extern uint8_t eouprot_process2index(eOuprot_process_t process)
{
    uint8_t ret = 255;
    
    switch(process)
    {
        case eLoader: 
        {            
            ret = 0; 
        } break;
        case eUpdater:   
        {            
            ret = 1; 
        } break;
        case eApplication:    
        case eApplPROGupdater:
        {
            ret = 2; 
        } break;
        default:
        {
            ret = 255;
        } break;
    }
    
    return ret;
}


extern eOuprot_process_t eouprot_raw2process(uint8_t rawvalue)
{
    eOuprot_process_t ret = uprot_proc_None;
    
    if(rawvalue < uprot_proc_numberofthem)
    {
        ret = (eOuprot_process_t) rawvalue;
    }
    
    return ret;
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




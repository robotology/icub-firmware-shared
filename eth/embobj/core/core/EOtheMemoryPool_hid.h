/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _THEMEMORYPOOL_HID_H_
#define _THEMEMORYPOOL_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOtheMemoryPool.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------

typedef struct
{
    uint8_t                         poolsmask;
    uint16_t                        uint08index;
    uint16_t                        uint16index;
    uint16_t                        uint32index;
    uint16_t                        uint64index;    
} eOmempool_pool_status_t;

typedef struct
{
    eOmempool_pool_config_t     config;
    eOmempool_pool_status_t     status;
} eOmempool_the_pool_t;

typedef struct
{
    uint32_t    usedbytesheap;
    uint32_t    usedbytespool;
} eOmempool_stats_t;

// - definition of the hidden struct implementing the object ----------------------------------------------------------

struct EOtheMemoryPool_hid 
{
    uint8_t                         initted;
    eOmempool_cfg_t                 config;
    eOmempool_heap_config_t         theheap;
    eOmempool_the_pool_t            thepool;
    EOVmutex                        *mutex;
    eOreltime_t                     tout;
    eOmempool_stats_t               stats;
}; 


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



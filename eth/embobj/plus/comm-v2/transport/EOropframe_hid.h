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
#ifndef _EOROPFRAME_HID_H_
#define _EOROPFRAME_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOropframe_hid.h
    @brief      This header file implements hidden interface to a packet object.
    @author     marco.accame@iit.it
    @date       0111/2010
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOropframe.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------

#define EOFRAME_START   0x12345678
#define EOFRAME_END     0x87654321


// - definition of the hidden struct implementing the object ----------------------------------------------------------

/** @typedef    typedef struct EOropframeHeader_t
    @brief      contains the definition of the header in the framedata according to the ethernet protocol 
 **/  
typedef struct  
{
    uint32_t            startofframe;       /**< it is the start of the frame: it is EOFRAME_START */
    uint16_t            ropssizeof;         /**< tells how many bytes are reserved for the rops: its value can be 0 to ... */
    uint16_t            ropsnumberof;       /**< tells how many rops are inside: its value can be 0 to ... */
    uint64_t            ageofframe;         /**< tells the time (in usec) of creation of the frame */
    uint64_t            sequencenumber;     /**< contains a sequence number */
} EOropframeHeader_t;   EO_VERIFYsizeof(EOropframeHeader_t, 24)


/** @typedef    struct EOropframeData_t
    @brief      contains the framedata, which is what travels inside a packet. This variable must be used as a pointer,
                with a cast to a received packet
 **/  
struct EOropframeData_hid 
{
    EOropframeHeader_t      header;
    uint8_t                 ropsfooter[8];    
};        


typedef struct  // 04 bytes
{
    uint32_t            endoframe;
} EOropframeFooter_t;   EO_VERIFYsizeof(EOropframeFooter_t, 4)


typedef struct  // 28 bytes ... 
{
    uint8_t             headerfooter[28];
} EOropframeEmpty_t;    EO_VERIFYsizeof(EOropframeEmpty_t, sizeof(EOropframeHeader_t)+sizeof(EOropframeFooter_t))

// the following is used to guarantee that eo_ropframe_sizeforZEROrops is equal to size of EOropframeEmpty_t.
EO_VERIFYproposition(EOropframe_hid_verifyzerorops, sizeof(EOropframeEmpty_t) == eo_ropframe_sizeforZEROrops)



typedef struct  // 32 bytes
{
    EOropframeHeader_t          header;
    uint8_t                     ropsfooter[8];
} EOropframeHeaderRopsFooter_t; EO_VERIFYsizeof(EOropframeHeaderRopsFooter_t, 32)




/** @struct     EOropframe_hid
    @brief      Hidden definition. Implements private data used only internally by the 
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/  
 
struct EOropframe_hid 
{
    uint16_t                        capacity;               // contains the maximum size of framedata
    uint16_t                        size;                   // contains the number of bytes effectively used by framedata. has values from eo_ropframe_sizeforZEROrops to .capacity
    uint16_t                        index2nextrop2beparsed; // it is an index to next rop to be parser. it starts from zero and is used from &rops[0]
    uint16_t                        dummy;
    EOropframeData*                 framedata;         // contains the header, the rops, the footer. in case of a ropframe unable to store rops its size must be eo_ropframe_sizeforZEROrops
}; 


// - declaration of extern hidden functions ---------------------------------------------------------------------------

uint8_t* eo_ropframe_hid_get_pointer_offset(EOropframe *p, uint16_t offset);



#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





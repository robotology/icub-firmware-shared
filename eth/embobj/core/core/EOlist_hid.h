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
#ifndef _EOLIST_HID_H_
#define _EOLIST_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOlist_hid.h
    @brief      This header file implements hidden interface to a list object.
    @author     marco.accame@iit.it
    @date       08/03/2011
**/ 


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOlist.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------
// empty-section


// - definition of the hidden struct implementing the object ----------------------------------------------------------

/* @struct     EOlistIter_hid
    @brief      hidden definition. implements private data used only internally by the 
                public or private (static) functions of EOlistIter object and protectde data
                used also and by derived objects.
 **/ 
struct EOlistIter_hid 
{
    EOlistIter  *prev;              /*< pointer to previous list iterator              */
    EOlistIter  *next;              /*< pointer to next list iterator                  */
    void        *data;              /*< data pointed by the current list iterator      */
};


/* @struct     EOlist_hid
    @brief      hidden definition. implements private data used only internally by the 
                public or private (static) functions of the list object
 **/ 
struct EOlist_hid 
{
    EOlistIter                  *head;                  /*< pointer to first list iterator                 */
    EOlistIter                  *tail;                  /*< pointer to last list iterator                  */
    eOsizecntnr_t               size;                   /*< current number of list iterators in the list   */
    eOsizecntnr_t               capacity;               /*< max number of list iterators in the list       */
    eOsizeitem_t                item_size;              /*< size of item contained in the EOlistIter->data */ 
    eOres_fp_voidp_uint32_t     item_init_fn;
    uint32_t                    item_init_par;        
    eOres_fp_voidp_voidp_t      item_copy_fn;           /*< copy constructor used on inserted data         */ 
    eOres_fp_voidp_t            item_clear_fn;             /*< destructor used on removed data                */ 
    EOlistIter                  *freeiters;             /*< pool of free iterators for the list            */
};

 

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



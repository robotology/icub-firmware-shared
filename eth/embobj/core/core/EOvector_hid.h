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
#ifndef _EOVECTOR_HID_H_
#define _EOVECTOR_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @file       EOvector_hid.h
    @brief      This header file implements hidden interface to a EOvector object.
    @author     marco.accame@iit.it
    @date       08/03/2011
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "EOvector.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------
// empty-section


// - definition of the hidden struct implementing the object ----------------------------------------------------------

/** @struct     EOvector_hid
    @brief      Hidden definition. Implements private data used only internally by the 
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/  
 
struct EOvector_hid 
{                    
    eOsizecntnr_t               size;               /**< number of items in the vector. used only by the vector. */                                       
    eOsizeitem_t                item_size;          /**< size in bytes of the item object. */   
    eOsizecntnr_t               capacity;           /**< max number of item objects in the array. */
    eOsizeitem_t                sizeofstoreditem;   /**< size in bytes of the item object when stored in the container. */
    void                        *stored_items;      /**< array of item object. */   
    eOres_fp_voidp_uint32_t     item_init_fn;
    uint32_t                    item_init_par;        
    eOres_fp_voidp_voidp_t      item_copy_fn;       /**< specialised copy for a single item object. It is called upon copy into the vector*/    
    eOres_fp_voidp_t            item_clear_fn;      /**< specialised remove for a single item object. It is called upon removal from the vector*/
};


// - declaration of extern hidden functions ---------------------------------------------------------------------------
// empty-section


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




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
#ifndef _EOCONSTVECTOR_H_
#define _EOCONSTVECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOconstvector.h
	@brief      This header file implements public interface to a constvector object.
	@author     marco.accame@iit.it
	@date       08/03/2011
**/

/** @defgroup eo_constvector Object EOconstvector
    The EOconstvector allows to manipulate vectors of any item object, implementing the constvector<type> template 
    of the standard C++ library.
    At creation, the EOconstvector receives the dimension of the item object which will contain, their maximum number
    (capacity) and optional copy and remove functions for the item objects.
    The EOconstvector receives a pointer to an item object and copies the pointed item inside its internal memory,
    calling the optional copy function on it or the default memcpy. When the EOconstvector is asked for an item object, 
    it returns a pointer to an internal item object. If the EOconstvector is requested to remove the item object, the 
    optional user-defined remove function is called or the default remove which set memory to zero.
    The EOconstvector is a base object and is used to derive a new object to manipulate specific items. 
    
    @{		
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"



// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 


/**	@typedef    typedef struct EOconstvector_hid EOconstvector
 	@brief 		EOconstvector is an opaque struct. It is used to implement data abstraction for the constvector 
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOconstvector_hid EOconstvector;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------


 
/** @fn         extern EOconstvector * eo_constvector_New(eOsizeitem_t item_size, eOsizecntnr_t size, const void *data) 
    @brief      Creates a new EOconstvector object. it uses externally passed const data
    @param      item_size       The size in bytes of the item object managed by the EOconstvector.
    @param      size            The number of item objects stored by the EOconstvector.
    @return     Pointer to the required EOconstvector object. The pointer is guaranteed to be always valid and will 
                never be NULL, because failure is managed by the memory pool.
 **/
extern EOconstvector * eo_constvector_New(eOsizeitem_t item_size, eOsizecntnr_t size, const void *data);


extern EOconstvector * eo_constvector_Load(EOconstvector *p, eOsizeitem_t item_size, eOsizecntnr_t size, const void *data);


/** @fn         extern void eo_constvector_Delete(EOconstvector *p)
    @brief      deletes the ram used for the constvector.
    @param      p           the EOconstvector object. 
 **/
extern void eo_constvector_Delete(EOconstvector *p);


/** @fn         extern eOsizecntnr_t eo_constvector_Size(EOconstvector *p)
    @brief      Returns the number of item objects that are currently stored in the EOconstvector.
    @param      cvect           Pointer to the EOconstvector object. 
    @return     Number of item objects.
 **/
extern eOsizecntnr_t eo_constvector_Size(const EOconstvector *p);


/** @fn         extern const void* eo_constvector_At(EOconstvector *p, eOsizecntnr_t pos)
    @brief      Retrieves a reference to the item object in position pos 
    @param      cvect           Pointer to the EOconstvector object. 
    @param      pos             Position of the desired item object.
    @return     Pointer to the item object (or NULL if pos points to an empty position or 
                beyond reserved space).
    @warning    Before use, the returned pointer needs to be casted to the desidered object type.
                If the item object is a uint32_t, use: 
                uint32_t *p = (uint32_t*) eo_constvector_At(cvect, pos); 
 **/
extern const void * eo_constvector_At(const EOconstvector *p, eOsizecntnr_t pos);



/** @}            
    end of group eo_constvector  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



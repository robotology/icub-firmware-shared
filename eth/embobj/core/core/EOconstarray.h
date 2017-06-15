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
#ifndef _EOCONSTARRAY_H_
#define _EOCONSTARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOconstarray.h
    @brief      This header file implements public interface to a array object.
    @author     marco.accame@iit.it
    @date       08/03/2011
**/

/** @defgroup eo_constarray Object EOconstarray
    The EOconstarray object is a container which is used as a read only EOarray.
     
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOarray.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef EOarray_of EOconstarray
    @brief      It has the same memory mapping of a EOarray... 
 **/  
typedef EOarray_of EOconstarray;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
 
 
/** @fn         extern EOconstarray* eo_constarray_Load(EOarray *array)
    @brief      it loads a EOconstarray object with data coming from a EOarray
    @param      array       the EOarray
    @return     The pointer to the required object.
 **/
extern EOconstarray* eo_constarray_Load(const EOarray *array);


/** @fn         extern uint8_t eo_constarray_Size(EOconstarray *p)
    @brief      Gets the number of items inside the object 
    @param      p               The pointer to the object.
    @return     The size.
 **/
extern uint8_t eo_constarray_Size(EOconstarray *p);


/** @fn         extern uint8_t eo_constarray_ItemSize(EOconstarray *p)
    @brief      Gets the size of items inside the object 
    @param      p               The pointer to the object.
    @return     The size.
 **/
extern uint8_t eo_constarray_ItemSize(EOconstarray *p);


/** @fn         extern const void * eo_constarray_At(EOconstarray *p, uint8_t pos)
    @brief      Gets a pointer to the item in position @e pos inside the object 
    @param      p               The pointer to the object.
    @param      pos             The position of teh item
    @return     The pointer, or NULL upon failure.
 **/
extern const void * eo_constarray_At(EOconstarray *p, uint8_t pos);




/** @}            
    end of group eo_constarray  
 **/

 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: Valentina Gaggero, Marco Accame
 * email:   valentina.gaggero@iit.it, marco.accame@iit.it
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
#ifndef _EOSKIN_H_
#define _EOSKIN_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoSkin.h
	@brief      This header file gives 
	@author     marco.accame@iit.it
	@date       09/06/2011
**/

/** @defgroup eo_cevcwervcrev2345 Configuation of the vrfverver
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoMeasures.h"
#include "EoUtilities.h"
#include "EOarray.h"
#include "iCubCanProto_types.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section




// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef enum eOsk_entity_t;
    @brief      It contains all the possible entities in skin.
                See definition of eOsk_skin_t for explanations
 **/
typedef enum
{
    eosk_entity_skin                    = 0
} eOsk_entity_t; 

enum { eosk_entities_numberof = 1 };


// -- all the possible enum

typedef enum
{
    eosk_sigmode_dontsignal                         = 0,
    eosk_sigmode_signal_oldway                      = 1,
    eosk_sigmode_signal                             = 2
} eOsk_sigmode_t;


typedef enum
{   // they must be equal to what the can protocol specifies
    eosk_skintype_withtempcomp                      = icubCanProto_skinType__withtempcomp,      /**< 0 */  
    eosk_skintype_palmfingertip                     = icubCanProto_skinType__palmfingertip,     /**< 1 */
    eosk_skintype_withouttempcomp                   = icubCanProto_skinType__withouttempcomp,   /**< 2 */   
    eosk_skintype_ergohand                          = icubCanProto_skinType__ergohand,          /**< 3 */  
    eosk_skintype_testmoderaw                       = icubCanProto_skinType__testmoderaw,       /**< 7 */
    eosk_skintype_withtempcompv2                    = icubCanProto_skinType__withtempcompv2     /**< 8 */       
} eOsk_skintype_t;

// -- all the possible data service structures

#define EOSK_CANDATA_INFO(size, idcan)  ( (uint16_t) ( ((size&0x000f)<<12) | (idcan&0x0fff) ) )
#define EOSK_CANDATA_INFO2SIZE(info)    ( (uint8_t) ((((uint16_t)info)&0xf000)>>12) ) 
#define EOSK_CANDATA_INFO2IDCAN(info)   ( (uint16_t) ((((uint16_t)info)&0x0fff)) ) 

typedef struct
{
    uint16_t        info;       // the bytes are formatted with following nibbles: [size | id | id | id], with 11 bits id
    uint8_t         data[8];    // the can payload  
} eOsk_candata_t;   EO_VERIFYsizeof(eOsk_candata_t, 10)

enum {eosk_capacity_arrayof_skincandata = 24 };

typedef struct              // size is 4+24*10 = 244
{
    eOarray_head_t          head;
    uint8_t                 data[eosk_capacity_arrayof_skincandata*sizeof(eOsk_candata_t)];      
} EOarray_of_skincandata_t; EO_VERIFYsizeof(EOarray_of_skincandata_t, 244)


// -- the definition of a skin entity 

/** @typedef    uint8_t  eOsk_skinId_t
    @brief      eOsk_skinId_t contains the values required to identify a skin;
		        currently a skin id identifies a set of 7 skin board attached on a single body part.
 **/
typedef uint8_t  eOsk_skinId_t;



typedef struct
{
    eOenum08_t                  skintype;       /**< use values from eOsk_skintype_t */
    uint8_t                     period;
    uint8_t                     noload;
    uint8_t                     filler1;
} eOsk_board_config_t;          EO_VERIFYsizeof(eOsk_board_config_t, 4)


typedef struct                      
{
    uint8_t                     enable;
    uint8_t                     shift;
    uint16_t                    CDCoffset;
} eOsk_triangle_config_t;       EO_VERIFYsizeof(eOsk_triangle_config_t, 4)


typedef struct                      
{
    eOenum08_t                  sigmode;        /**< use values from eOsk_sigmode_t */
    uint8_t                     filler[7];
} eOsk_config_t;                EO_VERIFYsizeof(eOsk_config_t, 8)


typedef struct                      
{
    uint8_t                     filler04[4];                           
} eOsk_inputs_t;                EO_VERIFYsizeof(eOsk_inputs_t, 4)

typedef struct                      
{
    uint16_t                    candestination[2];  // if = {0xffff, 0xffff} the cfg is sent to all boards in the entity. else to those sin the bitmap, even if not in the entity         
    uint8_t                     idstart;            // first triangle
    uint8_t                     idend;              // last triangle
    uint8_t                     filler[6];
    eOsk_triangle_config_t      cfg;
} eOsk_cmd_trianglesCfg_t;      EO_VERIFYsizeof(eOsk_cmd_trianglesCfg_t, 16)


typedef struct                      
{
    uint16_t                    candestination[2];  // if = {0xffff, 0xffff} the cfg is sent to all boards in the entity. else to those sin the bitmap, even if not in the entity
    eOsk_board_config_t         cfg;
} eOsk_cmd_boardsCfg_t;         EO_VERIFYsizeof(eOsk_cmd_boardsCfg_t, 8)

typedef struct                      
{
    eOsk_cmd_boardsCfg_t        boardscfg;
    eOsk_cmd_trianglesCfg_t     trianglescfg;
} eOsk_command_t;               EO_VERIFYsizeof(eOsk_command_t, 24)


typedef struct                  // size is: 124+4+0 = 128  or 94+2 = 96                  
{
    EOarray_of_skincandata_t    arrayofcandata;
    uint8_t                     filler04[4];                           
} eOsk_status_t;                EO_VERIFYsizeof(eOsk_status_t, 248)


typedef struct                  // size is: 8+248+24 = 272 
{
    eOsk_config_t               config; 
    eOsk_status_t               status;
    eOsk_command_t              cmmnds;    
} eOsk_skin_t;                  EO_VERIFYsizeof(eOsk_skin_t, 280)



// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section



/** @}            
    end of group eo_cevcwervcrev2345  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


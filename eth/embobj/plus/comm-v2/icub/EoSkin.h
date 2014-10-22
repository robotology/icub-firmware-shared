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

// - public #define  --------------------------------------------------------------------------------------------------





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

#define EOSK_SKINTYPE_WITHTEMPCOMP                     0 /**< new version of skin with temperature compensation */
#define EOSK_SKINTYPE_PALMFINGERTIP                    1 /**< skin for palm and fingertip */
#define EOSK_SKINTYPE_WITHOUTTEMPCOMP                  2 /**< old version of skin without temperature compensation */
typedef enum
{
    eosk_skintype_withtempcomp                      = EOSK_SKINTYPE_WITHTEMPCOMP,  
    eosk_skintype_palmfingertip                     = EOSK_SKINTYPE_PALMFINGERTIP,  
    eosk_skintype_withouttempcomp                   = EOSK_SKINTYPE_WITHOUTTEMPCOMP   
} eOsk_skintype_t;

// -- all the possible data service structures

typedef struct              // size is 4+10*16+0 = 164
{
    eOarray_head_t          head;
    uint8_t                 data[10*sizeof(eOutil_canframe_t)];
} EOarray_of_10canframes;   //EO_VERIFYsizeof(EOarray_of_10canframes, 164);


typedef struct              // size is 4+24*16+0 = 388
{
    eOarray_head_t          head;
    uint8_t                 data[24*sizeof(eOutil_canframe_t)];
} EOarray_of_24canframes;   //EO_VERIFYsizeof(EOarray_of_24canframes, 388);


/** @typedef    typedef struct eOsk_array_data_t
    @brief      eOsk_array_data_t contains up to 10 can frames as received by hal placed in an EOarray object
    @warning    This struct must be of fixed size and multiple of 4.
 **/
typedef struct                  // size is: 164+4+0 = 168 
{
    EOarray_of_10canframes  array;
    uint8_t                 filler04[4];
} eOsk_array_data_t;        //EO_VERIFYsizeof(eOsk_array_data_t, 168);


/** @typedef    typedef struct eOsk_array_largedata_t
    @brief      eOsk_array_largedata_t contains up to 24 can frames as received by hal placed in an EOarray object
    @warning    This struct must be of fixed size and multiple of 8.
 **/
typedef struct                  // size is: 388+4+0 = 392 
{
    EOarray_of_24canframes  array;
    uint8_t                 filler04[4];
} eOsk_array_largedata_t;   //EO_VERIFYsizeof(eOsk_array_largedata_t, 392);


// -- the definition of a skin entity 

/** @typedef    uint8_t  eOsk_skinId_t
    @brief      eOsk_skinId_t contains the values required to identify a skin;
		        currently a skin id identifies a set of 7 skin board attached on a single body part.
 **/
typedef uint8_t  eOsk_skinId_t;



typedef struct
{
    eOenum08_t                  skintype;                               /**< use values from eOsk_skintype_t */
    uint8_t                     period;
    uint8_t                     noload;
    uint8_t                     filler1;
} eOsk_board_config_t;          //EO_VERIFYsizeof(eOsk_config_board_t, 4);


typedef struct                      
{
    uint8_t                    enable;
    uint8_t                    shift;
    uint16_t                   CDCoffset;
} eOsk_triangle_config_t;                //EO_VERIFYsizeof(eOsk_triangle_config_t, 4);


typedef struct                      
{
    eOenum08_t                  sigmode;                                   /**< use values from eOsk_sigmode_t */
    uint8_t                     filler[7];
} eOsk_config_t;                //EO_VERIFYsizeof(eOsk_config_t, 4);


typedef struct                      
{
    uint8_t                     filler04[4];                           
} eOsk_inputs_t;                //EO_VERIFYsizeof(eOsk_inputs_t, 4);

typedef struct                      
{
    uint8_t                     boardaddr;
    uint8_t                     idstart;
    uint8_t                     idend;
    uint8_t                     filler01;
    eOsk_triangle_config_t      cfg;
} eOsk_cmd_trianglesCfg_t;               //EO_VERIFYsizeof(eOsk_cmd_tiangleCfg_t, 8);


typedef struct                      
{
    uint8_t                     addrstart;
    uint8_t                     addrend;
    uint8_t                     filler[2];
    eOsk_board_config_t         cfg;
} eOsk_cmd_boardsCfg_t;               //EO_VERIFYsizeof(eOsk_cmd_boardCfg_t, 8);

typedef struct                      
{
    eOsk_cmd_boardsCfg_t         boardscfg;
    eOsk_cmd_trianglesCfg_t      trianglescfg;
} eOsk_command_t;               //EO_VERIFYsizeof(eOsk_command_t, 16);


typedef struct                  // size is: 164+4+0 = 168                     
{
    EOarray_of_10canframes      arrayof10canframes;
    uint8_t                     filler04[4];                           
} eOsk_status_t;                //EO_VERIFYsizeof(eOsk_status_t, 168);


typedef struct                  // size is: 388+0 = 392                     
{
    eOsk_array_largedata_t      arrayofcanframes;                         
} eOsk_status_large_t;          EO_VERIFYsizeof(eOsk_status_large_t, 392);

typedef struct                  // size is: 4+4+168+0 = 176
{
    eOsk_config_t               config; //4
    eOsk_command_t              commands;    
    eOsk_status_t               status;
} eOsk_skin_t;                  //EO_VERIFYsizeof(eOsk_skin_t, 176);



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


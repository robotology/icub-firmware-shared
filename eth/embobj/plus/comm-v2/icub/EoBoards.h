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
#ifndef _EOBOARDS_H_
#define _EOBOARDS_H_


#ifdef __cplusplus
extern "C" {
#endif

/** @file       EoBoards.h
	@brief      This header file gives 
	@author     marco.accame@iit.it
	@date       09/06/2011
**/

/** @defgroup eo_cevcwervcrev5555 Configuation of the vrfverver
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "iCubCanProto_types.h"    


// - public #define  --------------------------------------------------------------------------------------------------





// - declaration of public user-defined types ------------------------------------------------------------------------- 


// using the same values as in can protocol (apart from none) 
typedef enum
{   
    eobrd_cantype_dsp               = ICUBCANPROTO_BOARDTYPE__DSP,      // 0, not used
    eobrd_cantype_pic               = ICUBCANPROTO_BOARDTYPE__PIC,      // 2, not used
    eobrd_cantype_2dc               = ICUBCANPROTO_BOARDTYPE__2DC,      // 2, not used
    eobrd_cantype_mc4               = ICUBCANPROTO_BOARDTYPE__4DC,      // 3
    eobrd_cantype_bll               = ICUBCANPROTO_BOARDTYPE__BLL,      // 4, not used
    eobrd_cantype_mtb               = ICUBCANPROTO_BOARDTYPE__SKIN,     // 5 
    eobrd_cantype_strain            = ICUBCANPROTO_BOARDTYPE__STRAIN,   // 6
    eobrd_cantype_mais              = ICUBCANPROTO_BOARDTYPE__MAIS,     // 7
    eobrd_cantype_foc               = ICUBCANPROTO_BOARDTYPE__2FOC,     // 8   
    eobrd_cantype_6sg               = ICUBCANPROTO_BOARDTYPE__6SG,      // 9, not used
    eobrd_cantype_jog               = ICUBCANPROTO_BOARDTYPE__JOG,      // 10, not used    
   
    eobrd_cantype_none              = 254, 	
    eobrd_cantype_unknown           = ICUBCANPROTO_BOARDTYPE__UNKNOWN   // 255 
} eObrd_cantype_t;

enum { eobrd_cantype_numberof = 11 };


typedef enum
{  
    eobrd_ethtype_ems4              = 32,      
    eobrd_ethtype_mc4plus           = 33,     
    eobrd_ethtype_mc2plus           = 34,   
   
    eobrd_ethtype_none              = 254, 	
    eobrd_ethtype_unknown           = 255   
} eObrd_ethtype_t;

enum { eobrd_ethtype_numberof = 3 };


typedef enum
{
    eobrd_type_ems4                 = eobrd_ethtype_ems4,       // name is brdEMS4   
    eobrd_type_mc4plus              = eobrd_ethtype_mc4plus,    // name is brdMC4PLUS     
    eobrd_type_mc2plus              = eobrd_ethtype_mc2plus,    // name is brdMC2PLUS  
        
    eobrd_type_mc4                  = eobrd_cantype_mc4,        // name is brdMC4can
    eobrd_type_mtb                  = eobrd_cantype_mtb,        // name is brdMTB
    eobrd_type_strain               = eobrd_cantype_strain,     // name is brdSTRAIN
    eobrd_type_mais                 = eobrd_cantype_mais,       // name is brdMAIS
    eobrd_type_foc                  = eobrd_cantype_foc,        // name is brdFOC
    
    // not used: ...
    eobrd_type_dspcan               = eobrd_cantype_dsp,        // name is brdDSPcan
    eobrd_type_piccan               = eobrd_cantype_pic,        // name is brdPICcan
    eobrd_type_2dccan               = eobrd_cantype_2dc,        // name is brd2DCcan
    eobrd_type_bllcan               = eobrd_cantype_bll,        // name is brdBLLcan
    eobrd_type_6sgcan               = eobrd_cantype_6sg,        // name is brd6SGcan  
    eobrd_type_jog                  = eobrd_cantype_jog,        // name is brdJOG      
    
    eobrd_type_none                 = 254,                      // name is brdNONE 	
    eobrd_type_unknown              = 255                       // name is brdUNKNOWN
} eObrd_type_t;

enum { eobrd_type_numberof = 8 };


typedef struct                  
{   // size is: 1+1+0 = 2
    uint8_t                     major;
    uint8_t                     minor;    
    uint8_t                     build;
} eObrd_firmwareversion_t;      EO_VERIFYsizeof(eObrd_firmwareversion_t, 3);


typedef struct                  
{   // size is: 1+1+0 = 2
    uint8_t                     major;
    uint8_t                     minor;    
} eObrd_protocolversion_t;      EO_VERIFYsizeof(eObrd_protocolversion_t, 2);


typedef struct                  
{   // size is: 1+1+2+2+0 = 6
    uint8_t                     type;
    eObrd_firmwareversion_t     firmware;
    eObrd_protocolversion_t     protocol;   
} eObrd_info_t;                 EO_VERIFYsizeof(eObrd_info_t, 6);


typedef enum
{
    eobrd_place_can     = 0,    // the place is on can bus, hence it requires a can address with (port, adr)
    eobrd_place_loc     = 1,    // the place is on the eth board. if more than one of the same type we need an identifier / index
    eobrd_place_extcan  = 2     // the place is on can bus but we need also an index (as for mc4can board)
} eObrd_place_t;


typedef struct
{
    uint8_t place : 2;       /**< use eObrd_place_t */    
    uint8_t dummy : 6;    
} eObrd_on_any_t;


typedef struct
{
    uint8_t place : 2;       /**< use eObrd_place_t */    
    uint8_t port  : 1;       /**< use eOcanport_t */
    uint8_t addr  : 4;       /**< use 0->14 */   
    uint8_t ffu   : 1;    
} eObrd_on_can_t;

typedef struct
{
    uint8_t place : 2;       /**< use eObrd_place_t */    
    uint8_t port  : 1;       /**< use eOcanport_t */
    uint8_t addr  : 4;       /**< use 0->14 */   
    uint8_t index : 1;       // use 0 or 1. 
} eObrd_on_extcan_t;

typedef struct
{
    uint8_t place : 2;       /**< use eObrd_place_t */    
    uint8_t id    : 6;       /**< not used for now */   
} eObrd_on_loc_t;


typedef union
{
    eObrd_on_any_t      any;
    eObrd_on_can_t      can;
    eObrd_on_extcan_t   extcan;
    eObrd_on_loc_t      loc;   
} eObrd_location_t;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


//extern uint8_t eoboards_maxsizeofnames(void);

extern eObool_t eoboards_is_ethboardtype(eObrd_type_t boardtype);

extern eObool_t eoboards_is_canboardtype(eObrd_type_t boardtype);

extern const char * eoboards_type2name(eObrd_type_t boardtype);

extern eObrd_type_t eoboards_name2type(const char * name);

extern eObrd_cantype_t eoboards_name2cantype(const char * name);

extern eObrd_ethtype_t eoboards_name2ethtype(const char * name);



/** @}            
    end of group eo_cevcwervcrev5555  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





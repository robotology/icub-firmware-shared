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
    eobrd_ethtype_unknown           = 255  // = ICUBCANPROTO_BOARDTYPE__UNKNOWN 
} eObrd_ethtype_t;

enum { eobrd_ethtype_numberof = 3 };


// use eoboards_is_can() / eoboards_is_eth() to check if the eObrd_type_t belongs also to eObrd_cantype_t / eObrd_ethtype_t group.
// use eoboards_type2string() / eoboards_string2type() to transform between string and value
// use eoboards_type2ethtype() / eoboards_type2cantype() when obtain a eObrd_type_t from a string and you need a specific type.

typedef enum
{
    eobrd_ems4                  = eobrd_ethtype_ems4,       // associated string is: "eobrd_ems4"
    eobrd_mc4plus               = eobrd_ethtype_mc4plus,    // etc ... the string is equal to the enum
    eobrd_mc2plus               = eobrd_ethtype_mc2plus,     
        
    eobrd_mc4                   = eobrd_cantype_mc4,        
    eobrd_mtb                   = eobrd_cantype_mtb,        
    eobrd_strain                = eobrd_cantype_strain,     
    eobrd_mais                  = eobrd_cantype_mais,       
    eobrd_foc                   = eobrd_cantype_foc,        
    
    // not used: ...
    eobrd_dsp                   = eobrd_cantype_dsp,        
    eobrd_pic                   = eobrd_cantype_pic,        
    eobrd_2dc                   = eobrd_cantype_2dc,        
    eobrd_bll                   = eobrd_cantype_bll,       
    eobrd_6sg                   = eobrd_cantype_6sg,        
    eobrd_jog                   = eobrd_cantype_jog,           
    
    eobrd_none                  = 254,                      
    eobrd_unknown               = 255  // = ICUBCANPROTO_BOARDTYPE__UNKNOWN                     
} eObrd_type_t;

enum { eobrd_type_numberof = 14 };


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


typedef struct
{
    eObrd_firmwareversion_t     firmware;
    eObrd_protocolversion_t     protocol; 
} eObrd_version_t;              EO_VERIFYsizeof(eObrd_version_t, 5);  


typedef enum
{
    eobrd_place_can     = 0,    // the place is on can bus, hence it requires a can address with (port, adr)
    eobrd_place_eth     = 1,    // the place is on the eth board. if more than one of the same type we need an identifier / index
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
    uint8_t index : 1;       // use eobrd_caninsideindex_first or eobrd_caninsideindex_second 
} eObrd_on_extcan_t;


typedef struct
{
    uint8_t place : 2;       /**< use eObrd_place_t */    
    uint8_t id    : 6;       /**< not used for now */   
} eObrd_on_eth_t;


// it is a compact location which can specify a can, an extended can (as used by mc can protocol), a simple eth placement 
typedef union
{
    eObrd_on_any_t      any;
    eObrd_on_can_t      can;
    eObrd_on_extcan_t   extcan;
    eObrd_on_eth_t      eth;   
} eObrd_location_t;     EO_VERIFYsizeof(eObrd_location_t, 1)


typedef struct
{
    eOipv4addr_t            ipv4address;        // it identifies the eth board
    eObrd_location_t        location;           // it tells where we locate inside the eth board 
    uint8_t                 ffu[3];
} eObrd_absolutelocation_t; EO_VERIFYsizeof(eObrd_absolutelocation_t, 8)


// it is an extended can location used fo far in .... EOtheCANmapping (actually in here we use eOcanmap_location_t) and in EoManagement (to describe placement of can boards)

typedef enum
{
    eobrd_caninsideindex_first          = 0,
    eobrd_caninsideindex_second         = 1,
    eobrd_caninsideindex_none           = 2
} eObrd_caninsideindex_t;


typedef struct
{
    uint8_t port : 1;               /**< use eOcanport_t */
    uint8_t addr : 4;               /**< use 0->14 */ 
    uint8_t insideindex : 2;        /**< use eObrd_caninsideindex_t*/
    uint8_t dummy : 1;              /**< unused ... but it could specify board or entity */
} eObrd_canlocation_t;


// with this we could remove eOcanmap_board_properties_t from EOtheCANmapping
typedef struct
{
    uint8_t                     type;               /**< use eObrd_cantype_t */
    eObrd_canlocation_t         location;           /**< its can location */
    eObrd_protocolversion_t     requiredprotocol;   /**<  */
} eObrd_canproperties_t;        EO_VERIFYsizeof(eObrd_canproperties_t, 4); 



    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


extern eObool_t eoboards_is_can(eObrd_type_t type);

extern eObool_t eoboards_is_eth(eObrd_type_t type);

extern eObrd_cantype_t eoboards_type2cantype(eObrd_type_t type);

extern eObrd_ethtype_t eoboards_type2ethtype(eObrd_type_t type);

extern const char * eoboards_type2string(eObrd_type_t type);

extern eObrd_type_t eoboards_string2type(const char * name);



/** @}            
    end of group eo_cevcwervcrev5555  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





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
    eobrd_cantype_pic               = ICUBCANPROTO_BOARDTYPE__PIC,      // 1, not used
    eobrd_cantype_2dc               = ICUBCANPROTO_BOARDTYPE__2DC,      // 2, not used
    eobrd_cantype_mc4               = ICUBCANPROTO_BOARDTYPE__4DC,      // 3
    eobrd_cantype_bll               = ICUBCANPROTO_BOARDTYPE__BLL,      // 4, not used
    eobrd_cantype_mtb               = ICUBCANPROTO_BOARDTYPE__SKIN,     // 5 
    eobrd_cantype_strain            = ICUBCANPROTO_BOARDTYPE__STRAIN,   // 6
    eobrd_cantype_mais              = ICUBCANPROTO_BOARDTYPE__MAIS,     // 7
    eobrd_cantype_foc               = ICUBCANPROTO_BOARDTYPE__2FOC,     // 8   
    eobrd_cantype_6sg               = ICUBCANPROTO_BOARDTYPE__6SG,      // 9, not used
    eobrd_cantype_jog               = ICUBCANPROTO_BOARDTYPE__JOG,      // 10, not used 
    eobrd_cantype_mtb4              = ICUBCANPROTO_BOARDTYPE__MTB4,     // 11 
    eobrd_cantype_strain2           = ICUBCANPROTO_BOARDTYPE__STRAIN2,  // 12  
    eobrd_cantype_rfe               = ICUBCANPROTO_BOARDTYPE__RFE,      // 13 (rfe = robot face expression)    
    eobrd_cantype_sg3               = ICUBCANPROTO_BOARDTYPE__SG3,      // 14 (sg3 = 3 strain gauges)
    eobrd_cantype_psc               = ICUBCANPROTO_BOARDTYPE__PSC,      // 15 (psc = proximal sensor collector)
    eobrd_cantype_mtb4w             = ICUBCANPROTO_BOARDTYPE__MTB4W,    // 16 (mtb4 for waseda university)
    eobrd_cantype_pmc               = ICUBCANPROTO_BOARDTYPE__PMC,      // 17 (pmc = piezo motor control)
    eobrd_cantype_amcbldc           = ICUBCANPROTO_BOARDTYPE__AMCBLDC,  // 18 (amcbldc)
    eobrd_cantype_bms               = ICUBCANPROTO_BOARDTYPE__BMS,      // 19 (bms)
    eobrd_cantype_mtb4c             = ICUBCANPROTO_BOARDTYPE__MTB4C,    // 20 (mtb4c)  
    eobrd_cantype_amc2c             = ICUBCANPROTO_BOARDTYPE__AMC2C,    // 21 (amc2c)    
    eobrd_cantype_strain2c          = ICUBCANPROTO_BOARDTYPE__STRAIN2C, // 22 (strain2c)
    eobrd_cantype_bat               = ICUBCANPROTO_BOARDTYPE__BAT,      // 23 (bat)
    eobrd_cantype_amcfoc2c          = ICUBCANPROTO_BOARDTYPE__AMCFOC2C, // 24 (amcfoc2c)

    eobrd_cantype_none              = 254, 	
    eobrd_cantype_unknown           = ICUBCANPROTO_BOARDTYPE__UNKNOWN   // 255 
} eObrd_cantype_t;

enum { eobrd_cantype_numberof = 25 };


typedef enum
{  
    eobrd_ethtype_ems4              = 32,      
    eobrd_ethtype_mc4plus           = 33,     
    eobrd_ethtype_mc2plus           = 34,   
    eobrd_ethtype_amc               = 35,
    eobrd_ethtype_amcfoc            = 36,
	
    eobrd_ethtype_none              = 254, 	
    eobrd_ethtype_unknown           = 255  // = ICUBCANPROTO_BOARDTYPE__UNKNOWN 
} eObrd_ethtype_t;

enum { eobrd_ethtype_numberof = 5 };


// use eoboards_is_can() / eoboards_is_eth() to check if the eObrd_type_t belongs also to eObrd_cantype_t / eObrd_ethtype_t group.
// use eoboards_type2string() / eoboards_string2type() to transform between string and value
// use eoboards_type2ethtype() / eoboards_type2cantype() when obtain a eObrd_type_t from a string and you need a specific type.

typedef enum
{
    eobrd_ems4                  = eobrd_ethtype_ems4,       // associated string is: "eobrd_ems4"
    eobrd_mc4plus               = eobrd_ethtype_mc4plus,    // etc ... the string is equal to the enum
    eobrd_mc2plus               = eobrd_ethtype_mc2plus,     
    eobrd_amc                   = eobrd_ethtype_amc,
	eobrd_amcfoc                = eobrd_ethtype_amcfoc,
    
    eobrd_mc4                   = eobrd_cantype_mc4,        
    eobrd_mtb                   = eobrd_cantype_mtb,        
    eobrd_strain                = eobrd_cantype_strain,     
    eobrd_mais                  = eobrd_cantype_mais,       
    eobrd_foc                   = eobrd_cantype_foc,        
    
    eobrd_dsp                   = eobrd_cantype_dsp,        
    eobrd_pic                   = eobrd_cantype_pic,        
    eobrd_2dc                   = eobrd_cantype_2dc,        
    eobrd_bll                   = eobrd_cantype_bll,       
    eobrd_6sg                   = eobrd_cantype_6sg,        
    eobrd_jog                   = eobrd_cantype_jog,   
    eobrd_mtb4                  = eobrd_cantype_mtb4,
    eobrd_strain2               = eobrd_cantype_strain2,  
    eobrd_rfe                   = eobrd_cantype_rfe,
    eobrd_sg3                   = eobrd_cantype_sg3,
    eobrd_psc                   = eobrd_cantype_psc,
    eobrd_mtb4w                 = eobrd_cantype_mtb4w,
    eobrd_pmc                   = eobrd_cantype_pmc,
    eobrd_amcbldc               = eobrd_cantype_amcbldc,
    eobrd_bms                   = eobrd_cantype_bms,
    eobrd_mtb4c                 = eobrd_cantype_mtb4c,
    eobrd_amc2c                 = eobrd_cantype_amc2c,
    eobrd_strain2c              = eobrd_cantype_strain2c,
    eobrd_bat                   = eobrd_cantype_bat,
    eobrd_amcfoc2c              = eobrd_cantype_amcfoc2c,


    eobrd_none                  = 254,                      
    eobrd_unknown               = 255  // = ICUBCANPROTO_BOARDTYPE__UNKNOWN                     
} eObrd_type_t;

enum { eobrd_type_numberof = 30 };


typedef struct                  
{   // size is: 1+1+0 = 2
    uint8_t                     major;
    uint8_t                     minor;    
    uint8_t                     build;
} eObrd_firmwareversion_t;      EO_VERIFYsizeof(eObrd_firmwareversion_t, 3)


typedef struct                  
{   // size is: 1+1+0 = 2
    uint8_t                     major;
    uint8_t                     minor;    
} eObrd_protocolversion_t;      EO_VERIFYsizeof(eObrd_protocolversion_t, 2)


typedef struct                  
{   // size is: 1+1+2+2+0 = 6
    uint8_t                     type;       // use eObrd_type_t
    eObrd_firmwareversion_t     firmware;
    eObrd_protocolversion_t     protocol;   
} eObrd_info_t;                 EO_VERIFYsizeof(eObrd_info_t, 6)


typedef struct
{
    eObrd_firmwareversion_t     firmware;
    eObrd_protocolversion_t     protocol; 
} eObrd_version_t;              EO_VERIFYsizeof(eObrd_version_t, 5)  


typedef enum
{
    eobrd_place_none    = 0,    // the place is ... nowhere
    eobrd_place_can     = 1,    // the place is on can bus, hence it requires a can address with (port, adr)
    eobrd_place_eth     = 2,    // the place is on the eth board. if more than one of the same type we need an identifier / index
    eobrd_place_extcan  = 3     // the place is on can bus but we need also an index (as for mc4can board)
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


// it is an extended can location used fo far in .... EOtheCANmapping and in EoManagement (to describe placement of can boards)

typedef enum
{
    eobrd_caninsideindex_first          = 0,
    eobrd_caninsideindex_second         = 1,
    eobrd_caninsideindex_none           = 2,  
    eobrd_caninsideindex_third          = 3, // in case we need a third 
    eobrd_caninsideindex_fourth         = 4  // or a fourth one
} eObrd_caninsideindex_t;


/** @typedef    typedef struct eObrd_canlocation_t
    @brief      tells the can location of a can board or of an entity (joint or motor or strain etc.) mapped into a can board.
                the location of the board uses port and addr. the location of an entity of motioncontrol (joint, motor) 
                uses also insideindex. all other entities dont need insideindex. when insideindex is not used, use value of eobrd_caninsideindex_none for it.
                about insideindex: legacy mc-boards such as the mc4can use only values eobrd_caninsideindex_first and eobrd_caninsideindex_second. 
                in jan 2021, however extra values eobrd_caninsideindex_third ... eobrd_caninsideindex_fourth were added. for the case of the pmc board which 
                manages three joints.
 **/  
typedef struct
{
    uint8_t port : 1;               /**< use eOcanport_t */
    uint8_t addr : 4;               /**< use 0->14 */ 
    uint8_t insideindex : 3;        /**< use eObrd_caninsideindex_t*/
} eObrd_canlocation_t;


/** @typedef    typedef struct eObrd_canproperties_t
    @brief      tells about properties of a can board or of an entity (joint or motor or strain etc.) mapped into a can board.
                it contains type of can board, it location, and teh required protocol.
 **/ 
typedef struct
{
    uint8_t                     type;               /**< use eObrd_cantype_t */
    eObrd_canlocation_t         location;           /**< its can location */
    eObrd_protocolversion_t     requiredprotocol;   /**< its protocol */
} eObrd_canproperties_t;        EO_VERIFYsizeof(eObrd_canproperties_t, 4) 


/** @typedef    typedef enum eObrd_connector_t
    @brief      contains numbers used for specifying a connector on a generic board. For instance, if we we want to refer to
                connector P10 of the ems board (which may be used by an AEA encoder), we use the value eobrd_conn_P10.
                The same we do if the connector is P10 on the mc4plus board, which incidentally is also an SPI connector.
                However, the physical ports in HAL terms corresponding to P10 in two different boards may be different.
                In ems P10 is linked to hal_encoder3 which has value 2, whereas in the mc4plus is linked to hal_encoder1
                which has value 0. Use proper function to convert between them.
 **/
typedef enum
{
    eobrd_conn_P1       = 1,
    eobrd_conn_P2       = 2,
    eobrd_conn_P3       = 3,
    eobrd_conn_P4       = 4,
    eobrd_conn_P5       = 5,
    eobrd_conn_P6       = 6,
    eobrd_conn_P7       = 7,
    eobrd_conn_P8       = 8,
    eobrd_conn_P9       = 9,
    eobrd_conn_P10      = 10,
    eobrd_conn_P11      = 11,
    eobrd_conn_P12      = 12,
    eobrd_conn_P13      = 13,
    eobrd_conn_P14      = 14,
    eobrd_conn_P15      = 15,
    eobrd_conn_J5_X1    = 16,
    eobrd_conn_J5_X2    = 17,
    eobrd_conn_J5_X3    = 18,
    eobrd_conn_J3_SDA0  = 19,
    eobrd_conn_J3_SDA1  = 20,
    eobrd_conn_J3_SDA2  = 21,
    eobrd_conn_J3_SDA3  = 22,
    eobrd_conn_J4       = 23,
    eobrd_conn_J5       = 24,
    eobrd_conn_J6       = 25,
    eobrd_conn_J7       = 26,  
    eobrd_conn_J20      = 27,
    eobrd_conn_J21      = 28,
    eobrd_conn_J22      = 29,
    eobrd_conn_J23      = 30,    
    eobrd_conn_J30      = 31,
    eobrd_conn_J31      = 32,
    eobrd_conn_J32      = 33,
    eobrd_conn_J33      = 34,
    eobrd_conn_P3P      = 35,
    eobrd_conn_P4P      = 36,
    eobrd_conn_J11      = 37,    
    eobrd_conn_J12      = 38,
    
    eobrd_conn_none     = 0,
    eobrd_conn_unknown  = 255
} eObrd_connector_t;

enum { eobrd_connectors_numberof = 38 };


typedef enum
{
    eobrd_port_none                 = 31,
    eobrd_port_unknown              = 30,
    eobrd_port_nolocal              = 29,

    eobrd_port_emsP6                = 0,        // SPI encoder: hal_encoder1
    eobrd_port_emsP7                = 3,        // SPI encoder: hal_encoder4
    eobrd_port_emsP8                = 1,        // SPI encoder: hal_encoder2
    eobrd_port_emsP9                = 4,        // SPI encoder: hal_encoder5
    eobrd_port_emsP10               = 2,        // SPI encoder: hal_encoder3
    eobrd_port_emsP11               = 5,        // SPI encoder: hal_encoder6

    eobrd_port_mc4plusP2            = 1,        // PWM & QUADENC: hal_motor2, hal_quad_enc2
    eobrd_port_mc4plusP3            = 0,        // PWM & QUADENC: hal_motor1, hal_quad_enc1
    eobrd_port_mc4plusP4            = 2,        // PWM & QUADENC: hal_motor3, hal_quad_enc3
    eobrd_port_mc4plusP5            = 3,        // PWM & QUADENC: hal_motor4, hal_quad_enc4
    eobrd_port_mc4plusP3P           = 4,        // PWM & QUADENC: hal_motor1, hal_quad_enc1 parrallelized driver
    eobrd_port_mc4plusP4P           = 5,        // PWM & QUADENC: hal_motor3, hal_quad_enc3 parrallelized driver
    eobrd_port_mc4plusP10           = 0,        // SPI encoder: hal_encoder1
    eobrd_port_mc4plusP11           = 1,        // SPI encoder: hal_encoder2
    
    eobrd_port_mc2plusP2            = 1,        // PWM & QUADENC: hal_motor2, hal_quad_enc2
    eobrd_port_mc2plusP3            = 0,        // PWM & QUADENC: hal_motor1, hal_quad_enc1
    eobrd_port_mc2plusP10           = 0,        // SPI encoder: hal_encoder1
    eobrd_port_mc2plusP11           = 1,        // SPI encoder: hal_encoder2   

    eobrd_port_amc_J5_X1            = 0,        // SPI encoder: embot::hw::encoder1
    eobrd_port_amc_J5_X2            = 1,        // SPI encoder: embot::hw::encoder2
    eobrd_port_amc_J5_X3            = 2,        // SPI encoder: embot::hw::encoder3

    eobrd_port_mtb4_J3_SDA0         = 0,
    eobrd_port_mtb4_J3_SDA1         = 1,
    eobrd_port_mtb4_J3_SDA2         = 2,
    eobrd_port_mtb4_J3_SDA3         = 3,  
    
    eobrd_port_mtb4c_J3_SDA0        = 0,
    eobrd_port_mtb4c_J3_SDA1        = 1,
    eobrd_port_mtb4c_J3_SDA2        = 2,
    eobrd_port_mtb4c_J3_SDA3        = 3,  
    
    eobrd_port_pmc_J4               = 0,        // I2C1 
    eobrd_port_pmc_J5               = 1,        // I2C2
    eobrd_port_pmc_J6               = 2,        // I2C3
    eobrd_port_pmc_J7               = 3,        // I2C4 
    
    eobrd_port_mtb4_mmaJ20          = 0,
    eobrd_port_mtb4_mmaJ21          = 1,
    eobrd_port_mtb4_mmaJ22          = 2,
    eobrd_port_mtb4_mmaJ23          = 3,      

    eobrd_port_mtb4c_mmaJ20         = 0,
    eobrd_port_mtb4c_mmaJ21         = 1,
    eobrd_port_mtb4c_mmaJ22         = 2,
    eobrd_port_mtb4c_mmaJ23         = 3, 
    
    eobrd_port_mtb4_mmaJ30          = 0,
    eobrd_port_mtb4_mmaJ31          = 1,
    eobrd_port_mtb4_mmaJ32          = 2,
    eobrd_port_mtb4_mmaJ33          = 3,      

    eobrd_port_mtb4c_mmaJ30         = 0,
    eobrd_port_mtb4c_mmaJ31         = 1,
    eobrd_port_mtb4c_mmaJ32         = 2,
    eobrd_port_mtb4c_mmaJ33         = 3,

    eobrd_port_amcfoc_J11           = 0,        // SPI encoder: embot::hw::encoder1
    eobrd_port_amcfoc_J12           = 1,        // SPI encoder: embot::hw::encoder2
       
} eObrd_port_t;

enum { eobrd_ports_numberof = 51 };


typedef enum
{
    eobrd_portmais_thumbproximal    = 0,
    eobrd_portmais_thumbdistal      = 1,
    eobrd_portmais_indexproximal    = 2,
    eobrd_portmais_indexdistal      = 3,
    eobrd_portmais_mediumproximal   = 4,
    eobrd_portmais_mediumdistal     = 5,
    eobrd_portmais_littlefingers    = 6,    
    eobrd_portmais_none             = 31,    // as ... eobrd_port_none
    eobrd_portmais_unknown          = 30     // as ... eobrd_port_unknown
} eObrd_portmais_t;

enum { eobrd_portmaiss_numberof = 7 };


typedef enum
{
    eobrd_portpsc_finger0           = 0,
    eobrd_portpsc_finger1           = 1,

    eobrd_portpsc_none              = 31,    // as ... eobrd_port_none
    eobrd_portpsc_unknown           = 30     // as ... eobrd_port_unknown
} eObrd_portpsc_t;

enum { eobrd_portpscs_numberof = 2 };


typedef enum
{
    eobrd_portpos_hand_thumb_oc         = 0,
    eobrd_portpos_hand_index_oc         = 1,
    eobrd_portpos_hand_middle_oc        = 2,
    eobrd_portpos_hand_ring_pinky_oc    = 3,
    eobrd_portpos_hand_thumb_add        = 4,
    eobrd_portpos_hand_tbd              = 5,
    eobrd_portpos_hand_index_add        = 6,    
    
    eobrd_portpos_none                  = 31,    // as ... eobrd_port_none
    eobrd_portpos_unknown               = 30     // as ... eobrd_port_unknown
} eObrd_portpos_t;

enum { eobrd_portposs_numberof = 7 };


typedef enum
{
    eobrd_canmonitor_reportmode_NEVER       = 0, // we never report
    eobrd_canmonitor_reportmode_justLOSTjustFOUND = 1, // report only once if boards disappears (or reappers)
    eobrd_canmonitor_reportmode_justLOSTjustFOUNDstillLOST = 2,
    eobrd_canmonitor_reportmode_ALL         = 3, // report changes but also period state (all present or still missing) 
    eobrd_canmonitor_reportmode_none        = 31,    
    eobrd_canmonitor_reportmode_unknown     = 30    
} eObrd_canmonitor_reportmode_t;

enum { eobrd_reportmodes_numberof = 4 };

typedef struct
{
    uint8_t         periodofcheck;      // in ms: period of check of presence of the boards. if 0 we dont do any check
    uint8_t         reportmode;         // use eObrd_canmonitor_reportmode_t
    uint16_t        periodofreport;     // in ms: period of reporting of presence or absence. if 0 we dont do any periodic report
} eObrd_canmonitor_cfg_t; EO_VERIFYsizeof(eObrd_canmonitor_cfg_t, 4) 
    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


extern eObool_t eoboards_is_can(eObrd_type_t type);

extern eObool_t eoboards_is_eth(eObrd_type_t type);

extern eObrd_cantype_t eoboards_type2cantype(eObrd_type_t type);

extern eObrd_ethtype_t eoboards_type2ethtype(eObrd_type_t type);

extern eObrd_type_t eoboards_ethtype2type(eObrd_ethtype_t type);

extern eObrd_type_t eoboards_cantype2type(eObrd_cantype_t type);

extern eObrd_type_t eoboards_string2type2(const char * name, eObool_t usecompactstring);
extern const char * eoboards_type2string2(eObrd_type_t type, eObool_t usecompactstring);

extern eObrd_type_t eoboards_string2type(const char * name);
extern const char * eoboards_type2string(eObrd_type_t type);

extern eObrd_connector_t eoboards_string2connector(const char * string, eObool_t usecompactstring);
extern const char * eoboards_connector2string(eObrd_connector_t connector, eObool_t usecompactstring);

extern eObrd_port_t eoboards_string2port(const char * string, eObool_t usecompactstring);
extern const char * eoboards_port2string(eObrd_port_t port, eObrd_type_t board, eObool_t usecompactstring);

extern eObrd_port_t eoboards_connector2port(eObrd_connector_t connector, eObrd_type_t board);
extern eObrd_connector_t eoboards_port2connector(eObrd_port_t port, eObrd_type_t board);

extern const char * eoboards_portmais2string(eObrd_portmais_t portmais, eObool_t usecompactstring);
extern eObrd_portmais_t eoboards_string2portmais(const char * string, eObool_t usecompactstring);

extern const char * eoboards_portpsc2string(eObrd_portpsc_t portpsc, eObool_t usecompactstring);
extern eObrd_portpsc_t eoboards_string2portpsc(const char * string, eObool_t usecompactstring);

extern const char * eoboards_portpos2string(eObrd_portpos_t portpos, eObool_t usecompactstring);
extern eObrd_portpos_t eoboards_string2portpos(const char * string, eObool_t usecompactstring);

extern const char * eoboards_reportmode2string(eObrd_canmonitor_reportmode_t mode, eObool_t usecompactstring);
extern eObrd_canmonitor_reportmode_t eoboards_string2reportmode(const char * string, eObool_t usecompactstring);

extern uint8_t eoboards_type2numberofcores(eObrd_type_t type);

/** @}            
    end of group eo_cevcwervcrev5555  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





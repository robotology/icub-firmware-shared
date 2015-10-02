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
#ifndef _EOMANAGEMENT_H_
#define _EOMANAGEMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EoManagement.h
	@brief      This header file gives 
	@author     marco.accame@iit.it
	@date       05/15/2012
**/

/** @defgroup eo_management Management of the ems board
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOarray.h"
#include "EOrop.h"



// - public #define  --------------------------------------------------------------------------------------------------




// it allows to fit a EOarray of 64 bytes (or 16 words)
#define EOMANAGEMENT_COMMAND_DATA_SIZE 68

// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef enum eOmn_entity_t;
    @brief      It contains all the possible entities in management.
                See definition of eOmn_comm_t, and eOmn_appl_t for explanations
 **/
typedef enum
{
    eomn_entity_comm                        = 0,
    eomn_entity_appl                        = 1,
    eomn_entity_info                        = 2,
    eomn_entity_service                     = 3
} eOmn_entity_t; 

enum { eomn_entities_numberof = 4 };

// -- all the possible enum


/** @typedef    typedef enum eOmn_appl_state_t;
    @brief      It contains the states of the state machine of the application.     
 **/
typedef enum 
{
    applstate_config        = 0,                /**< config */
    applstate_running       = 1,                /**< running */
    applstate_error         = 2,                /**< error */
    applstate_resetmicro    = 3,                /**< reset the micro and restart the system: loader, updater, ... */
    applstate_restartapp    = 4                 /**< reset the micro and restart the system BUT loader jumps immediately to application */
} eOmn_appl_state_t;



/** @typedef    typedef enum eOmn_commandtype_t;
    @brief      It contains all the operations which are possible to query the transceiver or to configure the regular EOrop 
                objects in the transceiver.   
 **/
typedef enum 
{
    eomn_opc_none                       = 0,                /**< it does nothing */
    
    eomn_opc_query_numof_EPs            = 1,                /**< ask how many endpoints are available */
    eomn_opc_reply_numof_EPs            = 2,                /**< tells how many endpoints are available */
    eomn_opc_query_array_EPs            = 3,                /**< ask the array of available endpoints  */
    eomn_opc_reply_array_EPs            = 4,                /**< tells the array of available endpoints  */
    eomn_opc_query_array_EPdes          = 5,                /**< ask the array of available eoprot_endpoint_descriptor_t (of 4 bytes) */
    eomn_opc_reply_array_EPdes          = 6,                /**< tells the array of available eoprot_endpoint_descriptor_t (of 4 bytes) */    
    eomn_opc_query_numof_ENs            = 7,                /**< ask how many entities are available in total */
    eomn_opc_reply_numof_ENs            = 8,                /**< tells how many entities are available in total */
    eomn_opc_query_array_ENdes          = 9,                /**< ask the array of available eoprot_entity_descriptor_t (of 4 bytes)*/
    eomn_opc_reply_array_ENdes          = 10,               /**< tells the array of available eoprot_entity_descriptor_t (of 4 bytes) */
    eomn_opc_query_numof_REGROPs        = 11,               /**< it asks the receiving device how many are the configured ropsigcfg */
    eomn_opc_reply_numof_REGROPs        = 12,               /**< it says the receiving device that inside two bytes of data[] there is the number of configured ropsigcfg */
    eomn_opc_query_array_REGROPs        = 13,               /**< it asks the receiving device the array of the configured ropsigcfg (one set) */
    eomn_opc_reply_array_REGROPs        = 14,                /**< it says the receiving device that inside data[] there are some configured ropsigcf */
    
    eomn_opc_config_REGROPs_clear       = 15,               /**< it forces a clear of all the regular rops in the transceiver */
    eomn_opc_config_REGROPs_assign      = 16,               /**< it copies in the reg rops all of the content of the delivered array */
    eomn_opc_config_REGROPs_append      = 17,               /**< it appends to the reg rops all of the content of the delivered array (if size is 1 then it is a pushback)*/
    eomn_opc_config_REGROPs_remove      = 18,               /**< it searches and removes from the reg rops all the items of the delivered array (its size must be > 0)*/

    eomn_opc_config_PROT_boardnumber    = 19,
    eomn_opc_config_PROT_endpoint       = 20

} eOmn_opc_t;


// -- all the possible data service structures


typedef struct
{
    eOenum08_t                      opc;            /**< use eOmn_opc_t w/ values eomn_opc_query_ */
    uint8_t                         endpoint;       /**< always eoprot_endpoint_all, otherwise it filters the reply for the specified endpoint */ 
    uint8_t                         dummy02[2];
} eOmn_opcpar_querynumof_t;


typedef struct
{
    eOenum08_t                      opc;            /**< use eOmn_opc_t w/ values eomn_opc_query_ */
    uint8_t                         endpoint;       /**< always eoprot_endpoint_all, otherwise it filters the reply for the specified endpoint */ 
    uint8_t                         setnumber;      /**< the number of the set, starting from 0 */
    uint8_t                         setsize;        /**< if 0 the reply will use the max capacity of its array, otherwise it will use specifeid setsize */  
} eOmn_opcpar_queryarray_t;

typedef struct
{
    eOenum08_t                      opc;            /**< use eOmn_opc_t w/ values eomn_opc_query_ */
    uint8_t                         endpoint;       /**< if not eoprot_endpoint_all it tells that the reply is about a specific endpoint */            
    uint16_t                        numberof;       /**< it contains the reply */ 
} eOmn_opcpar_replynumof_t;

typedef struct
{
    eOenum08_t                      opc;            /**< use eOmn_opc_t w/ values eomn_opc_query_ */
    uint8_t                         endpoint;       /**< if not eoprot_endpoint_all it tells that the reply is about a specific endpoint */            
    uint8_t                         setnumber;
    uint8_t                         setsize;  
} eOmn_opcpar_replyarray_t;

typedef struct
{
    eOenum08_t                      opc;                /**< use eOmn_opc_t w/ values eomn_opc_config_REGROPs_ */
    uint8_t                         plustime;
    uint8_t                         plussign;
    uint8_t                         dummy01;          
    uint32_t                        signature; 
} eOmn_opcpar_config_t;


typedef struct
{
    eOmn_opcpar_querynumof_t        opcpar;   
} eOmn_cmd_querynumof_t;

typedef struct
{
    eOmn_opcpar_queryarray_t        opcpar;   
} eOmn_cmd_queryarray_t;

typedef struct
{
    eOmn_opcpar_replynumof_t        opcpar; 
} eOmn_cmd_replynumof_t;

typedef struct
{
    eOmn_opcpar_replyarray_t        opcpar; 
    uint8_t                         array[EOMANAGEMENT_COMMAND_DATA_SIZE];     
} eOmn_cmd_replyarray_t;

typedef struct
{
    eOmn_opcpar_config_t            opcpar; 
    uint8_t                         array[EOMANAGEMENT_COMMAND_DATA_SIZE];     
} eOmn_cmd_config_t;

typedef union
{ 
    eOenum08_t                      opc;                /**< use vlaues from eOmn_opc_t to choose if we have a query, a reply, or a config */
    eOmn_cmd_querynumof_t           querynumof;
    eOmn_cmd_queryarray_t           queryarray;
    eOmn_cmd_replynumof_t           replynumof;
    eOmn_cmd_replyarray_t           replyarray;
    eOmn_cmd_config_t               config;
} eOmn_cmD_t;                       //EO_VERIFYsizeof(eOmn_cmD_t, 76);



/** @typedef    typedef struct eOmn_ropsigcfg_command_t;
    @brief      is the command used to configure the communication
 **/
typedef struct              // size is 76+4 = 80 bytes
{
    eOmn_cmD_t              cmd;                /** < eomn_opc_query_ uses only cmd, eomn_opc_reply_ and eomn_opc_config use data as well */
    uint8_t                 filler04[4];
} eOmn_command_t;           //EO_VERIFYsizeof(eOmn_command_t, 80);



/** @typedef    typedef enum eOmn_appl_runMode_t;
    @brief      contains ems application's run mode.
                Currently runMode is not set to ems by pc104, but application itself understands its runmode
                depending on connected CAN boards.
 **/
typedef enum
{
    applrunMode__default       = 0,
    applrunMode__skinOnly      = 1,
    applrunMode__mc4Only       = 2,
    applrunMode__skinAndMc4    = 3,
    applrunMode__2foc          = 4
} eOmn_appl_runMode_t;


typedef struct 
{
    uint8_t         major;
    uint8_t         minor;   
} eOmn_version_t;

typedef struct                     
{
    uint32_t            year  : 12;    /**< the year a.d. upto 2047 */
    uint32_t            month : 4;     /**< the month, where jan is 1, dec is 12 */
    uint32_t            day   : 5;     /**< the day from 1 to 31 */
    uint32_t            hour  : 5;     /**< the hour from 0 to 23 */
    uint32_t            min   : 6;     /**< the minute from 0 to 59 */
} eOmn_date_t; 


typedef struct
{
    uint16_t    listeningPort;
    uint16_t    destinationPort;
    uint16_t    maxsizeRXpacket;
    uint16_t    maxsizeTXpacket;
    uint16_t    maxsizeROPframeRegulars;
    uint16_t    maxsizeROPframeReplies;
    uint16_t    maxsizeROPframeOccasionals;
    uint16_t    maxsizeROP;
    uint16_t    maxnumberRegularROPs;
    uint8_t     filler06[6];
} eOmn_transceiver_properties_t;


// -- the definition of a comm entity

typedef struct
{
    uint8_t                         filler08[8];
} eOmn_comm_config_t;               //EO_VERIFYsizeof(eOmn_comm_config_t, 8);


typedef struct
{
    eOmn_version_t                  managementprotocolversion;  // of the mn endpoint
    uint8_t                         filler06[6];
    eOmn_transceiver_properties_t   transceiver;
} eOmn_comm_status_t;               //EO_VERIFYsizeof(eOmn_comm_status_t, 32);

typedef struct
{
    eOmn_command_t                  command;
} eOmn_comm_cmmnds_t;               //EO_VERIFYsizeof(eOmn_comm_cmmnds_t, 80);


/** @typedef    typedef struct eOmn_comm_t;
    @brief      used to represent the communication with config, status, commands. so far config and status are not used
 **/
typedef struct                      // size is 8+32+80+0 = 120 bytes
{
    eOmn_comm_config_t              config;
    eOmn_comm_status_t              status;
    eOmn_comm_cmmnds_t              cmmnds;
} eOmn_comm_t;                      //EO_VERIFYsizeof(eOmn_comm_t, 120);



// -- the definition of an appl entity

/** @typedef    typedef struct eOmn_appl_config_t;
    @brief      used to configure the application
 **/
typedef struct                      // size is 4+3+3 = 8 bytes
{
    eOreltime_t                     cycletime;      /**< FOR-FUTURE_USE: the time to be used for the control loop expressed in microseconds */
    uint8_t                         txratedivider;  /**< if equal to 1 (or 0) the cycle sends up packets at every cycles, if 2 it sends up packets every two cycles */ 
    uint8_t                         filler03[3];
} eOmn_appl_config_t;               //EO_VERIFYsizeof(eOmn_appl_config_t, 8);


/** @typedef    typedef struct eOmn_appl_status_t;
    @brief      used to report status of the application
 **/
typedef struct                      // size is 4+2+16+1+1+6+2 = 32 bytes
{
    eOmn_date_t                     buildate;
    eOmn_version_t                  version;
    uint8_t                         name[16];
    eOenum08_t                      currstate;          /**< use eOmn_appl_state_t */
    eOenum08_t                      runmode;            /**< use eOmn_appl_runMode_t */
	uint16_t						cloop_timings[3];
    uint8_t                         txdecimationfactor;
    uint8_t                         filler01;
} eOmn_appl_status_t;               EO_VERIFYsizeof(eOmn_appl_status_t, 32);

                                         
/** @typedef    typedef struct eOmn_appl_cmmnds_t;
    @brief      used to send commands to the application
 **/
typedef struct                      // size is 1+7 = 8 bytes
{
    eOenum08_t                      go2state;       /**< use eOmn_appl_state_t */
    uint8_t                         filler07[7];
} eOmn_appl_cmmnds_t;               //EO_VERIFYsizeof(eOmn_appl_cmmnds_t, 8);


/** @typedef    typedef struct eOmn_appl_t;
    @brief      used to represent the application with config, status, commands
 **/
typedef struct                      // size is 8+32+8 = 48 bytes
{
    eOmn_appl_config_t              config;
    eOmn_appl_status_t              status;
    eOmn_appl_cmmnds_t              cmmnds;
} eOmn_appl_t;                      //EO_VERIFYsizeof(eOmn_appl_t, 48);


// -- the definition of info entity

/** @typedef    typedef struct eOmn_info_config_t;
    @brief      used to configure the info
 **/
typedef struct                      // size is 1+7 = 8 bytes
{
    uint8_t                         enabled;
    uint8_t                         filler07[7];
} eOmn_info_config_t;               //EO_VERIFYsizeof(eOmn_info_config_t, 8);



typedef enum
{
    eomn_info_extraformat_none      = 0,    /**< there is no extra field. use it when eOmn_info_basic_t is used alone */
    eomn_info_extraformat_verbal    = 1,    /**< the extra field in eOmn_info_basic_t::extra is a string to be managed with sprintf() */
    eomn_info_extraformat_compact1  = 2     /**< the extra field in eOmn_info_basic_t::extra is coded in a compact mode of type 1 to be properly encoded/decoded  */   
} eOmn_info_extraformat_t;

typedef enum
{
    eomn_info_type_info             = 0,
    eomn_info_type_debug            = 1,   
    eomn_info_type_warning          = 2,
    eomn_info_type_error            = 3,
    eomn_info_type_fatal            = 4
} eOmn_info_type_t;

typedef enum
{
    eomn_info_source_board          = 0,
    eomn_info_source_can1           = 1,   
    eomn_info_source_can2           = 2
} eOmn_info_source_t;




#define EOMN_INFO_PROPERTIES_FLAGS_set_type(flags, type)                    ((flags) |= ((0x0007&((uint16_t)(type)))<<0))
#define EOMN_INFO_PROPERTIES_FLAGS_set_source(flags, source)                ((flags) |= ((0x0007&((uint16_t)(source)))<<3))
#define EOMN_INFO_PROPERTIES_FLAGS_set_address(flags, address)              ((flags) |= ((0x000f&((uint16_t)(address)))<<6))
#define EOMN_INFO_PROPERTIES_FLAGS_set_extraformat(flags, extraformat)      ((flags) |= ((0x0003&((uint16_t)(extraformat)))<<10))
#define EOMN_INFO_PROPERTIES_FLAGS_set_futureuse(flags, futureuse)          ((flags) |= ((0x000f&((uint16_t)(futureuse)))<<12))


#define EOMN_INFO_PROPERTIES_FLAGS_get_type(flags)                          (  ((flags)>>0)&0x0007  ) 
#define EOMN_INFO_PROPERTIES_FLAGS_get_source(flags)                        (  ((flags)>>3)&0x0007  ) 
#define EOMN_INFO_PROPERTIES_FLAGS_get_address(flags)                       (  ((flags)>>6)&0x000f  ) 
#define EOMN_INFO_PROPERTIES_FLAGS_get_extraformat(flags)                   (  ((flags)>>10)&0x0003  ) 
#define EOMN_INFO_PROPERTIES_FLAGS_get_futureuse(flags)                     (  ((flags)>>12)&0x000f  ) 

// this definition of eOmn_info_properties_t with flags guarantees portability. the mapping of bits remains unchanged across different machines.
// the bit field version, although more elegant, is not guaranteed about that because standard C does not specify the order in which the bits are packed. 
typedef struct
{
    uint32_t                code;           /**< the code of the info. used to communicate in a ultra-compact mode what happens. its value must be taken from a table */
    uint16_t                flags;          /**< the field flags contains the following sub-fields. type:3 uses eOmn_info_type_t; source:3 uses eOmn_info_source_t; address:4 contains the 
                                                 address of the source (0 for source == eomn_info_source_board); 
                                                 extraformat: 2 uses eOmn_info_extraformat_t to tell how eOmn_info_status_t::extra[] is used  */
    uint16_t                par16;          /**< these 2 bytes can be used to specify further the meaning of code. its use id code-dependant */
    uint64_t                par64;          /**< these 4 bytes can be used to specify further the meaning of code. its use id code-dependant */
} eOmn_info_properties_t;   EO_VERIFYsizeof(eOmn_info_properties_t, 16);




typedef struct
{
    uint64_t                timestamp;  /**< it keeps the absolute time in microseconds since the EMS has bootstrapped */
    eOmn_info_properties_t  properties; /**< specifies the properties of the info */          
} eOmn_info_basic_t;        EO_VERIFYsizeof(eOmn_info_basic_t, 24);


enum { eomn_info_status_extra_sizeof = 48 };

/** @typedef    typedef struct eOmn_info_status_t;
    @brief      used to report status of the info
 **/
typedef struct 
{
    eOmn_info_basic_t       basic;                                  /**< the basic info status */  
    uint8_t                 extra[eomn_info_status_extra_sizeof];   /**< contains either a descriptive string or a compact representation */
} eOmn_info_status_t;       EO_VERIFYsizeof(eOmn_info_status_t, 72); 



/** @typedef    typedef struct eOmn_info_t;
    @brief      used to represent the info with config, status
 **/
typedef struct                      // size is 8+32 = 40 bytes
{
    eOmn_info_config_t              config;
    eOmn_info_status_t              status;
} eOmn_info_t;                      EO_VERIFYsizeof(eOmn_info_t, 80);  



// -- the definition of service entity


// old wip mode: remove it later
//typedef union
//{
//    eOmn_serv_canlocation_t     canloc;
//    uint8_t                     pwmport;
//}  eOmn_serv_mc_actuator_t;    // 1B

//typedef struct
//{
//    uint8_t     type    : 4;            // aea, amo, whatever
//    uint8_t     port    : 4;            // the same as in hal    
//} eOmn_serv_mc_encoder_t;    // 1B

//typedef union
//{
//    eOmn_serv_canlocation_t     canloc;         // used by can based entities: mais, strain, skin, inertial
//    eOmn_serv_mc_actuator_t     mcactuator;     // used by jomo2foc, jomomc4, jomopwm
//    eOmn_serv_mc_encoder_t      mcencoder;      // used by jomo2foc, jomomc4, jomopwm
//} eOmn_serv_property_t;         // 1 B


//typedef struct
//{
//    uint8_t                     type    : 4;    // use values from eOmn_serv_item_type_t
//    uint8_t                     index   : 4;    // the index can be from 0 to ... 15. so far we have 12 jomos max.          
//    eOmn_serv_property_t        prop;
//} eOmn_serv_item_descriptor_t;  EO_VERIFYsizeof(eOmn_serv_item_descriptor_t, 2);




//// use it for services which use a single board: mais, strain, inertial, skin. 
//typedef struct
//{
//    uint8_t                         boardtype;              // use eObrd_cantype_t
//    uint8_t                         filler[3];
//    eOmn_version_t                  firmwareversion;
//    eOmn_version_t                  protocolversion;    
//    uint16_t                        canmap[eOcanports_number];  // use bitmap of required can addresses.
//} eOmn_serv_cmd_resources_can_t;    EO_VERIFYsizeof(eOmn_serv_cmd_resources_can_t, 12);

//// use it for services which use a single board: mais, strain, inertial, skin. 
//typedef struct
//{
//    eOmn_serv_cmd_resources_can_t   mc2foc;
//    eOmn_serv_mc_encoder_t          encoders[4]; // but i must specify 
//} eOmn_serv_cmd_resources_mc_2foc_enc_t;    EO_VERIFYsizeof(eOmn_serv_cmd_resources_mc_2foc_enc_t, 16);

//typedef union
//{
//    uint8_t                         data[32];
//    eOmn_serv_cmd_resources_can_t   can;
//    
//} eOmn_serv_cmd_resources_check_data_t;

//typedef struct
//{
//    uint8_t                     type;           // use eOmn_serv_type_t.
//    uint8_t                     filler[3];
//    uint8_t                     data[32];       // keeps some data ....
//} eOmn_serv_cmd_resources_check_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_check_t, 36);




//typedef struct
//{
//    uint8_t         boardtype;  // used by the emscontroller for ems-2foc / mc4plus to identify what particular actions to follow.
//    uint8_t         filler[3];
//    uint8_t         array[32];  // us as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_mcdata_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_mcdata_t, 36)

//typedef struct
//{
//    uint8_t         filler[4];
//    uint8_t         array[32];  // us as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_data_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_data_t, 36)


//typedef struct
//{
//    uint8_t                     type;           // use eOmn_serv_type_t.
//    uint8_t                     numofentities;  // num of jomos for MC_*, 1 for AS_*, 1 or 2 for SK_skin
//    uint8_t                     filler[2];
//    uint8_t                     data[36];       // keeps some data .... use eOmn_serv_cmd_resources_set_mcdata_t / eOmn_serv_cmd_resources_set_data_t
//} eOmn_serv_cmd_resources_set_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_t, 40);



// i try to define a single redundant type which is good both for check and for config or i use a single check-config command.
// by this i must 

//typedef struct
//{
//    eOmn_version_t      firmware;
//    eOmn_version_t      protocol; 
//} eOmn_board_versions_t;    
    

//typedef struct  // use it for 2foc mc control
//{
//    uint8_t                     boardtype;  // used by the emscontroller for ems-2foc / mc4plus to identify what particular actions to follow.
//    uint8_t                     filler[3];
//    eOmn_board_versions_t       version2foc;
//    uint8_t                     filler2[4];
//    uint8_t                     array[32];  // used as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_MC_2foc_enc_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_MC_2foc_enc_t, 44)

//typedef struct  // use it for mc4mais mc control
//{
//    uint8_t                     filler[4];
//    eOmn_board_versions_t       version2foc;
//    eOmn_board_versions_t       version2mais;
//    uint8_t                     array[32];      // used as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_MC_mc4_mais_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_MC_mc4_mais_t, 44)


//typedef struct  // use it for mais, strain, inertial, skin
//{
//    uint8_t                     filler[4];
//    eOmn_board_versions_t       version;
//    uint8_t                     filler2[4];
//    uint8_t                     array[32];  // used as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_CANonly_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_CANonly_t, 44)


//typedef struct  // use it for eomn_serv_MC_mc4plus
//{
//    uint8_t                     boardtype;  // used by the emscontroller for ems-2foc / mc4plus to identify what particular actions to follow.
//    uint8_t                     filler[3];
//    uint8_t                     filler2[8];
//    uint8_t                     array[32];  // used as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_MC_mc4plus_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_MC_mc4plus_t, 44)


//typedef struct  // use it for eomn_serv_MC_mc4plus_mais
//{
//    uint8_t                     boardtype;  // used by the emscontroller for ems-2foc / mc4plus to identify what particular actions to follow.
//    uint8_t                     filler[7];
//    eOmn_board_versions_t       version2mais;
//    uint8_t                     array[32];  // used as an EOarray<eOmn_serv_item_descriptor_t>     
//} eOmn_serv_cmd_resources_set_MC_mc4plus_mais_t; EO_VERIFYsizeof(eOmn_serv_cmd_resources_set_MC_mc4plus_mais_t, 44)




// START OF LATEST WIP DEFINITIONS

typedef enum
{
    eomn_serv_caninsideindex_first      = 0,
    eomn_serv_caninsideindex_second     = 1,
    eomn_serv_caninsideindex_none       = 2
} eOmn_serv_caninsideindex_t;


typedef struct
{
    uint8_t port : 1;               /**< use eOcanport_t */
    uint8_t addr : 4;               /**< use 0->14 */ 
    uint8_t insideindex : 2;        /**< use eOmn_serv_caninsideindex_t*/
    uint8_t dummy : 1;              /**< unused ... but it could specify board or entity */
} eOmn_serv_canlocation_t;


typedef struct
{
    eOmn_version_t      firmware;
    eOmn_version_t      protocol; 
} eOmn_serv_canboardversion_t;  

typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_mc_actuator_foc_t;

typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_mc_actuator_mc4_t;

typedef struct
{
    uint8_t                     port;
} eOmn_serv_mc_actuator_pwm_t;

typedef union
{
    eOmn_serv_mc_actuator_foc_t foc;
    eOmn_serv_mc_actuator_mc4_t mc4;
    eOmn_serv_mc_actuator_pwm_t pwm;
}  eOmn_serv_mc_actuator2_t;    // 1B


typedef struct
{
    uint8_t             port;
} eOmn_serv_mc_encoder2_aea_t;

typedef struct
{
    uint8_t             port;
} eOmn_serv_mc_encoder2_amo_t;


typedef struct
{
    uint8_t             port;
} eOmn_serv_mc_encoder2_inc_t;

typedef struct
{
    uint8_t             port;
} eOmn_serv_mc_hallsensor_t;


typedef union
{
    eOmn_serv_mc_encoder2_aea_t     aea;
    eOmn_serv_mc_encoder2_amo_t     amo;
    eOmn_serv_mc_encoder2_inc_t     inc;
}  eOmn_serv_mc_encoder2_t;    // 1B

typedef union
{   // if we need a different sensor whihc is not an encoder, we add in here
    eOmn_serv_mc_encoder2_t     encoder;
    eOmn_serv_mc_hallsensor_t   hall;
} eOmn_serv_mc_sensor_t;

typedef union
{
    eOmn_serv_mc_actuator2_t    actuator;     // used by jomo2foc, jomomc4, jomopwm
    //eOmn_serv_mc_encoder_t      encoder;        // used by jomo2foc, jomomc4, jomopwm
    eOmn_serv_mc_sensor_t       sensor;        // used by jomo2foc, jomomc4, jomopwm
} eOmn_serv_motionitem_t;    EO_VERIFYsizeof(eOmn_serv_motionitem_t, 1)


typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_as_mais_t;

typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_as_strain_t;

typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_as_inertial_t;

typedef union
{
    eOmn_serv_as_mais_t         mais;           // used by jomo2foc, jomomc4, jomopwm
    eOmn_serv_as_strain_t       strain;        // used by jomo2foc, jomomc4, jomopwm
    eOmn_serv_as_inertial_t     inertial;        // used by jomo2foc, jomomc4, jomopwm
} eOmn_serv_analogitem_t;       EO_VERIFYsizeof(eOmn_serv_analogitem_t, 1)


typedef struct
{
    eOmn_serv_canlocation_t     canloc;
} eOmn_serv_skinitem_t;         EO_VERIFYsizeof(eOmn_serv_skinitem_t, 1)

typedef union
{
    eOmn_serv_canlocation_t     canloc;         // used if eomn_serv_item_canloc
    eOmn_serv_motionitem_t      motion;         // used by eomn_serv_item_mc_*  
    eOmn_serv_analogitem_t      analog;         // used by eomn_serv_item_as_*
    eOmn_serv_skinitem_t        skin;           // used by eomn_serv_item_skin
} eOmn_serv_item_t;             EO_VERIFYsizeof(eOmn_serv_item_t, 1)



typedef enum
{
    eomn_serv_item_none                     = 0,
    eomn_serv_item_mc_actuator_foc          = 1,
    eomn_serv_item_mc_actuator_mc4          = 2,
    eomn_serv_item_mc_actuator_pwm          = 3,
    eomn_serv_item_mc_sensor_encoder_aea    = 4,
    eomn_serv_item_mc_sensor_encoder_amo    = 5,
    eomn_serv_item_mc_sensor_encoder_inc    = 6,
    eomn_serv_item_mc_sensor_hall           = 7,
    eomn_serv_item_as_mais                  = 8,
    eomn_serv_item_as_strain                = 9,
    eomn_serv_item_skin                     = 10,
    eomn_serv_item_as_inertial              = 11,
    eomn_serv_item_canloc                   = 15
} eOmn_serv_item_type_t;


typedef struct
{
    uint8_t                     type    : 4;    // use values from eOmn_serv_item_type_t
    uint8_t                     index   : 4;    // the index of the entity: can be from 0 to ... 15. so far we have 12 jomos max.          
    eOmn_serv_item_t            item;
} eOmn_serv_item_descriptor2_t;  EO_VERIFYsizeof(eOmn_serv_item_descriptor2_t, 2);



typedef enum 
{
    eomn_serv_NONE              = 0,
    eomn_serv_MC_foc            = 1,
    eomn_serv_MC_mc4            = 2,
    eomn_serv_MC_mc4plus        = 3,    // as in the head
    eomn_serv_MC_mc4plusmais    = 4,    // as in the arm, where we need also a mais
    eomn_serv_AS_mais           = 5,
    eomn_serv_AS_strain         = 6,
    eomn_serv_AS_inertial       = 7,
    eomn_serv_SK_skin           = 8
} eOmn_serv_type_t;



typedef struct
{
    eOmn_serv_canboardversion_t         versionofmais;
    eOmn_serv_item_descriptor2_t        descriptor; 
} eOmn_serv_config_data_as_mais_t;      EO_VERIFYsizeof(eOmn_serv_config_data_as_mais_t, 6);

typedef struct
{
    eOmn_serv_canboardversion_t         versionofmais;
    //eOmn_serv_item_descriptor2_t        descriptor;
    eOmn_serv_canlocation_t             canloc;
} eOmn_serv_config_data_as_mais_BIS_t;  EO_VERIFYsizeof(eOmn_serv_config_data_as_mais_BIS_t, 5);

typedef struct
{
    eOmn_serv_canboardversion_t         versionofstrain;
    eOmn_serv_item_descriptor2_t        descriptor; 
} eOmn_serv_config_data_as_strain_t;    EO_VERIFYsizeof(eOmn_serv_config_data_as_strain_t, 6);


typedef struct
{
    eOmn_serv_canboardversion_t         versionofstrain;
    //eOmn_serv_item_descriptor2_t        descriptor; 
    eOmn_serv_canlocation_t             canloc;
} eOmn_serv_config_data_as_strain_BIS_t;    EO_VERIFYsizeof(eOmn_serv_config_data_as_strain_BIS_t, 5);

typedef struct
{
    eOarray_head_t                      head;
    eOmn_serv_item_descriptor2_t        data[16];
} eOmn_serv_arrayof_16descriptors_t;    EO_VERIFYsizeof(eOmn_serv_arrayof_16descriptors_t, 36);

typedef struct
{ 
    eOmn_serv_canboardversion_t         versionofmtb;
    eOmn_serv_arrayof_16descriptors_t   descriptors; 
} eOmn_serv_config_data_as_inertial_t;  EO_VERIFYsizeof(eOmn_serv_config_data_as_inertial_t, 40);


typedef struct
{ 
    eOmn_serv_canboardversion_t         versionofmtb;
    uint16_t                            canmap[eOcanports_number]; 
} eOmn_serv_config_data_as_inertial_BIS_t;  EO_VERIFYsizeof(eOmn_serv_config_data_as_inertial_BIS_t, 8);


typedef union
{
    eOmn_serv_config_data_as_mais_t     mais;
    eOmn_serv_config_data_as_strain_t   strain;
    eOmn_serv_config_data_as_inertial_t inertial;   
} eOmn_serv_config_data_as_t;           EO_VERIFYsizeof(eOmn_serv_config_data_as_t, 40);


typedef union
{
    eOmn_serv_config_data_as_mais_BIS_t     mais;
    eOmn_serv_config_data_as_strain_BIS_t   strain;
    eOmn_serv_config_data_as_inertial_BIS_t inertial;   
} eOmn_serv_config_data_as_BIS_t;           EO_VERIFYsizeof(eOmn_serv_config_data_as_BIS_t, 8);

typedef struct
{
    uint8_t                             numofentities;  // skins
    uint8_t                             filler[3];
    eOmn_serv_canboardversion_t         versionofmtb;
    eOmn_serv_arrayof_16descriptors_t   arrayofdescriptors;    // contains at most 13 mtb-boards 
} eOmn_serv_config_data_sk_skin_t;      EO_VERIFYsizeof(eOmn_serv_config_data_sk_skin_t, 44);

typedef union
{
    eOmn_serv_config_data_sk_skin_t     skin;
} eOmn_serv_config_data_sk_t; 


#define maxskins 2
typedef struct
{
    eOmn_serv_canboardversion_t         versionofmtb;
    uint8_t                             numofskins;
    uint8_t                             filler[3];    
    uint16_t                            canmapskin[maxskins][2]; 
} eOmn_serv_config_data_sk_skin_BIS_t;  EO_VERIFYsizeof(eOmn_serv_config_data_sk_skin_BIS_t, 16);

typedef union
{
    eOmn_serv_config_data_sk_skin_BIS_t     skin;
} eOmn_serv_config_data_sk_BIS_t;       EO_VERIFYsizeof(eOmn_serv_config_data_sk_BIS_t, 16); 



typedef enum
{
    eomn_serv_mc_sensor_none                = eomn_serv_item_none,
    eomn_serv_mc_sensor_encoder_aea         = eomn_serv_item_mc_sensor_encoder_aea,
    eomn_serv_mc_sensor_encoder_amo         = eomn_serv_item_mc_sensor_encoder_amo,
    eomn_serv_mc_sensor_encoder_inc         = eomn_serv_item_mc_sensor_encoder_inc,
    eomn_serv_mc_sensor_hall                = eomn_serv_item_mc_sensor_hall
} eOmn_serv_mc_sensor_type_t;

typedef struct
{
    uint8_t     type : 4;       // use eOmn_serv_mc_sensor_type_t
    uint8_t     port : 4;       // use the hal_ corresponding port   
} eOmn_serv_mc_sensor2_t;

typedef struct
{
    eOmn_serv_mc_actuator2_t    actuator;
    eOmn_serv_mc_sensor2_t      sensor;
    eOmn_serv_mc_sensor2_t      extrasensor;
} eOmn_serv_jomo_descriptor_t;  EO_VERIFYsizeof(eOmn_serv_jomo_descriptor_t, 3);


typedef struct
{
    eOarray_head_t                      head;
    eOmn_serv_jomo_descriptor_t         data[4];
} eOmn_serv_arrayof_4jomodescriptors_t; EO_VERIFYsizeof(eOmn_serv_arrayof_4jomodescriptors_t, 16);


typedef struct
{
    eOmn_serv_canboardversion_t         versionoffoc;
    uint8_t                             numofjoints;  
    uint8_t                             boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                             filler[2];
    eOmn_serv_arrayof_16descriptors_t   arrayofdescriptors;    // contains at most 4 foc-boards and 4 encoders         
} eOmn_serv_config_data_mc_foc_t;       EO_VERIFYsizeof(eOmn_serv_config_data_mc_foc_t, 44);

typedef struct
{    
    uint8_t                                 boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                                 filler[3];    
    eOmn_serv_canboardversion_t             versionoffoc;
    eOmn_serv_arrayof_4jomodescriptors_t    arrayofjomodescriptors;           
} eOmn_serv_config_data_mc_foc_BIS_t;       EO_VERIFYsizeof(eOmn_serv_config_data_mc_foc_BIS_t, 24);


typedef struct
{
    eOmn_serv_canboardversion_t         versionofmc4; 
    eOmn_serv_canboardversion_t         versionofmais;
    uint8_t                             numofjoints;  // joints ... it is 12
    uint8_t                             filler[3];
    eOmn_serv_arrayof_16descriptors_t   arrayofdescriptors;    // contains exactly 12 mc4-boards and 1 mais board         
} eOmn_serv_config_data_mc_mc4_t;       EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4_t, 48);

typedef struct
{
    eOmn_serv_canboardversion_t         versionofmc4; 
    eOmn_serv_canboardversion_t         versionofmais;
    eOmn_serv_canlocation_t             mc4joints[12];    
    eOmn_serv_canlocation_t             maislocation;
    uint8_t                             filler[3];
} eOmn_serv_config_data_mc_mc4_BIS_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4_BIS_t, 24);



typedef struct
{
    uint8_t                             numofjoints;        
    uint8_t                             boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                             filler[2];
    eOmn_serv_arrayof_16descriptors_t   arrayofdescriptors;    // contains at most 4 actuators-pwm and 4 sensors (encoders-aea/inc but also hall). it is able to keep also 4 secondary sensors         
} eOmn_serv_config_data_mc_mc4plus_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plus_t, 40);


typedef struct
{ 
    uint8_t                             boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                             filler[3];
   eOmn_serv_arrayof_4jomodescriptors_t arrayofjomodescriptors;         
} eOmn_serv_config_data_mc_mc4plus_BIS_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plus_BIS_t, 20);


typedef struct
{ 
    eOmn_serv_canboardversion_t             versionofmais;
    eOmn_serv_canlocation_t                 maislocation;
    uint8_t                                 boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                                 filler[2];
    eOmn_serv_arrayof_4jomodescriptors_t    arrayofjomodescriptors;         
} eOmn_serv_config_data_mc_mc4plusmais_BIS_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plusmais_BIS_t, 24);

typedef struct
{
    eOmn_serv_canboardversion_t         versionofmais;
    uint8_t                             numofentities;          // joints
    uint8_t                             boardtype4mccontroller; // use eOemscontroller_board_t. that is required because the EOemsController needs to know which board it manages
    uint8_t                             filler[2];
    eOmn_serv_arrayof_16descriptors_t   arrayofdescriptors;    // contains at most 4 actuators-pwm and 4 sensors (encoders-aea/inc but also hall). it is able to keep also 4 secondary sensors         
} eOmn_serv_config_data_mc_mc4plusmais_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plusmais_t, 44);


typedef union
{
    eOmn_serv_config_data_mc_foc_t      foc_based;
    eOmn_serv_config_data_mc_mc4_t      mc4_based;
    eOmn_serv_config_data_mc_mc4plus_t  mc4plus_based;
    eOmn_serv_config_data_mc_mc4plusmais_t  mc4plusmais_based;
} eOmn_serv_config_data_mc_t;           EO_VERIFYsizeof(eOmn_serv_config_data_mc_t, 48); 




typedef union
{
    eOmn_serv_config_data_mc_foc_BIS_t          foc_based;
    eOmn_serv_config_data_mc_mc4_BIS_t          mc4_based;
    eOmn_serv_config_data_mc_mc4plus_BIS_t      mc4plus_based;
    eOmn_serv_config_data_mc_mc4plusmais_BIS_t  mc4plusmais_based;
} eOmn_serv_config_data_mc_BIS_t;               EO_VERIFYsizeof(eOmn_serv_config_data_mc_BIS_t, 24); 

typedef union
{
    eOmn_serv_config_data_as_t          as;
    eOmn_serv_config_data_mc_t          mc;
    eOmn_serv_config_data_sk_t          sk;   
} eOmn_serv_config_data_t;              EO_VERIFYsizeof(eOmn_serv_config_data_t, 48); 


typedef union
{
    eOmn_serv_config_data_as_BIS_t          as;
    eOmn_serv_config_data_mc_BIS_t          mc;
    eOmn_serv_config_data_sk_BIS_t          sk;   
} eOmn_serv_config_data_BIS_t;              EO_VERIFYsizeof(eOmn_serv_config_data_BIS_t, 24); 

// ok, devo ridurre la size of eOmn_serv_config_data_as_t

//typedef struct
//{
//    uint8_t                             type;           // use eOmn_serv_type_t to identify what kind of service it is
//    uint8_t                             filler[3];
//    eOmn_serv_config_data_t             data;   
//} eOmn_serv_configuration_t;            EO_VERIFYsizeof(eOmn_serv_configuration_t, 52); 



typedef struct
{
    uint8_t                             type;           // use eOmn_serv_type_t to identify what kind of service it is
    uint8_t                             filler[3];
    eOmn_serv_config_data_BIS_t         data;   
} eOmn_serv_configuration_t;            EO_VERIFYsizeof(eOmn_serv_configuration_t, 28); 

// use eOemscontroller_board_t and variable mccontrollerboardtype

// according to itemdes.type we treat item as follow:
// - eomn_serv_item_as_mais, eomn_serv_item_as_strain, eomn_serv_item_as_inertial:
//   we use itemdes.item.analog.[mais, strain, inertial].canloc to keep the can address of the entity.
//
// - eomn_serv_item_skin
//    we use itemdes.item.skin.canloc to keep the can address of the entity. 
//
// - eomn_serv_item_mc_actuator_foc, eomn_serv_item_mc_actuator_mc4,
//   we use itemdes.item.motion.actuator.[foc, mc4].canloc to keep the can address of 2foc or mc4 keeping the the joint / motor entity.
// - eomn_serv_item_mc_actuator_pwm
//   we use itemdes.item.motion.actuator.pwm.port to keep the hal port pwm for use in mc4plus boards associated to the joint/motor.
// - eomn_serv_item_mc_encoder_*
//   we use itemdes.item.motion.encoder.[amo, aea, inc] to keep the hal port of the encoder to use

typedef enum 
{
    eomn_serv_command_deactivate    = 0,        // it deactivates    
    eomn_serv_command_activate      = 1,        // it activate with the given config, but does not force activation if already active
    eomn_serv_command_reactivate    = 2         // it deactivates if active and then activates   
} eOmn_service_command_t;

typedef struct
{
    uint8_t                         command;               // use eOmn_service_command_t
    uint8_t                         filler[3];
    eOmn_serv_configuration_t       config;  
} eOmn_service_cmmnds_command_t;    EO_VERIFYsizeof(eOmn_service_cmmnds_command_t, 32);

typedef struct
{
    eOmn_service_cmmnds_command_t   command;    
} eOmn_service_cmmnds_t;            EO_VERIFYsizeof(eOmn_service_cmmnds_t, 32);


typedef struct
{
    eObool_t                        latestcommandisok;
    uint8_t                         command;                // use eOmn_service_command_t
    uint8_t                         type;                   // use eOmn_serv_type_t
    uint8_t                         filler[1];
    uint8_t                         data[28];               // it may keep some params (e.g., the fullscale of strain).
} eOmn_service_command_result_t;

typedef struct
{  
    eOmn_service_command_result_t   commandresult;
} eOmn_service_status_t;            EO_VERIFYsizeof(eOmn_service_status_t, 32); 


/** @typedef    typedef struct eOmn_info_t;
    @brief      used to represent the info with config, status
 **/
typedef struct                      // size is 8+32 = 40 bytes
{    
    eOmn_service_status_t           status;
    eOmn_service_cmmnds_t           cmmnds;
} eOmn_service_t;                   EO_VERIFYsizeof(eOmn_service_t, 64);  

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section




/** @}            
    end of group eo_management  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard
 

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





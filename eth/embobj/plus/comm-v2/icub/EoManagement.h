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

#include "EoAnalogSensors.h"



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

// category is the first division .... then there is the specific type
typedef enum
{
    eomn_serv_category_mc               = 0,
    eomn_serv_category_strain           = 1,
    eomn_serv_category_mais             = 2,
    eomn_serv_category_inertials        = 3,
    eomn_serv_category_skin             = 4,
    eomn_serv_category_all              = 128,
    eomn_serv_category_unknown          = 254,
    eomn_serv_category_none             = 255
} eOmn_serv_category_t;


// use eomn_servicetype2string() and eomn_string2servicetype() to convert to / from eOmn_serv_type_t and associated string
typedef enum 
{
    eomn_serv_MC_generic        = 0,        // associated string is: "eomn_serv_MC_generic"
    eomn_serv_MC_foc            = 1,        // etc ... the string is equal to the enum
    eomn_serv_MC_mc4            = 2,
    eomn_serv_MC_mc4plus        = 3,    
    eomn_serv_MC_mc4plusmais    = 4,    
    eomn_serv_AS_mais           = 5,
    eomn_serv_AS_strain         = 6,
    eomn_serv_AS_inertials      = 7,
    eomn_serv_SK_skin           = 8,
    eomn_serv_UNKNOWN           = 254,
    eomn_serv_NONE              = 255
} eOmn_serv_type_t;

enum { eomn_serv_types_numberof = 9 };


typedef struct
{
    eObrd_canlocation_t         canloc;
} eOmn_serv_mc_actuator_foc_t;

typedef struct
{
    eObrd_canlocation_t         canloc;
} eOmn_serv_mc_actuator_mc4_t;

typedef struct
{
    uint8_t                     port : 5;   // use eOmn_serv_mc_port_t
    uint8_t                     dummy : 3;
} eOmn_serv_mc_actuator_pwm_t;

typedef union
{
    eOmn_serv_mc_actuator_foc_t foc;
    eOmn_serv_mc_actuator_mc4_t mc4;
    eOmn_serv_mc_actuator_pwm_t pwm;
}  eOmn_serv_mc_actuator_t;    // 1B


typedef enum
{
    eomn_serv_item_none                             = 0,
    eomn_serv_item_mc_actuator_foc                  = 1,
    eomn_serv_item_mc_actuator_mc4                  = 2,
    eomn_serv_item_mc_actuator_pwm                  = 3,
//    eomn_serv_item_mc_sensor_encoder_spichainof2    = 3,    
//    eomn_serv_item_mc_sensor_encoder_aea            = 4,
//    eomn_serv_item_mc_sensor_encoder_amo            = 5,
//    eomn_serv_item_mc_sensor_encoder_inc            = 6,
//    eomn_serv_item_mc_sensor_hall                   = 7,
    eomn_serv_item_as_mais                          = 8,
    eomn_serv_item_as_strain                        = 9,
    eomn_serv_item_skin                             = 10,
    eomn_serv_item_as_inertial                      = 11,
    eomn_serv_item_canloc                           = 15
} eOmn_serv_item_type_t;


typedef struct
{   // 5+1=6
    eObrd_version_t                     version;
    eObrd_canlocation_t                 canloc;
} eOmn_serv_config_data_as_mais_t;      EO_VERIFYsizeof(eOmn_serv_config_data_as_mais_t, 6);



typedef struct
{   // 5+1=6
    eObrd_version_t                     version;
    eObrd_canlocation_t                 canloc;
} eOmn_serv_config_data_as_strain_t;    EO_VERIFYsizeof(eOmn_serv_config_data_as_strain_t, 6);


typedef struct
{   // 5+3+100=108 
    eObrd_version_t                     mtbversion;
    uint8_t                             filler[3];
    eOas_inertial_arrayof_sensors_t     arrayofsensors;
} eOmn_serv_config_data_as_inertial_t;  EO_VERIFYsizeof(eOmn_serv_config_data_as_inertial_t, 108);


typedef union
{   // max(6, 6, 108)
    eOmn_serv_config_data_as_mais_t     mais;
    eOmn_serv_config_data_as_strain_t   strain;
    eOmn_serv_config_data_as_inertial_t inertial;  
} eOmn_serv_config_data_as_t;           EO_VERIFYsizeof(eOmn_serv_config_data_as_t, 108);



enum { eomn_serv_skin_maxpatches = 4 };

typedef struct
{   // 5+1+2+2*4*2=24
    eObrd_version_t                     version;
    uint8_t                             numofpatches;
    uint8_t                             filler[2];    
    uint16_t                            canmapskin[eomn_serv_skin_maxpatches][eOcanports_number]; 
} eOmn_serv_config_data_sk_skin_t;      EO_VERIFYsizeof(eOmn_serv_config_data_sk_skin_t, 24);


typedef union
{   // max(24)
    eOmn_serv_config_data_sk_skin_t     skin;
} eOmn_serv_config_data_sk_t;           EO_VERIFYsizeof(eOmn_serv_config_data_sk_t, 24); 


typedef enum
{   // values can be from 1 to 255. 0 is none
    eomn_serv_mc_sensor_none                = eomn_serv_item_none,
    eomn_serv_mc_sensor_encoder_aea         = 1,
    eomn_serv_mc_sensor_encoder_amo         = 2,
    eomn_serv_mc_sensor_encoder_inc         = 3,
    eomn_serv_mc_sensor_encoder_spichainof2 = 4,
    eomn_serv_mc_sensor_encoder_absanalog   = 5,
    eomn_serv_mc_sensor_mais                = 6,
    eomn_serv_mc_sensor_encoder_spichainof3 = 7,
    eomn_serv_mc_sensor_encoder_onfoc       = 8,
    
    eomn_serv_mc_sensor_maxvalidvalue       = 255 
} eOmn_serv_mc_sensor_type_t;


typedef enum
{   // values can be from 1 to 255. 0 is none
    eomn_serv_mc_sensor_nuomofcomp_zero     = eomn_serv_item_none,
    eomn_serv_mc_sensor_nuomofcomp_one      = 1,
    eomn_serv_mc_sensor_nuomofcomp_two      = 2,
    eomn_serv_mc_sensor_nuomofcomp_three    = 3,
    eomn_serv_mc_sensor_nuomofcomp_max      = eomn_serv_mc_sensor_nuomofcomp_three
} eOmn_serv_mc_sensor_nuomofcomponents_t;


//#warning TODO: split eOmn_serv_mc_port_t into eOmn_serv_mc_sensor_port_t and eOmn_serv_mc_actuator_port_t. uniform 0 as the _none (and use port-1 in hal_) ...

typedef enum
{   // values from 0 up to .... maxport
    eomn_serv_mc_port_none                  = 31,   // max value in 5 bits.
    eomn_serv_mc_port_ems_spiP6             = 0,    // keep it as hal_encoder1. it is in spistream1
    eomn_serv_mc_port_ems_spiP7             = 3,    // keep it as hal_encoder4. it is in spistream2
    eomn_serv_mc_port_ems_spiP8             = 1,    // keep it as hal_encoder2. it is in spistream1
    eomn_serv_mc_port_ems_spiP9             = 4,    // keep it as hal_encoder5. it is in spistream2
    eomn_serv_mc_port_ems_spiP10            = 2,    // keep it as hal_encoder3. it is in spistream1
    eomn_serv_mc_port_ems_spiP11            = 5,    // keep it as hal_encoder6. it is in spistream2
    eomn_serv_mc_port_mc4plus_spiP10        = 0,    // keep it as hal_encoder1. it is in spistream1
    eomn_serv_mc_port_mc4plus_spiP11        = 1,    // keep it as hal_encoder2. it is in spistream2
    eomn_serv_mc_port_mc4plus_pwmP2         = 1,    // its is hal_motor2
    eomn_serv_mc_port_mc4plus_pwmP3         = 0,    // its is hal_motor1
    eomn_serv_mc_port_mc4plus_pwmP4         = 2,    // its is hal_motor3
    eomn_serv_mc_port_mc4plus_pwmP5         = 3,    // its is hal_motor4
    eomn_serv_mc_port_mc4plus_qencP2        = 1,    // its is hal_quad_enc2
    eomn_serv_mc_port_mc4plus_qencP3        = 0,    // its is hal_quad_enc1
    eomn_serv_mc_port_mc4plus_qencP4        = 2,    // its is hal_quad_enc3
    eomn_serv_mc_port_mc4plus_qencP5        = 3,    // its is hal_quad_enc4   

    eomn_serv_mc_port_mais_thumb_proximal   = 0,
    eomn_serv_mc_port_mais_thumb_distal     = 1,
    eomn_serv_mc_port_mais_index_proximal   = 2,
    eomn_serv_mc_port_mais_index_distal     = 3,
    eomn_serv_mc_port_mais_medium_proximal  = 4,
    eomn_serv_mc_port_mais_medium_distal    = 5,
    eomn_serv_mc_port_mais_littlefingers    = 6, 
       
    eomn_serv_mc_port_maxvalidvalue = 30
} eOmn_serv_mc_port_t;


typedef enum
{
    eomn_serv_mc_sensor_pos_none            = 0,    
    eomn_serv_mc_sensor_pos_atjoint         = 1,
    eomn_serv_mc_sensor_pos_atmotor         = 2
} eOmn_serv_mc_sensor_position_t;


typedef struct
{
    uint8_t     type;           // use eOmn_serv_mc_sensor_type_t
    uint8_t     port : 5;       // use eOmn_serv_mc_port_t 
    uint8_t     pos  : 3;       // use eOmn_serv_mc_sensor_position_t
} eOmn_serv_mc_sensor_t;        // 2B


typedef struct                  // 1+2+2=5
{
    eOmn_serv_mc_actuator_t     actuator;
    eOmn_serv_mc_sensor_t       sensor;
    eOmn_serv_mc_sensor_t       extrasensor;
} eOmn_serv_jomo_descriptor_t;  EO_VERIFYsizeof(eOmn_serv_jomo_descriptor_t, 5);


typedef struct                          
{   // 4+4*5=24
    eOarray_head_t                      head;
    eOmn_serv_jomo_descriptor_t         data[4];
} eOmn_serv_arrayof_4jomodescriptors_t; EO_VERIFYsizeof(eOmn_serv_arrayof_4jomodescriptors_t, 24);


typedef eOq17_14_t eOmn_4x4_matrix_t[4][4];

typedef struct 
{
    uint8_t                 joint2set[4];       // it contains the set each joint belongs to. the sets must be in increasing order 0, 1, etc.
    eOmn_4x4_matrix_t       joint2motor;
    eOmn_4x4_matrix_t       joint2encoder;    
} eOmn_4jomo_coupling_t;    EO_VERIFYsizeof(eOmn_4jomo_coupling_t, 132);


typedef struct
{   // 1+5+2+24+132=164   
    uint8_t                                 boardtype4mccontroller; 
    eObrd_version_t                         version;
    uint8_t                                 filler[2];  
    eOmn_serv_arrayof_4jomodescriptors_t    arrayofjomodescriptors;  
    eOmn_4jomo_coupling_t                   jomocoupling;  
} eOmn_serv_config_data_mc_foc_t;           EO_VERIFYsizeof(eOmn_serv_config_data_mc_foc_t, 164);

typedef struct
{
    uint8_t         velocity;
    uint8_t         estimJointVelocity      : 4;
    uint8_t         estimJointAcceleration  : 4;
    uint8_t         estimMotorVelocity      : 4;
    uint8_t         estimMotorAcceleration  : 4;    
} eOmn_serv_config_data_mc_mc4shifts_t;     EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4shifts_t, 3);


typedef struct
{   // 5+3+12+6=26
    eObrd_version_t                         mc4version; 
    eOmn_serv_config_data_mc_mc4shifts_t    mc4shifts; 
    eObrd_canlocation_t                     mc4joints[12];    
    eOmn_serv_config_data_as_mais_t         mais;                    
} eOmn_serv_config_data_mc_mc4_t;           EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4_t, 26);


typedef struct
{   // 1+3+24+132=160
    uint8_t                                 boardtype4mccontroller; 
    uint8_t                                 filler[3];
    eOmn_serv_arrayof_4jomodescriptors_t    arrayofjomodescriptors; 
    eOmn_4jomo_coupling_t                   jomocoupling;   
} eOmn_serv_config_data_mc_mc4plus_t;       EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plus_t, 160);


typedef struct
{   // 1+6+1+24+132=164
    uint8_t                                 boardtype4mccontroller; 
    eOmn_serv_config_data_as_mais_t         mais;
    uint8_t                                 filler[1];
    eOmn_serv_arrayof_4jomodescriptors_t    arrayofjomodescriptors;  
    eOmn_4jomo_coupling_t                   jomocoupling;   
} eOmn_serv_config_data_mc_mc4plusmais_t;   EO_VERIFYsizeof(eOmn_serv_config_data_mc_mc4plusmais_t, 164);



typedef union                               
{   // max(164, 26, 160, 164)
    eOmn_serv_config_data_mc_foc_t          foc_based;
    eOmn_serv_config_data_mc_mc4_t          mc4_based;
    eOmn_serv_config_data_mc_mc4plus_t      mc4plus_based;
    eOmn_serv_config_data_mc_mc4plusmais_t  mc4plusmais_based;
} eOmn_serv_config_data_mc_t;               EO_VERIFYsizeof(eOmn_serv_config_data_mc_t, 164); 


typedef union                               
{   // max(108, 164, 24)
    eOmn_serv_config_data_as_t              as;
    eOmn_serv_config_data_mc_t              mc;
    eOmn_serv_config_data_sk_t              sk;   
} eOmn_serv_config_data_t;                  EO_VERIFYsizeof(eOmn_serv_config_data_t, 164); 



typedef struct                              
{   // 1+3+164=168
    uint8_t                                 type;           // use eOmn_serv_type_t to identify what kind of service it is
    uint8_t                                 filler[3];
    eOmn_serv_config_data_t                 data;   
} eOmn_serv_configuration_t;                EO_VERIFYsizeof(eOmn_serv_configuration_t, 168); 

enum { eOmn_serv_capacity_arrayof_id32 = 41 };
typedef struct
{
    eOarray_head_t                          head;
    uint32_t                                data[eOmn_serv_capacity_arrayof_id32];   
} eOmn_serv_arrayof_id32_t;                 EO_VERIFYsizeof(eOmn_serv_arrayof_id32_t, 168); 

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


// notsupported -> notsupported

// idle         -> (idle, activated)
// activated    -> (idle, running)
// running      -> (idle, activated)

typedef enum
{
    eomn_serv_state_notsupported            = 0,
    eomn_serv_state_idle                    = 1,
    eomn_serv_state_verifying               = 2,
    eomn_serv_state_verified                = 3,
    eomn_serv_state_activated               = 4,
    eomn_serv_state_failureofverify         = 5, 
    eomn_serv_state_started                 = 6,
    eomn_serv_state_system_in_fatalerror    = 7
} eOmn_serv_state_t;



enum { eomn_serv_categories_numberof = 5 };


typedef enum 
{
    eomn_serv_operation_deactivate          = 0,    // it deactivates the service specified by .category (the status will be idle)
//    eomn_serv_operation_verify            = 1,    // it verifies the service specified by .category with configuration specified by .configuration        
//    eomn_serv_operation_activate          = 2,    // it activates the service .category w/ .configuration
    eomn_serv_operation_verifyactivate      = 3,    // it verifies and activates the service .category w/ .configuration  
    eomn_serv_operation_start               = 4,    // it starts the service .category (which must be already activated)
    eomn_serv_operation_stop                = 5,    // it stops the service .category 
    eomn_serv_operation_regsig_load         = 6,    // it loads the into the service all the regular sig rops specified by the array of id32 in parameter.arrayofid32
    eomn_serv_operation_regsig_clear        = 7,    // it clear all the regular sig rops in the service.
    eomn_serv_operation_none                = 255
} eOmn_serv_operation_t;


typedef union
{
    eOmn_serv_configuration_t   configuration;
    eOmn_serv_arrayof_id32_t    arrayofid32;
} eOmn_serv_parameter_t;


typedef struct                                
{   // 1+3+168=172
    uint8_t                                 operation;              // use eOmn_serv_operation_t
    uint8_t                                 category;               // use eOmn_serv_category_t
    uint8_t                                 filler[2];
    eOmn_serv_parameter_t                   parameter;
} eOmn_service_cmmnds_command_t;            EO_VERIFYsizeof(eOmn_service_cmmnds_command_t, 172);

typedef struct
{   // 172
    eOmn_service_cmmnds_command_t           command;    
} eOmn_service_cmmnds_t;                    EO_VERIFYsizeof(eOmn_service_cmmnds_t, 172);


typedef struct
{   // 1+1+1+1+28=32
    eObool_t                                latestcommandisok;
    uint8_t                                 operation;              // use eOmn_service_operation_t
    uint8_t                                 category;               // use eOmn_serv_category_t
    uint8_t                                 type;                   // use eOmn_serv_type_t
    uint8_t                                 data[28];               // it may keep some params (e.g., the fullscale of strain).
} eOmn_service_command_result_t;            EO_VERIFYsizeof(eOmn_service_command_result_t, 32); 


typedef struct
{
    uint8_t     applstate;              // use (CFG, RUN, ERR) from eOmn_appl_state_t. it tells if teh applciation is in run mode or not after the stop of the service 
    uint8_t     totalregulars;          // number of regulars for all services
    uint8_t     serviceregulars;        // number of regulars for the specified service (if  eomn_serv_category_all, then this number is equel to totalregulars).  
    uint8_t     dummy;
} eOmn_service_command_stop_result_data_t;


typedef struct
{   // 8 + 32
    uint8_t                                 stateofservice[eomn_serv_categories_numberof];     // use eOmn_serv_state_t
    uint8_t                                 filler[3];    
    eOmn_service_command_result_t           commandresult;
} eOmn_service_status_t;                    EO_VERIFYsizeof(eOmn_service_status_t, 40); 


/** @typedef    typedef struct eOmn_info_t;
    @brief      used to represent the info with config, status
 **/
typedef struct                      
{   // 40+172=212    
    eOmn_service_status_t                   status;
    eOmn_service_cmmnds_t                   cmmnds;
} eOmn_service_t;                           EO_VERIFYsizeof(eOmn_service_t, 212);  

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------

extern const char * eomn_servicetype2string(eOmn_serv_type_t service);

extern eOmn_serv_type_t eomn_string2servicetype(const char * string);

extern eOmn_serv_mc_sensor_nuomofcomponents_t eomn_mc_sensor_getnumofcomponets(eOmn_serv_mc_sensor_type_t type);


/** @}            
    end of group eo_management  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard
 

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





/*
 * Copyright (C) 2013 iCub Facility - Istituto Italiano di Tecnologia
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
#ifndef _EOPROTOCOL_H_
#define _EOPROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoProtocol.h
    @brief      This header file gives the ...
    @author     marco.accame@iit.it
    @date       06/05/2013
**/

/** @defgroup eo_EoProtocol ETH protocol for iCub 
    This file contains public interface for the iCub ETH protocol.
    
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

#include "EOnv.h"
#include "EOrop.h"
#include "EOarray.h"

#include "EoAnalogSensors.h"
#include "EoManagement.h"
#include "EoMotionControl.h"
#include "EoSkin.h"


// - public #define  --------------------------------------------------------------------------------------------------

// can be defined or undefined
//#define EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME
//#undef EOPROT_CFG_OVERRIDE_CALLBACKS_IN_RUNTIME

#define EOPROT_CFG_REMOTE_BOARDS_USE_DYNAMIC_MODE

#define eo_prot_ID32dummy       EOK_uint32dummy
#define eo_prot_PROGnumdummy    EOK_uint32dummy
#define eo_prot_BRDdummy        EOK_uint08dummy

// this macro can be used to obtain a eOprotID32_t value as function eoprot_ID_get() does, but as a macro it can be used for initialisation of constants  
#define EOPROT_ID_GET(end, ent, ind, tag)   ( (uint32_t)( (((end)&0xff)<<24) | (((ent)&0xff)<<16) | (((ind)&0xff)<<8) | (((tag)&0xff)) ) ) 

// - declaration of public user-defined types ------------------------------------------------------------------------- 


// - types used in definition of functions and for generic use


/** @typedef    typedef uint32_t eOprotIP_t
    @brief      used to identify a variable in a network, as the eOprotIP_t resembles the IP address of a device..
 **/
typedef uint32_t eOprotIP_t;


/** @typedef    typedef uint8_t eOprotBRD_t
    @brief      used to identify a board with values starting from 0 upto the max number of boards in the network.
 **/
typedef uint8_t eOprotBRD_t;

#if defined(EOPROT_CFG_REMOTE_BOARDS_USE_DYNAMIC_MODE)
enum { eoprot_board_remotes_maxnumberof = 128 };    // this number just limits the max number of allocable remote boards
#else
enum { eoprot_board_remotes_maxnumberof = 32 };    // this number forces static allocation of some data structure, thus keep it low
#endif

enum { eoprot_board_localboard = 254 };     // use this value to retrieve info about the board configured as local

/** @typedef    typedef uint32_t eOprotID32_t
    @brief      used to identify a variable inside a device. It can have holes in its representation
 **/
typedef uint32_t eOprotID32_t;


/** @typedef    uint32_t eOprotProgNumber_t
    @brief      it identifies a variable inside a device but it does not have holes in its representation.
 **/
typedef uint32_t eOprotProgNumber_t;


/** @typedef    typedef uint8_t eOprotEndpoint_t
    @brief      It is part of eOprotID32_t. The endpoint is used to group some variables of homogeneous type inside a given device. 
                for example it can be motion control in left lower arm. 
 **/
typedef uint8_t eOprotEndpoint_t;

enum { eoprot_endpoints_maxnumberof = 256 };  


/** @typedef    uint8_t eOprotEntity_t
    @brief      It is part of eOprotID32_t. it can be a joint or a motor in a motion control endpoint.
                It is used as a generic type for function arguments. You should use the relevant type of the endpoint
                in use: eOprotEntityEPas_t, eOprotEntityEPmc_t, eOprotEntityEPmn_t, or eOprotEntityEPsk_t
 **/
typedef uint8_t eOprotEntity_t;

enum { eoprot_entities_maxnumberof = 256 };  


/** @typedef    uint8_t eOprotIndex_t
    @brief      It is part of eOprotID32_t. It can be the index of a joint or a motor in a motion control endpoint.
 **/
typedef uint8_t eOprotIndex_t;

enum { eoprot_indices_maxnumberof = 256 }; 


/** @typedef    uint8_t eOprotTag_t
    @brief      It is part of eOprotID32_t. it can be associated to the PID of a joint, or to its measured position.
 **/
typedef uint8_t eOprotTag_t;

enum { eoprot_tags_maxnumberof = 256 };    // the current implementation uses at most 256 possible tags per entity of a given type per endpoint.


/** @typedef    typedef struct eoprot_version_t
    @brief      contains the version used in eoprotocol
 **/
typedef struct  
{
    uint8_t     major;                  /**< the major number */
    uint8_t     minor;                  /**< the minor number */
} eoprot_version_t;


/** @typedef    typedef struct eoprot_endpoint_descriptor_t
    @brief      contains a description for the entity
 **/
typedef struct  
{
    eOprotEndpoint_t    endpoint;               /**< the endpoint */
    uint8_t             entitiesinside;         /**< the number of entities inside. example: in analogsensors we use 1 if only strain is inside, even if there are two of them */
    eoprot_version_t    version;                /**< its version */
} eoprot_endpoint_descriptor_t;

/** @typedef    typedef struct eoprot_entity_descriptor_t
    @brief      contains a description for the entity
 **/
typedef struct  
{
    eOprotEndpoint_t    endpoint;               /**< the endpoint which contains the entity */
    eOprotEntity_t      entity;                 /**< the entity */
    uint8_t             multiplicity;           /**< tells how many of this entity are available: e.g., 4 joints */
    uint8_t             numberoftags;           /**< tells how many tags are available for this entity */
} eoprot_entity_descriptor_t;


// - specific values and types

/** @typedef    typedef enum eOprot_endpoint_t;
    @brief      It contains all the endpoints used in iCub. The values could be assigned in any way, however,
                its regularity helps to speed-up the encoding/decoding process (e.g., by using rom-based tables
                or cpu-efficient bitwise arithmetic).
 **/
typedef enum
{
    eoprot_endpoint_management              = 0,                /**< management of the ems board: comm+appl */   
    eoprot_endpoint_motioncontrol           = 1,                /**< management of motion control  */        
    eoprot_endpoint_analogsensors           = 2,                /**< management of analog sensors  */ 
    eoprot_endpoint_skin                    = 3,                /**< management of skin  */     
    eoprot_endpoint_all                     = 254,              /**< specifies all the endpoints in some operations */
    eoprot_endpoint_none                    = EOK_uint08dummy   /**< specified an invalid endpoint */
} eOprot_endpoint_t;

enum { eoprot_endpoints_numberof = 4 }; // it does not count the eoprot_endpoint_none and eoprot_endpoint_all.


/** @typedef    typedef enum eOprot_entity_t;
    @brief      It contains all the entities used in iCub. The used values are coming from the relevant files.
                For instance, the value for eoprot_entity_mc_joint is eomc_entity_joint which is defined in
                file EoMotionControl.h.               
 **/
typedef enum
{
    eoprot_entity_mn_comm                   = eomn_entity_comm,         /**<  */  
    eoprot_entity_mn_appl                   = eomn_entity_appl,         /**<  */
    eoprot_entity_mn_info                   = eomn_entity_info,         /**<  */ 
    eoprot_entity_mn_service                = eomn_entity_service,      /**<  */     
    eoprot_entity_mc_joint                  = eomc_entity_joint,        /**<  */ 
    eoprot_entity_mc_motor                  = eomc_entity_motor,        /**<  */     
    eoprot_entity_mc_controller             = eomc_entity_controller,   /**<  */    
    eoprot_entity_as_strain                 = eoas_entity_strain,       /**<  */   
    eoprot_entity_as_mais                   = eoas_entity_mais,         /**<  */            
    eoprot_entity_as_temperature            = eoas_entity_temperature,  /**<  */  
    eoprot_entity_as_inertial               = eoas_entity_inertial,     /**<  */   
    eoprot_entity_as_inertial3              = eoas_entity_inertial3,    /**<  */  
    eoprot_entity_as_psc                    = eoas_entity_psc,          /**<  */   
    eoprot_entity_as_pos                    = eoas_entity_pos,          /**<  */        
    eoprot_entity_sk_skin                   = eosk_entity_skin,         /**<  */
    eoprot_entity_none                      = EOK_uint08dummy
} eOprot_entity_t;

enum { eoprot_entities_numberof = 15 }; // it does not count the eoprot_entity_none.


/** @typedef    typedef enum eOprot_index_t
    @brief      It describes the index of the entity.  It is simply a number from 0 to EOK_uint08dummy-1.                
 **/
typedef enum
{
    eoprot_index_0                          = 0,                /**< the first index */
    eoprot_index_1                          = 1,                /**< the second index */
    eoprot_index_254                        = 254,              /**< the last index */
    eoprot_index_none                       = EOK_uint08dummy   /**< the dummy index */
} eOprot_index_t;


/** @typedef    typedef enum eOprot_tag_t;
    @brief      It describes the name structure of the tags. The tags are defined in the header files of the
                relevant endpoint. For instance, the tag of the endpoint motion control which identifies
                the whole configuration of the entity joint is eoprot_tag_mc_joint_config and is defined
                inside file EoProtocolMC.h.                
 **/
typedef enum
{
    eoprot_tag_xx_nameofentity_nameoftag    = 0,    /**< xx is the two-letter short for the entity: as, mn, mc, sk, etc. */
    eoprot_tag_none                         = EOK_uint08dummy   /**< the dummy tag */
} eOprot_tag_t;


// the callbacks are the same for every board and every index. they can be differentiated only by endpoint, entity and tag.
// however, when called by the protocol they are associated to a single variable. To clarify, if we specify a myinit() function
// for endpoint eoprot_endpoint_motioncontrol, entity eoprot_entity_mc_joint, and tag eoprot_tag_mc_joint_config, the same myinit()
// will be called for every board and for every joint in the board to initialise the jointconfig.
typedef struct
{
    eOprotEndpoint_t            endpoint;
    eOprotEntity_t              entity;
    eOprotTag_t                 tag;
    eOvoid_fp_cnvp_t            init;           /**< useful to initialise the ram (or else) of the single variable */ 
    eOvoid_fp_cnvp_cropdesp_t   update;         /**< useful to propagate a change of value of the variable to higher layers */  
} eOprot_callbacks_variable_descriptor_t;



typedef struct
{
    eOprotEndpoint_t            endpoint;
    eOvoid_fp_uint32_voidp_t    raminitialise;  /**< useful to initialise the entire ram of the endpoint */   
} eOprot_callbacks_endpoint_descriptor_t;


enum { eoprot_maxvalueof_entity = 6 }; // must be higher equal than the values inside eOprot_entity_t
enum { eoprot_entities_maxnumberofsupported = eoprot_maxvalueof_entity+1 }; // it is used to shape data structures
// it is what is enough to configure an endpoint
typedef struct                     
{
    eOprotEndpoint_t    endpoint;                                                   /*< the endpoint */
    uint8_t             numberofentities[eoprot_entities_maxnumberofsupported];     /*< the multiplicity of each entity in position of the entity */
} eOprot_EPcfg_t;

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOprot_EPcfg_t eoprot_arrayof_stdEPcfg[eoprot_endpoints_numberof];
extern const eOprot_EPcfg_t eoprot_arrayof_maxEPcfg[eoprot_endpoints_numberof];
extern const eOprot_EPcfg_t eoprot_arrayof_maxEPcfgOthers[eoprot_endpoints_numberof-1]; // all but management



// - declaration of extern public functions ---------------------------------------------------------------------------

// functions which just make conversions

/** @fn         extern eOprotID32_t eoprot_ID_get(eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index, eOprotTag_t tag)
    @brief      it retrieves the ID of a variable given (ep, entity, index, tag). The ID is equal for any board.
    @param      ep              the endpoint. Use values from eOprot_endpoint_t.
    @param      entity          the entity. Use relevant values for the enum of the specific endpoint: eOprotEntityEPas_t, 
                                eOprotEntityEPmc_t, eOprotEntityEPmn_t, or eOprotEntityEPsk_t
    @param      index           the index of the entity (from 0).  
    @param      tag             the tag of the variable of the index-th entity (from 0 to eoprot_tags_numberof-1).
    @return     the variable ID or EOK_uint32dummy if there is no variable defined by the quadruple (ep, entity, index, tag).
 **/
extern eOprotID32_t eoprot_ID_get(eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index, eOprotTag_t tag); 


/** @fn         extern eOprotEndpoint_t eoprot_ID2endpoint(eOprotID32_t id)
    @brief      it extracts the endpoint from the ID.
    @param      id              the identifier of the variable.
    @return     the entity or EOK_uint8dummy in case of invalid parameters.
 **/
extern eOprotEndpoint_t eoprot_ID2endpoint(eOprotID32_t id); 


/** @fn         extern eOprotEntity_t eoprot_ep_variable_ID2entity(eOprotID32_t id)
    @brief      it extracts the entity from the ID.
    @param      id              the identifier of the variable.
    @return     the entity or EOK_uint8dummy in case of invalid parameters.
 **/
extern eOprotEntity_t eoprot_ID2entity(eOprotID32_t id); 


/** @fn         extern eOprotIndex_t eoprot_ep_variable_ID2number(eOprotID32_t id)
    @brief      it extracts the ordinal number of entity from the ID.
    @param      id              the identifier of the variable.
    @return     the index of the entity or EOK_uint8dummy in case of invalid parameters.
 **/
extern eOprotIndex_t eoprot_ID2index(eOprotID32_t id); 


/** @fn         extern eOprotTag_t eoprot_ep_variable_ID2tag(eOprotID32_t id)
    @brief      it extract the tag of the variable from the ID.
    @param      id              the identifier of the variable.
    @return     the tag of the entity or EOK_uint8dummy in case of invalid parameters.
 **/
extern eOprotTag_t eoprot_ID2tag(eOprotID32_t id); 


/** @fn         extern const char* eoprot_EP2string(eOprotEndpoint_t ep)
    @brief      it returns a string which describes the endpoint in argument. 
                if the argument maps to one of the values defined in eOprot_endpoint_t, then the associated 
                string is returned. Otherwise, it is returned the string "eoprot_endpoint_unrecognised".                
    @param      ep              the endpoint value
    @return     the zero-terminated string associated to the endpoint or "eoprot_endpoint_unrecognised".
 **/
extern const char* eoprot_EP2string(eOprotEndpoint_t ep);


/** @fn         extern const char* eoprot_EN2string(eOprotEndpoint_t ep, eOprotEntity_t en)
    @brief      it returns a string which describes the entity inside the endpoint as in argument. 
                if the arguments pair maps to one of the valid values defined in eOprot_entity_t, then the associated 
                string is returned. Otherwise, it is returned the string "eoprot_entity_unrecognised".          
    @param      ep              the endpoint value
    @param      en              the entity value
    @return     the zero-terminated string associated to the entity or "eoprot_entity_unrecognised".
 **/
extern const char* eoprot_EN2string(eOprotEndpoint_t ep, eOprotEntity_t en);


/** @fn         extern const char* eoprot_TAG2string(eOprotEndpoint_t ep, eOprotEntity_t en, eOprotTag_t tag)
    @brief      it returns a string which describes the tag inside the endpoint and entity as in argument. 
                if the arguments triple maps to a valid tag as defined in the various eOprot_tag_xxx_yyy_t enum types, 
                then the associated string is returned. Otherwise, it is returned the string "eoprot_tag_unrecognised".          
    @param      ep              the endpoint value
    @param      en              the entity value
    @param      tag             the tag value
    @return     the zero-terminated string associated to the entity or "eoprot_tag_unrecognised".
 **/
extern const char* eoprot_TAG2string(eOprotEndpoint_t ep, eOprotEntity_t en, eOprotTag_t tag);


/** @fn         extern const char* eoprot_ID2stringOfEndpoint(eOprotID32_t id)
    @brief      it returns a string which describes the endpoint contained inside the ID.
    @param      id              the identifier of the variable.
    @return     the zero-terminated string associated to the endpoint or "eoprot_endpoint_unrecognised".
 **/
extern const char* eoprot_ID2stringOfEndpoint(eOprotID32_t id); 


/** @fn         extern const char* eoprot_ID2stringOfEntity(eOprotID32_t id)
    @brief      it returns a string which describes the entity contained inside the ID.
    @param      id              the identifier of the variable.
    @return     the zero-terminated string associated to the entity or "eoprot_entity_unrecognised".
 **/
extern const char* eoprot_ID2stringOfEntity(eOprotID32_t id);


/** @fn         extern const char* eoprot_ID2stringOfTag(eOprotID32_t id)
    @brief      it returns a string which describes the tag contained inside the ID.
                for example, use the following code to create printable information on a variable
                described by its id:
                snprintf(str, sizeof(str), "ID32 = 0x%08x -> INDEX = %d, TAG = %s", id, eoprot_ID2index(id), eoprot_ID2stringOfTag(id));
    @param      id              the identifier of the variable.
    @return     the zero-terminated string associated to the tag or "eoprot_tag_unrecognised".
 **/
extern const char* eoprot_ID2stringOfTag(eOprotID32_t id);


/** @fn         extern eOresult_t eoprot_ID2information(eOprotID32_t id, char* string, uint16_t size)
    @brief      it fills the passed string with information about the id. The string size must be able to contain the
                required characters, otherwise the information will be truncated. a good size is about 40 + size required
                for the tag description. a total of 96 or 128 is good enough.
                An example of formatted resulting string is the following, where the ID is related to the status of the
                motor number 2:  "ID32 = 0x01010203 -> IND = 2, TAG = eoprot_tag_mc_motor_status"
    @param      id              the identifier of the variable.
    @param      string          the non NULL string
    @param      size            the size of the string. at least 96-128.
    @return     eores_OK upon success, eores_NOK_generic if the string is NULL or size is 0. 
 **/
extern eOresult_t eoprot_ID2information(eOprotID32_t id, char* string, uint16_t size);



extern eObool_t eoprot_EPcfg_isvalid(eOprot_EPcfg_t *cfgofep);

// functions which manage protocol version


/** @fn         extern const eoprot_version_t * eoprot_version_of_endpoint_get(eOprotEndpoint_t ep)
    @brief      it extracts the version of the endpoint.
    @param      ep          the endpoint.
    @return     pointer to the internal version of the entity, or NULL is specified endpoit does not exists
 **/
extern const eoprot_version_t * eoprot_version_of_endpoint_get(eOprotEndpoint_t ep);


// functions which allow to use the variables etc.



/** @fn         extern eOresult_t eoprot_config_board_reserve(eOprotBRD_t brd)
    @brief      it configures the library so that this particular board can be managed.
                if the board is eoprot_board_localboard then the space is already allocated.
                if instead is a given number then if we use dynamic mode then the memory is allocated / reallocated
                to host an array index of value equal to brd. thus if we reserve for brd = 2, then 4, then 0, then 1
                the memory is allocated to host 3 boards, then 4, then nothing is done because 0 and 1 are already supported.
                in case of static allocation we never allocate.
                in both cases we retrun error if brd >= eoprot_boards_maxnumberof
    @param      brd                 the number of board 
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_board_reserve(eOprotBRD_t brd);


/** @fn         extern eOresult_t eoprot_config_board_numberof(uint8_t numofboards)
    @brief      it configures the library to use a given number of boards. it is the same as calling
                eoprot_config_board_reserve(numofboards-1);
    @param      numofboards         the number of boards.
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_board_numberof(uint8_t numofboards);


/** @fn         extern eObool_t eoprot_board_can_be_managed(eOprotBRD_t brd)
    @brief      it tells if a given board can be managed by the protocol.
    @param      brd                 the number of board 
    @return     eobool_true or eobool_false.
 **/
extern eObool_t eoprot_board_can_be_managed(eOprotBRD_t brd);


/** @fn         extern eOresult_t eoprot_config_board_local(eOprotBRD_t brd)
    @brief      it configure the library to consider a given board as the local one.
    @param      brd             the number of board to be set as the local one.
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_board_local(eOprotBRD_t brd);


/** @fn         extern eOresult_t eoprot_config_endpoint_entities(eOprotBRD_t brd, eOprotEndpoint_t ep, const uint8_t* numberofentities)
    @brief      it configures the library to use a given number of entities for that endpoint on that board.
    @param      brd                 the number of board 
    @param      ep                  the endpoint
    @param      numberofentities    the number of entities expressed as a const array whose address will be copied and used in the
                                    entire lifetime of the library. If NULL, then it de-configures. 
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_endpoint_entities(eOprotBRD_t brd, eOprotEndpoint_t ep, const uint8_t* numberofentities);


extern eOresult_t eoprot_config_proxied_variables(eOprotBRD_t brd, eOprotEndpoint_t ep, eObool_fp_uint32_t isvarproxied_fn);


/** @fn         extern eObool_t eoprot_endpoint_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep)
    @brief      it tells if a given board has a given endpoint configured.
    @param      brd                 the number of board 
    @param      ep                  the endpoint
    @return     eobool_true or eobool_false.
 **/
extern eObool_t eoprot_endpoint_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep);


/** @fn         extern eOresult_t eoprot_config_onsay_endpoint_set(eOprotEndpoint_t ep, eOvoid_fp_cnvp_cropdesp_t onsay)
    @brief      it sets the onsay() callback function which is common to every board but specific for a given endpoint.
    @param      ep                  the endpoint
    @param      onsay               the function
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_onsay_endpoint_set(eOprotEndpoint_t ep, eOvoid_fp_cnvp_cropdesp_t onsay);


/** @fn         extern eOvoid_fp_cnvp_cropdesp_t eoprot_onsay_endpoint_get(eOprotEndpoint_t ep)
    @brief      it gets the onsay() callback function which is common to every board but specific for a given endpoint.
    @param      ep                  the endpoint
    @return     the function pointer if configured or NULL if not configured. NULL also if the ep is wrong.
 **/
extern eOvoid_fp_cnvp_cropdesp_t eoprot_onsay_endpoint_get(eOprotEndpoint_t ep);


/** @fn         extern eOresult_t eoprot_config_endpoint_callback(const eOprot_callbacks_endpoint_descriptor_t* cbkdes)
    @brief      it configures the ram initialisation function functions associated to a given endpoint.
                it does so for every board.
                it the raminitialise function is NULL, then it is left the default one.
    @param      cbkdes              pointer to the endpoint callback descriptor
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_callbacks_endpoint_set(const eOprot_callbacks_endpoint_descriptor_t* cbkdes);



/** @fn         extern eOresult_t eoprot_config_callbacks_variable_set(const eOprot_callbacks_variable_descriptor_t *cbkdes)
    @brief      it overrides the callback functions associated to a given variable with functions init() and update().
                it does so for every board. if any of these function is NULL, then it is left the default one.
    @param      cbkdes              pointer to the variable callback descriptor
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_config_callbacks_variable_set(const eOprot_callbacks_variable_descriptor_t *cbkdes);


// functions which tell about board properties


/** @fn         extern eOprotBRD_t eoprot_board_local_get(void)
    @brief      tells the id of the local board
    @return     the local board id in range [0, ...] or eo_prot_BRDdummy if the localboard is not configured yet
    @warning    DO NOT EVER use the value returned as a parameter for other functions.
                if you want to refer to the local board use eoprot_board_localboard.
 **/
extern eOprotBRD_t eoprot_board_local_get(void);


// functions which tell about properties of the endpoints and of the entities.


/** @fn         extern uint8_t eoprot_endpoints_numberof_get(eOprotBRD_t brd)
    @brief      tells how many endpoints in the board
    @param      brd         the board
    @return     the number of endpoint in the specified board. zero if board is not initialised or supported.
 **/
extern uint8_t eoprot_endpoints_numberof_get(eOprotBRD_t brd);


/** @fn         extern eOresult_t eoprot_endpoints_array_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
    @brief      fills the array with all the endpoints in the board, starting from a given number.
    @param      brd         the board
    @param      array       an array initialised with itemsize = sizeof(eOprotEndpoint_t). the array will be filled
                            until its capacity or until the endpoints are over.
    @param      startfrom   it tells to put inside the array the endpoints after position startfrom.
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_endpoints_array_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom);


/** @fn         extern eOresult_t eoprot_endpoints_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
    @brief      fills the array with all the endpoint descriptors in the board, starting from a given number.
    @param      brd         the board
    @param      array       an array initialised with itemsize = sizeof(eoprot_endpoint_descriptor_t). the array will be filled
                            until its capacity or until the endpoints are over.
    @param      startfrom   it tells to put inside the array the endpoints after position startfrom. 
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_endpoints_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom);


/** @fn         extern uint16_t eoprot_entities_numberof_get(eOprotBRD_t brd)
    @brief      tells how many entities are in the board, considering every endpoint.
    @param      brd         the board
    @return     the number of entities in the specified board. zero if board is not initialised or supported.
 **/
extern uint16_t eoprot_entities_numberof_get(eOprotBRD_t brd);


/** @fn         extern eOresult_t eoprot_entities_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom)
    @brief      fills the array with all the entity descriptors in the board, starting from a given number.
    @param      brd         the board
    @param      array       an array initialised with itemsize = sizeof(eoprot_entity_descriptor_t). the array will be filled
                            until its capacity or until the entities are over.
    @param      startfrom   it tells to put inside the array the entities after position startfrom. 
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_entities_arrayofdescriptors_get(eOprotBRD_t brd, EOarray* array, uint8_t startfrom);


/** @fn         extern eoprot_entities_in_endpoint_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
    @brief      tells how many entities are in the board, considering only the specified endpoint.
    @param      brd         the board
    @param      ep          the endpoint
    @return     the number of entities in the specified board for that endpoint. zero if board or endpoint is not initialised 
                or supported.
 **/
extern uint16_t eoprot_entities_in_endpoint_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep);


/** @fn         extern eOresult_t eoprot_entities_in_endpoint_arrayofdescriptors_get(eOprotBRD_t brd, eOprotEndpoint_t ep, EOarray* array, uint8_t startfrom)
    @brief      fills the array with all the entity descriptors in the board, starting from a given number, considering only the
                specifeid endpoint.
    @param      brd         the board
    @param      ep          the endpoint
    @param      array       an array initialised with itemsize = sizeof(eoprot_entity_descriptor_t). the array will be filled
                            until its capacity or until the entities are over.
    @param      startfrom   it tells to put inside the array the entities after position startfrom. 
    @return     eores_OK or eores_NOK_generic upon failure.
 **/
extern eOresult_t eoprot_entities_in_endpoint_arrayofdescriptors_get(eOprotBRD_t brd, eOprotEndpoint_t ep, EOarray* array, uint8_t startfrom);


extern eOvoid_fp_uint32_voidp_t eoprot_endpoint_get_initialiser(eOprotEndpoint_t ep);

extern uint8_t eoprot_endpoint_get_numberofentities(eOprotEndpoint_t ep);

/** @fn         extern uint16_t eoprot_endpoint_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
    @brief      it tells the size of the ram used for a given board and endpoint.
    @param      brd                 the number of board 
    @param      ep                  the endpoint
    @return     the size or 0 upon failure.
 **/
extern uint16_t eoprot_endpoint_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep);


/** @fn         extern eOresult_t eoprot_config_endpoint_ram(eOprotBRD_t brd, eOprotEndpoint_t ep, void* ram, uint16_t sizeofram)
    @brief      it configures the library to use some ram for that board and endpoint. The ram must be externally allocated and be
                of the correct dimension. One can use a sizeof(eOprot_bxx_endpointname_t) or use the function 
                eoprot_endpoint_sizeof_get(). If ram is NULL, then it de-configures.
    @param      brd                 the number of board 
    @param      ep                  the endpoint
    @param      ram                 the ram
    @param      sizeofram           the size of the ram.    
    @return     eores_OK or eores_NOK_generic upon failure (also if sizeofram is not coherent with brd and ep).
 **/
extern eOresult_t eoprot_config_endpoint_ram(eOprotBRD_t brd, eOprotEndpoint_t ep, void* ram, uint16_t sizeofram);


/** @fn         extern void* eoprot_variable_ramof_get(eOprotBRD_t brd, eOprotID32_t id)
    @brief      it gets the ram of the variable on a given (board, ID). The dependency from the board is necessary because
                for the same endpoint the number of entities may be different.
    @param      brd             the number of the board.
    @param      id              the identifier of the variable.
    @return     the ram of the variable or NULL in case of invalid parameters.
 **/
extern void* eoprot_variable_ramof_get(eOprotBRD_t brd, eOprotID32_t id);


/** @fn         extern uint16_t eoprot_variable_sizeof_get(eOprotBRD_t brd, eOprotID32_t id)
    @brief      it gets the size of the variable on a given (board, ID). The dependency from the board is necessary because
                for the same endpoint the number of entities may be different.
    @param      brd             the number of the board.
    @param      id              the identifier of the variable.
    @return     the size of the variable or 0 in case of invalid parameters.
 **/
extern uint16_t eoprot_variable_sizeof_get(eOprotBRD_t brd, eOprotID32_t id);


/** @fn         extern eObool_t eoprot_variable_is_proxied(eOprotBRD_t brd, eOprotID32_t id)
    @brief      tells if the variable is proxied. the result is meaningful only after eoprot_config_proxied_variables()
                has been called, otherwise this function returns false.
    @param      brd             the number of the board.
    @param      id              the identifier of the variable.
    @return     a boolean value.
 **/
extern eObool_t eoprot_variable_is_proxied(eOprotBRD_t brd, eOprotID32_t id);


/** @fn         extern void* eoprot_variable_nvrom_get(eOprotBRD_t brd, eOprotID32_t id)
    @brief      retrieves the rom of the EOnv on the given board and given id32.
    @param      brd             the number of the board.
    @param      id              the identifier of the variable.
    @return     the pointer to the requested EOnv_rom_t or NULL if the (brd, id) pair does not exists.
 **/
extern void* eoprot_variable_romof_get(eOprotBRD_t brd, eOprotID32_t id);

/** @fn         extern eObool_t eoprot_entity_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
    @brief      tells if the entity is configured.
    @param      brd             the number of the board.
    @param      ep              the endpoint
    @param      entity          the entity.
    @return     a boolean value.
 **/
extern eObool_t eoprot_entity_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity);


/** @fn         extern eObool_t eoprot_entity_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
    @brief      it tells if the entity on a given (board, endpoint, entity) is configured.
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @param      entity          the entity.
    @return     eobool_true if configured, eobool_false if not configured or if parameters are invalid.
 **/
extern eObool_t eoprot_entity_configured_is(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity);


/** @fn         extern void* eoprot_entity_ramof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index)
    @brief      it gets the ram of the entity on a given (board, endpoint, entity). The dependency from the board is necessary because
                for the same endpoint the number of entities may be different.
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @param      entity          the entity.
    @param      index           the index.
    @return     the ram of the entity or NULL in case of invalid parameters.
 **/
extern void* eoprot_entity_ramof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity, eOprotIndex_t index);


/** @fn         extern uint16_t eoprot_entity_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
    @brief      it gets the size of the entity on a given (board, endpoint, entity). The dependency from the board is necessary because
                for the same endpoint the number of entities may be different.
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @param      entity          the entity.
    @return     the size of the entity or 0 in case of invalid parameters.
 **/
extern uint16_t eoprot_entity_sizeof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity);


/** @fn         extern uint8_t eoprot_entity_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity)
    @brief      it gets the number of the entities on a given (board, endpoint, entity). The dependency from the board is necessary because
                for the same endpoint the number of entities may be different..
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @param      entity          the entity.
    @return     the number of the entities or 0 in case of invalid parameters.
 **/
extern uint8_t eoprot_entity_numberof_get(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotEntity_t entity);


/** @fn         extern eObool_t eoprot_id_isvalid(eOprotBRD_t brd, eOprotID32_t id)
    @brief      it tells if a given ID is valid on that board.
    @param      brd             the number of the board.
    @param      id              the ID.
    @return     eobool_true or eobool_false.
 **/
extern eObool_t eoprot_id_isvalid(eOprotBRD_t brd, eOprotID32_t id);


/** @fn         extern uint16_t eoprot_endpoint_numberofvariables_get(eOprotBRD_t brd, eOprotEndpoint_t ep)
    @brief      it retrieves number of variables contained inside a given endpoint of a given board.
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @return     the number or 0 in case of invalid parameters or endpoint not present on the board.
 **/
extern uint16_t eoprot_endpoint_numberofvariables_get(eOprotBRD_t brd, eOprotEndpoint_t ep);


/** @fn         extern eOprotID32_t eoprot_endpoint_prognum2id(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotProgNumber_t prog)
    @brief      it converts the progressive number into the ID for a given endpoint on a given board 
    @param      brd             the number of the board.
    @param      ep              the endpoint.
    @param      prog            the progressive number.
    @return     the ID or EOK_uint32dummy in case of invalid parameters.
 **/
extern eOprotID32_t eoprot_endpoint_prognum2id(eOprotBRD_t brd, eOprotEndpoint_t ep, eOprotProgNumber_t prog);

/** @fn         extern eOprotProgNumber_t eoprot_endpoint_id2prognum(eOprotBRD_t brd, eOprotID32_t id)
    @brief      it converts the ID into the progressive number for a given endpoint on a given board
    @param      brd             the number of the board.
    @param      id              the ID.
    @return     the progressive number or EOK_uint32dummy in case of invalid parameters.
 **/
extern eOprotProgNumber_t eoprot_endpoint_id2prognum(eOprotBRD_t brd, eOprotID32_t id);


extern void* eoprot_endpoint_ramof_get(eOprotBRD_t brd, eOprotEndpoint_t ep);


/** @}            
    end of group eo_EoProtocol  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





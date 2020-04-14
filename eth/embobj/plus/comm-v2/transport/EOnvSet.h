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
#ifndef _EONVSET_H_
#define _EONVSET_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EOnvSet.h
    @brief      This header file implements public interface to a collector of EOnv objects.
    @author     marco.accame@iit.it
    @date       06/07/2013
**/

/** @defgroup eo_nvset EOnvSet
    cefcewqfcew
      
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EOnv.h"
#include "EOconstvector.h"
#include "EOVmutex.h"
#include "EoProtocol.h"

// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types -------------------------------------------------------------------------    


/** @typedef    typedef const struct EOnvSet_hid EOnvSet
    @brief      EOaction is an opaque struct. It is used to implement data abstraction for the datagram 
                object so that the user cannot see its private fields and he/she is forced to manipulate the
                object only with the proper public functions. 
 **/  
typedef struct EOnvSet_hid EOnvSet;


typedef enum
{
    eo_nvset_ownership_local       = 0,
    eo_nvset_ownership_remote      = 1
} eOnvsetOwnership_t; 


enum { eonvset_max_endpoint_value = 7 };     // or 15 (they start from 0 and go to ...)


///** @typedef    typedef struct eOnvset_EPcfg_t
//    @brief      It contains the configuration of the managed NVs inside an endpoint. 
// **/ 
//typedef struct                     
//{
//    eOnvEP8_t           endpoint;                                       /*< the endpoint value */
//    uint8_t             numberofsentities[eonvset_max_entity_value+1];  /*< the multiplicity of each entity in position of the entity */
//} eOnvset_EPcfg_t;


/** @typedef    typedef struct eOnvset_BRDcfg_t
    @brief      It contains the configuration of the managed NVs in a board, organised by endpoints 
 **/  
typedef struct
{
    eOnvBRD_t           boardnum;
    uint8_t             dummy[3];
    EOconstvector*      epcfg_constvect; /*< a const vector of eOprot_EPcfg_t items, of size equal to the number of managed endpoints */
} eOnvset_BRDcfg_t;
                           

/** @typedef    typedef enum eOnvset_protection_t
    @brief      It contains the protection to be used in the NV set. 
 **/ 
typedef enum
{
    eo_nvset_protection_none               = 0,    /**< we dont protect vs concurrent access at all */
    eo_nvset_protection_one_per_board      = 2,    /**< all the NVs in a booard share the same mutex */
    eo_nvset_protection_one_per_endpoint   = 3,    /**< all the NVs in an endpoint inside each board share the same mutex */
    eo_nvset_protection_one_per_netvar     = 4     /**< every NV has its own mutex: heavy use of memory but maximum concurrency */
} eOnvset_protection_t;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern EMBOBJ_API const eOnvset_BRDcfg_t eonvset_BRDcfgBasic;  // it contains numofboard 99, and only management (eoprot_mn_basicEPcfg)

extern EMBOBJ_API const eOnvset_BRDcfg_t eonvset_BRDcfgStd;    // it contains numboard 99, and all endpoints with max capabilities apart for jomos which are 4.

extern EMBOBJ_API const eOnvset_BRDcfg_t eonvset_BRDcfgMax;   // it contains numboard 99, and all endpoints with max capabilities (12 jomos).

// - declaration of extern public functions ---------------------------------------------------------------------------

extern EOnvSet* eo_nvset_New(eOnvset_protection_t prot, eov_mutex_fn_mutexderived_new mtxnew);


extern void eo_nvset_Delete(EOnvSet* p);


extern eOresult_t eo_nvset_InitBRD(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvBRD_t brdnum);


extern eOresult_t eo_nvset_LoadEP(EOnvSet* p, eOprot_EPcfg_t *cfgofep, eObool_t initNVs);

// it calls eo_nvset_InitBRD() and then for all the endpoint descriptors inside cfgofdev: { eo_nvset_EPcfg_IsValid() and eo_nvset_LoadEP() }
extern eOresult_t eo_nvset_InitBRD_LoadEPs(EOnvSet* p, eOnvsetOwnership_t ownership, eOipv4addr_t ipaddress, eOnvset_BRDcfg_t* cfgofdev, eObool_t initNVs);


extern eOresult_t eo_nvset_BRDlocalsetnumber(EOnvSet* p, eOnvBRD_t brdnum);

extern eOresult_t eo_nvset_DeinitBRD(EOnvSet* p);

// local, 0, 1, 2, 3
extern eOresult_t eo_nvset_BRD_Get(EOnvSet* p, eOnvBRD_t* brd);

extern eOresult_t eo_nvset_NV_Get(EOnvSet* p, eOnvID32_t id32, EOnv* thenv);

extern void* eo_nvset_RAMofEndpoint_Get(EOnvSet* p, eOnvEP8_t ep8);

extern void* eo_nvset_RAMofEntity_Get(EOnvSet* p, eOnvEP8_t ep8, eOnvENT_t ent, uint8_t index);

extern void* eo_nvset_RAMofVariable_Get(EOnvSet* p, eOnvID32_t id32);


/** @}            
    end of group eo_nvset 
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




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
#ifndef _EOCOMMON_H_
#define _EOCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif



/** @file       EoCommon.h
    @brief      This header file implements public interface to the common types used by the EmbObj
    @author     marco.accame@iit.it
    @date       10/26/2010
**/

/** @defgroup eo_common Common types in embOBJ
    The embOBJ uses some common types and constants for its objects. Those types all follow the eOtypename_t
    naming conventions, whereas the constants use the eo_constantname naming convention.

    Moreover, the objects use one of the following naming conventions:  EOobjName, EOVobjName, EOMobjName, 
    and EOSobjName.  Singleton also use the keyword "the" to express uniqueness (e.g., EOtheObjName).
    The EOobjName name is used when the object objName can be used in any ececution environment, be it
    singletask or multitask.
    The EOVobjName name is used when the object objName has one or more virtual members, and can thus be used 
    only if another object is derived from it.
    The EOMobjName name is used when the object is suited for a multitask execution environment only. Some objects
    of this kind may have some internal tasks used to offer some services to the caller.
    The EOSobjName name is used when the object is suited for the singletask execution environment only.
    
    In general an object objName can be instantiated as many times as there is RAM memory available with the
    _New() method, which gives back an opaque pointer to the object. Other member functions operate on a particular
    object by using the opaque pointer passed as first argument.
    
    However, the object can also be a singleton and as such it can be instantiated only once. The singleton must be 
    explicitely initialised by calling a _Initialise() method which returns an opaque pointer to the correct object.  
    The _Initialise() function is always protected vs multiple calls.
    The same opaque pointer can be retrieved at any time with the _GetHandle() method. 
    Other member functions operate on the singleton by using the opaque pointer passed as first argument.
    
    
    List of objects is the following.
    The embOBJ runs by starting a 
    basic:      EOVmutex, EOtheErrorManager, EOtheMemoryPool, EOV
    Containers: EOdeque, EOlist, EOvector, EOarray
    
    
    
    @todo doxy documentation
    
   @{        
 */
 
  

// - external dependencies --------------------------------------------------------------------------------------------

#include "embOBJporting.h" 

#include "stdlib.h"
#include "string.h"
#include "stdio.h"


// issues a compiler error if the sizeof the struct is not what in second argument
#define EO_VERIFYsizeof(sname, ssize)       __emBODYportingVERIFYsizeof(sname, ssize)

// - declaration of public user-defined types -------------------------------------------------------------------------


/** @typedef    typedef enum eOresult_t
    @brief      eOresult_t contains the results that a function or an operation can have. It is safe to verify the 
                correct result by checking its value being >= 0 (or eores_OK).
    @warning    If you change it, please be sure that the osal, ipal, hal, fsal all have a similar type w/ equal values,
                as sometimes the functions just use a cast of the return values of the abstraction layer. 
 **/
typedef enum  
{
    eores_OK                =  0,       /**< correct result: generic */
    eores_NOK_generic       = -1,       /**< error: generic */
    eores_NOK_nullpointer   = -2,       /**< error: a null pointer was used */
    eores_NOK_timeout       = -3,       /**< error: the caller (task) did not get the osal resource after it waited (even for zero time) */
    eores_NOK_nowait        = -4,       /**< error: the calling isr did not get the osal resource when called the not waiting version of the function */ 
    eores_NOK_nodata        = -5,       /**< error: there was not data to be retrieved */
    eores_NOK_busy          = -6,       /**< error: the service was busy and request has not been served */
    eores_NOK_unsupported   = -15       /**< error: unsupported feature */
} eOresult_t;


/** @typedef    typedef enum eOboolvalues_t
    @brief      eOboolvalue_t contains the two boolean values.
    @warning    C99 contains bool, true, and false. To use C99 include "stdbool.h" and --c99 compiler option.
                At this point please redefine eobool_false and eobool_true to be equal to false and true.
                Also, eObool_t must be typedeffed as bool.
 **/
typedef enum 
{
    eobool_false = 0,                       /**< false */
    eobool_true  = 1                        /**< true */
} eOboolvalues_t;


/** @typedef    typedef uint8_t eObool_t
    @brief      eObool_t contains the boolean type of size 1 byte.
 **/
typedef uint8_t eObool_t;


/** @typedef    typedef emBODYporting_enum08_t eOenum08_t
    @brief      eOenum08_t contains a generic enum which must be contained in exactly 8 bit. It is a int8_t
 **/
typedef emBODYporting_enum08_t eOenum08_t;

/** @typedef    typedef uint8_t eOflags08_t
    @brief      eOflags08_t contains up to 8 flags 
 **/
typedef uint8_t eOflags08_t;


/** @typedef    typedef uint16_t eOflags16_t
    @brief      eOflags16_t contains up to 16 flags 
 **/
typedef uint16_t eOflags16_t;


/** @typedef    typedef uint32_t eOflags32_t
    @brief      eOflags32_t contains up to 32 flags 
 **/
typedef uint32_t eOflags32_t;


/** @typedef    typedef struct eOversion_t
    @brief      eOversion_t contains the version of a embOBJ object or library
 **/
typedef struct  
{
    uint8_t     major;                  /**< the major number */
    uint8_t     minor;                  /**< the minor number */
} eOversion_t;


/** @typedef    typedef uint32_t eOreltime_t
    @brief      eOreltime_t contains the relative time expressed in micro-seconds. It is used for relative timing
                operations because its maximum value is about 4294 seconds. 
    @warning    It is strongly advised to use the associated eok_reltime* constants (e.g., eok_reltime1sec etc.).
 **/
typedef uint32_t    eOreltime_t; 


/** @typedef    typedef uint64_t eOabstime_t
    @brief      eOabstime_t contains the time expressed in micro-seconds in a much longer range. It is 
                used for absolute timing, usually measured from bootstrap of the device (or from start
                of the singletask or multitask exution environment). A eOabstime_t variable keeps time
                up to 584 thousand years ...
 **/
typedef uint64_t    eOabstime_t; 


/** @typedef    typedef uint32_t eOabstime_ms_t
    @brief      eOabstime_ms_t contains the time expressed in milli-seconds in a much longer range. It can be used
                instead of eOabstime_t when we want to save memory (4B vs 8B) but ... at a lower resolution (ms vs us) 
                at a lower duration (4294967 sec ~= 49 days)
 **/
typedef uint32_t    eOabstime_ms_t; 


/** @typedef    typedef struct eOnanotime_t
    @brief      eOnanotime_t contains the time expressed at its finest resolution: nanoseconds.
                It is used for both relative and absolute time when a finer resultion is needed.
                A eOnanotime_t variable keeps time at the nano-second resolution up to 136 years ...
 **/
typedef uint64_t    eOnanotime_t; 


/** @typedef    typedef uint16_t    eOsizeitem_t
    @brief      eOsizeitem_t contains the size of an object (or item) as used by the container objects
                EOdeque, EOlist, EOvector and its derived objects.
 **/
typedef uint16_t    eOsizeitem_t;


/** @typedef    typedef uint16_t    eOsizecntnr_t
    @brief      eOsizecntnr_t contains the size of internal buffer as used by the container objects
                EOdeque, EOlist, EOvector and its derived objects.
 **/
typedef uint16_t    eOsizecntnr_t;    


/** @typedef    typedef uint32_t    eOevent_t
    @brief      eOevent_t contains an event expressed as a single bit flag. It is used by some objects to
                exchange signalling in a not enqueued manner, for instance by EOaction or by EOVtask and its derivates.
 **/
typedef uint32_t    eOevent_t;


/** @typedef    typedef uint32_t    eOmessage_t
    @brief      eOmessage_t contains a message. It is used by some objects to exchange signalling in an enqueued manner, 
                for instance by EOaction or by EOVtask and its derivates. 
 **/
typedef uint32_t    eOmessage_t;


/** @typedef    typedef void (*eOcallback_t) (void* p)
    @brief      eOcallback_t contains a pointer to a function which can be executed as a result of an action object.
 **/
typedef void (*eOcallback_t) (void* p);


/** @typedef    typedef uint8_t     eOid08_t
    @brief      eOid08_t is a type which contains an ID which can assume up to 256 values. It can be used to retrieve the handle 
                of a ... multiton, which is an object that is instantiated only once and which is uniquely defined by an ID.
 **/
typedef uint8_t     eOid08_t;


/** @typedef    typedef void        eOderived_t
    @brief      eOderived_t is the generic type used for polymorphism.  It is used in the virtual methods of the objects.
 **/
typedef void        eOderived_t;


/** @typedef    typedef void        eOsafelyderived_t
    @brief      It is a derived type with a safety guard. 
 **/ 
typedef void        eOsafelyderived_t;


/** @typedef    typedef uint32_t eOipv4addr_t
    @brief      The address is stored in four consecutive bytes as it is in ipal_ipv4addr_t and as
                specified by macro EO_COMMON_IPV4ADDR().
                As an example, the address ip1.ip2.ip3.ip4 = 10.255.37.204 is stored in the 32-bit word with
                ip1 in least significant byte, ip2 in next etc, thus 0xCC25FF0A. 
                In little endian architectures, the memory layout is the same as in  uint8_t addr[] = {10, 255, 37, 204}. 
 **/  
typedef uint32_t eOipv4addr_t;


/** @typedef    typedef uint32_t eOipv4addr_t
    @brief      The address is stored in sizx consecutive bytes as specified by macro EO_COMMON_IPV4ADDR().
                As an example, the address 03-0E-01-00-01-F0 is stored in the 6 least significant bytes
                03 in least significant byte, 0E in next etc, thus 0x0000F00100010E03. 
                In little endian architectures, the memory layout is the same as in  uint8_t addr[] = {0x03, etc}. 
 **/
typedef uint64_t eOmacaddr_t;


/** @typedef    typedef uint16_t eOipv4port_t
    @brief      The port used for IP v4. 
 **/ 
typedef uint16_t eOipv4port_t;



typedef struct 
{
    eOipv4addr_t    addr;
    eOipv4port_t    port;
} eOipv4addressing_t; 


/** @typedef    typedef enum eOcanframe_IDtype_t 
    @brief      It is used to identify the type of CAN frames (must be the same as hal_can_frameID_format_t).
 **/ 
typedef enum
{ 
    eocanframeID_std11bits  = 0,   /**< CAN frame ID of standard 11 bits format. In iCub we use only this one */
    eocanframeID_ext29bits  = 1    /**< CAN frame ID of extended format */
} eOcanframe_IDtype_t; 


/** @typedef    typedef enum hal_can_frame_type_t 
    @brief      hal_can_frame_type_t is used to identify the type of CAN frames.
 **/ 
typedef enum
{ 
    eocanframetype_data     = 0,    /**< CAN frame of type DATA. In iCub we use only this one */
    eocanframetype_remote   = 1     /**< CAN frame of type REMOTE */
} eOcanframe_type_t; 


/** @typedef    typedef struct eOutil_canframe_t
    @brief      eOcanframe_t contains a can frame as defined in hal_can.h. 
 **/
typedef struct              // size is 16 bytes
{
    uint32_t                id;             /**< can frame id    */
    uint8_t                 id_type;        /**< can frame id format. use eOcanframe_IDtype_t*/
    uint8_t                 frame_type;     /**< frame type. use eOcanframe_type_t */
    uint8_t                 size;           /**< data size */
    uint8_t                 unused;         /**< filler */
    uint8_t                 data[8];        /**< the data (payload) */    
} eOcanframe_t;


typedef struct 
{
    uint8_t                 f00 : 1;
    uint8_t                 f01 : 1;
    uint8_t                 f02 : 1;
    uint8_t                 f03 : 1;
    uint8_t                 f04 : 1;
    uint8_t                 f05 : 1;
    uint8_t                 f06 : 1;
    uint8_t                 f07 : 1;    
} eO08flags_t; 

typedef struct 
{
    uint16_t                f00 : 1;
    uint16_t                f01 : 1;
    uint16_t                f02 : 1;
    uint16_t                f03 : 1;
    uint16_t                f04 : 1;
    uint16_t                f05 : 1;
    uint16_t                f06 : 1;
    uint16_t                f07 : 1;  
    uint16_t                f08 : 1;
    uint16_t                f09 : 1;
    uint16_t                f10 : 1;
    uint16_t                f11 : 1;
    uint16_t                f12 : 1;
    uint16_t                f13 : 1;
    uint16_t                f14 : 1;
    uint16_t                f15 : 1;       
} eO16flags_t; 


typedef struct 
{
    uint32_t                f00 : 1;
    uint32_t                f01 : 1;
    uint32_t                f02 : 1;
    uint32_t                f03 : 1;
    uint32_t                f04 : 1;
    uint32_t                f05 : 1;
    uint32_t                f06 : 1;
    uint32_t                f07 : 1;  
    uint32_t                f08 : 1;
    uint32_t                f09 : 1;
    uint32_t                f10 : 1;
    uint32_t                f11 : 1;
    uint32_t                f12 : 1;
    uint32_t                f13 : 1;
    uint32_t                f14 : 1;
    uint32_t                f15 : 1;   
    uint32_t                f16 : 1;
    uint32_t                f17 : 1;
    uint32_t                f18 : 1;
    uint32_t                f19 : 1;
    uint32_t                f20 : 1;
    uint32_t                f21 : 1;
    uint32_t                f22 : 1;
    uint32_t                f23 : 1;
    uint32_t                f24 : 1;
    uint32_t                f25 : 1;
    uint32_t                f26 : 1;
    uint32_t                f27 : 1;  
    uint32_t                f28 : 1;
    uint32_t                f29 : 1;
    uint32_t                f30 : 1;
    uint32_t                f31 : 1;       
} eO32flags_t; 


/** @typedef    typedef enum  eOcanport_t
    @brief      eOcanport_t contains a can port as defined in hal_can.h. 
 **/
typedef enum              
{
     eOcanport1    = 0,          /**< CAN1        */
     eOcanport2    = 1           /**< CAN2        */
} eOcanport_t;    
  
enum {eOcanports_number = 2};


/** @define     eOvirtual
    @brief      eOvirtual is a qualifier used to identify the virtual methods of an object.
 **/
#define eOvirtual


/** @define     epureOvirtual
    @brief      eOpurevirtual is a qualifier used to identify the pure virtual methods of an abstract object
 **/
#define eOpurevirtual  


typedef     uint16_t    (*eOuint16_fp_uint8_uint8_t)                (uint8_t, uint8_t);
typedef     uint16_t    (*eOuint16_fp_uint16_t)                     (uint16_t);
typedef     uint16_t    (*eOuint16_fp_uint32_t)                     (uint32_t);
typedef     uint8_t     (*eOuint8_fp_voidp_t)                       (void *);
typedef     uint8_t     (*eOuint8_fp_uint16_t)                      (uint16_t);
typedef     uint8_t     (*eOuint8_fp_cvoidp_t)                      (const void *);
typedef     void        (*eOvoid_fp_void_t)                         (void);
typedef     void*       (*eOvoidp_fp_void_t)                        (void);
typedef     void        (*eOvoid_fp_voidp_uint32_t)                 (void *, uint32_t);
typedef     void        (*eOvoid_fp_vuint32p_uint32_t)              (volatile uint32_t *, uint32_t);
typedef     void        (*eOvoid_fp_voidp_t)                        (void *);
typedef     void        (*eOvoid_fp_uint32_voidp_t)                 (uint32_t, void *);
typedef     void        (*eOvoid_fp_voidp_voidp_t)                  (void *, void *);
typedef     void        (*eOvoid_fp_uint16_voidp_t)                 (uint16_t, void *);
typedef     void        (*eOvoid_fp_uint16_voidp_voidp_t)           (uint16_t, void *, void *);
typedef     void        (*eOvoid_fp_cvoidp_t)                       (const void *);
typedef     eObool_t    (*eObool_fp_cvoidp_t)                       (const void *);
typedef     eOresult_t  (*eOres_fp_void_t)                          (void);
typedef     eOresult_t  (*eOres_fp_uint8_t)                         (uint8_t);
typedef     eOresult_t  (*eOres_fp_voidp_uint32_uint32_t)           (void *, uint32_t, uint32_t);
typedef     void        (*eOvoid_fp_uint32_uint32_t)                (uint32_t, uint32_t);
typedef     eOresult_t  (*eOres_fp_voidp_evt_t)                     (void *, eOevent_t);
typedef     eOresult_t  (*eOres_fp_voidp_msg_tim_t)                 (void *, eOmessage_t, eOreltime_t);
typedef     eOresult_t  (*eOres_fp_voidp_cbk_voidp_t)               (void *, eOcallback_t, void *);
typedef     eOresult_t  (*eOres_fp_voidp_cbk_voidp_tim_t)           (void *, eOcallback_t, void *p, eOreltime_t);
typedef     eOresult_t  (*eOres_fp_voidp_msg_t)                     (void *, eOmessage_t);
typedef     eOresult_t  (*eOres_fp_voidp_uint32_t)                  (void *, uint32_t);
typedef     eOresult_t  (*eOres_fp_voidp_t)                         (void *);
typedef     eOresult_t  (*eOres_fp_voidp_uint32p_t)                 (void *, uint32_t *);
typedef     eOresult_t  (*eOres_fp_voidp_uint8p_t)                  (void *, uint8_t *);
typedef     eOresult_t  (*eOres_fp_voidp_cvoidpp_t)                 (void *, const void **);
typedef     eOresult_t  (*eOres_fp_voidp_cvoidpp_uint8p_t)          (void *, const void **, uint8_t *);
typedef     eOresult_t  (*eOres_fp_voidp_voidp_t)                   (void *, void *);
typedef     void        (*eOvoid_fp_uint64_t)                       (uint64_t);
typedef     void        (*eOvoid_fp_uint8_evt_t)                    (uint8_t, eOevent_t);
typedef     void        (*eOvoid_fp_msg_t)                          (eOmessage_t);
typedef     void        (*eOvoid_fp_uint16_t)                       (uint16_t);
typedef     void        (*eOvoid_fp_uint32_t)                       (uint32_t);
typedef     uint64_t    (*eOuint64_fp_void_t)                       (void);
typedef     uint8_t     (*eOuint8_fp_void_t)                        (void);
typedef     eOresult_t  (*eOres_fp_voidfpvoid_t)                    (void (*)(void));
typedef     eOresult_t  (*eOres_fp_voidp_uint32_uint32_cvoidp_t)    (void *, uint32_t, uint32_t, const void *);
typedef     eOresult_t  (*eOres_fp_voidp_uint32_uint32_voidp_t)     (void *, uint32_t, uint32_t, void *);
typedef     void        (*eOvoid_fp_voidfpvoiduint32_t)             (void (*)(void), uint32_t);
typedef     void        (*eOvoid_fp_voidfpvoiduint32uint8_t)       	(void (*)(void), uint32_t, uint8_t);
typedef     void        (*eOvoid_fp_uint32_uint64_uint64_t)         (uint32_t, uint64_t, uint64_t);
typedef     uint16_t    (*eOuint16_fp_uint16_uint16_t)              (uint16_t, uint16_t);
typedef     uint32_t    (*eOuint32_fp_uint8_uint16_t)               (uint8_t, uint16_t);
typedef     uint32_t    (*eOuint32_fp_uint8_uint8_uint16_t)         (uint8_t, uint8_t, uint16_t);
typedef     uint16_t    (*eOuint16_fp_uint8_uint32_t)               (uint8_t, uint32_t);
typedef     uint16_t    (*eOuint16_fp_uint8_t)                      (uint8_t);
typedef     uint32_t    (*eOuint32_fp_uint8_uint32_t)               (uint8_t, uint32_t);
typedef     eOresult_t  (*eOres_fp_voidp_uint8_t)                   (void*, uint8_t);
typedef     uint32_t    (*eOuint32_fp_uint8_uint8_uint32_t)         (uint8_t, uint8_t, uint32_t);
typedef     uint16_t    (*eOuint16_fp_voidp_uint8_t)                (void*, uint8_t);
typedef     void*       (*eOvoidp_fp_uint16_uint16_t)               (uint16_t, uint16_t);
typedef     void*       (*eOvoidp_fp_uint8_uint32_t)                (uint8_t, uint32_t);
typedef     eObool_t    (*eObool_fp_uint16_uint16_t)                (uint16_t, uint16_t);
typedef     void*       (*eOvoidp_fp_voidp_uint16_uint16_t)         (void*, uint16_t, uint16_t);
typedef     eObool_t    (*eObool_fp_uint8_uint32_t)                 (uint8_t, uint32_t);
typedef     void*       (*eOvoidp_fp_voidp_uint8_uint32_t)          (void*, uint8_t, uint32_t);
typedef     void*       (*eOvoidp_fp_uint32_t)                      (uint32_t);
typedef     void*       (*eOvoidp_fp_voidp_uint32_t)                (void*, uint32_t);
typedef     void        (*eOvoid_fp_uint8_uint8_voidp_t)            (uint8_t, uint8_t, void*);
typedef     void*       (*eOvoidp_fp_uint8_uint32_voidp_t)          (uint8_t, uint32_t, void*);
typedef     eOresult_t  (*eOres_fp_uint8_voidp_uint16_t)            (uint8_t, void*, uint16_t);
typedef     eOresult_t  (*eOres_fp_uint8_uint8_voidp_uint16_t)      (uint8_t, uint8_t, void*, uint16_t);
typedef     int8_t      (*eOint8_fp_uint8_cvoidp_t)                 (uint8_t, const void*);
typedef     int8_t      (*eOint8_fp_uint8_t)                        (uint8_t);
typedef     eObool_t    (*eObool_fp_uint32_t)                       (uint32_t);
typedef     double      (*eOdouble_fp_void_t)                       (void);
typedef     int8_t      (*eOint8_fp_voidp_uint32_t)                 (void*, uint32_t);
typedef     int8_t      (*eOint8_fp_voidp)                          (void*);
typedef     int         (*eOint_fp_constcharp)                      (const char*);



/** @typedef    typedef struct eOcallbackData_t
    @brief      eOcallbackData_t contains data of a callback: function pointer and its argument.
 **/  
typedef struct
{
    eOcallback_t     fn;
    void             *argoffn;
} eOcallbackData_t;



/** @typedef    typedef int32_t  eOq17_14_t
    @brief      Q17_14_t represents a number in fixed point format: 1 bit is used for the sign,
                17 bits are used for integer part and 14 bits for fractional part.
 **/  
typedef int32_t eOq17_14_t;


// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const eOversion_t    eok_version;            /**< = major, minor */
extern const uint32_t       eok_u32version;         /**< = LSB->MSB: major (1B), minor (1B) w/ same layout as eok_version */

extern const uint8_t        eok_uint04dummy;        /**< = EOK_uint04dummy = 0xf;                   // dummy for a nibble */
extern const uint8_t        eok_uint08dummy;        /**< = EOK_uint08dummy = 0xff;                  // dummy for a uint8_t */
extern const uint16_t       eok_uint16dummy;        /**< = EOK_uint16dummy = 0xffff;                // dummy for a uint16_t */
extern const uint32_t       eok_uint32dummy;        /**< = EOK_uint32dummy = 0xffffffffL;           // dummy for a uint32_t */
extern const uint64_t       eok_uint64dummy;        /**< = EOK_uint64dummy = 0xffffffffffffffffLL;  // dummy for a uint64_t */

extern const eOreltime_t    eok_reltimeZERO;        /**< = 0;                                       // Zero time */
extern const eOreltime_t    eok_reltime1ms;         /**< = 1000;                                    // 1 milli-sec */
extern const eOreltime_t    eok_reltime10ms;        /**< = 10*1000;                                 // 10 milli-sec */
extern const eOreltime_t    eok_reltime100ms;       /**< = 100*1000;                                // 100 milli-sec */
extern const eOreltime_t    eok_reltime1sec;        /**< = 1000*1000;                               // 1 sec */
extern const eOreltime_t    eok_reltimeMAX;         /**< = 0xFFFFFFFE;                              // equals 4294 seconds */
extern const eOreltime_t    eok_reltimeINFINITE;    /**< = 0xFFFFFFFF;                              // means that the wait must be infinite */

extern const eOabstime_t    eok_abstimeNOW;         /**< = 0xffffffffffffffffLL; */


extern const eOipv4addr_t   eok_ipv4addr_localhost;


enum { eo_sizecntnr_dynamic = 0xffff /*EOK_uint16dummy*/ };


/** @typedef    typedef struct eOdate_t
    @brief      eOdate_t contains a date. 
 **/
typedef struct              
{
    uint32_t            year  : 12;    /**< the year a.d. upto 2047 */
    uint32_t            month : 4;     /**< the month, where Jan is 1, Dec is 12 */
    uint32_t            day   : 5;     /**< the day from 1 to 31 */
    uint32_t            hour  : 5;     /**< the hour from 0 to 23 */
    uint32_t            min   : 6;     /**< the minute from 0 to 59 */
} eOdate_t;    


/** @typedef    typedef struct eOmap_str_u08_t
    @brief      it can be used to build a map containing a string and a value. 
 **/
typedef struct
{
    const char *    str0;
    uint8_t         val0;   
} eOmap_str_u08_t;


/** @typedef    typedef struct eOmap_str_u08_u08_t
    @brief      it can be used to build a map containing a string and two values. 
 **/
typedef struct
{
    const char *    str0;
    uint8_t         val0;  
    uint8_t         val1;    
} eOmap_str_u08_u08_t;


/** @typedef    typedef struct eOmap_str_str_u08_t
    @brief      it can be used to build a map containing two strings and a value. 
 **/
typedef struct
{
    const char *    str0;
    const char *    str1;
    uint8_t         val0;    
} eOmap_str_str_u08_t;


/** @typedef    typedef struct eOmap_str_str_u08_u08_t
    @brief      it can be used to build a map containing two strings and two values. 
 **/
typedef struct
{
    const char *    str0;
    const char *    str1;
    uint8_t         val0;
    uint8_t         val1;    
} eOmap_str_str_u08_u08_t;


/** @typedef    typedef struct eOmap_str_str_u08_u08_u08_t
    @brief      it can be used to build a map containing two strings and three values. 
 **/
typedef struct
{
    const char *    str0;
    const char *    str1;
    uint8_t         val0;
    uint8_t         val1;    
    uint8_t         val2;    
} eOmap_str_str_u08_u08_u08_t;

/** @typedef    typedef struct eOmap_int32_u08_t
    @brief      it can be used to build a map containing one int32_t and a value. 
 **/
typedef struct
{
    int32_t         int0;
    uint8_t         val0;    
} eOmap_int32_u08_t;


// --

typedef enum
{ 
    eobus_can1 = 0, eobus_can2 = 1, eobus_local = 2, eobus_icc1 = 3, eobus_icc2 = 4, eobus_ffu1 = 5, eobus_ffu2 = 6, eobus_none = 7             
} eObus_t; 

typedef struct
{
    uint8_t bus:3; // use only values from eObus_t
    uint8_t ffu:1; 
    uint8_t adr:4;      
} eOlocation_t; EO_VERIFYsizeof(eOlocation_t, 1)

// - public #define  --------------------------------------------------------------------------------------------------


#define EOK_VER_MAJ                         (0x01)
#define EOK_VER_MIN                         (0x00)


#define EO_MIN(a, b)                        (((a)<(b))?(a):(b))
#define EO_MAX(a, b)                        (((a)>(b))?(a):(b))

#define EO_CLIP_INT32(i32)                  ( ((i32)>EO_INT32_MAX) ? (EO_INT32_MAX) : ( ((i32)<(-EO_INT32_MAX)) ? (-EO_INT32_MAX) : (i32) )  )
#define EO_CLIP_INT16(i16)                  ( ((i16)>EO_INT16_MAX) ? (EO_INT16_MAX) : ( ((i16)<(-EO_INT16_MAX)) ? (-EO_INT16_MAX) : (i16) )  )
#define EO_CLIP_UINT16(u16)                 ( ((u16)>EO_UINT16_MAX) ? (EO_UINT16_MAX) : (u16) )


// issues a compiler error if the prop is false
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define EO_VERIFYproposition(name, prop)    typedef uint8_t TOKENPASTE2(name, __LINE__)[ ( 0 == (prop) ) ? (-1) : (1)];

// converts a macro into a string: #define ciao --> if(0 == strcmp(EO_STRINGIFY(ciao), "ciao")) { printf("ok\n"); }  
#define EO_STRINGIFY(v) _EO_STRINGIFY(v)
#define _EO_STRINGIFY(v) #v

#define EO_U8toU32(a)                       ((uint32_t)(a)&0xff)
#define EO_U16toU32(a)                      ((uint32_t)(a)&0xffff)
#define EO_U8toU64(a)                       ((uint64_t)(a)&0xff)
#define EO_4BtoU32(a, b, c, d)              ((EO_U8toU32(a))|(EO_U8toU32(b)<<8)|(EO_U8toU32(c)<<16)|(EO_U8toU32(d)<<24)) 
#define EO_8BtoU64(a, b, c, d, e, f, g, h)  (((uint64_t)EO_4BtoU32(a, b, c, d))|((uint64_t)EO_4BtoU32(e, f, g, h)<<32))


#define EOK_U32VERSION                      ((EO_U8toU32(EOK_VER_MAJ))|((EO_U8toU32(EOK_VER_MIN)<<8)))

#define EOK_uint04dummy                     (0xf)
#define EOK_uint08dummy                     (0xff)
#define EOK_uint16dummy                     (0xffff)
#define EOK_uint32dummy                     (0xffffffffL)
#define EOK_uint64dummy                     (0xffffffffffffffffLL)

#define EOK_int08dummy                      (0xff)

#define EOK_reltimeZERO                     (0)
#define EOK_reltime1ms                      (1000)
#define EOK_reltime10ms                     (10*1000)
#define EOK_reltime100ms                    (100*1000)
#define EOK_reltime1sec                     (1000*1000)
#define EOK_reltimeMAX                      (0xFFFFFFFE)
#define EOK_reltimeINFINITE                 (0xFFFFFFFF)

#define EOK_abstimeNOW                      (0xffffffffffffffffLL)

// 0.0f
#define EOK_Q17_14_ZERO                 ((eOq17_14_t)0x00000000)
// +1.0f
#define EOK_Q17_14_ONE                  ((eOq17_14_t)0x00004000)
// -1.0f
#define EOK_Q17_14_MINUS_ONE            ((eOq17_14_t)0xFFFFC000)

// +0.00006103515625f = +2^-14
#define EOK_Q17_14_POS_SMALLEST         ((eOq17_14_t)0x00000001)
#define EOK_FLOATQ17_14_POS_SMALLEST    (+0.00006103515625f)

// -0.00006103515625f = -2^-14
#define EOK_Q17_14_NEG_SMALLEST         ((eOq17_14_t)0xffffffff)
#define EOK_FLOATQ17_14_NEG_SMALLEST    (-0.00006103515625f)

// +131071.99993896484375f = 131072.0f - 0.00006103515625f
#define EOK_Q17_14_POS_BIGGEST          ((eOq17_14_t)0x7fffffff)
#define EOK_FLOATQ17_14_POS_BIGGEST     (+131071.99993896484375f)

// -131072.0f
#define EOK_Q17_14_NEG_BIGGEST          ((eOq17_14_t)0x80000000)
#define EOK_FLOATQ17_14_NEG_BIGGEST     (-131072.0f)

#define EO_COMMON_FLOAT_TO_Q17_14(v)                ((eOq17_14_t)(16384.0f*(v))) 

#define EO_COMMON_IPV4ADDR(ip1, ip2, ip3, ip4)      ((eOipv4addr_t)EO_4BtoU32(ip1, ip2, ip3, ip4))
#define EO_COMMON_MACADDR(m1, m2, m3, m4, m5, m6)   ((eOmacaddr_t)EO_8BtoU64(m1, m2, m3, m4, m5, m6, 0, 0))

#define EO_COMMON_IPV4ADDR_LOCALHOST                EO_COMMON_IPV4ADDR(127, 0, 0, 1)

#define EO_COMMON_CHECK_FLAG(var, flagmask)         ((flagmask) == ((var)& (flagmask)))


// - declaration of extern public functions ---------------------------------------------------------------------------

extern void * eo_common_getbaseobject(eOderived_t *derived);

extern void * eo_common_getsafebaseobject(eOsafelyderived_t *derived);

extern eOresult_t eo_common_verifysafebaseobject(eOsafelyderived_t *derived, const uint32_t tag);

// size of pointer allocated with malloc/calloc/realloc
extern size_t eo_common_msize(void *p);


extern eObool_t eo_common_byte_bitcheck(uint8_t byte, uint8_t bit);

extern void eo_common_byte_bitset(uint8_t* byte, uint8_t bit);

extern void eo_common_byte_bitclear(uint8_t* byte, uint8_t bit);

extern void eo_common_byte_bittoggle(uint8_t* byte, uint8_t bit);

extern uint8_t eo_common_byte_bitsetcount(uint8_t byte);



extern eObool_t eo_common_hlfword_bitcheck(uint16_t hword, uint8_t bit);

extern eObool_t eo_common_hlfword_maskcheck(uint16_t hword, uint16_t mask);

extern void eo_common_hlfword_bitset(uint16_t* hword, uint8_t bit);

extern void eo_common_hlfword_maskset(uint16_t* hword, uint16_t mask);

extern void eo_common_hlfword_bitclear(uint16_t* hword, uint8_t bit);

extern void eo_common_hlfword_maskclear(uint16_t* hword, uint16_t mask);

extern void eo_common_hlfword_bittoggle(uint16_t* hword, uint8_t bit);

extern uint8_t eo_common_hlfword_bitsetcount(uint16_t hword);


extern eObool_t eo_common_word_bitcheck(uint32_t word, uint8_t bit);

extern void eo_common_word_bitset(uint32_t* word, uint8_t bit);

extern void eo_common_word_bitclear(uint32_t* word, uint8_t bit);

extern void eo_common_word_bittoggle(uint32_t* word, uint8_t bit);

extern uint8_t eo_common_word_bitsetcount(uint32_t word);


extern eObool_t eo_common_dword_bitcheck(uint64_t dword, uint8_t bit);

extern void eo_common_dword_bitset(uint64_t* dword, uint8_t bit);

extern void eo_common_dword_bitclear(uint64_t* dword, uint8_t bit);

extern void eo_common_dword_bittoggle(uint64_t* dword, uint8_t bit);

extern uint8_t eo_common_dword_bitsetcount(uint64_t dword);


// all the Q17_4 functions clip to upper or lower limit.

extern eOq17_14_t eo_common_float_to_Q17_14(float f_num);

extern float eo_common_Q17_14_to_float(eOq17_14_t q_num);

// it sums two q17-14 numbers and manages positive and negative overflows. 
// if the sum overflows, the result is clipped to biggest pos / neg and funtions returns eores_NOK_generic.
// if the sum is valid return is eores_OK
extern eOresult_t eo_common_Q17_14_add(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res);

extern eOresult_t eo_common_Q17_14_multiply(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res);

extern eOresult_t eo_common_Q17_14_divide(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res);


extern uint64_t eo_common_canframe_data2u64(eOcanframe_t *frame);

extern void eo_common_ipv4addr_to_string(eOipv4addr_t ipv4, char *str, uint8_t size);

extern void eo_common_ipv4addr_to_decimal(eOipv4addr_t ipv4, uint8_t *ip1, uint8_t *ip2, uint8_t *ip3, uint8_t *ip4);

extern void eo_common_date_to_string(eOdate_t date, char *str, uint8_t size);

extern eObool_t eo_common_string_to_date(const char *str, eOdate_t *date);

extern eObool_t eo_common_compiler_string_to_date(const char *str, eOdate_t *date);

extern const char * eo_common_map_str_str_u08__value2string(const eOmap_str_str_u08_t * map, uint8_t size, uint8_t value, eObool_t usestr0);

extern uint8_t eo_common_map_str_str_u08__string2value(const eOmap_str_str_u08_t * map, uint8_t size, const char * string, eObool_t usestr0, uint8_t defvalue);


extern eOipv4addr_t eo_common_ipv4addr(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);
extern eOmacaddr_t eo_common_macaddr(uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4, uint8_t m5, uint8_t m6);
extern eObool_t eo_common_event_check(eOevent_t event, eOevent_t mask);



/** @}            
    end of group eo_common  
 **/
 

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




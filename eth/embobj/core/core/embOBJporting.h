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
// ----
// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EMBODYPORTING_H_
#define _EMBODYPORTING_H_


#ifdef __cplusplus
extern "C" {
#endif

// - doxy begin -------------------------------------------------------------------------------------------------------

/** @file       emBODYporting.h
    @brief      This header file cevcervcwer.
    @author     marco.accame@iit.it
    @date       11/03/2011
**/

/** @defgroup cecedcedc complier specifics for emBODY environment
    The embENV allows ...... 
 
    @todo acemor-facenda: do documentation.
    

    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "stdint.h"


// - public #define  --------------------------------------------------------------------------------------------------

                       
//#ifdef __cplusplus                                          
//    #warning __cplusplus is defined: COMPILING IN C++            
//#endif

// from nicolo.genesio@iit.it :
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define EMBOBJ_HELPER_DLL_IMPORT __declspec(dllimport)
  #define EMBOBJ_HELPER_DLL_EXPORT __declspec(dllexport)
  #define EMBOBJ_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define EMBOBJ_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define EMBOBJ_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define EMBOBJ_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define EMBOBJ_HELPER_DLL_IMPORT
    #define EMBOBJ_HELPER_DLL_EXPORT
    #define EMBOBJ_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define EMBOBJ_API and EMBOBJ_LOCAL.
// EMBOBJ_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// EMBOBJ_LOCAL is used for non-api symbols.

#ifdef EMBOBJ_DLL // defined if EMBOBJ is compiled as a DLL
  #ifdef embobj_EXPORTS// defined if we are building the EMBOBJ DLL (instead of using it)
    #define EMBOBJ_API EMBOBJ_HELPER_DLL_EXPORT
  #else
    #define EMBOBJ_API EMBOBJ_HELPER_DLL_IMPORT
  #endif // EMBOBJ_DLL_EXPORTS
  #define EMBOBJ_LOCAL EMBOBJ_HELPER_DLL_LOCAL
#else // EMBOBJ_DLL is not defined: this means EMBOBJ is a static lib.
  #define EMBOBJ_API
  #define EMBOBJ_LOCAL
#endif // EMBOBJ_DLL



#if defined(__arm__)

    // __arm__ or __ARMCC_VERSION are always defined in armcc compiler
    #define EO_extern_inline        extern inline
    #define EO_static_inline        static inline
        
#if defined(__cplusplus)
    // c++ does not have designated initializers ... they are only in c99
    #define EO_INIT(f)

    // c++ does not have INT32_MAX etc.
    // i dont use std::numeric_limits<std::int32_t>::max() because the inclusion of <cstdint> and <limits> gives me problem on linkage
    #define EO_INT32_MAX  2147483647
    #define EO_INT16_MAX  32767
    #define EO_INT16_MIN  (-32768)
    #define EO_UINT16_MAX 65535

#else 
    
    #define EO_INIT(f)     f =
    #define EO_INT32_MAX   INT32_MAX
    #define EO_INT16_MAX   INT16_MAX
    #define EO_INT16_MIN   INT16_MIN
    #define EO_UINT16_MAX  UINT16_MAX
    
#endif
    
    #pragma pack(8)
#if (__ARMCC_VERSION > 6000000)
    #define EO_weak         __attribute__((weak))
#else
    #define EO_weak         __weak
#endif
    
    typedef float float32_t;
    #define EO_TAILOR_CODE_FOR_ARM    
    #define EO_READ_PREV_WORD_OF_MALLOC_FOR_SIZEOF_ALLOCATION
    
#elif defined(_MSC_VER)
    // msc does not support c99, thus inline must be redefined as __inline
    #define EO_extern_inline       extern __inline
    #define EO_static_inline       static __inline
    //#define inline          __inline
    // msc does not support c99, thus designated initializers in structs (i.e., .item = val) must be moved to the old way
    #define EO_INIT(f) 
    #define EO_INT32_MAX   INT32_MAX
    #define EO_INT16_MAX   INT16_MAX
    #define EO_UINT16_MAX  UINT16_MAX    
    #define EO_INT16_MIN   INT16_MIN
    //#pragma pack(4) 
    #pragma pack(8)
    #if _MSC_VER < 1900
    #define snprintf        sprintf_s
    #endif
    typedef float float32_t;
    #define __weak 
    #define EO_weak 
    #define EO_TAILOR_CODE_FOR_WINDOWS
    #define EO_WARNING(a)   __pragma(message("EOWARNING-> "##a))
    #define OVERRIDE_eo_receiver_callback_incaseoferror_in_sequencenumberReceived
//#pragma message(a)
#elif defined(__linux__) & defined(__GNUC__)
    // gcc-linux
    //#define inline          inline
    #define EO_extern_inline       static inline
    #define EO_static_inline       static inline
    #define EO_INIT(f)
    #define EO_INT32_MAX   INT32_MAX
    #define EO_INT16_MAX   INT16_MAX
    #define EO_UINT16_MAX  UINT16_MAX
    #pragma pack(8)
    #define snprintf        snprintf
    typedef float float32_t;
    #define EO_weak          __attribute__((weak))
    #define EO_TAILOR_CODE_FOR_LINUX
    #define EO_WARNING(a)   _Pragma(message("EOWARNING-> "##a))
    #define OVERRIDE_eo_receiver_callback_incaseoferror_in_sequencenumberReceived
    #define _PEDANT_WARNING_ON_COMPILATION_CALLBACK_
    #define _STATS_DEBUG_FOR_CYCLE_TIME_
#elif defined(_DSPIC_) & defined(__GNUC__)
    // for dspic ...
    #define EO_extern_inline        extern inline
    #define EO_static_inline        static inline
    // other compilers which support c99 can keep the designated initializers in structs
    #define EO_INIT(f)      f =
    //#pragma pack(4)
    // or pack(8) ???
    //#define snprintf        snprintf   
    //#define stdint    dspic_stdint
    #define EO_TAILOR_CODE_FOR_DSPIC
    #define __weak      __attribute__((__weak__))
#elif defined(__APPLE__)
    #define EO_extern_inline       static inline
    #define EO_static_inline       static inline
    #define EO_INIT(f)
    #pragma pack(8)
    #define snprintf        snprintf
    typedef float float32_t;
    #define EO_weak         __attribute__((weak))
#else
    #error architecture not defined 
#endif
    


#define __emBODYportingVERIFYsizeof(sname, ssize)    typedef uint8_t GUARD##sname[ ( ssize == sizeof(sname) ) ? (1) : (-1)];


// - declaration of public user-defined types ------------------------------------------------------------------------- 

typedef int8_t emBODYporting_enum08_t;

// - declaration of extern public functions ---------------------------------------------------------------------------

 

/** @}            
    end of group cecedcedc 
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard 

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------




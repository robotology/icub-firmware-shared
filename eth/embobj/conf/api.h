/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef EMBOBJ_CONF_API_H
#define EMBOBJ_CONF_API_H


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


#endif // EMBOBJ_CONF_API_H

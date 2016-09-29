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
#ifndef _EOMEASURES_H_
#define _EOMEASURES_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       EoMeasures.h
	@brief      This header file gives 
	@author     marco.accame@iit.it
	@date       09/06/2011
**/

/** @defgroup eo_c2e2rd3f3egfvr3 Configuation of  vfef ve e
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoUtilities.h"



// - public #define  --------------------------------------------------------------------------------------------------

#define EOMEAS_1DEG_IDG (182.0444444444444f)
#define EOMEAS_1IDG_DEG (0.0054931640625f)

#define EOMEAS_DEG2IDG(deg)     ((float)(deg)*EOMEAS_1DEG_IDG)

#define EOMEAS_IDG2DEG(idg)     ((float)(idg)*EOMEAS_1IDG_DEG)
    

// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef int32_t             eOmeas_position_t
    @brief      eOmeas_position_t contains the position values.
                the measurement unit is the icubdegree, or idegree or idg, where 
                1 idegree = 360 degrees / 65536 = 0.0054931640625 degrees.
                1 degree = 182.0444444444444 idegrees.                                
 **/
typedef int32_t             eOmeas_position_t;

/** @typedef    typedef struct             eOmeas_position_limits_t
    @brief      eOmeas_position_limits_t contains a pair of values which express a limit.                                
 **/
typedef struct
{
    eOmeas_position_t           min;        /**< the minimum position  */
    eOmeas_position_t           max;        /**< the maximum position  */    
} eOmeas_position_limits_t;


/** @typedef    typedef uint16_t            eOmeas_time_t
    @brief      eOmeas_time_t contains the time values.
                the measurement unit is the ms.                                
 **/
typedef uint16_t            eOmeas_time_t;


/** @typedef    typedef int32_t             eOmeas_velocity_t
    @brief      eOmeas_velocity_t contains the velocity values.
                the measurement unit is the idegree/s
 **/
typedef int32_t             eOmeas_velocity_t;


/** @typedef    typedef int32_t             eOmeas_acceleration_t
    @brief      eOmeas_acceleration_t contains the acceleration values.
                the measurement unit is idegree/s^2.
 **/
typedef int32_t             eOmeas_acceleration_t;


/** @typedef    typedef int16_t             eOmeas_force_t
    @brief      eOmeas_force_t contains the force values.
                the measurement unit is mN (milli Newton).
 **/
typedef int32_t             eOmeas_force_t;


/** @typedef    typedef int32_t             eOmeas_torque_t
    @brief      eOmeas_torque_t contains the torque values.
                the measurement unit is mN m (milli Newton metre).
 **/
typedef float             eOmeas_torque_t;


/** @typedef    typedef int16_t             eOmeas_current_t
    @brief      eOmeas_current_t contains the current values.
    the measurement unit is milli Ampere
 **/
typedef int16_t             eOmeas_current_t;


/** @typedef    typedef uint32_t             eOmeas_stiffness_t
    @brief      eOmeas_stiffness_t contains the stiffness values.
                the measurement unit is mN m / degree (milli Newton metre per degree, not idg!).
 **/
typedef float            eOmeas_stiffness_t;


/** @typedef    typedef uint32_t             eOmeas_damping_t
    @brief      eOmeas_damping_t contains the damping values.
                the measurement unit is mN m / (degree/sec) (milli Newton metre per degree/sec, not idg/sec!).
 **/
typedef float            eOmeas_damping_t;


/** @typedef    typedef int16_t             eOmeas_temperature_t
    @brief      eOmeas_temperature_t contains the temperature values.
                the measurement unit is 0.1 C (range is thus +/- 3200 C, well below the 0 kelvin ....).
 **/
typedef int16_t             eOmeas_temperature_t;



/** @typedef    typedef int16_t             eOmeas_pwm_t
    @brief      eOmeas_pwm_t contains the pwm values.
                the measurement unit is ....... mV/degree
 **/
typedef int16_t             eOmeas_pwm_t;
    
    
    
/** @typedef    typedef int16_t             eOmeas_voltage_t
    @brief      eOmeas_voltage_t contains the voltage values.
                the measurement unit is 0.01 V.
 **/
typedef int16_t             eOmeas_voltage_t;

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section




/** @}            
    end of group eo_c2e2rd3f3egfvr3  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

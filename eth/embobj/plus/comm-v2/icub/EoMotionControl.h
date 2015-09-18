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

#ifndef _EOMOTIONCONTROL_H_
#define _EOMOTIONCONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       EoMotionControl.h
	@brief      This header file gives 
	@author     marco.accame@iit.it
	@date       09/06/2011
**/

/** @defgroup eo_motioncontrol Types used for motion control in the Ethernet boards
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoUtilities.h"
#include "EoMeasures.h"



// - public #define  --------------------------------------------------------------------------------------------------
// empty-section



// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef enum eOmc_entity_t;
    @brief      It contains all the possible entities managed by motion control.
                See definition of eOmc_joint_t, eOmc_motor_t, and eOmc_controller_t for explanations
 **/
typedef enum
{
    eomc_entity_joint                           = 0,
    eomc_entity_motor                           = 1,
    eomc_entity_controller                      = 2
} eOmc_entity_t; 

enum { eomc_entities_numberof = 3 };


// -- all the possible enum


/** @warning    DO NOT CHANGE these values, as they are given by iCub protocol.
                It is important to use negative values instead of their representation in hexadecimal: in this way compiler 
                defines eOmc_controlmode_command_t and eOmc_controlmode_t like int8_t and not like uint8_t.
                In this way fields, that contain values of eOmc_controlmode_t,  are compatible with their declared type (eOenum08_t).
                In the following structures, fields of enumeration type have type eOenum08_t (int8_t), instead of the specific enum type like eOmc_controlmode_t,
                in order to force the structure to a specific size, because this structure are transported in Ethernet protocol,
                so fixed size is basic for parsing and forming messages.
 **/
enum {  eomc_ctrlmval_idle          = 0x00, 
        eomc_ctrlmval_position      = 0x01, 
        eomc_ctrlmval_velocity      = 0x02, 
        eomc_ctrlmval_torque        = 0x03, 
        eomc_ctrlmval_impedance_pos = 0x04, //to be removed
        eomc_ctrlmval_impedance_vel = 0x05, //to be removed
        eomc_ctrlmval_current       = 0x06, 
        eomc_ctrlmval_velocity_pos  = 0x07, //to be removed
        eomc_ctrlmval_mixed         = 0x08,
        eomc_ctrlmval_direct        = 0x09, 
        eomc_ctrlmval_hwFault       = 0x0A, 
        eomc_ctrlmval_notConfigured = 0x0B, 
        eomc_ctrlmval_configured    = 0x0C, 
        eomc_ctrlmval_forceIdle     = 0x0D, 
        eomc_ctrlmval_openloop      = 0x50, 
        eomc_ctrlmval_everything_off= -16,   /*0xf0*/  //to be removed
        eomc_ctrlmval_calib         = -2,    /*0xfe*/
        eomc_ctrlmval_unknownError  = -1     /*0xff*/
     };


/** @typedef    typedef enum eOmc_controlmode_command_t
    @brief      eOmc_controlmode_command_t contains command to set the control mode.
    @warning    On an EMS only modes eomc_controlmode_cmd_idle, eomc_controlmode_cmd_position, eomc_controlmode_cmd_velocity, eomc_controlmode_cmd_torque,
                eomc_controlmode_cmd_impedance_pos, eomc_controlmode_cmd_impedance_vel, and eomc_controlmode_cmd_openloop are allowed.
                On a 2FOC the only possible mode is eomc_controlmode_cmd_current.  
                Pay attention if you change numbers. they must be align with eOmc_controlmode_t.
 **/
typedef enum
{
    eomc_controlmode_cmd_idle                       = eomc_ctrlmval_idle,
    eomc_controlmode_cmd_force_idle                 = eomc_ctrlmval_forceIdle,
    eomc_controlmode_cmd_position                   = eomc_ctrlmval_position,
    eomc_controlmode_cmd_velocity                   = eomc_ctrlmval_velocity,      /**< velocity control loop */ 
    eomc_controlmode_cmd_torque                     = eomc_ctrlmval_torque,
    eomc_controlmode_cmd_impedance_pos              = eomc_ctrlmval_impedance_pos, //to be removed
    eomc_controlmode_cmd_impedance_vel              = eomc_ctrlmval_impedance_vel, //to be removed
    eomc_controlmode_cmd_current                    = eomc_ctrlmval_current,
    eomc_controlmode_cmd_openloop                   = eomc_ctrlmval_openloop,
    eomc_controlmode_cmd_switch_everything_off      = eomc_ctrlmval_everything_off,    //to be removed   /**< it imposes a zero current on the motor and also turns the pwm off */    
    eomc_controlmode_cmd_mixed                      = eomc_ctrlmval_mixed,
    eomc_controlmode_cmd_direct                     = eomc_ctrlmval_direct, 
} eOmc_controlmode_command_t;


/** @typedef    typedef enum eOmc_controlmode_t
    @brief      eOmc_controlmode_t contains all the possible modes for motion control.
    @warning    On an EMS only modes eomc_controlmode_idle, eomc_controlmode_position, eomc_controlmode_velocity, eomc_controlmode_torque,
                eomc_controlmode_impedance_pos, eomc_controlmode_impedance_vel, and eomc_controlmode_openloop are allowed.
                On a 2FOC the only possible mode is eomc_controlmode_current.
                when command eomc_controlmode_cmd_switch_everything_off is received the motor controller is in eomc_controlmode_idle.
                when command calibration (see eOmc_calibrator_t in joint commands) the motor control is in eomc_controlmode_calib.
                Pay attention if you change numbers. they must be align with eOmc_controlmode_command_t.
 **/
typedef enum
{
    eomc_controlmode_idle                       = eomc_ctrlmval_idle,     /**< pid and pwm off*/
    eomc_controlmode_position                   = eomc_ctrlmval_position,
    eomc_controlmode_velocity                   = eomc_ctrlmval_velocity,      /**< velocity control loop */    
    eomc_controlmode_torque                     = eomc_ctrlmval_torque,
    eomc_controlmode_impedance_pos              = eomc_ctrlmval_impedance_pos, //to be removed
    eomc_controlmode_impedance_vel              = eomc_ctrlmval_impedance_vel, //to be removed
    eomc_controlmode_current                    = eomc_ctrlmval_current, 
    eomc_controlmode_velocity_pos               = eomc_ctrlmval_velocity_pos,   /*to be removed*/  /**< The controller is in position, but the controller switches to eomc_controlmode_velocity_pos
                                                                                    automatically when it receives a velocity set setpoint. 
                                                                                    In icub can proto there is not differences between velocity and velocity_pos */  
    eomc_controlmode_openloop                   = eomc_ctrlmval_openloop,
    eomc_controlmode_calib                      = eomc_ctrlmval_calib,      /**< it means joint is in calibration, without specifing wich type of calibartion joint is using. this value doesn't belong to icub can proto. */ 
    eomc_controlmode_mixed                      = eomc_ctrlmval_mixed,
    eomc_controlmode_direct                     = eomc_ctrlmval_direct,
    eomc_controlmode_hwFault                    = eomc_ctrlmval_hwFault,
    eomc_controlmode_notConfigured              = eomc_ctrlmval_notConfigured,
    eomc_controlmode_configured                 = eomc_ctrlmval_configured,
    eomc_controlmode_unknownError               = eomc_ctrlmval_unknownError
} eOmc_controlmode_t;



enum
{
    eomc_imodeval_stiff                  = 0x00,
    eomc_imodeval_compliant              = 0x01,
    eomc_imodeval_unknownError           = -1
};




/** @typedef    typedef enum eOmc_interactionmode_t
    @brief      contains all the possible interaction modes
 **/
typedef enum
{
    eOmc_interactionmode_stiff                  = eomc_imodeval_stiff,
    eOmc_interactionmode_compliant              = eomc_imodeval_compliant
} eOmc_interactionmode_t;


/** @typedef    typedef enum eOmc_motionmonitormode_t
    @brief      contains all the possible modes for motion monitoring.
                It is used to configure the reporting of the motion in relation to reaching a given setpoint,
                as the message motiondone over CAN.    
 **/
typedef enum
{
    eomc_motionmonitormode_dontmonitor      = 0,            /**< the motion is not monitored */
    eomc_motionmonitormode_untilreached     = 1,            /**< the motion is monitored only until the setpoint is reached (even if the setpoint is changed before being reached) */
    eomc_motionmonitormode_forever          = 2             /**< the motion is monitored vs any current of future setpoint until the mode is explicitly changed */
} eOmc_motionmonitormode_t;


/** @typedef    typedef enum eOmc_motionmonitorstatus_t
    @brief      contains the possible status for motion monitoring.
                It is used to reporting inside the status of teh joint about the reaching of a given setpoint,
                as the message motiondone over CAN.    
 **/
typedef enum
{
    eomc_motionmonitorstatus_notmonitored           = EOK_int08dummy,       /**< the motion is not monitored vs a desired setpoint */
    eomc_motionmonitorstatus_setpointnotreachedyet  = 0,                    /**< the motion is monitored but the desired setpoint is not reached yet */
    eomc_motionmonitorstatus_setpointisreached      = 1                     /**< the motion is monitored and the desired setpoint is reached */
} eOmc_motionmonitorstatus_t;



/** @typedef    typedef enum eOmc_controlstatus_t
    @brief      contains the possible status for a motor control.
 **/
typedef enum
{
    eomc_controlstatus_idle                     = 0x00,
    eomc_controlstatus_running                  = 0x01
} eOmc_controlstatus_t;


/** @typedef    typedef enum eOmc_setpoint_type_t
    @brief      contains the possible types of setpoints.
 **/
typedef enum
{
    eomc_setpoint_position                      = 0,
    eomc_setpoint_velocity                      = 1,
    eomc_setpoint_torque                        = 2,
    eomc_setpoint_current                       = 3,
    eomc_setpoint_positionraw                   = 4
} eOmc_setpoint_type_t;


/** @typedef    typedef enum eOmc_calibration_types_t
    @brief      contains the possible types of calibration. 
                the values cannot be changed because they are given by icub can protocol
 **/
typedef enum
{
    eomc_calibration_type0_hard_stops               = 0,    // cannot change
    eomc_calibration_type1_abs_sens_analog          = 1,    // cannot change
    eomc_calibration_type2_hard_stops_diff          = 2,    // cannot change
    eomc_calibration_type3_abs_sens_digital         = 3,    // cannot change
    eomc_calibration_type4_abs_and_incremental      = 4,    // cannot change
    eomc_calibration_type5_hard_stops_mc4plus       = 5,    // cannot change
    eomc_calibration_type6_mais_mc4plus             = 6,    // cannot change /* NAME still to be defined */
    eomc_calibration_type5_hall_sensor_mc4plus      = 7,    // cannot change /* NAME still to be defined */
    eomc_calibration_typeUndefined                  = 255   // cannot change
} eOmc_calibration_type_t;



// -- all the possible data service structures


/** @typedef    typedef struct eOmc_calibrator16_params_type0_hard_stops_t
    @brief      contains the params in case of eomc_calibration_type0_hard_stops
 **/
typedef struct  
{
    int16_t                    pwmlimit;
    int16_t                    velocity;
} eOmc_calibrator16_params_type0_hard_stops_t;


/** @typedef    typedef struct eOmc_calibrator16_params_type1_abs_sens_analog_t
    @brief      contains the params in case of eomc_calibration_type1_abs_sens_analog_t
 **/
typedef struct  
{
    int16_t                    position;
    int16_t                    velocity;
} eOmc_calibrator16_params_type1_abs_sens_analog_t;


/** @typedef    typedef struct eOmc_calibrator16_params_type2_hard_stops_diff_t
    @brief      contains the params in case of eomc_calibration_type2_hard_stops_diff_t
 **/
typedef struct  
{
    int16_t                    pwmlimit;
    int16_t                    velocity;
} eOmc_calibrator16_params_type2_hard_stops_diff_t;


/** @typedef    typedef struct eOmc_calibrator16_params_type3_abs_sens_digital_t
    @brief      contains the params in case of eomc_calibration_type3_abs_sens_digital_t
 **/
typedef struct  
{
    int16_t                    position;
    int16_t                    velocity;
    int16_t                    offset;
} eOmc_calibrator16_params_type3_abs_sens_digital_t;


/** @typedef    typedef struct eOmc_calibrator16_params_type4_abs_and_incremental_t
    @brief      contains the params in case of eomc_calibration_type4_abs_and_incremental_t
 **/
typedef struct  
{
    int16_t                    position;
    int16_t                    velocity;
    int16_t                    maxencoder;
} eOmc_calibrator16_params_type4_abs_and_incremental_t;


/** @typedef    typedef struct eOmc_calibrator_params_type0_hard_stops_t
    @brief      contains the params in case of eomc_calibration_type0_hard_stops
 **/
typedef struct  
{
    int16_t                     pwmlimit;
    eOmeas_velocity_t           velocity;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type0_hard_stops_t;


/** @typedef    typedef struct eOmc_calibrator_params_type1_abs_sens_analog_t
    @brief      contains the params in case of eomc_calibration_type1_abs_sens_analog_t
 **/
typedef struct  
{
    eOmeas_position_t           position;
    eOmeas_velocity_t           velocity;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type1_abs_sens_analog_t;


/** @typedef    typedef struct eOmc_calibrator_params_type2_hard_stops_diff_t
    @brief      contains the params in case of eomc_calibration_type2_hard_stops_diff_t
 **/
typedef struct  
{
    int16_t                     pwmlimit;
    eOmeas_velocity_t           velocity;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type2_hard_stops_diff_t;


/** @typedef    typedef struct eOmc_calibrator_params_type3_abs_sens_digital_t
    @brief      contains the params in case of eomc_calibration_type3_abs_sens_digital_t
 **/
typedef struct  
{
    eOmeas_position_t           position;
    eOmeas_velocity_t           velocity;
    int32_t                     offset;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type3_abs_sens_digital_t;


/** @typedef    typedef struct eOmc_calibrator_params_type4_abs_and_incremental_t
    @brief      contains the params in case of eomc_calibration_type4_abs_and_incremental_t
 **/
typedef struct  
{
    eOmeas_position_t           position;
    eOmeas_velocity_t           velocity;
    uint32_t                    maxencoder;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type4_abs_and_incremental_t;


/** @typedef    typedef struct eOmc_calibrator_params_type5_hard_stops_mc4plus_t
    @brief      contains the params in case of eomc_calibration_type5_hard_stops_mc4plus
 **/
typedef struct  
{
    int32_t                     pwmlimit;
    int32_t                     final_pos;
    int32_t                     calibrationZero;
} eOmc_calibrator_params_type5_hard_stops_mc4plus_t;


// -- all the possible data holding structures used in a joint


/** @typedef    typedef struct eOmc_PID32_t
    @brief      eOmc_PID32_t contains the values required to configure a PID control compatible with CAN board.
                For the meaning of the values of a PID to be used on a CAN board see ...
                On a ARM-based board, the values kp, ki, kd, limitonlintegral, linitonoutput and offset represent
                the integer part of the floating point values which are effectively used.
 **/
typedef struct
{
    int32_t                 kp;                 /**< proportional gain */
    int32_t                 ki;                 /**< integral gain */
    int32_t                 kd;                 /**< derivative gain */
    int32_t                 kff;                /**< stiction gain */
    int32_t                 limitonintegral;    /**< limit of integral term */ 
    int32_t                 limitonoutput;      /**< limit of the output of the pid */
    int32_t                 offset;             /**< the k0 in the pid formula */
    int32_t                 stiction_up_val;
    int32_t                 stiction_down_val;
    int8_t                  scale;              /**< scale factor for the gains (used for a rigth shift */
    uint8_t                 filler[3];
} eOmc_PID32_t;             EO_VERIFYsizeof(eOmc_PID32_t, 40);


typedef struct
{
    float32_t               kp;                 /**< proportional gain */
    float32_t               ki;                 /**< integral gain */
    float32_t               kd;                 /**< derivative gain */
    float32_t               kff;
    float32_t               limitonintegral;    /**< limit of integral term */ 
    float32_t               limitonoutput;      /**< limit of the output of the pid */
    float32_t               offset;             /**< the k0 in the pid formula */
    float32_t               stiction_up_val;
    float32_t               stiction_down_val;
    int8_t                  scale;
    uint8_t                 filler[3];
} eOmc_PIDfl_t;             EO_VERIFYsizeof(eOmc_PIDfl_t, 40);

// the PID currently used is eOmc_PIDfl_t
//typedef eOmc_PID32_t eOmc_PID_t;
typedef eOmc_PIDfl_t eOmc_PID_t;


/** @typedef    typedef struct eOmc_impedance_t
    @brief      eOmc_impedance_t specifies a the parameters used in control of kind 
                eomc_controlmode_impedance_pos or eomc_controlmode_impedance_vel.
 **/
typedef struct                  // size is 4+4+2+2+0 = 12
{
    eOmeas_stiffness_t          stiffness;                          /**< the Ks parameter */
    eOmeas_damping_t            damping;                            /**< the Kd parameter */
    eOmeas_torque_t             offset;                             /**< the Ko parameter */
    uint8_t                     filler02[2];                        
} eOmc_impedance_t;             //EO_VERIFYsizeof(eOmc_impedance_t, 12);



/** @typedef    typedef struct eOmc_calibrator16_t
    @brief      eOmc_calibrator16_t specifies a calibrator with type and parameters
 **/
typedef struct                  // size is 1+1+2+2+2+0 = 8
{
    uint8_t                     filler1[1];                         /**< in front to match the memory mapping of the can message */
    eOenum08_t                  type;                               /**< use eOmc_calibration_type_t */
    union                           
    {
        uint16_t                                                any[3];
        eOmc_calibrator16_params_type0_hard_stops_t             type0;
        eOmc_calibrator16_params_type1_abs_sens_analog_t        type1;
        eOmc_calibrator16_params_type2_hard_stops_diff_t        type2;
        eOmc_calibrator16_params_type3_abs_sens_digital_t       type3;
        eOmc_calibrator16_params_type4_abs_and_incremental_t    type4;
    } params;                                                       /**< the params of the calibrator */   
} eOmc_calibrator16_t;           //EO_VERIFYsizeof(eOmc_calibrator16_t, 8);



/** @typedef    typedef struct eOmc_calibrator32_t
    @brief      eOmc_calibrator32_t specifies a calibrator with type and parameters for teh new definition of measures
 **/
typedef struct                  // size is 1+3+4*3 = 16
{
    eOenum08_t                  type;                               /**< use eOmc_calibration_type_t */
    uint8_t                     filler03[3];
    union
    {
        uint32_t                                                any[4];
        eOmc_calibrator_params_type0_hard_stops_t               type0;
        eOmc_calibrator_params_type1_abs_sens_analog_t          type1;
        eOmc_calibrator_params_type2_hard_stops_diff_t          type2;
        eOmc_calibrator_params_type3_abs_sens_digital_t         type3;
        eOmc_calibrator_params_type4_abs_and_incremental_t      type4;
        eOmc_calibrator_params_type5_hard_stops_mc4plus_t       type5;
    } params;                                                       /**< the params of the calibrator */   
} eOmc_calibrator32_t;           //EO_VERIFYsizeof(eOmc_calibrator32_t, 16);

typedef eOmc_calibrator32_t eOmc_calibrator_t;


/*  This proposition is used to check if used calibrator is eOmc_calibrator32_t.
    This check is important because calibrators'params are sent to can and so conrveted. 
    If used calibrator is a different one, please modify converter object. */
EO_VERIFYproposition(isusedcalibrator32, (sizeof(eOmc_calibrator_t) == sizeof(eOmc_calibrator32_t)));


/** @typedef    typedef struct eOmc_setpoint_t
    @brief      eOmc_setpoint_t contains the setpoint to be sent to a pid
 **/
typedef struct              // size is 1+3+8+0 = 12
{
    eOenum08_t              type;                   /**< use eOmc_setpoint_type_t */
    uint8_t                 filler03[3];
    union
    {
        uint8_t             sizeofunionis08[8];     
        struct 
        { 
            eOmeas_position_t       value; 
            eOmeas_velocity_t       withvelocity; 
        } position;
        struct 
        { 
            eOmeas_velocity_t       value; 
            eOmeas_acceleration_t   withacceleration; 
        } velocity;
        struct 
        { 
            eOmeas_torque_t        value; 
        } torque;
        struct
        { 
            eOmeas_current_t       value; 
        } current;
    } to;                       /**< the union containing the value field and optional param */
} eOmc_setpoint_t;              //EO_VERIFYsizeof(eOmc_setpoint_t, 12);

typedef struct
{
    float       bemf_value;
    float       ktau_value;
    int8_t      bemf_scale;
    int8_t      ktau_scale;
    int8_t      filler02[2];
} eOmc_motor_params_t;  EO_VERIFYsizeof(eOmc_motor_params_t, 12);

// -- all the possible data holding structures used in a motor


typedef struct
{
    uint16_t tresh;         /**< Current threshold  */
    uint16_t time;          /**< Filter time parameter  */
} eOmc_i2tParams_t;



// -- the definition of a joint



/** @typedef    typedef uint8_t  eOmc_jointId_t
    @brief      eOmc_jointId_t contains the values required to identify a joint
 **/
typedef uint8_t  eOmc_jointId_t;


/** @typedef    typedef uint8_t  eOmc_torqueControlFilterType_t
    @brief      eOmc_torqueControlFilterType_t contains a value (0-255) which select a specific filter type for the torque control algorithm
 **/
typedef uint8_t  eOmc_torqueControlFilterType_t;


/** @typedef    typedef struct eOmc_joint_config_t
    @brief      eOmc_joint_config_t contains the values required to configure a joint
 **/
typedef struct                  // size is: 24+24+24+8+12+2+1+1+4+4+4 = 104/80 
{
    eOmc_PID_t                  pidposition;                /**< the pid for position control */
    eOmc_PID_t                  pidvelocity;                /**< the pid for velocity control */
    eOmc_PID_t                  pidtorque;                  /**< the pid for torque control */
    eOmeas_position_limits_t    limitsofjoint;              /**< the minimum and maximum position of the joint */
    eOmc_impedance_t            impedance;                  /**< the impedance to use in control of the relevant kind */                 
    eOmeas_time_t               velocitysetpointtimeout;    /**< max time between two setpoints in eomc_controlmode_velocity before going back to eomc_controlmode_position */              
    eOenum08_t                  motionmonitormode;          /**< use values from eOmc_motionmonitormode_t. it tells if and how to monitor the motion. the result is placed inside jstatus.jstatusbasic.motionmonitorstatus */
    uint8_t                     filler01[1];      
    eOutil_emulfloat32_t        DEPRECATED_encoderconversionfactor;
    eOutil_emulfloat32_t        DEPRECATED_encoderconversionoffset;
    int32_t                     jntEncoderResolution;
    eOmc_motor_params_t         motor_params;
    eOmc_torqueControlFilterType_t tcfiltertype;
    uint8_t                     jntEncoderType;
    uint8_t                     filler02[2];
} eOmc_joint_config_t;          EO_VERIFYsizeof(eOmc_joint_config_t, 172);




/** @typedef    typedef struct eOmc_joint_status_basic_t
    @brief      eOmc_joint_status_basic_t contains the basic status of the joint
 **/
typedef struct                  // size is: 4+4+4+2+1+1+0 = 16
{
    eOmeas_position_t           jnt_position;               /**< the position of the joint */           
    eOmeas_velocity_t           jnt_velocity;               /**< the velocity of the joint */          
    eOmeas_acceleration_t       jnt_acceleration;           /**< the acceleration of the joint */       
    eOmeas_torque_t             jnt_torque;                 /**< the torque of the joint when locally measured */
    eOenum08_t                  motionmonitorstatus;        /**< use eOmc_motionmonitorstatus_t. it is eomc_motionmonitorstatus_notmonitored unless the monitor is activated in jconfig.motionmonitormode */  
    eOenum08_t                  controlmodestatus;          /**< use eOmc_controlmode_t. */
} eOmc_joint_status_basic_t;    //EO_VERIFYsizeof(eOmc_joint_status_basic_t, 16);



/** @typedef    typedef struct eOmc_joint_status_ofpid_t
    @brief      eOmc_joint_status_ofpid_t contains the status of a PID. The variables reference, error, and output are defined as
                signed integers of 32 bits, but the must contain the relevant measurement units (eOmeas_position_t, eOmeas_velocity_t,
                eOmeas_torque_t, or eOmeas_current_t).
 **/
typedef struct                  // size is: 4+4+4+0 = 16
{
    int32_t                     positionreference;      /**< the reference of the position pid */
    int32_t                     torquereference;        /**< the reference of the torque pid */
    int32_t                     error;                  /**< the error of the pid */ 
    int32_t                     output;                 /**< the output of the pid */ 
} eOmc_joint_status_ofpid_t;    //EO_VERIFYsizeof(eOmc_joint_status_ofpid_t, 16);



/** @typedef    typedef struct eOmc_joint_status_t
    @brief      eOmc_joint_status_t contains the status of a joint. 
 **/
typedef struct                  // size is:  16+16+1+3 = 36
{
    eOmc_joint_status_basic_t   basic;                      /**< the basic status */
    eOmc_joint_status_ofpid_t   ofpid;                      /**< the pid status   */ 
    eOenum08_t                  interactionmodestatus;      /**< use values from eOmc_interactionmode_t */
    uint8_t                     chamaleon03[3];             /**< these bytes are available for the application for debug purposes */
} eOmc_joint_status_t;          //EO_VERIFYsizeof(eOmc_joint_status_t, 36);



typedef struct                  // size is 2+6 = 8
{
    eOmeas_torque_t             externallymeasuredtorque;   /**< the torque at the joint when externally measured or estimated */
    uint8_t                     filler06[6];
} eOmc_joint_inputs_t;          //EO_VERIFYsizeof(eOmc_joint_inputs_t, 8);



typedef struct                  // size is 16+12+1+1+2+0 = 32
{
    eOmc_calibrator_t           calibration;                /**< the calibrator to use */
    eOmc_setpoint_t             setpoint;                   /**< the setpoint of the joint */
    eObool_t                    stoptrajectory;             /**< it is an order to stop the current trajectory on the joint */
    eOenum08_t                  controlmode;                /**< use values from eOmc_controlmode_command_t */
    eOenum08_t                  interactionmode;            /**< use values from eOmc_interactionmode_t */
    uint8_t                     filler01;
} eOmc_joint_commands_t;        //EO_VERIFYsizeof(eOmc_joint_commands_t, 32);



typedef struct                  // size is 116+36+8+32+0 = 192
{
    eOmc_joint_config_t         config;                     /**< the configuration of the joint */
    eOmc_joint_status_t         status;                     /**< the status of the joint */
    eOmc_joint_inputs_t         inputs;                     /**< it contains all the values that a host can send to a joint as inputs */
    eOmc_joint_commands_t       cmmnds;                     /**< it contains all the commands that a host can send to a joint */
} eOmc_joint_t;                 EO_VERIFYsizeof(eOmc_joint_t, 252);


// -- the definition of a motor



/** @typedef    typedef uint8_t  eOmc_motorId_t
    @brief      eOmc_motorId_t contains the values required to identify a motor
 **/
typedef uint8_t  eOmc_motorId_t;



/** @typedef    typedef struct eOmc_motor_config_t
    @brief      eOmc_motor_config_t contains the values required to configure a motor
    @warning    This struct must be of fixed size and multiple of 4.
 **/
typedef struct                  // size is: 40+4+4+4+4+2+2+1+1+1+1+1 = 56
{
    eOmc_PID_t                  pidcurrent;                 /**< the pid for current control */
    int32_t                     gearboxratio;               /**< the gearbox reduction ration */
    int32_t                     rotorEncoderResolution;     /**< the rotorencoder resolution  */
    int32_t                     filler01;                   /**< reserved */
    eOmeas_velocity_t           maxvelocityofmotor;         /**< the maximum velocity in the motor */
    eOmeas_current_t            maxcurrentofmotor;          /**< the maximum current in the motor */
    uint16_t                    rotorIndexOffset;           /**< index offset for the rotor encoder*/
    uint8_t                     motorPoles;                 /**< number of poles of the motor */
    eObool_t                    hasHallSensor;              /**< true if the motor is equipped with hall effect sensors */
    eObool_t                    hasTempSensor;              /**< true if the motor is equipped with temperature sensors */
    eObool_t                    hasRotorEncoder;            /**< true if the motor is equipped with rotor encoder */
    eObool_t                    hasRotorEncoderIndex;       /**< true if the motor is equipped with rotor encoder */
    uint8_t                     rotorEncoderType;           /**< rotor encoder type */
    eOmeas_position_limits_t    limitsofrotor;              /**< rotor limits */
} eOmc_motor_config_t;          EO_VERIFYsizeof(eOmc_motor_config_t, 76);


/** @typedef    typedef struct eOmc_motor_status_t
    @brief      eOmc_motor_status_t contains the status of a motor
    @warning    This struct must be of fixed size and multiple of 4.
 **/
typedef struct                  // size is: 4+4+4+2+2+0 = 16
{
    eOmeas_position_t           mot_position;                   /**< the position of the motor */         
    eOmeas_velocity_t           mot_velocity;                   /**< the velocity of the motor */
    eOmeas_acceleration_t       mot_acceleration;               /**< the acceleration of the motor */ 
    eOmeas_current_t            mot_current;                    /**< the current of the motor */  
    eOmeas_temperature_t        mot_temperature;                /**< the temperature of the motor */
} eOmc_motor_status_basic_t;    //EO_VERIFYsizeof(eOmc_motor_status_basic_t, 16);


/** @typedef    typedef struct eOmc_motor_status_t
    @brief      eOmc_motor_status_t contains the status of a motor
    @warning    This struct must be of fixed size and multiple of 4.
 **/
typedef struct                  // size is: 16+4+0 = 16
{
    eOmc_motor_status_basic_t   basic;                  /**< the basic status of a motor */
    uint8_t                     filler04[4];            
} eOmc_motor_status_t;          EO_VERIFYsizeof(eOmc_motor_status_t, 20);



typedef struct                  // size is 76+20+0 = 76
{
    eOmc_motor_config_t         config;                     /**< the configuration of the motor */
    eOmc_motor_status_t         status;                     /**< the status of the motor */   
} eOmc_motor_t;                 EO_VERIFYsizeof(eOmc_motor_t, 96); 
 

// -- the definition of a controller containing a given number of joints and motors  

enum { eom_coupledjoints_maxnumber = 4 };

typedef eOq17_14_t eOmc_jointcouplingmatrix_t[eom_coupledjoints_maxnumber][eom_coupledjoints_maxnumber];



/** @typedef    typedef enum eOmc_stateofcontroller_t
    @brief      contains the possible states of the controller.
 **/
typedef enum
{
    eomc_stateofcontroller_config                       = 0,
    eomc_stateofcontroller_ready                        = 1,
    eomc_stateofcontroller_active                       = 2
} eOmc_stateofcontroller_t;
 

typedef struct                  // size is 4+4+64+0 = 72
{
    eOreltime_t                 durationofctrlloop;         /**< the duration of the control-loop in micro-sec. its default is 1000. so far it cannot be changed. */
    uint8_t                     filler04[4];   
    eOmc_jointcouplingmatrix_t  jointcoupling;  
} eOmc_controller_config_t;     //EO_VERIFYsizeof(eOmc_controller_config_t, 72); 


typedef struct                  // size is 1+1+1+1+2+2+0 = 8
{
    eOenum08_t                  stateofcontroller;          /**< it holds a value from enum eOmc_stateofcontroller_t */               
    uint8_t                     numofjoints;                /**< the number of joints */   
    uint8_t                     numofmotors;                /**< the number of motors */ 
    eObool_t                    alljomoinitted;             /**< it is eobool_true only when every joint and motor is initted */    
    eO16flags_t                 flagsinittedjoints;         /**< bit position 0 (1, 2, ..) keeps 1 if the joint 0 (1, 2, ..) is fully initted. */
    eO16flags_t                 flagsinittedmotors;         /**< bit position 0 (1, 2, ..) keeps 1 if the motor 0 (1, 2, ..) is fully initted. */ 
} eOmc_controller_status_t;     //EO_VERIFYsizeof(eOmc_controller_status_t, 8); 


// typedef struct                  // size is 1+7+0 = 8
// {
//     eOenum08_t                  go2stateofcontroller;       /**< it forces a transition towards the specified state. use a value from enum eOmc_stateofcontroller_t */
//     uint8_t                     filler07[7];                
// } eOmc_controller_commands_t;   //EO_VERIFYsizeof(eOmc_controller_commands_t, 8); 


typedef struct                  // size is 72+8+0 = 80
{
    eOmc_controller_config_t    config;                     /**< controller configuration */
    eOmc_controller_status_t    status;                     /**< controller status  */
} eOmc_controller_t;            //EO_VERIFYsizeof(eOmc_controller_t, 80); 





// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section




/** @}            
    end of group eo_motioncontrol  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





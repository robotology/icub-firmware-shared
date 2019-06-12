/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author:  Valentina Gaggero
 * email:   valentina.gaggero@iit.it
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

#ifndef _ICUBCANPROTO_ANALOGSENSORMESSAGES_H_
#define _ICUBCANPROTO_ANALOGSENSORMESSAGES_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       iCubCanProto_analogSensorMessages.h
    @brief      This header file gives the definitions of the iCub CAN protocols commands relating to analog sensor
    @author     valentina.gaggero@iit.it
    @date       12/03/2012
    @ingroup    iCubCanProtocol
**/

/** @ingroup    iCubCanProtocol
    @{
 **/




// - external dependencies --------------------------------------------------------------------------------------------
// empty-section


// - public #define  --------------------------------------------------------------------------------------------------

/***************************************************************************/
// command messages of class ICUBCANPROTO_CLASS_POLLING_ANALOGSENSOR 
/***************************************************************************/

#define ICUBCANPROTO_POL_AS_CMD__NONE                           0x00
#define ICUBCANPROTO_POL_AS_CMD__SET_IIR                        0x01    // select IIR filters parameters 
#define ICUBCANPROTO_POL_AS_CMD__SET_MATRIX_RC                  0x03    // Set SG to TF trasformation matrix 
#define ICUBCANPROTO_POL_AS_CMD__SET_CH_DAC                     0x04    // set DAC for channel x
#define ICUBCANPROTO_POL_AS_CMD__SELECT_ACTIVE_CH               0x05    // set active channels (activation mask) only active channels are transmitted
#define ICUBCANPROTO_POL_AS_CMD__CALIBRATE_OFFSET               0x06    // set the calibration offset
#define ICUBCANPROTO_POL_AS_CMD__SET_TXMODE                     0x07    // set continuous/on demand transmission mode
#define ICUBCANPROTO_POL_AS_CMD__SET_CANDATARATE                0x08    // set board CAN speed in milliseconds minimum, datarate 210ms  
#define ICUBCANPROTO_POL_AS_CMD__SAVE2EE                        0x09    // save Config to EE
#define ICUBCANPROTO_POL_AS_CMD__GET_MATRIX_RC                  0x0A    // Get TF trasformation matrix 
#define ICUBCANPROTO_POL_AS_CMD__GET_CH_DAC                     0x0B    // Get DAC for channel x
#define ICUBCANPROTO_POL_AS_CMD__GET_CH_ADC                     0x0C    // Get ADC for channel x
#define ICUBCANPROTO_POL_AS_CMD__FILTER_EN                      0x0D    // ENABLE/DISABLES FILTER
#define ICUBCANPROTO_POL_AS_CMD__MUX_EN                         0x0E    // ENABLE/DISABLES MUX
#define ICUBCANPROTO_POL_AS_CMD__MUX_NUM                        0x0F
#define ICUBCANPROTO_POL_AS_CMD__SET_RESOLUTION                 0x10    // set data resolution
#define ICUBCANPROTO_POL_AS_CMD__SET_MATRIX_G                   0x11    // set matrix gain
#define ICUBCANPROTO_POL_AS_CMD__GET_MATRIX_G                   0x12    // get matrix gain
#define ICUBCANPROTO_POL_AS_CMD__SET_CALIB_TARE                 0x13    // set/reset calibration tare
#define ICUBCANPROTO_POL_AS_CMD__GET_CALIB_TARE                 0x14    // get calibration tare
#define ICUBCANPROTO_POL_AS_CMD__SET_CURR_TARE                  0x15    // set/reset current tare
#define ICUBCANPROTO_POL_AS_CMD__GET_CURR_TARE                  0x16    // get current tare
#define ICUBCANPROTO_POL_AS_CMD__SET_FULL_SCALES                0x17    // set/reset current tare
#define ICUBCANPROTO_POL_AS_CMD__GET_FULL_SCALES                0x18    // get current tare
#define ICUBCANPROTO_POL_AS_CMD__SET_SERIAL_NO                  0x19    // set/reset current tare
#define ICUBCANPROTO_POL_AS_CMD__GET_SERIAL_NO                  0x1A    // get current tare
#define ICUBCANPROTO_POL_AS_CMD__GET_EEPROM_STATUS              0x1B    // get eeprom status (saved/not saved)
#define ICUBCANPROTO_POL_AS_CMD__GET_FW_VERSION                 0x1C    // get fw version and check can protocol compatibility
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_RESET                0x1D    // tbd
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_RANGE_OF_GAIN_GET    0x1E    // tbd    
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_RANGE_OF_OFFSET_GET  0x1F    // tbd
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_GAINOFFSET_GET       0x20    // tbd  
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_GAINOFFSET_SET       0x21    // tbd
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_OFFSET_AUTOCALIB     0x22    // tbd  
// RESERVED: { 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29 } for possible new generic commands for the amplifier
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_PGA308_CFG1_GET      0x2A    // tbd
#define ICUBCANPROTO_POL_AS_CMD__AMPLIFIER_PGA308_CFG1_SET      0x2B    // xxx  
// RESERVED: { 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31 } for possible new commands specific for extra config of PGA308 or for any new amplifier
#define ICUBCANPROTO_POL_AS_CMD__SET_BOARD_ADX                  0x32    // Set board CAN address (see also ICUBCANPROTO_POL_MC_CMD__SET_BOARD_ID = 0x32 = ICUBCANPROTO_BL_SET_CAN_ADDRESS)
#define ICUBCANPROTO_POL_AS_CMD__IMU_CONFIG_GET                 0x33    // tbd
#define ICUBCANPROTO_POL_AS_CMD__IMU_CONFIG_SET                 0x34    // tbd
#define ICUBCANPROTO_POL_AS_CMD__IMU_TRANSMIT                   0x35    // tbd
// RESERVED: { 0x36, 0x37 } for possible new IMU commands    
#define ICUBCANPROTO_POL_AS_CMD__THERMOMETER_CONFIG_GET         0x38    // tbd  
#define ICUBCANPROTO_POL_AS_CMD__THERMOMETER_CONFIG_SET         0x39    // tbd
#define ICUBCANPROTO_POL_AS_CMD__THERMOMETER_TRANSMIT           0x3A    // tbd
// RESERVED: { 0x3B, 0x3C } for possible new THERMOMETER commands 
#define ICUBCANPROTO_POL_AS_CMD__REGULATIONSET_SET              0x3D    // tbd
#define ICUBCANPROTO_POL_AS_CMD__REGULATIONSET_GET              0x3E    // tbd
// HOLE: [0x3F, ... , 0x4B]. there are 13 free values.
#define ICUBCANPROTO_POL_SK_CMD__TACT_SETUP                     0x4C    // obsolete. it was used to configure mtb. now are used commands 0x4D and 0x50. 
#define ICUBCANPROTO_POL_SK_CMD__SET_BRD_CFG                    0x4D    // used to configure skin data in mtb/mtb4 + its tx rate
#define ICUBCANPROTO_POL_SK_CMD__TACT_SETUP2                    0x4E    // obsolete
#define ICUBCANPROTO_POL_SK_CMD__ACC_GYRO_SETUP                 0x4F    // used by mtb to config acc-gyro + its tx rate (mtb4 still supports it, but please use ICUBCANPROTO_POL_AS_CMD__IMU_CONFIG_SET)
#define ICUBCANPROTO_POL_SK_CMD__SET_TRIANG_CFG                 0x50    // used to configure skin data in mtb/mtb4
#define ICUBCANPROTO_POL_AS_CMD__POS_CONFIG_GET                 0x51    // used to configure position reading in the psc board
#define ICUBCANPROTO_POL_AS_CMD__POS_CONFIG_SET                 0x52    // used to read back configuration of position reading in the psc board
#define ICUBCANPROTO_POL_AS_CMD__POS_TRANSMIT                   0x53    // used to start/stop position reading in the psc board with a specified frequency

// HOLE: [0x54, ... , 0xFD]. there are 170 free values.


#define ICUBCANPROTO_POL_AS_CMD_MAXNUM                          (ICUBCANPROTO_POL_SK_CMD__POS_POS_TRANSMIT + 1)


/***************************************************************************/
// command messages of class ICUBCANPROTO_CLASS_PERIODIC_ANALOGSENSOR 
/***************************************************************************/

#define ICUBCANPROTO_PER_AS_MSG__USERDEF                        0x0     // used by a specific board / application for userdef purposes (e.g., debug). dont use in a final system
#define ICUBCANPROTO_PER_AS_MSG__POS                            0x1     // used to broadcast position values in a general way. used by psc.

#define ICUBCANPROTO_PER_AS_MSG__UNCALIBFORCE_VECTOR_DEBUGMODE  0x8     // used by strain to transmit Force uncalib data when is enabled DebugMode (ICUBCANPROTO_POL_AS_CMD__SET_TXMODE with payload[1]=4)
#define ICUBCANPROTO_PER_AS_MSG__UNCALIBTORQUE_VECTOR_DEBUGMODE 0x9     // used by strain to transmit Force uncalib data when is enabled DebugMode (ICUBCANPROTO_POL_AS_CMD__SET_TXMODE with payload[1]=4)
#define ICUBCANPROTO_PER_AS_MSG__FORCE_VECTOR                   0xA     // Transmit Torque values t1 t1 t2 t2 t3 t3
#define ICUBCANPROTO_PER_AS_MSG__TORQUE_VECTOR                  0xB     // Transmit Force  values f1 f1 f2 f2 f3 f3
#define ICUBCANPROTO_PER_AS_MSG__HES0TO6                        0xC     // hall effect sensors from 0  to 6  *8bits
#define ICUBCANPROTO_PER_AS_MSG__HES7TO14                       0xD     // hall effect sensors from 7  to 14  *8bits
#define ICUBCANPROTO_PER_AS_MSG__THERMOMETER_MEASURE            0xE     // used by strain2 and mtb4 to transmit temperature         


#define ICUBCANPROTO_PER_AS_MSG_MAXNUM                          (16)



//the following msg are deprecated!!!

// hall effect sensors from 0 to 3*16bits
#define ICUBCANPROTO_PER_AS_MSG__HES0TO3           0xC // 010 SOURCE DEST 0xC h0 h0 h1 h1 h2 h2 h3 h3
// hall effect sensors from 4 to 7*16bits
#define ICUBCANPROTO_PER_AS_MSG__HES4TO7           0xD // 010 SOURCE DEST 0xC h4 h4 h5 h5 h6 h6 h7 h7
// hall effect sensors from 8 to 11*16bits
#define ICUBCANPROTO_PER_AS_MSG__HES8TO11          0xE // 010 SOURCE DEST 0xC h8 h8 h9 h9 h10 h10 h11 h11
// hall effect sensors from 12 to 14*16bits
#define ICUBCANPROTO_PER_AS_MSG__HES12TO14         0xF // 010 SOURCE DEST 0xC h12 h12 h13 h13 h14 h14 h15 h15

// hall effect sensors from 0 to 7*8bits
#define ICUBCANPROTO_PER_AS_MSG__HES0TO6           0xC // 010 SOURCE DEST 0xC h0 h1 h2 h3 h4 h5 h6 
// hall effect sensors from 8 to 14*8bits
#define ICUBCANPROTO_PER_AS_MSG__HES7TO14          0xD // 010 SOURCE DEST 0xD h7 h8 h9 h10 h11 h12 h13 h14 

// hall effect sensors from 0 to 4*12bits
#define ICUBCANPROTO_PER_AS_MSG__HES0TO4           0xC // 010 SOURCE DEST 0xC h0 h1 h2 h3 h4 
// hall effect sensors from 5 to 9*12bits
#define ICUBCANPROTO_PER_AS_MSG__HES5TO9           0xD // 010 SOURCE DEST 0xD h5 h6 h7 h8 h9 
// hall effect sensors from 10 to 14*12bits
#define ICUBCANPROTO_PER_AS_MSG__HES10TO14          0xE // 010 SOURCE DEST 0xD h10 h11 h12 h13 h14 








// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section


// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section


/** @} **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------








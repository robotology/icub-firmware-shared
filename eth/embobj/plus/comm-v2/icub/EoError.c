/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
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

/* @file       EoError.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       Nov 10 2014
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoError.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

// it contains a pair {value, string}
typedef struct
{
    uint32_t        value;
    const char*     string;
} eoerror_valuestring_t;


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const uint32_t s_eoerror_maxvalue_in_category[] =
{
    eoerror_value_SYS_numberof,
    eoerror_value_HW_numberof,
    eoerror_value_MC_numberof,
    eoerror_value_SK_numberof,
    eoerror_value_DEB_numberof,
    eoerror_value_CFG_numberof,
    eoerror_value_ETHMON_numberof,
    eoerror_category_InertialSensor,
    eoerror_category_AnalogSensor
};  EO_VERIFYsizeof(s_eoerror_maxvalue_in_category, eoerror_category_numberof*sizeof(const uint32_t))    

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------

const eOerror_code_t eoerror_code_dummy = EOERROR_CODE_DUMMY;
const eOerror_value_t eoerror_value_dummy = EOERROR_VALUE_DUMMY;


// - begin of: section for error strings 
// - fill strings in here in accordance with relevant enums 


const eoerror_valuestring_t eoerror_valuestrings_SYS[] =
{   // very important: fill table with order of eOerror_value_SYS_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_SYS_unspecified,             "SYS: unspecified code. It may be that EOtheErrorManager is still called with a NULL eOerrmanDescriptor_t* param"},  
    {eoerror_value_SYS_tobedecided,             "SYS: tbd code. We must decide how to manage this code. it it wip"},  
    {eoerror_value_SYS_memory_zerorequested,    "SYS: the EOtheMemoryPool is requested 0 bytes"},       
    {eoerror_value_SYS_memory_notinitialised,   "SYS: the EOtheMemoryPool is requested bytes but it is not yet initialised"},        
    {eoerror_value_SYS_memory_missing,          "SYS: the EOtheMemoryPool cannot give any more memory"},     
    {eoerror_value_SYS_mutex_timeout,           "SYS: a mutex has timed out unexpectedly"},     
    {eoerror_value_SYS_wrongparam,              "SYS: a function has a wrong parameter"},  
    {eoerror_value_SYS_wrongusage,              "SYS: a function is used in the wrong way"},  
    {eoerror_value_SYS_runtimeerror,            "SYS: detected a runtime error"},  
    {eoerror_value_SYS_runninghappily,          "SYS: the board is running happily. application state is in par16&0x000f : 0 -> just (re)started, 1 -> idle, 2 -> running."},  
    {eoerror_value_SYS_runninginfatalerrorstate,"SYS: the board is running in fatal error state. check previous errors to verify the reason"},
    {eoerror_value_SYS_ctrloop_execoverflowRX,  "SYS: the RX phase of the control loop has last more than wanted. In par16: RX execution time [usec]. In par64: latest previous execution times of TX, RX, DO, TX [usec]"},  
    {eoerror_value_SYS_ctrloop_execoverflowDO,  "SYS: the DO phase of the control loop has last more than wanted. In par16: DO execution time [usec]. In par64: latest previous execution times of RX, DO, TX, RX [usec]"},    
    {eoerror_value_SYS_ctrloop_execoverflowTX,  "SYS: the TX phase of the control loop has last more than wanted. In par16: TX execution time [usec]. In par64: num of tx can2 and can1 frames and latest previous execution times of TX, RX, DO [usec]"},  
    {eoerror_value_SYS_udptxfailure,            "SYS: a UDP packet could not be transmitted"},  
    {eoerror_value_SYS_ropparsingerror,         "SYS: there was a parsing error of a received ROP. In par16 there is the parsing error type expressed in eOparserResult_t"},  
    {eoerror_value_SYS_halerror,                "SYS: the HAL triggered an error. In par16 there is the relevant hal code"},  
    {eoerror_value_SYS_osalerror,               "SYS: the OSAL triggered an error. In par16 there is the relevant osal code"},  
    {eoerror_value_SYS_ipalerror,               "SYS: the IPAL triggered an error. In par16 there is the relevant ipal code"}, 
    {eoerror_value_SYS_dispatcherfifooverflow,  "SYS: the EOtheInfoDispatcher could not accept a eOmn_info_properties_t item inside its transmitting queue. In par16 there is the number of lost items."}, 
    {eoerror_value_SYS_configurator_udptxfailure,"SYS: the EOMtheEMSconfigurator could not tx a UDP packet with eom_emssocket_Transmit()."},
    {eoerror_value_SYS_runner_udptxfailure,     "SYS: the EOMtheEMSrunner could not tx a UDP packet with eom_emssocket_Transmit()."},
    {eoerror_value_SYS_runner_transceivererror, "SYS: the EOMtheEMSrunner could not either parse or form a UDP packet."},
    {eoerror_value_SYS_canservices_rxfifooverflow, "SYS: the HAL could not put a CAN frame in rx fifo. In par16 there is: on msb the rx fifo size, in lsb a code."},
    {eoerror_value_SYS_canservices_txfifooverflow, "SYS: EOtheCANservice could not add a CAN frame in tx fifo. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data."},
    {eoerror_value_SYS_canservices_txbusfailure, "SYS: EOtheCANservice could not tx frames on CAN bus. In par16 there is: on msb the size of txfifo, on lsb a code."},
    {eoerror_value_SYS_canservices_formingfailure, "SYS: EOtheCANservice could not form a can packet. In par16 there is class in msb and cmdid in lsb."},
    {eoerror_value_SYS_canservices_parsingfailure, "SYS: EOtheCANservice could not parse a rx can frame. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_SYS_canservices_txfifoputisok, "SYS: EOtheCANservice have just put a can frame in tx fifo. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data."},
	{eoerror_value_SYS_canservices_genericerror, "SYS: EOtheCANservice has detected a generic error. In par16 there is a code"},
    {eoerror_value_SYS_ctrloop_rxphaseaverage, 	 "SYS: RX phase of the control loop average execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_dophaseaverage, 	 "SYS: DO phase of the control loop average execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_txphaseaverage, 	 "SYS: TX phase of the control loop average execution time in usec, monitored for the last 10secs, stored in par16 "},
    {eoerror_value_SYS_ctrloop_rxphasemax, 	     "SYS: RX phase of the control loop max execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_dophasemax, 	     "SYS: DO phase of the control loop max execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_txphasemax, 	     "SYS: TX phase of the control loop max execution time in usec, monitored for the last 10secs, stored in par16 "},
    {eoerror_value_SYS_ctrloop_rxphasemin, 	     "SYS: RX phase of the control loop min execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_dophasemin, 	     "SYS: DO phase of the control loop min execution time in usec, monitored for the last 10secs, stored in par16 "},
	{eoerror_value_SYS_ctrloop_txphasemin, 	     "SYS: TX phase of the control loop min execution time in usec, monitored for the last 10secs, stored in par16 "},
    {eoerror_value_SYS_proxy_forward_ok,  "SYS: the proxy has forwareded a request."},
    {eoerror_value_SYS_proxy_forward_fails,      "SYS: the proxy fails to forward a request. in par64 there are the [sign|id32], in par16 [capacity|size] of proxy list "},
    {eoerror_value_SYS_proxy_forward_callback_fails,  "SYS: the update callback called by a proxy forward fails to operate properly."},
    {eoerror_value_SYS_proxy_reply_ok, "SYS: the proxy has received a reply. "},
    {eoerror_value_SYS_proxy_reply_fails,        "SYS: the proxy fails to forward a request. in par64 there are the [sign|id32], in par16 [capacity|size] of proxy list. if par16 is 0, then the rop could not be tx."},
    {eoerror_value_SYS_proxy_ropdes_notfound,   "SYS: the proxy fails to find an internally stored rop. maybe forward request was expired. in par64 there are the [0|id32]"},
    {eoerror_value_SYS_canservices_canprint,    "SYS: CAN print message received. The message is stored inside par64. In par16 is stored the size of the message for the single frame"},
    {eoerror_value_SYS_canservices_rxmaisbug,   "SYS: EOtheCANservice received a canframe caused by MAISbug. The message is stored inside par64. In par16 is stored the size of the message for the single frame"},
    {eoerror_value_SYS_canservices_rxfromwrongboard, "SYS: EOtheCANservice received a canframe from a wrong board. The message is stored inside par64. In par16 is stored the size of the message for the single frame"},
    {eoerror_value_SYS_transceiver_rxseqnumber_error, "SYS: the board has detected a hole in rx ropframe sequence number. In par64 there is the expected number. The received is obtained by adding par64 with par16 treated as a int16_t"},    
    {eoerror_value_SYS_canservices_boards_missing, "SYS: the can discovery service detected that some boards are missing. Check with canLoader. In par64 MS 32 bits there is the bit mask of the searched, in LS 32 bits the mask of the found ones."},    
    {eoerror_value_SYS_canservices_boards_searched, "SYS: the can discovery service searched the following boards on the specified CAN bus. In par64 there are the searched addresses represented as nibbles."},    
    {eoerror_value_SYS_canservices_boards_found, "SYS: the can discovery service found the following boards on the specified CAN bus. In par64 there are the found addresses represented as nibbles."},
    {eoerror_value_SYS_transceiver_rxseqnumber_restarted, "SYS: the board has detected a restart of the remote transceiver because the rx ropframe sequence number is 1. In par64 there is the expected number."},
    {eoerror_value_SYS_canservices_board_detected, "SYS: the can discovery service found the board on the specified CAN bus / adr. In par64 there is the type, fw and prot version."},
    {eoerror_value_SYS_canservices_board_wrongprotversion, "SYS: the board on the specified CAN bus / adr has incompatible fw protocol version. In par16 there is required prot, in par64 there is detected type, fw and prot version."},
    {eoerror_value_SYS_canservices_board_notfound, "SYS: the board on the specified CAN bus / adr was not found during the discovery phase. In par16 there is the board type"},
    {eoerror_value_SYS_transceiver_rxinvalidframe_error, "SYS: the board has detected an invalid ropframe in reception."},
    {eoerror_value_SYS_canservices_boards_lostcontact, "SYS: a service has detected that some CAN boards are not broacasting anymore. In par16 the type of boards, in par64 LS 32 bits the bit mask of lost board (CAN1 in MS 16 bits and CAN2 in LS 16 bits)"},
    {eoerror_value_SYS_canservices_boards_retrievedcontact, "SYS: a service has recovered some CAN boards that were not broacasting anymore. In par16 the type of boards)"},
    {eoerror_value_SYS_bootstrapping, "SYS: the board is bootstrapping"}
};  EO_VERIFYsizeof(eoerror_valuestrings_SYS, eoerror_value_SYS_numberof*sizeof(const eoerror_valuestring_t)) 


const eoerror_valuestring_t eoerror_valuestrings_HW[] =
{   // very important: fill table with order of eOerror_value_HW_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_HW_first,                "HW: first error message of category hw"},       
    {eoerror_value_HW_second,               "HW: second error message of category hw"},
    {eoerror_value_HW_strain_saturation,    "HW: strain values saturation. In p16 there is the channel involved, in p64 there are the saturation counts for the last sec of both types (lower: LS32bits, upper MS32bits"},
};  EO_VERIFYsizeof(eoerror_valuestrings_HW, eoerror_value_HW_numberof*sizeof(const eoerror_valuestring_t)) 


const eoerror_valuestring_t eoerror_valuestrings_MC[] =
{   // very important: fill table with order of eOerror_value_MC_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_MC_motor_external_fault,  "MC: exernal fault button pressed."},
    {eoerror_value_MC_motor_overcurrent,     "MC: overcurrent. The motor has been turned off to prevent it from being damaged by an impulsive spike of current. par16 = ID of joint."},
    {eoerror_value_MC_motor_i2t_limit,       "MC: i2t limit exceeded. The motor has been turned off to prevent it from being damaged by overheating due to a continuous high current. par16 = ID of joint."},
    {eoerror_value_MC_motor_hallsensors,     "MC: 2FOC hall sensors fault. Invalid sequence in motor Hall effect sensors, please check motor hall cable connections. par16 = ID of joint."},
    {eoerror_value_MC_motor_can_invalid_prot,"MC: 2FOC CAN invalid protocol. The EMS and 2FOC firmware versions are incompatible, please update. par16 = ID of joint."},
    {eoerror_value_MC_motor_can_generic,     "MC: 2FOC CAN generic error. Errors happened in the CAN bus between the EMS and the 2FOC board. par16 = ID of joint."},
    {eoerror_value_MC_motor_can_no_answer,   "MC: 2FOC CAN no answer. The communication between the EMS and the 2FOC board has been lost for more than 50 ms. par16 = ID of joint."},
    {eoerror_value_MC_axis_torque_sens,      "MC: torque sensor timeout. The joint is in a compliant interaction mode or torque control mode, and data from torque sensor have been unavailable for more than 100 ms. par16 = ID of joint."},
    {eoerror_value_MC_aea_abs_enc_invalid,   "MC: AEA encoder invalid data. Hardware problem in the magnetic position sensor of the joint caused invalid position readings. par16 = AEA port (msb) and ID of joint (lsb)."},
    {eoerror_value_MC_aea_abs_enc_timeout,   "MC: AEA encoder timeout. No answer from the magnetic position sensor of the joint (cable broken?). par16 = AEA port (msb) and ID of joint (lsb)."},
    {eoerror_value_MC_aea_abs_enc_spikes,    "MC: AEA encoder has spikes. There is impulsive noise in the measures of the magnetic position sensor of the joint. par16 = AEA port (msb) and ID of joint (lsb)."},
    {eoerror_value_MC_motor_qencoder_dirty,  "MC: 2FOC quadrature encoder dirty. The number of thicks in a complete revolution of the motor was lower than expected, the optical disks need to be cleaned. In par64 0xFF is the mask of raw encoder value. par16 = ID of joint."},
    {eoerror_value_MC_motor_qencoder_index,  "MC: 2FOC quadrature encoder index broken. The reference special thick was not detected during a complete revolution of the motor, please check motor encoder cables. In par64 0xFF is the mask of raw encoder value. par16 = ID of joint."},
    {eoerror_value_MC_motor_qencoder_phase,  "MC: 2FOC quadrature encoder phase broken. The motor encoder is not counting even if the motor is moving, please check motor encoder cables. In par64 0xFF is the mask of raw encoder value. par16 = ID of joint."},
    {eoerror_value_MC_generic_error,         "MC: generic motor error (see 64 bit mask parameter)."},
    {eoerror_value_MC_motor_wrong_state,     "MC: 2FOC wrong state. The 2FOC motor controller is in a control state different from required by the EMS. In par64 0xF0 is the mask of requested state, 0x0F is the mask of actual state. par16 = ID of joint."},
    {eoerror_value_MC_joint_hard_limit,      "MC: hard limit reached. The joint position is outside its hardware boundaries. par16 = ID of joint."}
    
};  EO_VERIFYsizeof(eoerror_valuestrings_MC, eoerror_value_MC_numberof*sizeof(const eoerror_valuestring_t)) 



const eoerror_valuestring_t eoerror_valuestrings_SK[] =
{   // very important: fill table with order of eOerror_value_MC_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_SYS_unspecified,             "SK: unspecified code."},
    {eoerror_value_SK_arrayofcandataoverflow,   "SK: cannot put rx can frames into arrayofcandata, thus some skin readings will be lost. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_SK_onoroff,                  "SK: the skin transmission has been switched on or off. In par16 there's the corresponding code (0: OFF, 1: ON)"},
    {eoerror_value_SK_unexpecteddata,           "SK: the board has received a message from the skin even if it should be in a silenced modality. In par16 there's the actual state of the EMS board (0: CFG, 1: RUN)"},
    {eoerror_value_SK_obsoletecommand,          "SK: the board has received an obsolete command of type eosk_sigmode_signal_oldway. use eosk_sigmode_signal instead."}
};  EO_VERIFYsizeof(eoerror_valuestrings_SK, eoerror_value_SK_numberof*sizeof(const eoerror_valuestring_t)) 


const eoerror_valuestring_t eoerror_valuestrings_DEB[] =
{   // very important: fill table with order of eOerror_value_DEB_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_DEB_tag00,               "DEBUG: tag00"},
    {eoerror_value_DEB_tag01,               "DEBUG: tag01"},
    {eoerror_value_DEB_tag02,               "DEBUG: tag02"},
    {eoerror_value_DEB_tag03,               "DEBUG: tag03"},
    {eoerror_value_DEB_tag04,               "DEBUG: tag04"},
    {eoerror_value_DEB_tag05,               "DEBUG: tag05"},
    {eoerror_value_DEB_tag06,               "DEBUG: tag06"},
    {eoerror_value_DEB_tag07,               "DEBUG: tag07"}, 
    {eoerror_value_DEB_hwfault1,            "DEBUG: hw fault1 detected: in par16 there are controlmodestatus in lsb and frame[1] in msb"},
    {eoerror_value_DEB_hwfault2,            "DEBUG: hw fault2 detected: in par16 there are controlmodestatus in lsb and frame[1] in msb"},
    {eoerror_value_DEB_hwfault3,            "DEBUG: hw fault3 detected: in par16 there are controlmodestatus in lsb and frame[1] in msb"}
};  EO_VERIFYsizeof(eoerror_valuestrings_DEB, eoerror_value_DEB_numberof*sizeof(const eoerror_valuestring_t)) 


const eoerror_valuestring_t eoerror_valuestrings_CFG[] =
{   // very important: fill table with order of eOerror_value_CFG_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_CFG_candiscovery_started, "CFG: CANdiscovery started the search for boards. In p16: can2 map in 0xffff. In p64: can1 map in 0xffff000000000000, board type in 0x0000ff0000000000, req prot in 0x000000ffff000000, req fw in 0x0000000000ffffff. If prot and fw are zero, search on bus is not done"},
    {eoerror_value_CFG_candiscovery_ok, "CFG: CANdiscovery successful. In p16: fake search in 0xf000, number of boards in 0x00ff. In p64: search time [ms] in 0xffff000000000000, board type in 0x0000ff0000000000, req prot in 0x000000ffff000000, req fw in 0x0000000000ffffff."},
    {eoerror_value_CFG_candiscovery_detectedboard, "CFG: CANdiscovery has detected this board. In p16: board address in 0x000f. In p64: search time [ms] in 0xffff000000000000, board type in 0x0000ff0000000000, prot in 0x000000ffff000000, fw in 0x0000000000ffffff."},       
    {eoerror_value_CFG_candiscovery_boardsmissing, "CFG: CANdiscovery cannot find some boards. In p16: target board type in 0xff00 and number of missing in 0x000f. In p64: search time [ms] in 0xffff000000000000, mask of missing addresses in 0x000000000000ffff"},
    {eoerror_value_CFG_candiscovery_boardsinvalid, "CFG: CANdiscovery detected invalid boards. In p16: target board type in 0xff00 and number of invalid in 0x000f. In p64: each nibble contains 0x0 if ok, mask 0x1 if wrong type, mask 0x2 if wrong fw, mask 0x4 if wrong prot"},
    {eoerror_value_CFG_skin_ok, "CFG: EOtheSKIN can be correctly configured. num of patches in p16, can mapping in p64 upper 32 bits, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_skin_failed_toomanyboards, "CFG: EOtheSKIN cannot be configured. too many boards. In p16: number of requested boards in 0xff00, max number in 0x00ff. In p64: mask of requested boards in 0x00000000ffff0000 (can2) and 0x000000000000ffff (can1)."},
    {eoerror_value_CFG_skin_failed_candiscovery, "CFG: EOtheSKIN cannot be configured. can discovery fails. num of patches in p16. in p64 from lsb to msb masks of: missing can1, can2, found but incompatible can1, can2"},
    {eoerror_value_CFG_strain_ok, "CFG: EOtheSTRAIN can be correctly configured. can address in p16, prot and vers in p64 lower 32 bits. Byte 5 of p64: 0x01 is strain, 0x02 is strain2"},
    {eoerror_value_CFG_strain_failed_candiscovery, "CFG: EOtheSTRAIN cannot be configured. can discovery fails. can address in p16, prot and vers in p64 lower 32 bits. Byte 5 of p64: 0x01 is strain, 0x02 is strain2"},
    {eoerror_value_CFG_mais_ok, "CFG: EOtheMAIS can be correctly configured. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_mais_failed_candiscovery, "CFG: EOtheMAIS cannot be configured. can discovery fails. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_mais_failed_verify_because_active, "CFG: EOtheMAIS cannot be configured. it was already activated with different configuration."}, 
    {eoerror_value_CFG_mc_foc_ok, "CFG: EOtheMotionController can correctly configure 2foc-based motion. more info will follow"},
    {eoerror_value_CFG_mc_foc_failed_candiscovery_of_foc, "CFG: EOtheMotionController cannot be configured. can discovery of 2foc boards fails. see CANdiscovery messages for more details"},
    {eoerror_value_CFG_mc_foc_failed_encoders_verify, "CFG: EOtheMotionController cannot be configured. verification of encoders fails. see other messages for more details"},    
    {eoerror_value_CFG_mc_mc4_ok, "CFG: EOtheMotionController can correctly configure mc4-based motion. more info will follow"},
    {eoerror_value_CFG_mc_mc4_failed_candiscovery_of_mc4, "CFG: EOtheMotionController cannot be configured. can discovery of mc4 boards fails. see CANdiscovery messages for more details"},
    {eoerror_value_CFG_mc_mc4_failed_mais_verify, "CFG: EOtheMotionController cannot be configured. verification of mais fails. see other messages for more details"},
    {eoerror_value_CFG_mc_mc4plus_ok, "CFG: EOtheMotionController can correctly configure mc4plus-based motion. more info will follow"},
    {eoerror_value_CFG_mc_mc4plus_failed_encoders_verify, "CFG: EOtheMotionController cannot be configured. verification of encoder fails. see other messages for more details"},

    {eoerror_value_CFG_encoders_ok, "CFG: EOtheEncoderReader can be correctly configured. p16&0xf000: number of joint; primary encs: failure mask in p16&0x000f and errorcodes in p64&0x0000ffff; secondary encs: failure mask in p16&0x00f0 and errorcodes in p64&0xffff0000"},
    {eoerror_value_CFG_encoders_failed_verify, "CFG: EOtheEncodeerReader cannot be configured: verification fails. p16&0xf000: number of joint; primary encs: failure mask in p16&0x000f and errorcodes in p64&0x0000ffff; secondary encs: failure mask in p16&0x00f0 and errorcodes in p64&0xffff0000"},        

    {eoerror_value_CFG_inertials_ok, "CFG: EOtheInertial can be correctly configured. tbd"},
    {eoerror_value_CFG_inertials_failed_toomanyboards, "CFG: EOtheInertial cannot be configured. too many boards. In p16: number of requested boards in 0xff00, max number in 0x00ff. In p64: mask of requested boards in 0x00000000ffff0000 (can2) and 0x000000000000ffff (can1)."},
    {eoerror_value_CFG_inertials_failed_candiscovery, "CFG: EOtheInertial cannot be configured. can discovery fails. in p64 from lsb to msb masks of: missing can1, can2, found but incompatible can1, can2"},
    
    {eoerror_value_CFG_comm_cannotloadaregularrop, "CFG: cannot load a regular rop. tbd"},

    {eoerror_value_CFG_mc_mc4plusmais_ok, "CFG: EOtheMotionController can correctly configure mc4plusmais-based motion. more info will follow"},
    {eoerror_value_CFG_mc_mc4plusmais_failed_encoders_verify, "CFG: EOtheMotionController cannot be configured. verification of encoder fails for mc4plusmais. see other messages for more details"},
    {eoerror_value_CFG_mc_mc4plusmais_failed_candiscovery_of_mais, "CFG: EOtheMotionController cannot be configured. verification of mais for mc4plusmais fails. see other messages for more details"},
    
    {eoerror_value_CFG_services_not_verified_yet, "CFG: EOtheServices has not yet finished verifying the required services. restart robotInterface."},
    {eoerror_value_CFG_mc_not_verified_yet, "CFG: EOtheMotionController service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_strain_not_verified_yet, "CFG: EOtheSTRAIN service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_mais_not_verified_yet, "CFG: EOtheMAIS service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_skin_not_verified_yet, "CFG: EOtheSKIN service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_inertials_not_verified_yet, "CFG: EOtheInertials service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_inertials3_not_verified_yet, "CFG: EOtheInertials3 service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_encoders_not_verified_yet, "CFG: EOtheEncoderReader service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_mc_using_onboard_config, "CFG: EOtheMotionController service is using the local default configuration based on its IP address."},
    {eoerror_value_CFG_strain_using_onboard_config, "CFG: EOtheSTRAIN service is using the local default configuration based on its IP address."},
    {eoerror_value_CFG_mais_using_onboard_config, "CFG: EOtheMAIS service is using the local default configuration based on its IP address."},
    {eoerror_value_CFG_inertials_using_onboard_config, "CFG: EOtheInertials service is using the local default configuration based on its IP address."},
    {eoerror_value_CFG_inertials3_using_onboard_config, "CFG: EOtheInertials3 service is using the local default configuration based on its IP address."},
    {eoerror_value_CFG_skin_using_onboard_config, "CFG: EOtheSKIN service is using the local default configuration based on its IP address."},
    
    {eoerror_value_CFG_inertials_failed_unsupportedsensor, "CFG: EOtheInertial cannot be configured. some sensors are not supported. in p16 their number."},    
    {eoerror_value_CFG_inertials_changed_requestedrate, "CFG: EOtheInertial has changed the requested rate. in p16 the requested (MSB) and the assigned (LSB)."},

    {eoerror_value_CFG_inertials3_failed_unsupportedsensor, "CFG: EOtheInertial3 cannot be configured. some sensors are not supported. in p16 their number."},    
    {eoerror_value_CFG_inertials3_changed_requestedrate, "CFG: EOtheInertial3 has changed the requested rate. in p16 the requested (MSB) and the assigned (LSB)."},
    
    {eoerror_value_CFG_inertials3_ok, "CFG: EOtheInertial3 can be correctly configured. tbd"},
    {eoerror_value_CFG_inertials3_failed_toomanyboards, "CFG: EOtheInertial3 cannot be configured. too many boards. In p16: number of requested boards in 0xff00, max number in 0x00ff. In p64: mask of requested boards in 0x00000000ffff0000 (can2) and 0x000000000000ffff (can1)."},
    {eoerror_value_CFG_inertials3_failed_candiscovery, "CFG: EOtheInertial3 cannot be configured. can discovery fails. in p64 from lsb to msb masks of: missing can1, can2, found but incompatible can1, can2"},
    {eoerror_value_CFG_inertials3_failed_generic, "CFG: EOtheInertial3 cannot be configured. generic error. more details in p16 and p64 and in source code"},
    
    {eoerror_value_CFG_temperatures_not_verified_yet, "CFG: EOtheTemperatures service was not verified yet, thus it cannot start."},
    {eoerror_value_CFG_temperatures_ok, "CFG: EOtheTemperatures can be correctly configured. tbd"},
    {eoerror_value_CFG_temperatures_failed_toomanyboards, "CFG: EOtheTemperatures cannot be configured. too many boards. In p16: number of requested boards in 0xff00, max number in 0x00ff. In p64: mask of requested boards in 0x00000000ffff0000 (can2) and 0x000000000000ffff (can1)."},
    {eoerror_value_CFG_temperatures_failed_candiscovery, "CFG: EOtheTemperatures cannot be configured. can discovery fails. in p64 from lsb to msb masks of: missing can1, can2, found but incompatible can1, can2"},
    {eoerror_value_CFG_temperatures_failed_generic, "CFG: EOtheTemperatures cannot be configured. generic error. more details in p16 and p64 and in source code"},
    {eoerror_value_CFG_temperatures_changed_requestedrate, "CFG: EOtheTemperatures has changed the requested rate. in p16 the requested (MSB) and the assigned (LSB)."},
    {eoerror_value_CFG_temperatures_using_onboard_config, "CFG: EOtheTemperatures service is using the local default configuration based on its IP address."},
    
    {eoerror_value_CFG_psc_ok, "CFG: EOthePSC can be correctly configured. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_psc_failed_candiscovery, "CFG: EOthePSC cannot be configured. can discovery fails. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_psc_failed_verify_because_active, "CFG: EOthePSC cannot be configured. it was already activated with different configuration."},
    {eoerror_value_CFG_psc_not_verified_yet, "CFG: EOthePSC service was not verified yet, thus it cannot start."},  
    {eoerror_value_CFG_psc_using_onboard_config, "CFG: EOthePSC service is using the local default configuration based on its IP address."},    
    {eoerror_value_CFG_psc_changed_requestedrate, "CFG: EOthePSC has changed the requested rate. in p16 the requested (MSB) and the assigned (LSB)."},
    
    {eoerror_value_CFG_mc_mc2pluspsc_ok, "CFG: EOtheMotionController can correctly configure mc2pluspsc-based motion. more info will follow"},
    {eoerror_value_CFG_mc_mc2pluspsc_failed_encoders_verify, "CFG: EOtheMotionController cannot be configured. verification of encoder fails for mc2pluspsc. see other messages for more details"},
    {eoerror_value_CFG_mc_mc2pluspsc_failed_candiscovery_of_pscs, "CFG: EOtheMotionController cannot be configured. verification of pscs for mc2pluspsc fails. see other messages for more details"},
    
    {eoerror_value_CFG_inertials_failed_notsupported, "CFG: EOtheInertials cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_inertials3_failed_notsupported, "CFG: EOtheInertials3 cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_temperatures_failed_notsupported, "CFG: EOtheTemperatures cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_mais_failed_notsupported, "CFG: EOtheMAIS cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_strain_failed_notsupported, "CFG: EOtheSTRAIN cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_skin_failed_notsupported, "CFG: EOtheSKIN cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_psc_failed_notsupported, "CFG: EOthePSC cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_mc_failed_notsupported, "CFG: EOtheMotionController cannot be configured. This board does not support this service."},
    {eoerror_value_CFG_encoders_failed_notsupported, "CFG: EOtheEncoderReader cannot be configured. This board does not support this service."},
    
    {eoerror_value_CFG_pos_ok, "CFG: EOthePOS can be correctly configured. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_pos_failed_candiscovery, "CFG: EOthePOS cannot be configured. can discovery fails. can address in p16, prot and vers in p64 lower 32 bits"},
    {eoerror_value_CFG_pos_failed_verify_because_active, "CFG: EOthePOS cannot be configured. it was already activated with different configuration."},
    {eoerror_value_CFG_pos_not_verified_yet, "CFG: EOthePOS service was not verified yet, thus it cannot start."},  
    {eoerror_value_CFG_pos_using_onboard_config, "CFG: EOthePOS service is using the local default configuration based on its IP address."},    
    {eoerror_value_CFG_pos_changed_requestedrate, "CFG: EOthePOS has changed the requested rate. in p16 the requested (MSB) and the assigned (LSB)."},
    {eoerror_value_CFG_pos_failed_notsupported, "CFG: EOthePOS cannot be configured. This board does not support this service."},

    {eoerror_value_CFG_mc_mc4plusfaps_ok, "CFG: EOtheMotionController can correctly configure mc4plusfaps-based motion. more info will follow"},
    {eoerror_value_CFG_mc_mc4plusfaps_failed_encoders_verify, "CFG: EOtheMotionController cannot be configured. verification of encoder fails for mc4plusfaps. see other messages for more details"},
    {eoerror_value_CFG_mc_mc4plusfaps_failed_candiscovery_of_pmc, "CFG: EOtheMotionController cannot be configured. verification of pos for mc4plusfaps fails. see other messages for more details"},
           
};  EO_VERIFYsizeof(eoerror_valuestrings_CFG, eoerror_value_CFG_numberof*sizeof(const eoerror_valuestring_t))



const eoerror_valuestring_t eoerror_valuestrings_ETHMON[] =
{   // very important: fill table with order of eOerror_value_ETHMON_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_ETHMON_link_goes_up,     "ETH monitor: link goes up in port specified by par16 (0 = ETH input (P2/P13/J4) , 1 = ETH output (P3/P12/J5) , 2 = internal). Application state is in most significant nibble of par64: 0 -> N/A, 1 -> idle, 3 -> running."},        
    {eoerror_value_ETHMON_link_goes_down,   "ETH monitor: link goes down in port specified by par16 (0 = ETH input (P2/P13/J4) , 1 = ETH output (P3/P12/J5) , 2 = internal).  Application state is most significant nibble of par64: 0 -> N/A, 1 -> idle, 3 -> running."},
    {eoerror_value_ETHMON_error_rxcrc,      "ETH monitor: detected RX CRC error in port specified by par16 (0 = ETH input (P2/P13/J4) , 1 = ETH output (P3/P12/J5) , 2 = internal).  Application state is in most significant nibble of par64: 0 -> N/A, 1 -> idle, 3 -> running. Number of errors in par64&0xffffffff"},
    {eoerror_value_ETHMON_txseqnumbermissing, "ETH monitor: the board low level ETH detected a missing ropframe w/ expected sequence number in par64 and number of detected in par16"},
    {eoerror_value_ETHMON_juststarted,      "ETH monitor: just started."},
    {eoerror_value_ETHMON_justverified, "ETH monitor: just verified, no news"}
};  EO_VERIFYsizeof(eoerror_valuestrings_ETHMON, eoerror_value_ETHMON_numberof*sizeof(const eoerror_valuestring_t)) 

const eoerror_valuestring_t eoerror_valuestrings_IS[] =
{   // very important: fill table with order of eOerror_value_IS_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_IS_arrayofinertialdataoverflow, "IS: cannot store rx can frames of inertial data, thus some inertial readings will be lost. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_IS_unknownsensor, "IS: unknown sensor"}
};  EO_VERIFYsizeof(eoerror_valuestrings_IS, eoerror_value_IS_numberof*sizeof(const eoerror_valuestring_t)) 

const eoerror_valuestring_t eoerror_valuestrings_AS[] =
{   // very important: fill table with order of eOerror_value_AS_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_AS_arrayoftemperaturedataoverflow, "AS: cannot store rx can frames of tempeature data, thus some readings will be lost. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_AS_unknownsensor, "AS: unknown sensor"}
};  EO_VERIFYsizeof(eoerror_valuestrings_AS, eoerror_value_AS_numberof*sizeof(const eoerror_valuestring_t)) 


const eoerror_valuestring_t * const eoerror_valuestrings[] = 
{   // very important: fill table with order of eOerror_category_t: pos 0 is eoerror_category_EthBoardSystem etc.
    //                 in case of holes use: NULL,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_SYS,   
    (const eoerror_valuestring_t *)&eoerror_valuestrings_HW,        
    (const eoerror_valuestring_t *)&eoerror_valuestrings_MC,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_SK,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_DEB,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_CFG,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_ETHMON,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_IS,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_AS
};  EO_VERIFYsizeof(eoerror_valuestrings, eoerror_category_numberof*sizeof(const eoerror_valuestring_t *))  


// - end of: section for error strings 


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern eOerror_code_t eoerror_code_get(eOerror_category_t cat, eOerror_value_t val)
{
    eOerror_code_t ec = 0;
    
    uint32_t c = (uint32_t)cat;
    
    if(c > eoerror_category_numberof)
    {
        return(eoerror_code_dummy);
    }
    
    if(val > s_eoerror_maxvalue_in_category[c])
    {
        return(eoerror_code_dummy);
    }
     
    ec = EOERRORCODE(c, val);

    return(ec);
}


extern eOerror_category_t eoerror_code2category(eOerror_code_t code)
{
    uint32_t cat = (code & EOERROR_CATEG_MASK) >> EOERROR_CATEG_SHIFT;
        
    if(cat > eoerror_category_numberof)
    {
        return(eoerror_category_dummy);
    }
    
    return((eOerror_category_t)cat);    
}


extern eOerror_value_t eoerror_code2value(eOerror_code_t code)
{  
    eOerror_category_t cat = eoerror_code2category(code);
    uint32_t val = code & EOERROR_VALUE_MASK;
    
    if(eoerror_category_dummy == cat)
    {
        return(eoerror_value_dummy);
    }
       
    if(val > s_eoerror_maxvalue_in_category[cat])
    {
        return(eoerror_code_dummy);
    }
        
    return(val);    
}


extern const char* eoerror_code2string(eOerror_code_t code)
{
    static const char interror_codeunrecognised[] = "eoerror_INTERNALERROR__codeunrecognised";
    static const char interror_categoryisempty[] = "eoerror_INTERNALERROR__categoryisempty";
    static const char interror_valueisempty[] = "eoerror_INTERNALERROR__valueisempty";
    static const char interror_stringatwrongplace[] = "eoerror_INTERNALERROR__stringatwrongplace";
       
    eOerror_category_t cat = eoerror_code2category(code);
    eOerror_value_t val = eoerror_code2value(code);
    
    if((eoerror_category_dummy == cat) || (eoerror_value_dummy == val))
    {
        return(interror_codeunrecognised);
    }
    
    if(NULL == eoerror_valuestrings[cat])
    {
        return(interror_categoryisempty);
    }

    if(NULL == eoerror_valuestrings[cat][val].string)
    {
        return(interror_valueisempty);
    }

    if(val != eoerror_valuestrings[cat][val].value)
    {
        return(interror_stringatwrongplace);
    }
    
    return(eoerror_valuestrings[cat][val].string);
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




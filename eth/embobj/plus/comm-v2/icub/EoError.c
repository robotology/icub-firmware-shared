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
    eoerror_value_CFG_numberof
};  EO_VERIFYsizeof(s_eoerror_maxvalue_in_category, eoerror_category_numberof*sizeof(const uint32_t));    

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
    {eoerror_value_SYS_runninghappily,          "SYS: the board is running happily"},  
    {eoerror_value_SYS_runninginfatalerrorstate,"SYS: the board is running in fatal error state. check previous errors to verify the reason"},
    {eoerror_value_SYS_ctrloop_execoverflowRX,  "SYS: the RX phase of the control loop has last more than wanted. In par16 there is the total execution time in usec"},  
    {eoerror_value_SYS_ctrloop_execoverflowDO,  "SYS: the DO phase of the control loop has last more than wanted. In par16 there is the total execution time in usec"},    
    {eoerror_value_SYS_ctrloop_execoverflowTX,  "SYS: the TX phase of the control loop has last more than wanted. In par16 there is the total execution time in usec"},  
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
    {eoerror_value_SYS_canservices_txfifooverflow, "SYS: the EOappCanSP could not add a CAN frame in tx fifo. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data."},
    {eoerror_value_SYS_canservices_txbusfailure, "SYS: the EOappCanSP could not tx frames on CAN bus. In par16 there is: on msb the size of txfifo, on lsb a code."},
    {eoerror_value_SYS_canservices_formingfailure, "SYS: the EOappCanSP could not form a can packet. In par16 there is class in msb and cmdid in lsb."},
    {eoerror_value_SYS_canservices_parsingfailure, "SYS: the EOappCanSP could not parse a rx can frame. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_SYS_canservices_txfifoputisok, "SYS: the EOappCanSP have just put a can frame in tx fifo. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data."},
	{eoerror_value_SYS_canservices_genericerror, "SYS: the EOappCanSP has detected a generic error. In par16 there is a code"},
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
    {eoerror_value_SYS_canservices_rxmaisbug,   "SYS: the EOappCanSP received a canframe caused by MAISbug. The message is stored inside par64. In par16 is stored the size of the message for the single frame"},
    {eoerror_value_SYS_canservices_rxfromwrongboard, "SYS: the EOappCanSP received a canframe from a wrong board. The message is stored inside par64. In par16 is stored the size of the message for the single frame"},
    {eoerror_value_SYS_transceiver_rxseqnumber_error, "SYS: the board has detected a hole in rx ropframe sequence number. In par64 there is the expected number. The received is obtained by adding par64 with par16 treated as a int16_t"},    
    {eoerror_value_SYS_canservices_boards_missing, "SYS: the can discovery service detected that some boards are missing. Check with canLoader. In par64 MS 32 bits there is the bit mask of the searched, in LS 32 bits the mask of the found ones."},    
    {eoerror_value_SYS_canservices_boards_searched, "SYS: the can discovery service searched the following boards on the specified CAN bus. In par64 there are the searched addresses represented as nibbles."},    
    {eoerror_value_SYS_canservices_boards_found, "SYS: the can discovery service found the following boards on the specified CAN bus. In par64 there are the found addresses represented as nibbles."},
    {eoerror_value_SYS_transceiver_rxseqnumber_restarted, "SYS: the board has detected a restart of the remote transceiver because the rx ropframe sequence number is 1. In par64 there is the expected number."},
    {eoerror_value_SYS_canservices_board_detected, "SYS: the can discovery service found the board on the specified CAN bus / adr. In par64 there is the eObrd_typeandversions_t."},
    {eoerror_value_SYS_canservices_board_wrongprotversion, "SYS: the board on the specified CAN bus / adr has incompatible fw protocol version. In par16 there is required prot, in par64 there is detected eObrd_typeandversions_t."},
    {eoerror_value_SYS_canservices_board_notfound, "SYS: the board on the specified CAN bus / adr was not found during the discovery phase. In par16 there is the board type"},
    {eoerror_value_SYS_transceiver_rxinvalidframe_error, "SYS: the board has detected an invalid ropframe in reception."}
};  EO_VERIFYsizeof(eoerror_valuestrings_SYS, eoerror_value_SYS_numberof*sizeof(const eoerror_valuestring_t)); 


const eoerror_valuestring_t eoerror_valuestrings_HW[] =
{   // very important: fill table with order of eOerror_value_HW_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_HW_first,                "HW: first error message of category hw"},       
    {eoerror_value_HW_second,               "HW: second error message of category hw"},
    {eoerror_value_HW_strain_saturation,    "HW: strain values saturation. In p16 there is the channel involved, in p64 there are the saturation counts for the last sec of both types (lower: LS32bits, upper MS32bits"},
};  EO_VERIFYsizeof(eoerror_valuestrings_HW, eoerror_value_HW_numberof*sizeof(const eoerror_valuestring_t)); 


const eoerror_valuestring_t eoerror_valuestrings_MC[] =
{   // very important: fill table with order of eOerror_value_MC_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_MC_motor_external_fault,  "MC: 2FOC exernal fault button pressed."},
    {eoerror_value_MC_motor_overcurrent,     "MC: 2FOC overcurrent. The motor has been turned off to prevent it from being damaged by an impulsive spike of current."},
    {eoerror_value_MC_motor_i2t_limit,       "MC: 2FOC i2t limit exceeded. The motor has been turned off to prevent it from being damaged by overheatig due to a continuous absorbsion of high current."},
    {eoerror_value_MC_motor_hallsensors,     "MC: 2FOC hall sensors fault. Invalid sequence in motor Hall effect sensors, please check motor hall cable connections."},
    {eoerror_value_MC_motor_can_invalid_prot,"MC: 2FOC CAN invalid protocol. The EMS and 2FOC firmware versions are incompatible, please update."},
    {eoerror_value_MC_motor_can_generic,     "MC: 2FOC CAN generic error. Errors happened in the CAN bus between the EMS and the 2FOC board."},
    {eoerror_value_MC_motor_can_no_answer,   "MC: 2FOC CAN no answer. The communication between the EMS and the 2FOC board has been lost for more than 50 ms."},
    {eoerror_value_MC_axis_torque_sens,      "MC: torque sensor timeout. The joint is in a compliant interaction mode or torque control mode, and data from torque sensor have been unavailable for more than 100 ms."},
    {eoerror_value_MC_aea_abs_enc_invalid,   "MC: AEA encoder invalid data. Hardware problem in the magnetic position sensor of the joint caused invalid position readings."},
    {eoerror_value_MC_aea_abs_enc_timeout,   "MC: AEA encoder timeout. No answer from the magnetic position sensor of the joint (cable broken?)."},
    {eoerror_value_MC_aea_abs_enc_spikes,    "MC: AEA encoder has spikes. There is impulsive noise in the measures of the magnetic position sensor of the joint. In p64 there's the count for the last sec, in p16 there's the joint number"},
    {eoerror_value_MC_motor_qencoder_dirty,  "MC: 2FOC quadrature encoder dirty. The number of thicks in a complete revolution of the motor was lower than expected, the optical disks need to be cleaned."},
    {eoerror_value_MC_motor_qencoder_index,  "MC: 2FOC quadrature encoder index broken. The reference special thick was not detected during a complete revolution of the motor, please check motor encoder cables."},
    {eoerror_value_MC_motor_qencoder_phase,  "MC: 2FOC quadrature encoder phase broken. The motor encoder is not counting even if the motor is moving, please check motor encoder cables."},
    {eoerror_value_MC_motor_wrong_state,     "MC: 2FOC wrong state. The 2FOC motor controller is in a control state different from required by the EMS. In par64 0xF0 is the mask of requested state, 0x0F is the mask of actual state."},  
    {eoerror_value_MC_generic_error,         "MC: generic motor error (see 64 bit mask parameter)."}
};  EO_VERIFYsizeof(eoerror_valuestrings_MC, eoerror_value_MC_numberof*sizeof(const eoerror_valuestring_t)); 



const eoerror_valuestring_t eoerror_valuestrings_SK[] =
{   // very important: fill table with order of eOerror_value_MC_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_SYS_unspecified,             "SK: unspecified code."},
    {eoerror_value_SK_arrayofcandataoverflow,   "SK: cannot put rx can frames into arrayofcandata, thus some skin readings will be lost. In par16 there is frame.id and frame.size (in most significant nibble). In par64 there is the frame.data"},
    {eoerror_value_SK_onoroff,                  "SK: the skin transmission has been switched on or off. In par16 there's the corresponding code (0: OFF, 1: ON)"},
    {eoerror_value_SK_unexpecteddata,           "SK: the board has received a message from the skin even if it should be in a silenced modality. In par16 there's the actual state of the EMS board (0: CFG, 1: RUN)"}
};  EO_VERIFYsizeof(eoerror_valuestrings_SK, eoerror_value_SK_numberof*sizeof(const eoerror_valuestring_t)); 


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
};  EO_VERIFYsizeof(eoerror_valuestrings_DEB, eoerror_value_DEB_numberof*sizeof(const eoerror_valuestring_t)); 


const eoerror_valuestring_t eoerror_valuestrings_CFG[] =
{   // very important: fill table with order of eOerror_value_CFG_t
    //                 in case of holes, use {0, NULL}
    {eoerror_value_CFG_candiscovery_boardsmissing, "CFG: CANdiscovery cannot find some boards. In p16: target board type in 0xff00 and number of missing in 0x000f. In p64: search time [ms] in 0xffff000000000000, mask of missing addresses in 0x000000000000ffff"},
    {eoerror_value_CFG_candiscovery_boardsinvalid, "CFG: CANdiscovery detected invalid boards. In p16: target board type in 0xff00 and number of invalid in 0x000f. In p64: each nibble contains 0x0 if ok, mask 0x1 if wrong type, mask 0x2 if wrong fw, mask 0x4 if wrong prot"},
    {eoerror_value_CFG_candiscovery_ok, "CFG: CANdiscovery successful. In p16: target board type in 0xff00,  number of boards in 0x000f. In p64: search time [ms] in 0xffff000000000000, req prot in 0x00000000ffff0000, req fw in 0x000000000000ffff,"},
    {eoerror_value_CFG_candiscovery_detectedboard, "CFG: CANdiscovery has detected this board. In p16: board type in 0xff00, board address in 0x000f. In p64: search time [ms] in 0xffff000000000000, prot in 0x00000000ffff0000, fw in 0x000000000000ffff."}
};  EO_VERIFYsizeof(eoerror_valuestrings_CFG, eoerror_value_CFG_numberof*sizeof(const eoerror_valuestring_t)); 


const eoerror_valuestring_t * const eoerror_valuestrings[] = 
{   // very important: fill table with order of eOerror_category_t: pos 0 is eoerror_category_EthBoardSystem etc.
    //                 in case of holes use: NULL,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_SYS,   
    (const eoerror_valuestring_t *)&eoerror_valuestrings_HW,        
    (const eoerror_valuestring_t *)&eoerror_valuestrings_MC,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_SK,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_DEB,
    (const eoerror_valuestring_t *)&eoerror_valuestrings_CFG
};  EO_VERIFYsizeof(eoerror_valuestrings, eoerror_category_numberof*sizeof(const eoerror_valuestring_t *));  


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




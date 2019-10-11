/*
 * Copyright (C) 2019 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
 * website: https://github.com/icub-tech-iit
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
#ifndef _EOMDIGNOSTICROPMSG_H_
#define _EOMDIGNOSTICROPMSG_H_

#include "EoCommon.h"
#include "MsgDescription.h"

#include <array>
#include <sstream>
#include <iostream>
#include <iterator>
#include <cstring>
#include <iomanip>

/** 
  @file       EOMDiagnosticRopMsg.h
	@brief      This header file implements public interface to the EMS diagnostic message.
 	@author     luca.tricerri@iit.it
	@date       09/2019
 **/

class EOMDiagnosticRopMsg
{
public:
    struct Info
    {
        uint16_t code_{(uint16_t)DiagnosticRopCode::empty};
        uint16_t severity_{(uint16_t)DiagnosticRopSeverity::empty};
        uint16_t param2_{0};
        uint16_t param3_{0};
        uint16_t param4_{0};
        uint16_t param5_{0};
        uint16_t param6_{0};
        uint16_t param7_{0};
        uint64_t time_{0};
    };
    EO_VERIFYsizeof(Info, 24)

        constexpr static uint16_t getSize()
    {
        return sizeof(Info);
    }

    EOMDiagnosticRopMsg(void *data, uint8_t size);
    EOMDiagnosticRopMsg(const std::array<uint8_t, sizeof(Info)> &data);
    EOMDiagnosticRopMsg(const Info &data) : data_(data){};
    EOMDiagnosticRopMsg(){};

    uint8_t *data() const;
    void rawdump() const;
    void dump(const std::map<DiagnosticRopSeverity,std::string>* ,const std::map<DiagnosticRopCode,std::string>* ,const std::map<DiagnosticRopString,std::string>* ) const;
    void reset();
    DiagnosticRopCode getCode();
    DiagnosticRopSeverity getSeverity();

private:
    Info data_;
};

inline void EOMDiagnosticRopMsg::reset()
{
    data_ = (const struct Info){0};
}

inline DiagnosticRopCode EOMDiagnosticRopMsg::getCode()
{
    return (DiagnosticRopCode)(data_.code_);
}

inline DiagnosticRopSeverity EOMDiagnosticRopMsg::getSeverity()
{
    return static_cast<DiagnosticRopSeverity>(data_.severity_);
}

inline EOMDiagnosticRopMsg::EOMDiagnosticRopMsg(void *data, uint8_t size)
{
    if (size > sizeof(Info))
    {
        //TODO
        return;
    }

    std::memcpy(&data_, data, sizeof(Info));
}

inline EOMDiagnosticRopMsg::EOMDiagnosticRopMsg(const std::array<uint8_t, sizeof(Info)> &data)
{
    std::memcpy(&data_, data.data(), sizeof(Info));
}

inline uint8_t *EOMDiagnosticRopMsg::data() const
{
    return (uint8_t *)(&data_);
}

inline void EOMDiagnosticRopMsg::rawdump() const
{
    std::stringstream ss;
    std::array<uint8_t, getSize()> *tmp;
    tmp = (std::array<uint8_t, getSize()> *)&data_;

    std::copy(tmp->begin(), tmp->end(), std::ostream_iterator<int>(ss, " "));
    std::cout << std::hex << ss.str() << std::endl;
}

inline void EOMDiagnosticRopMsg::dump(const std::map<DiagnosticRopSeverity,std::string>* ropSeverity,const std::map<DiagnosticRopCode,std::string>* ropCode,const std::map<DiagnosticRopString,std::string>* ropString) const
{
    std::string codeStr{"unknown"};
    if(ropCode)
    {
        if(ropCode->find((DiagnosticRopCode)data_.code_)!=ropCode->end())
            codeStr=ropCode->at((DiagnosticRopCode)data_.code_);
    }      
    std::string severityStr{"unknown"};
    if(ropSeverity)
    {
        if(ropSeverity->find((DiagnosticRopSeverity)data_.severity_)!=ropSeverity->end())
            severityStr=ropSeverity->at((DiagnosticRopSeverity)data_.severity_);    
    }
        
    std::string stringStr{"unknown"};
    if(ropString)
    {
        if(ropString->find((DiagnosticRopString)data_.param2_)!=ropString->end())
            stringStr=ropString->at((DiagnosticRopString)data_.param2_);        
    }

    if((DiagnosticRopCode)data_.code_==DiagnosticRopCode::empty)
        return;
    
    std::cout << "------ROP";
    std::cout << "code:" <<std::left <<std::setw (15)<<codeStr << " ";
    std::cout << "severity:" <<std::left<<std::setw (15)<<severityStr << " ";
    std::cout << "param2:" <<std::left<<std::setw (15)<<stringStr << " ";
    std::cout << "param3:" << data_.param3_ << " ";
    std::cout << "param4:" << data_.param4_ << " ";
    std::cout << "param5:" << data_.param5_ << " ";
    std::cout << "param6:" << data_.param6_ << " ";
    std::cout << "param7:" << data_.param7_ << " ";
    std::cout << "time:" << data_.time_ << " ";
    std::cout << std::endl;
};

#endif // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

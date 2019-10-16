/*
 * Copyright (C) 2019 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
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
#ifndef _EOMDIGNOSTICUDPMSG_H_
#define _EOMDIGNOSTICUDPMSG_H_

/** @file     EOMDiagnosticUdpMsg.h
    @brief      This header file implements public interface to the EMS diagnostic UDP msg.
     @author     luca.tricerri@iit.it
    @date       09/2019
 **/

#include "EoCommon.h"

#include <array>
#include <algorithm>
#include <iterator>

#include "EOMDiagnosticRopMsg.h"
#include "EOMDiagnosticUdpHeader.h"
#include "EOMDiagnosticUdpFooter.h"

class EOMDiagnosticUdpMsg
{
private:
    constexpr static uint16_t ropNumber_{10};

public:
    constexpr static uint16_t getRopNumber()
    {
        return ropNumber_;
    }
    constexpr static uint16_t getSize()
    {
        return udpPacketDataSize_;
    }

private:
    constexpr static uint16_t udpPacketDataSize_{EOMDiagnosticUdpHeader::getSize() + EOMDiagnosticRopMsg::getSize() * EOMDiagnosticUdpMsg::ropNumber_ + EOMDiagnosticUdpFooter::getSize()};

    EOMDiagnosticUdpHeader header_;
    EOMDiagnosticUdpFooter footer_;
    std::array<EOMDiagnosticRopMsg, ropNumber_> body_;
    uint16_t currentRopNumber_{0};

public:
    EOMDiagnosticUdpMsg(){};

    bool getRop(EOMDiagnosticRopMsg &msg, uint8_t pos);
    bool addRop(const EOMDiagnosticRopMsg &msg);
    bool addRop(const EOMDiagnosticRopMsg::Info &info);
    EOMDiagnosticRopMsg& getRopForModify(bool res);
    bool createUdpPacketData(std::array<uint8_t, udpPacketDataSize_> &udpMsg); //fill udpPacketData_
    bool parse(const std::array<uint8_t, udpPacketDataSize_> &rxData);         //fill header,footer,body
    bool parse(uint8_t *data, uint16_t size);                                  //fill header,footer,body

    void reset();
    void dump(const std::map<DiagnosticRopSeverity,std::string>* ropSeverity,const std::map<DiagnosticRopCode,std::string>* ropCode,const std::map<DiagnosticRopString,std::string>* ropString) const;
    uint8_t getCurrentRopNumber() { return currentRopNumber_; }

private:
    std::array<uint8_t, udpPacketDataSize_> udpPacketData_;
    bool createUdpHeader();
    bool createUdpFooter();
    bool createUdpBody();
};

inline bool EOMDiagnosticUdpMsg::getRop(EOMDiagnosticRopMsg &msg, uint8_t pos)
{
    if (pos >= getRopNumber())
    {
        //TODO error
        return false;
    }
    msg = body_[pos];
    return true;
}

inline bool EOMDiagnosticUdpMsg::addRop(const EOMDiagnosticRopMsg::Info &info)
{
    if (currentRopNumber_ > getRopNumber())
    {
        //TO do error
        return false;
    }
   
    body_[currentRopNumber_].data_ = info;
    ++currentRopNumber_;
    return true;
    
};

inline bool EOMDiagnosticUdpMsg::addRop(const EOMDiagnosticRopMsg &msg)
{
    if (currentRopNumber_ > getRopNumber())
    {
        //TO do error
        return false;
    }

    body_[currentRopNumber_] = msg;
    ++currentRopNumber_;
    return true;
};

inline EOMDiagnosticRopMsg& EOMDiagnosticUdpMsg::getRopForModify(bool res)
{
    if (currentRopNumber_ > getRopNumber())
    {
        //TO do error
        res=false;
        return body_[0];
    }
    
    EOMDiagnosticRopMsg& rop=body_[currentRopNumber_];
    ++currentRopNumber_;
    return rop;
};

inline bool EOMDiagnosticUdpMsg::createUdpPacketData(std::array<uint8_t, udpPacketDataSize_> &udpMsg)
{
    if (currentRopNumber_ == 0)
        return false; //Nothing to create
       
    header_.updateHeader(currentRopNumber_ * EOMDiagnosticRopMsg::getSize(), currentRopNumber_, 0);

    createUdpHeader();
    createUdpBody();
    createUdpFooter();

    udpMsg = udpPacketData_;
    
    return true;
}
inline void EOMDiagnosticUdpMsg::reset()
{
    currentRopNumber_ = 0;
    udpPacketData_.fill(0);
    for (auto &current : body_)
    {
        current.reset();
    }
    header_.reset();
    footer_.reset();

    //TODO delete ROP
}
inline bool EOMDiagnosticUdpMsg::parse(const std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> &rxData)
{
    std::array<uint8_t, EOMDiagnosticUdpHeader::getSize()> headerData;
    std::copy(rxData.begin(), rxData.begin() + EOMDiagnosticUdpHeader::getSize(), headerData.begin());
    EOMDiagnosticUdpHeader header(headerData);
    header_ = header;

    for (size_t index = 0; index < EOMDiagnosticUdpMsg::getRopNumber(); ++index)
    {
        std::array<uint8_t, EOMDiagnosticRopMsg::getSize()> ropData;
        std::copy(rxData.begin() + EOMDiagnosticUdpHeader::getSize() + EOMDiagnosticRopMsg::getSize() * index, rxData.begin() + EOMDiagnosticUdpHeader::getSize() + EOMDiagnosticRopMsg::getSize() * (index + 1), ropData.begin());

        EOMDiagnosticRopMsg rop(ropData);
        body_[currentRopNumber_] = rop;
        currentRopNumber_++;
    }

    unsigned int startFooter = EOMDiagnosticUdpHeader::getSize() + EOMDiagnosticRopMsg::getSize() * EOMDiagnosticUdpMsg::getRopNumber();
    std::array<uint8_t, EOMDiagnosticUdpFooter::getSize()> footerData;
    std::copy(rxData.begin() + startFooter, rxData.begin() + startFooter + EOMDiagnosticUdpFooter::getSize(), footerData.begin());
    EOMDiagnosticUdpFooter footer(footerData);
    footer_ = footer;
    return true;
}

inline bool EOMDiagnosticUdpMsg::parse(uint8_t *data, uint16_t size)
{
    if (size != udpPacketDataSize_)
    {
        //TODO error
        return false;
    }

    std::array<uint8_t, udpPacketDataSize_> out;
    for (int index = 0; index < udpPacketDataSize_; ++index)
    {
        out[index] = data[index];
    }

    return parse(out);
}

inline bool EOMDiagnosticUdpMsg::createUdpHeader()
{
    std::memcpy(udpPacketData_.data(), header_.data(), EOMDiagnosticUdpHeader::getSize());
    return true;
}

inline bool EOMDiagnosticUdpMsg::createUdpFooter()
{
    uint16_t currentFooterAddress = currentRopNumber_ * EOMDiagnosticRopMsg::getSize() + EOMDiagnosticUdpHeader::getSize();
    std::memcpy(udpPacketData_.data() + currentFooterAddress, footer_.data(), EOMDiagnosticUdpFooter::getSize());
    return true;
}

inline bool EOMDiagnosticUdpMsg::createUdpBody()
{
    //TODO mutex
    /*uint16_t currentROPAddress{0};
    for (int index = 0; index < getCurrentRopNumber(); ++index)
    {
        currentROPAddress = EOMDiagnosticUdpHeader::getSize() + index * EOMDiagnosticRopMsg::getSize();
        std::memcpy(udpPacketData_.data() + currentROPAddress, body_[index].data(), EOMDiagnosticRopMsg::getSize());
    }
    return true; */
    
    uint16_t currentROPAddress = EOMDiagnosticUdpHeader::getSize();
    uint16_t currentROPSize=EOMDiagnosticRopMsg::getSize()*getCurrentRopNumber();
    std::memcpy(udpPacketData_.data() + currentROPAddress, body_[0].data(), currentROPSize);
    return true;  
}

inline void EOMDiagnosticUdpMsg::dump(const std::map<DiagnosticRopSeverity,std::string>* ropSeverity,const std::map<DiagnosticRopCode,std::string>* ropCode,const std::map<DiagnosticRopString,std::string>* ropString) const
{
    header_.dump();
    for (auto currentRop : body_)
    {
        currentRop.dump(ropSeverity,ropCode,ropString);
    };
    footer_.dump();
}

#endif // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

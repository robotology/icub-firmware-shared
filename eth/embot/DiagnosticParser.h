#pragma once

#include <array>

#include "EOMDiagnosticUdpMsg.h"
#include "EOMDiagnosticUdpHeader.h"

class DiagnosticParser
{
    public:
        bool parse(const std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& rxData);
};

inline bool DiagnosticParser::parse(const std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& rxData)
{
    std::array<uint8_t,EOMDiagnosticUdpHeader::getSize()> headerData;
    std::copy(rxData.begin(),rxData.begin()+EOMDiagnosticUdpHeader::getSize(),headerData.begin());   
    EOMDiagnosticUdpHeader header(headerData);
    header.dump();

    for(size_t index=0;index<EOMDiagnosticUdpMsg::bodyNumber_;++index)
    {
        std::array<uint8_t,EOMDiagnosticRopMsg::getSize()> ropData;
        std::copy(rxData.begin()+EOMDiagnosticUdpHeader::getSize()+EOMDiagnosticRopMsg::getSize()*index,rxData.begin()+EOMDiagnosticUdpHeader::getSize()+EOMDiagnosticRopMsg::getSize()*(index+1),ropData.begin());

        EOMDiagnosticRopMsg rop(ropData);
        rop.dump();
    }

    
    unsigned int startFooter=EOMDiagnosticUdpHeader::getSize()+EOMDiagnosticRopMsg::getSize()*EOMDiagnosticUdpMsg::bodyNumber_;
    std::array<uint8_t,EOMDiagnosticUdpFooter::getSize()> footerData;
    std::copy(rxData.begin()+startFooter,rxData.begin()+startFooter+EOMDiagnosticUdpFooter::getSize(),footerData.begin());
    EOMDiagnosticUdpFooter footer(footerData);
    footer.dump();

    return true;
}
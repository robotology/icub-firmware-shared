
/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

// - brief
//   it contains pimpl class(es) for handling diagnostics in an ETH node
//

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EMBOT_PROT_ETH_DIAGNOSTIC_NODE_H_
#define _EMBOT_PROT_ETH_DIAGNOSTIC_NODE_H_

#include "embot_core.h"
#include "embot_core_utils.h"
#include "embot_prot_eth_rop.h"

#include "embot_prot_eth_diagnostic.h"

namespace embot { namespace prot { namespace eth { namespace diagnostic {
           
    class Node 
    {
    public:
        struct Config
        {
            bool concurrentuse {false}; // ffu
            uint16_t singleropstreamcapacity {128};
            uint16_t ropframecapacity {512};
            // todo: 
            // - add any customisation such as: capacityofropframe, maxropsize, capacity of fifo of ropframes, etc.  
            Config() = default;
            constexpr Config(bool cu, uint16_t src, uint16_t rfc) : concurrentuse(cu), singleropstreamcapacity(src), ropframecapacity(rfc) {}
            bool isvalid() const { return (ropframecapacity > (28+32)) && (singleropstreamcapacity > 32); }
        };         
               
        Node();  
        ~Node();

        // usage: init(), then add() as many rops one wants, then when one wants to attempt transmit: 
        // if(prepare()) { retrieve(data); <alert the sender>}

        bool init(const Config &config);
        bool initted() const;
        bool add(const embot::core::Data &ropstream);
        bool add(const embot::prot::eth::rop::Descriptor &ropdes); 
        bool add(const embot::prot::eth::diagnostic::InfoBasic &infobasic);
        bool add(const embot::prot::eth::diagnostic::Info &info);
        bool add(const embot::prot::eth::diagnostic::InfoLarge &infolarge);
        bool prepare(size_t &sizeofropframe); // returns true if anything to retrieve. in sizeofropframe the size of required mem
        bool retrieve(embot::core::Data &datainropframe); // it copies the ropframe. 
        uint16_t getNumberOfROPs() const;

    private:    
        struct Impl;
        Impl *pImpl;
    };  
    

}}}} // namespace embot { namespace prot { namespace eth { namespace diagnostic {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

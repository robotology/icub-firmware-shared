
/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/


// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EMBOT_PROT_ETH_DIAGNOSTIC_HOST_H_
#define _EMBOT_PROT_ETH_DIAGNOSTIC_HOST_H_

#include "embot_core.h"
#include "embot_core_utils.h"
#include "embot_prot_eth_rop.h"


namespace embot { namespace prot { namespace eth { namespace diagnostic {
           
    class Host 
    {
    public:
        struct Config
        {
            bool concurrentuse {false};
            size_t ropcapacity {128};
            embot::prot::eth::rop::fpOnROP onrop {nullptr};
            Config() = default;
            constexpr Config(bool cu, size_t rc, embot::prot::eth::rop::fpOnROP o) 
               : concurrentuse(cu), ropcapacity(rc), onrop(o) {}
            bool isvalid() const { return (!onrop) && (ropcapacity >= 40); }
        };         
               
        Host();  
        ~Host();

        // usage: init(), then accept(ipv4, rxropframe). 

        bool init(const Config &config);
        bool initted() const;
        bool accept(const embot::prot::eth::IPv4 &ipv4, const embot::core::Data &ropframedata, embot::prot::eth::rop::fpOnROPext onrop ,void* orig);  
        bool accept(const embot::prot::eth::IPv4 &ipv4, const embot::core::Data &ropframedata, embot::prot::eth::rop::fpOnROP onrop = nullptr);  
    
    private:    
        struct Impl;
        Impl *pImpl;
    };  
    

}}}} // namespace embot { namespace prot { namespace eth { namespace diagnostic {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

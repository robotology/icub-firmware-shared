
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EMBOT_CORE_H_
#define _EMBOT_CORE_H_


#include <cstdint>
#include <cstring>
#include <type_traits>
#include <string>

namespace embot { namespace core {
    
    // data structures
        
    // the time is always expressed in micro-seconds
    
    // Time keeps absolute time since bootstrap. by using 8-bytes it expresses up to 599 years.
    using Time          = std::uint64_t;  
    // relTime keep relative time (timeouts, periods, ect). by using 4-bytes it expresses at least up to 1 hour
    using relTime       = std::uint32_t;    
    
    // some useful Time constants: use 5*time1millisec rather than 5000
    constexpr Time time1microsec    = 1;    
    constexpr Time time1millisec    = 1000;
    constexpr Time time1second      = 1000000;
    constexpr Time time1minute      = 60*time1second;
    constexpr Time time1hour        = 60*time1minute;
    constexpr Time time1day         = 24*time1hour;
    constexpr Time timeNone         = 0xffffffffffffffff;   // used to express an invalid time
    
    // the above, up to time1hour, can be used also as relTime constants. but here are others used by blocking calls.
    constexpr relTime reltimeWaitNone     = 0;                 // the call does not apply any timeout            
    constexpr relTime reltimeWaitForever  = 0xffffffff;        // the call has an infinite timeout      
    
    // it formats Time (or relTime) for print purposes
    struct TimeFormatter
    {   
        uint16_t    D {0}; // day
        uint8_t     H {0}; // hour
        uint8_t     M {0}; // minute
        uint8_t     S {0}; // second
        uint16_t    m {0}; // milli
        uint16_t    u {0}; // micro
        
        TimeFormatter() = default;
        
        constexpr TimeFormatter(const embot::core::Time &t) 
        {
            uint64_t tmp = t;
            u = tmp%1000; tmp /= 1000;
            m = tmp%1000; tmp /= 1000;
            S = tmp%60; tmp /= 60;
            M = tmp%60; tmp /= 60;
            H = tmp%24; tmp /= 24;
            D = tmp;            
        }
        
        uint64_t to_seconds() const { return S+60*M+3600*H+86400*D; }        
        
        embot::core::Time to_time() const
        {
            return static_cast<embot::core::Time>(u) + time1millisec*static_cast<embot::core::Time>(m) + time1second*to_seconds();
        }        
        
        enum class Mode : std::uint8_t { compact, full };
        
        std::string to_string(const Mode &mode = Mode::compact) const
        {
            if(Mode::compact == mode)
            {
                return "S" + std::to_string(to_seconds()) + ":m" + std::to_string(m) + ":u" + std::to_string(u);        
            }
            else
            {
                return "D" + std::to_string(D) + ":H" + std::to_string(H) + ":M" + std::to_string(M) + ":S" + std::to_string(S) + ":m" + std::to_string(m) + ":u" + std::to_string(u); 
            }
        }        

    };    
        
    
    // some pointer to functions
    using fpGetU32      = std::uint32_t (*)(void);
    using fpGetU64      = std::uint64_t (*)(void);
    using fpParU32      = void (*)(std::uint32_t);
    using fpBoolParU32  = bool (*)(std::uint32_t);
    using fpI08ParU32   = std::int8_t (*)(std::uint32_t);
    using fpWorker      = void (*)(void);
    using fpCaller      = void (*)(void *);
    using fpPrint       = int (*)(const std::string &str);
    
    // useful converter for enum class
    template<typename E>         // see C++14 [Meyers, pag. 73]
    constexpr auto tointegral(E enumerator) noexcept 
    {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }   
             
    // it represents a function call with a generic argument
    // note that we can construct it using a lambda:  {[](void *p){ /*use p*/ }, arg}
    struct Callback
    {
        fpCaller call {nullptr};
        void * arg {nullptr};
        
        Callback() = default;
        constexpr Callback(fpCaller _c, void *_a) : call(_c), arg(_a) {}
        void load(fpCaller _c, void *_a) { call = _c; arg = _a; }
        void clear() { call = nullptr; arg = nullptr; }
        bool isvalid() const { if(nullptr != call) { return true; } else { return false; } } 
        void execute() const { if(true == isvalid()) { call(arg); } }
    };

    // it is a holder of data, either mutable or constexpr
    // we dont want to change what is inside pointer unless basic initialization
    struct Data
    {    
        void * pointer {nullptr};
        size_t capacity {0};
        
        constexpr Data() = default;
        constexpr Data(void *p, size_t s) : pointer(p), capacity(s) {} 
                    
        void load(void *littleendianmemory, const size_t s) { pointer = littleendianmemory; capacity = s; }
        
        void clear() { pointer = nullptr; capacity = 0; }
        
        bool isvalid() const { return (nullptr != pointer) && (0 != capacity); }
        
        size_t copyfrom(void *littleendianmemory, const size_t s)
        {
           if((!isvalid()) || (nullptr == littleendianmemory) || (0 == s))
           {
                return 0;
           }  
           size_t n = std::min(s, capacity);
           std::memmove(pointer, littleendianmemory, n);
           return n;
        }
        
        size_t copyfrom(const Data &other)
        {
           if((!isvalid()) || (!other.isvalid()))
           {
                return 0;
           }  
           
           size_t n = std::min(other.capacity, capacity);
           std::memmove(pointer, other.pointer, n);
           return n;
        }
        
        std::uint8_t * getU08ptr() const { return reinterpret_cast<std::uint8_t*>(pointer); }
    };    

    
    // initialization of embot::core
    // to use embot::core as a standalone, you must call embot::core::init(cfg), where cfg specifies a time base
    
    struct TimeConfig
    {         
        embot::core::fpWorker init {nullptr};  
        embot::core::fpGetU64 get {nullptr};         
        
        constexpr TimeConfig() = default;
        constexpr TimeConfig(embot::core::fpWorker _init, embot::core::fpGetU64 _get) : init(_init), get(_get) {}
        bool isvalid() const { if(nullptr != get) { return true; } else { return false; } }
    };  


    struct PrintConfig
    {
        fpPrint fpprint {nullptr};
        
        PrintConfig() = default;
        constexpr PrintConfig(fpPrint p) : fpprint(p) {}
        bool isvalid() const { if(nullptr != fpprint) { return true; } else { return false; } }
    };        
    
    struct Config
    {         
        TimeConfig timeconfig {}; 
        PrintConfig printconfig {};           
        
        constexpr Config() = default;
        constexpr Config(const TimeConfig &tc, const PrintConfig &pc = nullptr) : timeconfig(tc.init, tc.get), printconfig(pc) {}
        bool isvalid() const { return timeconfig.isvalid(); }
    }; 
       
    
    bool initialised();
    
    bool init(const Config &config);

    embot::core::Time now();
        
    void wait(embot::core::relTime t);

    int print(const std::string &str);
    
}} // namespace embot { namespace core {



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

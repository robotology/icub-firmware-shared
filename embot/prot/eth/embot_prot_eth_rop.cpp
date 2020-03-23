
/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "embot_prot_eth_rop.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include <algorithm>

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------


struct embot::prot::eth::rop::Stream::Impl
{    
    uint8_t *stream {nullptr};
    Header *ref2header {nullptr};
    uint8_t *ref2data {nullptr};
    size_t capacityofstream {0};
    Descriptor descriptor {};
    size_t sizeofstream {0};
    
    Impl(size_t capacity)
    {
        if(capacity < Stream::minimumsize)
        {
            capacity = Stream::minimumsize;
        }
        capacityofstream = (capacity+3)/4;
        capacityofstream *= 4;
        stream = new uint8_t[capacityofstream];
        
        ref2header = reinterpret_cast<Header*>(stream);
        if(capacityofstream > Stream::minimumsize)
        {
            ref2data = stream + sizeof(Header);
        }
    }   

    ~Impl()
    {
        delete[] stream;
        // DONT delete ref2header or ref2data
        //constexpr bool force = true;
        //deinit(force);
    }
    
    void fillheader(Header *header, const embot::prot::eth::rop::Descriptor &des)
    {        
        header->fmt.fill(des.plus, RQST::none, CONF::none);
        header->opc = des.opcode;
        header->datasize = (des.value.capacity+3)/4;
        header->datasize *= 4;
        header->id32 = des.id32;
    }

    bool load(const Descriptor &des)
    {
        size_t required = Stream::capacityfor(des.opcode, des.value.capacity, des.plus);
        
        if(required > capacityofstream)
        {
            return false;
        }
        
        descriptor = des;
        sizeofstream = required;
        
        // ok, now i shape the memory of the ropstream
        
        // header
        fillheader(ref2header, des);
        
        // data
        if(des.value.isvalid())
        {   
            uint16_t nbytes = std::min(ref2header->datasize, static_cast<uint16_t>(des.value.capacity));
            std::memmove(ref2data, des.value.pointer, nbytes);
        }
        
        // signature
        if(des.hassignature())
        {
            std::memmove(ref2data+ref2header->datasize, &des.signature, sizeof(des.signature));
        }   
        
        // time
        if(des.hastime())
        {   // DONT attempt to use uint64_t* from ref2data+ref2header->datasize+4 and do a direct assignment because the memory is not guarantted to be 8-aligned.
            std::memmove(ref2data+ref2header->datasize+4, &des.time, sizeof(des.time));
        }
                     
        return true;
    }
    
    
    bool retrieve(uint8_t **data, size_t &size)
    {
        if(nullptr == data)
        {
            return false;
        }
        
        *data = stream;        
        size = sizeofstream;
        return true;
    }
 
    bool update(const embot::core::Data &data, const embot::prot::eth::rop::SIGN signature = embot::prot::eth::rop::signatureNone, const embot::core::Time time = embot::core::timeNone)
    {
        bool r = false;
        
        if(data.isvalid())
        {
            uint16_t nbytes = std::min(ref2header->datasize, static_cast<uint16_t>(data.capacity));
            std::memmove(ref2data, data.pointer, nbytes);   
            r = true;
        }
    
        if((ref2header->fmt.isPLUSsignature()))
        {
            std::memmove(ref2data+ref2header->datasize, &signature, sizeof(signature));
            r = true;
        } 
        
        if((ref2header->fmt.isPLUStime()))
        {
            std::memmove(ref2data+ref2header->datasize+4, &time, sizeof(time));
            r = true;
        }
        
        return r;
    }
    
    size_t getcapacity() const
    {
        return capacityofstream;
    }

};

// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------



embot::prot::eth::rop::Stream::Stream(size_t capacity)
: pImpl(new Impl(capacity))
{ 

}

embot::prot::eth::rop::Stream::~Stream()
{   
    delete pImpl;
}

bool embot::prot::eth::rop::Stream::load(const Descriptor &des)
{
    return pImpl->load(des);
}

bool embot::prot::eth::rop::Stream::update(const embot::core::Data &data, const embot::prot::eth::rop::SIGN signature, const embot::core::Time time)
{
   return pImpl->update(data, signature, time);
}

bool embot::prot::eth::rop::Stream::retrieve(uint8_t **data, size_t &size)
{
    return pImpl->retrieve(data, size);
}    

size_t embot::prot::eth::rop::Stream::getcapacity() const
{
    return pImpl->getcapacity();
}


// - descriptor


bool embot::prot::eth::rop::Descriptor::load(embot::core::Data &stream, uint16_t &consumed)
{
    reset();
    consumed = 0;
    if(false == stream.isvalid())
    {
        return false;
    }
    
    if(stream.capacity < Header::sizeofobject)
    {
        return false;
    }
    
    Header *rophead = reinterpret_cast<Header*>(stream.pointer);
    
    // now i need to see how big the ropstream is expected to be
    size_t expectedsize = Stream::capacityfor(rophead->opc, rophead->datasize, rophead->fmt.getPLUS());
    if((expectedsize > stream.capacity) || (0 != (rophead->datasize % 4))) // it was < ...
    {
        return false;
    }
    
    // ok, we can consume the bytes
    consumed = expectedsize;
    
    // and we fill the fields of the Descriptor
    opcode = rophead->opc;
    id32 = rophead->id32;
    value.capacity = rophead->datasize;
    value.pointer = (0 == value.capacity) ? nullptr : (stream.getU08ptr() + sizeof(Header));
    uint16_t offset_time = 0;
    if(rophead->fmt.isPLUSsignature())
    {
        offset_time = sizeof(signature);
        std::memmove(&signature, stream.getU08ptr() + sizeof(Header) + rophead->datasize, sizeof(signature));
    }
    if(rophead->fmt.isPLUStime())
    {
        std::memmove(&time, stream.getU08ptr() + sizeof(Header) + rophead->datasize + offset_time, sizeof(time));
    }  
    
    rophead->fmt.extract(plus, rqst, conf);
    
    return true;  
}



// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


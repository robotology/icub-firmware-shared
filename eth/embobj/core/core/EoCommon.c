/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "string.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoCommon.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

const eOversion_t eok_version               = {
                                                EO_INIT(.major)     EOK_VER_MAJ, 
                                                EO_INIT(.minor)     EOK_VER_MIN, 
                                                EO_INIT(.release)   EOK_VER_REL
                                              };            

const uint32_t  eok_u32version              = EOK_U32VERSION;

const uint8_t   eok_uint04dummy             = EOK_uint04dummy;
const uint8_t   eok_uint08dummy             = EOK_uint08dummy;
const uint16_t  eok_uint16dummy             = EOK_uint16dummy;
const uint32_t  eok_uint32dummy             = EOK_uint32dummy;
const uint64_t  eok_uint64dummy             = EOK_uint64dummy;

const eOreltime_t eok_reltimeZERO           = 0;
const eOreltime_t eok_reltime1ms            = 1000;
const eOreltime_t eok_reltime10ms           = 10000;
const eOreltime_t eok_reltime100ms          = 100000;
const eOreltime_t eok_reltime1sec           = 1000000;
const eOreltime_t eok_reltimeMAX            = 0xfffffffe;
const eOreltime_t eok_reltimeINFINITE       = 0xffffffff;

const eOabstime_t eok_abstimeNOW            = 0xffffffffffffffffLL;

const eOipv4addr_t eok_ipv4addr_localhost   = EO_COMMON_IPV4ADDR_LOCALHOST;

//const uint8_t  eo_dummy08            = 255;



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

// used to retrieve the base object when it is the first pointer in the struct
// the idea is to use: typedef struct { base_t *name; } derived_t;
extern void * eo_common_getbaseobject(eOderived_t *derived) 
{
	void **pp_base = (void **)derived;
	
	if(NULL == pp_base) // pp_base is equal to derived. 
	{
		return(NULL);	
	}
	
	return(*pp_base);
}

// used to retrieve teh base object from a safely derived object.
// the idea is to use: typedef struct { uint32_t tag; base_t *name; } safelyderived_t;
extern void * eo_common_getsafebaseobject(eOsafelyderived_t *derived)
{
    uint32_t* peoidentifier = (uint32_t*)derived;
    void **pp_base = NULL;

    pp_base = (void **) (peoidentifier+1);
	
	return(*pp_base);
}

// used to verify that the derived object is effectively derived from the base with the passed tag.
// the idea is to use: typedef struct { uint32_t tag; base_t *name; } safelyderived_t;
extern eOresult_t eo_common_verifysafebaseobject(eOsafelyderived_t *derived, const uint32_t tag)
{
    const uint32_t* peoidentifier = (uint32_t*)derived;
    return((tag == *peoidentifier) ? (eores_OK) : (eores_NOK_generic));
}


extern size_t eo_common_msize(void *p)
{   // the heap on arm keil compiler stores the size of the allcated ram in previous location of the returned pointer.
    // however, that may not work with otehr compilers and different architecture. on pc104 valgrind says that we read an invalid location
    // and thus ... i prefer removing this feature for gcc compiler.
#if defined(EO_READ_PREV_WORD_OF_MALLOC_FOR_SIZEOF_ALLOCATION)    
    size_t* xx = (size_t*)p;
    xx --;
    return(*xx);  
#else
    return(0);
#endif    
}



extern eObool_t eo_common_byte_bitcheck(uint8_t byte, uint8_t bit)
{
    if(byte & (1<<bit))
    {
        return(eobool_true);
    }
    else
    {
        return(eobool_false);
    }  
}

extern void eo_common_byte_bitset(uint8_t* byte, uint8_t bit)
{
    (*byte) |= (1<<bit);    
}

extern void eo_common_byte_bitclear(uint8_t* byte, uint8_t bit)
{
    (*byte) &= (~(1<<bit));    
}

extern void eo_common_byte_bittoggle(uint8_t* byte, uint8_t bit)
{
    (*byte) ^= (1<<bit);    
}


extern eObool_t eo_common_hlfword_bitcheck(uint16_t hword, uint8_t bit)
{
    if(hword & (1<<bit))
    {
        return(eobool_true);
    }
    else
    {
        return(eobool_false);
    }
}

extern eObool_t eo_common_hlfword_maskcheck(uint16_t hword, uint16_t mask)
{
    if(hword & mask)
    {
        return(eobool_true);
    }
    else
    {
        return(eobool_false);
    }
}

extern void eo_common_hlfword_bitset(uint16_t* hword, uint8_t bit)
{
    (*hword) |= (1<<bit);    
}

extern void eo_common_hlfword_maskset(uint16_t* hword, uint16_t mask)
{
    (*hword) |= (mask);    
}

extern void eo_common_hlfword_bitclear(uint16_t* hword, uint8_t bit)
{
    (*hword) &= (~(1<<bit));    
}

extern void eo_common_hlfword_maskclear(uint16_t* hword, uint16_t mask)
{
    (*hword) &= (~(mask));    
}

extern void eo_common_hlfword_bittoggle(uint16_t* hword, uint8_t bit)
{
    (*hword) ^= (1<<bit);    
}



extern eObool_t eo_common_word_bitcheck(uint32_t word, uint8_t bit)
{
    if(word & (1<<bit))
    {
        return(eobool_true);
    }
    else
    {
        return(eobool_false);
    }
}


extern void eo_common_word_bitset(uint32_t* word, uint8_t bit)
{
    (*word) |= (1<<bit);    
}

extern void eo_common_word_bitclear(uint32_t* word, uint8_t bit)
{
    (*word) &= (~(1<<bit));    
}

extern void eo_common_word_bittoggle(uint32_t* word, uint8_t bit)
{
    (*word) ^= (1<<bit);    
}


extern eObool_t eo_common_dword_bitcheck(uint64_t dword, uint8_t bit)
{
    if(dword & (1<<bit))
    {
        return(eobool_true);
    }
    else
    {
        return(eobool_false);
    }
}


extern void eo_common_dword_bitset(uint64_t* dword, uint8_t bit)
{
    (*dword) |= (1<<bit);    
}

extern void eo_common_dword_bitclear(uint64_t* dword, uint8_t bit)
{
    (*dword) &= (~(1<<bit));    
}

extern void eo_common_dword_bittoggle(uint64_t* dword, uint8_t bit)
{
    (*dword) ^= (1<<bit);    
}

// see http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
static const uint8_t s_eocommon_oneBitsInU8[256] = 
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F (<- n)
//  =====================================================
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, // 0n
    
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 1n (+1)
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 2n (+1)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 3n (+2)    
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 4n (+1)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 3n (+2)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 6n (+2)
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // 7n (+3)
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 8n (+1)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 9n (+2)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // An (+2)
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // Bn (+3)
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // Cn (+2)
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // Dn (+3)
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // En (+3)  
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8  // Fn (+4)
};

extern uint8_t eo_common_byte_bitsetcount(uint8_t byte)
{
    return(s_eocommon_oneBitsInU8[byte & 0xff]);
}

extern uint8_t eo_common_hlfword_bitsetcount(uint16_t hword)
{
    return(eo_common_byte_bitsetcount(hword&0xff)+eo_common_byte_bitsetcount((hword>>8)&0xff));
}

extern uint8_t eo_common_word_bitsetcount(uint32_t word)
{
    return(eo_common_hlfword_bitsetcount(word&0xffff) + eo_common_hlfword_bitsetcount((word>>16)&0xffff));
}

extern uint8_t eo_common_dword_bitsetcount(uint64_t dword)
{
    return(eo_common_word_bitsetcount(dword&0xffffffff) + eo_common_word_bitsetcount((dword>>32)&0xffffffff));
}


//extern eObool_t eo_common_test_bitsetcount(void)
//{
//    uint32_t i = 0;
//    
//    for(i=0; i<777777; i++)
//    {
//        uint8_t count8 = 0;
//        uint8_t count16 = 0;
//        uint8_t count32 = 0;
//        
//        uint8_t t8 = i & 0xff;
//        uint16_t t16 = i & 0xffff;
//        uint32_t t32 = i;
//        
//        
//        uint8_t j=0;
//        for(j=0; j<8; j++)
//        {
//            if(eo_common_byte_bitcheck(t8, j))
//            {
//                count8 ++;
//            }
//        }
//        for(j=0; j<16; j++)
//        {
//            if(eo_common_hlfword_bitcheck(t16, j))
//            {
//                count16 ++;
//            }
//        } 
//        for(j=0; j<32; j++)
//        {
//            if(eo_common_word_bitcheck(t32, j))
//            {
//                count32 ++;
//            }
//        } 
//        
//        if(count8 != eo_common_byte_bitsetcount(t8))
//        {
//            return(eobool_false);
//        }
//        
//        if(count16 != eo_common_hlfword_bitsetcount(t16))
//        {
//            return(eobool_false);
//        }        
//        if(count32 != eo_common_word_bitsetcount(t32))
//        {
//            return(eobool_false);
//        }         
//    }
//    
//    
//    return(eobool_true);
//}

static eOresult_t s_eo_common_Q17_14_clip(int64_t in, eOq17_14_t *out)
{
    // verify positive overflow
    if(in > EOK_Q17_14_POS_BIGGEST)
    {
        *out = EOK_Q17_14_POS_BIGGEST;
        return(eores_NOK_generic);
    }

    // verify negative overflow
    if(in < EOK_Q17_14_NEG_BIGGEST)
    {
        *out = EOK_Q17_14_NEG_BIGGEST;
        return(eores_NOK_generic);
    }
    
    // no overflow
    *out = (eOq17_14_t)in; 
    return(eores_OK);        
}

extern eOq17_14_t eo_common_float_to_Q17_14(float f_num)
{   // check vs overflow and clip ....
    eOq17_14_t ret = 0;
    int64_t rr = ((int64_t)f_num * 16384.0f);    // note: 16384.0 = 2^14
    s_eo_common_Q17_14_clip(rr, &ret);
    return(ret);
}

extern float eo_common_Q17_14_to_float(eOq17_14_t q_num)
{   // in here we cannot overflow
    return((float)(q_num / 16384.0f) ); // note: 16384.0 = 2^14
}


// it sums two q17-14 numbers and manages positive and negative overflows. 
// if the sum overflows, the result is clipped to biggest pos / neg and funtions returns eores_NOK_generic.
// if the sum is valid return is eores_OK
extern eOresult_t eo_common_Q17_14_add(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res)
{
    int64_t rr = (int64_t)q_num1 + (int64_t)q_num1;

    return(s_eo_common_Q17_14_clip(rr, q_res));
}


extern eOresult_t eo_common_Q17_14_multiply(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res)
{
    int64_t rr = ((int64_t)q_num1) * ((int64_t)q_num2);

    rr = (rr + (1<<13))>>14;  // acemor: split the following two to remove a warning on visual studio    
    
    return(s_eo_common_Q17_14_clip(rr, q_res));
}

extern eOresult_t eo_common_Q17_14_divide(eOq17_14_t q_num1, eOq17_14_t q_num2, eOq17_14_t *q_res)
{  
    int64_t n1 = q_num1;
    int64_t n2 = q_num2;
    int64_t rr = 0;
    uint8_t neg = 0;
    
    if(0 == n2)
    {
        return(eores_NOK_generic);
    }
    
    if(n1 < 0)
    {
        neg = !neg;
        n1 = -n1;
    }
    
    if(n2 < 0)
    {
        neg = !neg;
        n2 = -n2;
    }
    
    rr = (n1 << 14) / n2;
    
    if(0 != neg)
    {
        rr = - rr;
    }
    
    return(s_eo_common_Q17_14_clip(rr, q_res));
}


extern uint64_t eo_common_canframe_data2u64(eOcanframe_t *frame)
{
    if(NULL == frame)
    {
        return(0);
    }
    // it works as long as data is aligned at 8 bytes
    return(*((uint64_t*)(frame->data)));
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


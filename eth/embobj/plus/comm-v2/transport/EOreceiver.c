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
#include "EoCommon.h"
#include "string.h"
#include "EOtheMemoryPool.h"
#include "EOtheParser.h"
#include "EOtheFormer.h"




// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOreceiver.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOreceiver_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static void s_eo_receiver_on_error_invalidframe(EOreceiver* p);

static void s_eo_receiver_on_error_seqnumber(EOreceiver* p);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

//static const char s_eobj_ownname[] = "EOreceiver";

const eOreceiver_cfg_t eo_receiver_cfg_default = 
{
    EO_INIT(.sizes)
    {
        EO_INIT(.capacityofropframereply)   256, 
        EO_INIT(.capacityofropinput)        128, 
        EO_INIT(.capacityofropreply)        128
    }, 
    EO_INIT(.agent)                         NULL,
    EO_INIT(.extfn)                         
    {
        EO_INIT(.onerrorseqnumber)          NULL,
        EO_INIT(.onerrorinvalidframe)       NULL
    } 
};



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


 
extern EOreceiver* eo_receiver_New(const eOreceiver_cfg_t *cfg)
{
    EOreceiver *retptr = NULL;   

    if(NULL == cfg)
    {    
        cfg = &eo_receiver_cfg_default;
    }
    
    // i get the memory for the object
    retptr = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOreceiver), 1);
    retptr->ropframeinput       = eo_ropframe_New();
    retptr->ropframereply       = eo_ropframe_New();
    retptr->ropinput            = eo_rop_New(cfg->sizes.capacityofropinput);
    retptr->ropreply            = eo_rop_New(cfg->sizes.capacityofropreply);
    retptr->agent               = cfg->agent;
    retptr->ipv4addr            = 0;
    retptr->ipv4port            = 0;
    retptr->bufferropframereply = (0 == cfg->sizes.capacityofropframereply) ? (NULL) : (eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, cfg->sizes.capacityofropframereply, 1));
    retptr->rx_seqnum           = eok_uint64dummy;
    retptr->tx_ageofframe       = eok_uint64dummy;
    memset(&retptr->error_seqnumber, 0, sizeof(retptr->error_seqnumber));       // even if it is already zero.
    memset(&retptr->error_invalidframe, 0, sizeof(retptr->error_invalidframe)); // even if it is already zero. 
    retptr->on_error_seqnumber  = cfg->extfn.onerrorseqnumber;
    retptr->on_error_invalidframe = cfg->extfn.onerrorinvalidframe;
    // now we need to allocate the buffer for the ropframereply

#if defined(USE_DEBUG_EORECEIVER)    
    memset(&retptr->debug, 0, sizeof(EOreceiverDEBUG_t));
#endif  
    
    eo_ropframe_Load(retptr->ropframereply, retptr->bufferropframereply, eo_ropframe_sizeforZEROrops, cfg->sizes.capacityofropframereply);
    eo_ropframe_Clear(retptr->ropframereply);
    
    return(retptr);
}


extern void eo_receiver_Delete(EOreceiver *p)
{
    if(NULL == p)
    {
        return;
    }
    
    if(NULL == p->ropinput)
    {
        return;
    }
    
    eo_mempool_Delete(eo_mempool_GetHandle(), p->bufferropframereply);
    eo_rop_Delete(p->ropreply);
    eo_rop_Delete(p->ropinput);
    eo_ropframe_Delete(p->ropframereply);
    eo_ropframe_Delete(p->ropframeinput);

    
    memset(p, 0, sizeof(EOreceiver));
    eo_mempool_Delete(eo_mempool_GetHandle(), p);
    return;    
}



extern eOresult_t eo_receiver_Process(EOreceiver *p, EOpacket *packet, uint16_t *numberofrops, eObool_t *thereisareply, eOabstime_t *transmittedtime)
{
    uint16_t rxremainingbytes = 0;
    uint16_t txremainingbytes = 0;
    uint8_t* payload;
    uint16_t size;
    uint16_t capacity;
    uint16_t nrops;
    uint16_t i;
    eOresult_t res;
    eOipv4addr_t remipv4addr;
    eOipv4port_t remipv4port;
    uint64_t rec_seqnum;
    uint64_t rec_ageoframe;
    uint16_t numofprocessedrops = 0;

    
    if((NULL == p) || (NULL == packet)) 
    {
        return(eores_NOK_nullpointer);
    }
    
    
    // clear the ropframereply w/ eo_ropframe_Clear(). the clear operation also makes it safe to manipulate p->ropframereplay with *_quickversion
    
    eo_ropframe_Clear(p->ropframereply);
    
    
    // we get the ip address and port of the incoming packet.
    // the remaddr can be any. however, if the eo_receiver_Process() is called by the EOtransceiver, it will be only the one of the remotehost
    eo_packet_Addressing_Get(packet, &remipv4addr, &remipv4port);
    
   
    // then we assign them to the ones of the EOreceiver. by doing so we force the receive to accept packets from everyboby.
    //p->ipv4addr = remipv4addr;
    //p->ipv4port = remipv4port;
    
    // retrieve payload from the incoming packet and load the ropframe with it
    eo_packet_Payload_Get(packet, &payload, &size);
    eo_packet_Capacity_Get(packet, &capacity);
    eo_ropframe_Load(p->ropframeinput, payload, size, capacity);
    
    // verify if the ropframeinput is valid w/ eo_ropframe_IsValid()
    if(eobool_false == eo_ropframe_IsValid(p->ropframeinput))
    {
#if defined(USE_DEBUG_EORECEIVER)         
        {   // DEBUG
            p->debug.rxinvalidropframes ++;
        }
#endif  
        p->error_invalidframe.remipv4addr = remipv4addr;
        p->error_invalidframe.ropframe = p->ropframeinput;
        s_eo_receiver_on_error_invalidframe(p);
        
        if(NULL != thereisareply)
        {
            *thereisareply = eobool_false;
        }
        return(eores_NOK_generic);
    }
    
    
    // check sequence number
    
    rec_seqnum = eo_ropframe_seqnum_Get(p->ropframeinput);
    rec_ageoframe = eo_ropframe_age_Get(p->ropframeinput);
    
    if(p->rx_seqnum == eok_uint64dummy)
    {
        //this is the first received ropframe or ... the sender uses dummy seqnum
        p->rx_seqnum = rec_seqnum;
        p->tx_ageofframe = rec_ageoframe;
    }
    else
    {
        if(rec_seqnum != (p->rx_seqnum+1))
        {
#if defined(USE_DEBUG_EORECEIVER)             
            {
                p->debug.errorsinsequencenumber ++;
            }
#endif  
            // must set values
            p->error_seqnumber.remipv4addr = remipv4addr;
            p->error_seqnumber.rec_seqnum = rec_seqnum;
            p->error_seqnumber.exp_seqnum =  p->rx_seqnum+1;
            p->error_seqnumber.timeoftxofcurrent = rec_ageoframe;
            p->error_seqnumber.timeoftxofprevious = p->tx_ageofframe;
            
            s_eo_receiver_on_error_seqnumber(p);
        }
        p->rx_seqnum = rec_seqnum;
        p->tx_ageofframe = rec_ageoframe;
    }
    

    nrops = eo_ropframe_ROP_NumberOf_quickversion(p->ropframeinput);
    
    for(i=0; i<nrops; i++)
    {
        // - get the rop w/ eo_ropframe_ROP_Parse()
              
        // if we have a valid ropinput the following eo_ropframe_ROP_Parse() returns OK. 
        // in all cases rxremainingbytes contains the number of bytes we still need to parse. in case of 
        // unrecoverable error in the ropframe res is NOK and rxremainingbytes is 0.
        
        res = eo_ropframe_ROP_Parse(p->ropframeinput, p->ropinput, &rxremainingbytes);
                
        if(eores_OK == res)
        {   // we have a valid ropinput
            
            numofprocessedrops++;

            // - use the agent w/ eo_agent_InpROPprocess() and retrieve the ropreply.      
            eo_agent_InpROPprocess(p->agent, p->ropinput, remipv4addr, p->ropreply);
            
            // - if ropreply is ok w/ eo_rop_GetROPcode() then add it to ropframereply w/ eo_ropframe_ROP_Add()           
            if(eo_ropcode_none != eo_rop_GetROPcode(p->ropreply))
            {
                res = eo_ropframe_ROP_Add(p->ropframereply, p->ropreply, NULL, NULL, &txremainingbytes);
                
                #if defined(USE_DEBUG_EORECEIVER)             
                {   // DEBUG
                    if(eores_OK != res)
                    {
                        p->debug.lostreplies ++;
                    }
                }
                #endif            
            }
        
        }
        
        // we stop the decoding if rxremainingbytes has reached zero 
        if(0 == rxremainingbytes)
        {
            break;
        }        
    }

    
    if(NULL != numberofrops)
    {
        *numberofrops = numofprocessedrops;
    }

    // if any rop inside ropframereply w/ eo_ropframe_ROP_NumberOf() then sets thereisareply  
    if(NULL != thereisareply)
    {
        *thereisareply = (0 == eo_ropframe_ROP_NumberOf(p->ropframereply)) ? (eobool_false) : (eobool_true);
        // dont use the quickversion because it may be that ropframereply is dummy
        //*thereisareply = (0 == eo_ropframe_ROP_NumberOf_quickversion(p->ropframereply)) ? (eobool_false) : (eobool_true);
    } 
    
    if(NULL != transmittedtime)
    {
        *transmittedtime = eo_ropframe_age_Get(p->ropframeinput);
    }   
    
    return(eores_OK);   
}


static void s_eo_receiver_on_error_invalidframe(EOreceiver* p)
{
    if(NULL != p->on_error_invalidframe)
    {
        p->on_error_invalidframe(p);
    }        
}


static void s_eo_receiver_on_error_seqnumber(EOreceiver* p)
{
    if(NULL != p->on_error_seqnumber)
    {
        p->on_error_seqnumber(p);
    } 
}


extern eOresult_t eo_receiver_GetReply(EOreceiver *p, EOropframe **ropframereply)
{
    if((NULL == p) || (NULL == ropframereply)) 
    {
        return(eores_NOK_nullpointer);
    }
    
    if(0 == eo_ropframe_ROP_NumberOf(p->ropframereply))
    {
        *ropframereply  = p->ropframereply;
        return(eores_NOK_generic);
    }
    
    *ropframereply  = p->ropframereply;
    
    return(eores_OK);
}  

extern const eOreceiver_seqnum_error_t * eo_receiver_GetSequenceNumberError(EOreceiver *p)
{
    if(NULL == p) 
    {
        return(NULL);
    }  

    return(&p->error_seqnumber);   
}

extern const eOreceiver_invalidframe_error_t * eo_receiver_GetInvalidFrameError(EOreceiver *p)
{
    if(NULL == p) 
    {
        return(NULL);
    }  

    return(&p->error_invalidframe);        
}


// extern eOresult_t eo_receiver_set_fn_on_seqnumber_error(EOreceiver *p, eOvoid_fp_uint32_uint64_uint64_t onerrorseqnumber)
// {
//     if(NULL == p) 
//     {
//         return(eores_NOK_nullpointer);
//     }
//     p->on_error_seqnumber = onerrorseqnumber;
//     
//     return(eores_OK);
// }



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





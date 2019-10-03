/*
 * Copyright (C) 2013 iCub Facility - Istituto Italiano di Tecnologia
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

#include "EOMDiagnosticRopMsg.h"
#include "EOMDiagnosticUdpHeader.h"
#include "EOMDiagnosticUdpFooter.h"

class EOMDiagnosticUdpMsg
{
	public:
		constexpr static uint16_t bodyNumber_{10};
	
	private:	
		constexpr static uint16_t	udpPacketDataSize_{EOMDiagnosticUdpHeader::getSize()+EOMDiagnosticRopMsg::getSize()*bodyNumber_+EOMDiagnosticUdpFooter::getSize()};
				
		EOMDiagnosticUdpHeader header_;
		EOMDiagnosticUdpFooter footer_;
		std::array<EOMDiagnosticRopMsg,bodyNumber_> body_;
		uint8_t currentBodySize_{0};
	
	public:	
		std::array<uint8_t,udpPacketDataSize_>	udpPacketData_;
		
		EOMDiagnosticUdpMsg(){};	

		bool addRop(const EOMDiagnosticRopMsg& msg);
		uint8_t* data() const;
		bool createUdpPacketData();
		void resetMsg();

		constexpr static uint16_t getSize()
		{
				return udpPacketDataSize_;
		}				
		
	private:
		bool createUdpHeader()
		{
			std::memcpy(udpPacketData_.data(),header_.data(),EOMDiagnosticUdpHeader::getSize());
			return true;
		}
					
		bool createUdpFooter()
		{
			uint16_t currentFooterAddress=currentBodySize_*EOMDiagnosticRopMsg::getSize()+EOMDiagnosticUdpHeader::getSize();
			std::memcpy(udpPacketData_.data()+currentFooterAddress,footer_.data(),EOMDiagnosticUdpFooter::getSize());
			return true;	
		}

		bool createUdpBody()
		{
			//TODO mutex
			uint16_t currentROPStartAddress=EOMDiagnosticUdpHeader::getSize();
			for(int index=0;index<currentBodySize_;++index)
			{
				currentROPStartAddress=currentROPStartAddress+index*EOMDiagnosticRopMsg::getSize();
				std::memcpy(udpPacketData_.data()+currentROPStartAddress,body_[index].data(),EOMDiagnosticRopMsg::getSize());
			}	
			return true;	
		}
};


	inline bool EOMDiagnosticUdpMsg::addRop(const EOMDiagnosticRopMsg& msg)
	{
		if(currentBodySize_>bodyNumber_)
			return false;
		body_[currentBodySize_]=msg;
		++currentBodySize_;
		return true;
	};

	inline uint8_t* EOMDiagnosticUdpMsg::data() const
	{
			return nullptr;//(uint8_t*)(&data_); 
	}	

	inline bool EOMDiagnosticUdpMsg::createUdpPacketData()
	{
		if(currentBodySize_==0)
			return false;//Nothing to transmit

		header_.updateHeader(currentBodySize_*EOMDiagnosticRopMsg::getSize(),currentBodySize_,0);
		
		createUdpHeader();
		createUdpBody();
		createUdpFooter();

		return true;
	}

	inline void EOMDiagnosticUdpMsg::resetMsg()
	{
		currentBodySize_=0;
		udpPacketData_.fill(0);
	}


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

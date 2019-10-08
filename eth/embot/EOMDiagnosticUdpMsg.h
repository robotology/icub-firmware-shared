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
	uint8_t currentBodySize_{0};

public:
	EOMDiagnosticUdpMsg(){};

	bool addRop(const EOMDiagnosticRopMsg &msg);
	bool createUdpPacketData(std::array<uint8_t, udpPacketDataSize_>& udpMsg);//fill udpPacketData_
	bool parse(const std::array<uint8_t, udpPacketDataSize_> &rxData);//fill header,footer,body
	bool parse(uint8_t *data, uint16_t size);//fill header,footer,body

	void resetMsg();
	void dump();


	std::array<uint8_t, udpPacketDataSize_> udpPacketData_;
private:
	bool createUdpHeader();
	bool createUdpFooter();
	bool createUdpBody();
};

inline bool EOMDiagnosticUdpMsg::addRop(const EOMDiagnosticRopMsg &msg)
{
	if (currentBodySize_ > getRopNumber())
	{
		//TO do error
		return false;		
	}
		
	body_[currentBodySize_] = msg;
	++currentBodySize_;
	return true;
};
inline bool EOMDiagnosticUdpMsg::createUdpPacketData(std::array<uint8_t, udpPacketDataSize_>& udpMsg)
{
	if (currentBodySize_ == 0)
		return false; //Nothing to transmit

	header_.updateHeader(currentBodySize_ * EOMDiagnosticRopMsg::getSize(), currentBodySize_, 0);

	createUdpHeader();
	createUdpBody();
	createUdpFooter();

	udpMsg=udpPacketData_;
	return true;
}
inline void EOMDiagnosticUdpMsg::resetMsg()
{
	currentBodySize_ = 0;
	udpPacketData_.fill(0);
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
		body_[currentBodySize_] = rop;
		currentBodySize_++;
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
	uint16_t currentFooterAddress = currentBodySize_ * EOMDiagnosticRopMsg::getSize() + EOMDiagnosticUdpHeader::getSize();
	std::memcpy(udpPacketData_.data() + currentFooterAddress, footer_.data(), EOMDiagnosticUdpFooter::getSize());
	return true;
}

inline bool EOMDiagnosticUdpMsg::createUdpBody()
{
	//TODO mutex
	uint16_t currentROPStartAddress = EOMDiagnosticUdpHeader::getSize();
	for (int index = 0; index < EOMDiagnosticUdpMsg::getRopNumber(); ++index)
	{
		currentROPStartAddress = EOMDiagnosticUdpHeader::getSize() + index * EOMDiagnosticRopMsg::getSize();
		std::memcpy(udpPacketData_.data() + currentROPStartAddress, body_[index].data(), EOMDiagnosticRopMsg::getSize());
	}
	return true;
}

inline void EOMDiagnosticUdpMsg::dump()
{
	header_.dump();
	for(auto currentRop:body_)
		{currentRop.dump();};
	footer_.dump();
}

#endif // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

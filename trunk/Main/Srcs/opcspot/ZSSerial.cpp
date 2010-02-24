//------------------------------------------------------------------------------------------
// File: <ZSSerial.cpp>
// Purpose: implement <Data access for ZS weight through RS-232.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/22
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "ZSSerial.h"

using namespace boost;

const unsigned char ZSSerial::begin = 0xbb;
const unsigned char ZSSerial::end = 0xee;

ZSSerial::ZSSerial(std::string port, unsigned int baudRate, const ZSSerialProtocol& protocol) : 
io(), serial(io, port), protocol(protocol)
{
	serial.set_option(asio::serial_port_base::baud_rate(baudRate));
}

ZSSerial::~ZSSerial(void)
{
}

// Read data from the device.
std::vector<ZSDataItem> ZSSerial::ReadData(DataGroup group, unsigned char station)
{
	std::vector<ZSDataItem> vec;
	return vec;
}

// Write data to the device
void ZSSerial::WriteData(const ZSDataItem& item, unsigned char station)
{
	
}

// Write command to the device
void ZSSerial::WriteCommand(int commandID, unsigned char station)
{
	
}

// Calculate the sum of BCD codes
unsigned char ZSSerial::CalculateBCDSum(const std::vector<unsigned char>& bcdVec)
{
	unsigned char result = 0x00;
	for (std::size_t i = 0; i < bcdVec.size(); i++)
	{
		unsigned char inc = (
			(result & 0x0f) + (result >> 4) * 10 + (bcdVec.at(i) & 0x0f) + (bcdVec.at(i) >> 4) * 10
			) % 100; 
		result = ((inc / 10) << 4) | (inc % 10);
	}
	return result;
}

// Check BCD code sum
bool ZSSerial::CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected)
{
	return CalculateBCDSum(bcdVec) == expected;
}

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

ZSSerial::ZSSerial(const std::string& devName, const ZSSerialProtocol& protocol) : 
protocol(protocol), port(devName, 9600)
{

}

ZSSerial::~ZSSerial(void)
{
}

// Read data from the device.
std::vector<ZSDataItem> ZSSerial::ReadData(DataGroup group, unsigned char station)
{
	_ASSERT(station <= 99);
	std::vector<ZSDataItem> vec;
	//if (station > 99 || !serial.is_open())
	//{
	//	return vec;
	//}

	std::string readCmd = MakeReadCmd(group, station);
	//asio::write(serial, asio::buffer(readCmd.c_str(), readCmd.size()));
	//std::string result;
	//char c;
	//for(;;)
	//{
	//	asio::read(serial, asio::buffer(&c, 1));
	//	result += c;
	//	if (end == c)
	//	{
	//		break;
	//	}
	//}

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

// Convert a decimal to BCD code. IF the input value larger than 99, the result will always be 0
unsigned char ZSSerial::Dec2BCD(unsigned char dec)
{
	_ASSERT(dec <= 99);
	if (dec > 99)
	{
		return 0;
	}
	return ((dec / 10) << 4) | (dec % 10);
}

// Make a read data command
std::string ZSSerial::MakeReadCmd(DataGroup group, unsigned char station)
{
	_ASSERT(station <= 99);
	std::string readCmd;
	if (station > 99)
	{
		return readCmd;
	}

	const std::vector<ZSReadDataCmd>& vecCmd = protocol.GetReadDataCmd();
	readCmd += begin;
	readCmd += Dec2BCD(station);
	readCmd += 0x4;
	if (one == group)
	{
		readCmd += vecCmd.at(0).cmd;
	}
	else if (two == group)
	{
		readCmd += vecCmd.at(1).cmd;
	}
	
	std::string sumPart = readCmd.substr(1, 3);
	readCmd += CalculateBCDSum(std::vector<unsigned char>(sumPart.begin(), sumPart.end()));
	readCmd += end;

	return readCmd;
}

// Check BCD code sum
bool ZSSerial::CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected)
{
	return CalculateBCDSum(bcdVec) == expected;
}

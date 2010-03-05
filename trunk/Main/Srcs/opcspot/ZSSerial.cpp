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
#include "commonlib/stringstext.h"
#include <cmath>

using namespace boost;
using namespace CommonLib;

ZSSerial::ZSSerial(const std::string& devName, const ZSSerialProtocol& protocol) : 
protocol(protocol)
{
	const std::vector<ZSSerialSetting>& settings = protocol.GetPortSetting();
	std::size_t i;
	for (i = 0; i < settings.size(); ++i)
	{
		if (StringsText::CaseInsCompare(devName, settings.at(i).devName))
		{
			asio::serial_port_base::parity parity(asio::serial_port_base::parity::none);
			if (odd == settings.at(i).parity)
			{
				parity = asio::serial_port_base::parity(asio::serial_port_base::parity::odd);
			}
			else if (even == settings.at(i).parity)
			{
				parity = asio::serial_port_base::parity(asio::serial_port_base::parity::even);
			}

			asio::serial_port_base::stop_bits stopBits(asio::serial_port_base::stop_bits::one);
			if (onepointfive == settings.at(i).stopBits)
			{
				stopBits = asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::onepointfive);
			}
			else if (two == settings.at(i).stopBits)
			{
				stopBits = asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::two);
			}

			port.Open(devName, settings.at(i).baudRate, parity, 
				asio::serial_port_base::character_size(settings.at(i).csize), 
				asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none), 
				stopBits);
			break;
		}
	}
	_ASSERTE(i < settings.size());
	if (i == settings.size())
	{
		Close();
		throw std::runtime_error("The devName is not matched.");
	}
}

ZSSerial::~ZSSerial(void)
{
}

// Read data from the device.
std::vector<ZSDataItem> ZSSerial::ReadData(DataGroup group, unsigned char station)
{
	_ASSERT(station <= 99);
	std::vector<ZSDataItem> vec;
	if (station > 99)
	{
		throw std::invalid_argument("Station number is larger than 99");
	}
	if (!port.IsOpen())
	{
		return vec;
	}

	std::vector<char> readCmd = MakeReadCmd(group, station);
	port.Write(readCmd);
	std::string endString(1, '\xee');
	std::string retString = port.ReadStringUntil(endString);

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

// Convert a BCD stream to a float
float ZSSerial::BCD2Float(ZSSerial::ByteStream::const_iterator begin, 
						  ZSSerial::ByteStream::const_iterator end, unsigned int digitNum)
{
	std::size_t len = std::distance(begin, end);
	_ASSERTE(len > 0 && digitNum < 7);
	if (0 == len || digitNum > 6)
	{
		throw std::invalid_argument("Empty BCD stream or the digit number exceeds 6");
	}

	float value = 0.0f;
	float base = 1.0f;
	float baseDigit = powf(0.1, digitNum);
	bool isEven = (0 == (digitNum / 2)) ? true : false;
	ByteStream::const_iterator itStream = begin;
	unsigned int digitIndex = 0;

	if (digitNum > 0)
	{
		for (; itStream != end, digitIndex < digitNum - 1; itStream++, digitIndex++)
		{
			value += baseDigit * ((*itStream) & 0x0f);
			baseDigit *= 10.0f;
			value += baseDigit * ((*itStream) >> 4);
			baseDigit *= 10.0f;
		}
	}
	if (!isEven)
	{
		value += baseDigit * ((*itStream) & 0x0f);
		value += base * ((*itStream) >> 4);
		base *= 10.0f;
		itStream++;
	}

	for (; itStream != end; itStream++)
	{

		value += base * ((*itStream) & 0x0f);
		base *= 10.0f;
		value += base * ((*itStream) >> 4);
		base *= 10.0f;
	}
	
	return 0.0f;
}

// Convert a BCD stream to an integer
unsigned int ZSSerial::BCD2Int(ZSSerial::ByteStream::iterator begin, 
					 ZSSerial::ByteStream::iterator end)
{
	std::size_t len = std::distance(begin, end);
	_ASSERTE(len > 0 && len < 6);
	if (0 == len || len > 5)
	{
		throw std::invalid_argument("Empty BCD stream or the stream length is two long.");
	}

	unsigned int value = 0;
	unsigned int base = 1;
	
	for (ByteStream::iterator itStream = begin; itStream != end; itStream++)
	{
		value += base * ((*itStream) & 0x0f);
		base *= 10;
		value += base * ((*itStream) >> 4);
		base *= 10;
	}
	return value;
}

// Make a read data command
std::vector<char> ZSSerial::MakeReadCmd(DataGroup group, unsigned char station)
{
	_ASSERT(station <= 99);
	std::vector<char> readCmd;
	if (station > 99)
	{
		return readCmd;
	}

	readCmd.resize(6, 0x0);
	const std::vector<ZSReadDataCmd>& vecCmd = protocol.GetReadDataCmd();
	readCmd.at(0) = begin;
	readCmd.at(1) = Dec2BCD(station);
	readCmd.at(2) = Dec2BCD(0x4);
	if (one == group)
	{
		readCmd.at(3) = vecCmd.at(0).cmd;
	}
	else if (two == group)
	{
		readCmd.at(3) = vecCmd.at(1).cmd;
	}
	readCmd.at(4) = CalculateBCDSum(std::vector<unsigned char>(&readCmd.at(1), &readCmd.at(4)));
	readCmd.at(5) = end;

	return readCmd;
}

// Check BCD code sum
bool ZSSerial::CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected)
{
	return CalculateBCDSum(bcdVec) == expected;
}

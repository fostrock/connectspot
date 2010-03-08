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
#include <iostream>

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
	std::string beginStr(1, begin);
	port.ReadStringUntil(beginStr);
	std::string endStr(1, end);
	std::string dataStr = port.ReadStringUntil(endStr);	// The return string excludes the END char
	_ASSERTE(dataStr.size() > 3);

	// Check the station No.
	if (CommonLib::BCD2Int(reinterpret_cast<unsigned char*>(&(dataStr.at(0))), 1) != station)
	{
		_ASSERTE(!"Check station No. error");
		return vec;
	}

	// Check the data stream's length
	if (CommonLib::BCD2Int(reinterpret_cast<unsigned char*>(&(dataStr.at(1))), 1) != dataStr.size())
	{
		_ASSERTE(!"Check data length error");
		return vec;
	}

	// Check the BCD sum
    std::vector<unsigned char> vecTemp(&(dataStr.at(0)), &(dataStr.at(dataStr.size() - 1)));
	if (!ZSSerial::CheckSumEqual(vecTemp, *(dataStr.rbegin())))
	{
		return vec;
	}

	// Parse data
	const std::vector<ZSReadDataInfo>& vecCmd = protocol.GetReadDataCmd().at(0).info;
	const ZSSerialProtocol::DataSetDef& dataset = protocol.GetDataSetInfo();
	ZSSerialProtocol::DataSetDef::const_iterator itDef;
	for (std::size_t i = 0; i < vecCmd.size(); ++i)
	{
		itDef = dataset.find(vecCmd.at(i).index);
		if (itDef != dataset.end())
		{
			ZSDataItem item;
			item.index = vecCmd.at(i).index;

			bool isFloat = (*itDef).second.get<3>();
			unsigned short lenDataItem = (*itDef).second.get<2>();

			if (isFloat)
			{
				//item.variant = ZSSerial::BCD2Float(dataStr.rbegin() + 1 + 37 - vecCmd.at(i).offset - vecCmd.at(i).length,
				//	dataStr.rbegin() + 1 + 37 - vecCmd.at(i).offset);
			}
			else
			{
				//item.variant = ZSSerial::BCD2Int(dataStr.rbegin() + 1 + 37 - vecCmd.at(i).offset - vecCmd.at(i).length,
				//	dataStr.rbegin() + 1 + 37 - vecCmd.at(i).offset);
			}
			vec.push_back(item);
		}
	}

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

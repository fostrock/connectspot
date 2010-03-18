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

#pragma warning(disable:4996) // std::copy() unsafe iterator, _SCL_SECURE_NO_WARNINGS

using namespace boost;
using namespace CommonLib;

ZSSerial::ZSSerial(const std::string& devName, const ZSSerialProtocol& protocol) : 
devName(devName), protocol(protocol)
{
	try
	{
		Open();
	}
	catch (boost::system::system_error& e)
	{
#ifdef _DEBUG
		std::cout << e.what() << std::endl;
#endif
		// add log here
	}
}

ZSSerial::~ZSSerial(void)
{
}

void ZSSerial::Open()
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

	std::vector<unsigned char> readCmd = MakeReadCmd(group, station);
	port.Write(readCmd);
	std::string beginStr(1, begin);
	port.ReadStringUntil(beginStr);
	std::string endStr(1, end);
	std::string dataStr = port.ReadStringUntil(endStr);	// The return string excludes the END char
	_ASSERTE(dataStr.size() > 3);

	// Check the station No.
	if (BCD2Int(reinterpret_cast<unsigned char*>(&(dataStr.at(0))), 1) != station)
	{
		_ASSERTE(!"Check station No. error");
		return vec;
	}

	// Check the data stream's length
	if (BCD2Int(reinterpret_cast<unsigned char*>(&(dataStr.at(1))), 1) != dataStr.size())
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

	// Parse read data
	return ParseReadData(group, dataStr);
}

// Write data to the device
void ZSSerial::WriteData(const ZSDataItem& item, unsigned char station)
{
	_ASSERT(station <= 99);
	if (station > 99)
	{
		throw std::invalid_argument("Station number is larger than 99");
	}
	if (!port.IsOpen())
	{
		return;
	}

	std::vector<unsigned char> command = MakeWriteCmd(item.index, item.variant, station);
	port.Write(command);
}

// Write command to the device
void ZSSerial::WriteCommand(unsigned short commandID, unsigned char station)
{
	_ASSERT(station <= 99);
	if (station > 99)
	{
		throw std::invalid_argument("Station number is larger than 99");
	}
	if (!port.IsOpen())
	{
		return;
	}

	std::vector<unsigned char> command = MakeCommonCmd(commandID, station);
	port.Write(command);
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

// Make a read data command
std::vector<unsigned char> ZSSerial::MakeReadCmd(DataGroup group, unsigned char station)
{
	_ASSERT(station <= 99);
	std::vector<unsigned char> readCmd;
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

// Make a write data command
std::vector<unsigned char> ZSSerial::MakeWriteCmd(unsigned short dataID, 
												  const boost::variant<unsigned int, double>& val, 
												  unsigned char station
												  )
{
	_ASSERT(station <= 99);
	std::vector<unsigned char> writeCmd;
	if (station > 99)
	{
		return writeCmd;
	}

	const ZSWriteDataCmd& cmdDef = protocol.GetWriteDataCmd();
	ZSWriteDataCmd::ParamDef::const_iterator iterCmd = cmdDef.param.find(dataID);
	_ASSERTE(iterCmd != cmdDef.param.end());
	if (cmdDef.param.end() == iterCmd)
	{
		return writeCmd;
	}
	const ZSSerialProtocol::DataSetDef& dataset = protocol.GetDataSetInfo();
	ZSSerialProtocol::DataSetDef::const_iterator itData = dataset.find(dataID);
	_ASSERTE(itData != dataset.end());
	if (dataset.end() == itData)
	{
		return writeCmd;
	}
	unsigned short len = (*itData).second.get<ZSSerialProtocol::ZS_DATA_LENGTH_INDEX>();

	writeCmd.resize(len + 7, 0x0);
	writeCmd.at(0) = begin;
	writeCmd.at(1) = Dec2BCD(station);	// RS485 station No.
	writeCmd.at(2) = Dec2BCD(len + 5);	// write command length
	writeCmd.at(3) = cmdDef.cmd;		// write command
	writeCmd.at(4) = Dec2BCD((*iterCmd).second);	// write command parameter
	std::vector<unsigned char> dataStream(len, 0x0);

	bool isFloat = (*itData).second.get<ZSSerialProtocol::ZS_DATA_TYPE_INDEX>();
	if (isFloat)
	{
		dataStream = Dec2BCD_R(boost::get<double>(val), len);
	}
	else
	{
		dataStream = Dec2BCD_R(boost::get<unsigned int>(val), len);
	}
	std::copy(dataStream.begin(), dataStream.end(), &writeCmd.at(5));

	writeCmd.at(len + 5) = CalculateBCDSum(	// sum check
		std::vector<unsigned char>(&writeCmd.at(1), &writeCmd.at(5 + len))
		);
	writeCmd.at(len + 6) = end;
	return writeCmd;
}

// Make a common command
std::vector<unsigned char> ZSSerial::MakeCommonCmd(unsigned short commandID, unsigned char station)
{
	_ASSERT(station <= 99);
	std::vector<unsigned char> commonCmd;
	if (station > 99)
	{
		return commonCmd;
	}

	const ZSSerialProtocol::CommonCmdDef& vecCmd = protocol.GetCommonCmd();
	ZSSerialProtocol::CommonCmdDef::const_iterator iter = vecCmd.find(commandID);
	_ASSERTE(iter != vecCmd.end());
	if (vecCmd.end() == iter)
	{
		return commonCmd;
	}

	commonCmd.resize(6, 0x0);
	commonCmd.at(0) = begin;
	commonCmd.at(1) = Dec2BCD(station);
	commonCmd.at(2) = Dec2BCD(0x4);
    commonCmd.at(3) = (*iter).second;
	commonCmd.at(4) = CalculateBCDSum(std::vector<unsigned char>(&commonCmd.at(1), &commonCmd.at(4)));
	commonCmd.at(5) = end;
	return commonCmd;
}

// Check BCD code sum
bool ZSSerial::CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected)
{
	return CalculateBCDSum(bcdVec) == expected;
}

// Parse the read data stream to ZSDataItem vector
std::vector<ZSDataItem> ZSSerial::ParseReadData(DataGroup group, const std::string &dataStr)
{
	std::vector<ZSDataItem> vec; 
	std::size_t readCmdIndex = 0;;
	if (one == group)
	{
		readCmdIndex = 0;
	}
	else if (two == group)
	{
		readCmdIndex = 1;
	}
	const std::vector<ZSReadDataInfo>& vecDataInfo = protocol.GetReadDataCmd().at(readCmdIndex).info;
	for (std::size_t i = 0; i < vecDataInfo.size(); ++i)
	{
		ZSDataItem item;
		item.index = vecDataInfo.at(i).index;
		std::size_t offset = vecDataInfo.at(i).offset;
		if (vecDataInfo.at(i).isFloat)
		{
			item.variant = BCD2FloatR(
				reinterpret_cast<const unsigned char*>( &(dataStr.at(2 + offset)) ), 
				vecDataInfo.at(i).length);
		}
		else
		{
			item.variant = BCD2IntR(
				reinterpret_cast<const unsigned char*>( &(dataStr.at(2 + offset)) ), 
				vecDataInfo.at(i).length);
		}
		vec.push_back(item);
	}

	return vec;
}

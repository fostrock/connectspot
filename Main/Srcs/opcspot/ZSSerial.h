//------------------------------------------------------------------------------------------
// File: <ZSSerial.h>
// Purpose: declare <Data access for ZS weight through RS-232.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/22
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include <string>
#include <vector>

#define BOOST_SYSTEM_DYN_LINK
#define BOOST_DATE_TIME_DYN_LINK
#include "boost/asio.hpp"
#include "boost/variant.hpp"
#include "ZSSerialProtocol.h"

struct ZSDataItem 
{
	unsigned short index;
	boost::variant<int, float> variant;
};

class ZSSerial
{
public:
	enum DataGroup { one, two };

	// Constructor.
	// @param <port> device name, example "/dev/ttyUSB0" or "COM4"
	// @param <baudRate> communication speed, example 9600 or 115200
	// @param <protocol> the serial communication protocol
	// @throws boost::system::system_error if cannot open the serial device
	ZSSerial(std::string port, unsigned int baudRate, const ZSSerialProtocol& protocol);
	~ZSSerial(void);

public:
	// Whether the serial port is opened.
	// @return true if the serial is opened, otherwise false
	bool IsOpened()
	{
		return serial.is_open();
	}

	// Read data from the device.
	// @param <group> one represents the frequent access data set,
	//				two represents the less frequent access data set
	// @param <station> RS-485 station No.
	// @return a vector containing the safe and generic union
	// @throws boost::system::system_error on failure
	std::vector<ZSDataItem> ReadData(DataGroup group, unsigned char station);

	// Write data to the device
	// @param <item> the data item will be write to the device
	// @param <station> RS-485 station No.
	// @throws boost::system::system_error on failure
	void WriteData(const ZSDataItem& item, unsigned char station);

	// Write command to the device
	// @param <commandID> command ID
	// @param <station> RS-485 station No.
	// @throws boost::system::system_error on failure
	void WriteCommand(int commandID, unsigned char station);

	// Make a read data command
	// @param <group> one represents the frequent access data set,
	//				two represents the less frequent access data set
	// @param <station> RS-485 station No.
	// @return command string
	std::string MakeReadCmd(DataGroup group, unsigned char station);

	// Calculate the sum of BCD codes
	// @param <bcdVec> the given BCD vector
	// @return the sum
	static unsigned char CalculateBCDSum(const std::vector<unsigned char>& bcdVec);

	// Convert a decimal to BCD code. IF the input value larger than 99, the result will always be 0
	// @param <dec> a decimal code, it shall not larger than 99
	// @return a BCD code
	static unsigned char Dec2BCD(unsigned char dec);

private:
	// Check BCD code sum
	// @param <bcdVec> the given BCD vector
	// @param <expected> the expected value
	// @return true if the BCD sum is equal to expected value, otherwise false
	bool CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected);

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;

	static const unsigned char begin = 0xbb;
	static const unsigned char end = 0xee;

	const ZSSerialProtocol& protocol;
};

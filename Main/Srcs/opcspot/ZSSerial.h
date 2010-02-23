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
#include "boost/asio/serial_port.hpp"
#include "boost/variant.hpp"

class ZSSerial
{
public:
	enum DataGroup { one, two };
	typedef boost::variant<int, float> ZSVariant;

	// Constructor.
	// @param <port> device name, example "/dev/ttyUSB0" or "COM4"
	// @param <baudRate> communication speed, example 9600 or 115200
	// @throws boost::system::system_error if cannot open the serial device
	ZSSerial(std::string port, unsigned int baudRate);
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
	// @return a vector containing the safe and generic union
	// @throws boost::system::system_error on failure
	std::vector<ZSVariant> ReadData(DataGroup group);

	// Write data to the device
	// @param <dataID> the data ID
	// @param <data> the data content represented by a safe union
	// @throws boost::system::system_error on failure
	void WriteData(int dataID, const ZSVariant& data);

	// Write command to the device
	// @param <commandID> command ID
	// @throws boost::system::system_error on failure
	void WriteCommand(int commandID);

	// Calculate the sum of BCD codes
	// @param <bcdVec> the given BCD vector
	// @return the sum
	static unsigned char CalculateBCDSum(const std::vector<unsigned char>& bcdVec);

private:
	// Check BCD code sum
	// @param <bcdVec> the given BCD vector
	// @param <expected> the expected value
	// @return true if the BCD sum is equal to expected value, otherwise false
	bool CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected);

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
	static const unsigned char begin;
	static const unsigned char end;
};

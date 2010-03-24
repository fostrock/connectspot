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
#include "boost/variant.hpp"
#include "ZSSerialProtocol.h"
#include "commonlib/timeoutserial.h"
#include "commonlib/BCDConvert.h"

struct ZSDataItem 
{
	unsigned short index;
	boost::variant<unsigned int, double> variant;
};

class ZSSerial
{
public:
	enum DataGroup { one, two };

	typedef std::vector<unsigned char> ByteStream;

	// Constructor.
	// @param <devName> device name, example "/dev/ttyUSB0" or "COM4"
	// @param <protocol> the serial communication protocol
	// @throws boost::system::system_error if cannot open the serial device
	ZSSerial(const std::string& devName, const ZSSerialProtocol& protocol);
	~ZSSerial(void);

public:
	// Whether the serial port is opened.
	// @return true if the serial is opened, otherwise false
	bool IsOpened() const
	{
		return port.IsOpen();
	}

	// Set the timeout on read/write operations.
	// To disable the timeout, call setTimeout(boost::posix_time::seconds(0));
	// @param <t> timeout duration 
	void SetTimeout(const boost::posix_time::time_duration& t)
	{
		port.SetTimeout(t);
	}

	// @throws boost::system::system_error if cannot open the serial device  
	void Open();

	void Close()
	{
		port.Close();
	}

	// Read data from the device.
	// @param <group> one represents the frequent access data set,
	//				two represents the less frequent access data set
	// @param <station> RS-485 station No.
	// @return a vector containing the safe and generic union
	// @throws boost::system::system_error on failure
	// @throws timeout_exception in case of timeout
	std::vector<ZSDataItem> ReadData(DataGroup group, unsigned char station);

	// Write data to the device
	// @param <item> the data item will be write to the device
	// @param <station> RS-485 station No.
	// @return true if it succeeded, otherwise false
	// @throws boost::system::system_error on failure
	bool WriteData(const ZSDataItem& item, unsigned char station);

	// Write command to the device
	// @param <commandID> command ID
	// @param <station> RS-485 station No.
	// @return true if it succeeded, otherwise false
	// @throws boost::system::system_error on failure
	bool WriteCommand(unsigned short commandID, unsigned char station);

	// Make a read data command
	// @param <group> one represents the frequent access data set,
	//				two represents the less frequent access data set
	// @param <station> RS-485 station No.
	// @return command byte vector
	std::vector<unsigned char> MakeReadCmd(DataGroup group, unsigned char station);

	// Make a write data command
	// @param <dataID> data ID
	// @param <val> variant value, it can be a float or an integer
	// @param <station> RS-485 station No.
	// @throws boost::system::system_error on failure
	std::vector<unsigned char> MakeWriteCmd(unsigned short dataID, 
		const boost::variant<unsigned int, double>& val, unsigned char station);

	// Make a common command
	// @param <commandID> command ID
	// @param <station> RS-485 station No.
	// @return command byte vector
	std::vector<unsigned char> MakeCommonCmd(unsigned short commandID, unsigned char station);

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

	// Parse the read data stream to ZSDataItem vector
	// @param <group> the read data group
	// @param <dataStr> the input read data stream
	// @return the ZSDataItem vector
	std::vector<ZSDataItem> ParseReadData(DataGroup group, const std::string &dataStr);

	// Check the result of the send messages to confirm whether the writing data command or
	// the writing common command is successful. 
	// The return stream is "bb station len 00 checksum ee" if it is OK
	// Or "bb station len 99 checksum ee" if it failed.
	// @param <station> the RS-485 station No.
	// @return true if the writing data operation is successfully, otherwise false 
	bool IsWrittenCmdOK(unsigned char station);

private:
	static const unsigned char begin = 0xbb;
	static const unsigned char end = 0xee;
	const std::string devName;

	const ZSSerialProtocol& protocol;
	TimeoutSerial port;
};

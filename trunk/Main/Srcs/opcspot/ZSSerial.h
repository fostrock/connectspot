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

struct ZSDataItem 
{
	unsigned short index;
	boost::variant<int, float> variant;
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

	void Open()
	{
		
	}

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
	// @return command byte vector
	std::vector<char> MakeReadCmd(DataGroup group, unsigned char station);

	// Calculate the sum of BCD codes
	// @param <bcdVec> the given BCD vector
	// @return the sum
	static unsigned char CalculateBCDSum(const std::vector<unsigned char>& bcdVec);

	// Convert a decimal to BCD code. IF the input value larger than 99, the result will always be 0
	// @param <dec> a decimal code, it shall not larger than 99
	// @return a BCD code
	static unsigned char Dec2BCD(unsigned char dec);

	// Convert a BCD stream to a float. The stream is big-endian.
	// @param <begin> the stream's begin iterator
	// @param <end> the stream's end iterator
	// @param <digitNum> the number of digit. It can not exceed 6.
	// @return the converted unsigned integer
	static float BCD2Float(ByteStream::const_iterator begin, 
		ByteStream::const_iterator end, unsigned int digitNum = 4);

	// Convert a BCD stream to an integer. The stream is big-endian.
	// @param <begin> the stream's begin iterator
	// @param <end> the stream's end iterator
	// @return the converted unsigned integer
	static unsigned int BCD2Int(ByteStream::iterator begin, 
		ByteStream::iterator end);



private:
	// Check BCD code sum
	// @param <bcdVec> the given BCD vector
	// @param <expected> the expected value
	// @return true if the BCD sum is equal to expected value, otherwise false
	bool CheckSumEqual(const std::vector<unsigned char>& bcdVec, unsigned char expected);

private:
	static const unsigned char begin = 0xbb;
	static const unsigned char end = 0xee;

	const ZSSerialProtocol& protocol;
	TimeoutSerial port;
};

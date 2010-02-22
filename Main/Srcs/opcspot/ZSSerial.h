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
#define BOOST_SYSTEM_DYN_LINK
#define BOOST_DATE_TIME_DYN_LINK
//#define BOOST_REGEX_DYN_LINK	// for "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"

class ZSSerial
{
public:
	ZSSerial(std::string port, unsigned int baudRate);
	~ZSSerial(void);

public:
	bool IsOpened()
	{
		return serial.is_open();
	}

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};

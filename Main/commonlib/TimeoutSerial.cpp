//------------------------------------------------------------------------------------------
// File: <TimeoutSerial.cpp>
// Purpose: implement <a timeout serial based on boost::asio>
//
// @author <Yun Hua>
// @version 1.0 2010/03/03
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include "TimeoutSerial.h"
#include <string>
#include <algorithm>
#include <istream>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

TimeoutSerial::TimeoutSerial(): io(), port(io), timer(io),
timeout(posix_time::seconds(0))
{

}

TimeoutSerial::~TimeoutSerial()
{

}

TimeoutSerial::TimeoutSerial(const std::string& devname, unsigned int baud_rate,
							 asio::serial_port_base::parity opt_parity,
							 asio::serial_port_base::character_size opt_csize,
							 asio::serial_port_base::flow_control opt_flow,
							 asio::serial_port_base::stop_bits opt_stop)
							 : io(), port(io), timer(io), timeout(posix_time::seconds(0))
{
	Open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}

void TimeoutSerial::Open(const std::string& devname, unsigned int baud_rate,
						 asio::serial_port_base::parity opt_parity,
						 asio::serial_port_base::character_size opt_csize,
						 asio::serial_port_base::flow_control opt_flow,
						 asio::serial_port_base::stop_bits opt_stop)
{
	if(IsOpen()) Close();
	port.open(devname);
	port.set_option(asio::serial_port_base::baud_rate(baud_rate));
	port.set_option(opt_parity);
	port.set_option(opt_csize);
	port.set_option(opt_flow);
	port.set_option(opt_stop);
}

bool TimeoutSerial::IsOpen() const
{
	return port.is_open();
}

void TimeoutSerial::Close()
{
	if(IsOpen() == false) return;
	port.close();
}

void TimeoutSerial::SetTimeout(const posix_time::time_duration& t)
{
	timeout = t;
}

void TimeoutSerial::Write(const char *data, size_t size)
{
	asio::write(port, asio::buffer(data, size));
}

void TimeoutSerial::Write(const std::vector<char>& data)
{
	asio::write(port, asio::buffer(&data[0], data.size()));
}

void TimeoutSerial::Write(const std::vector<unsigned char>& data)
{
	asio::write(port, asio::buffer(&data[0], data.size()));
}

void TimeoutSerial::WriteString(const std::string& s)
{
	asio::write(port, asio::buffer(s.c_str(), s.size()));
}

void TimeoutSerial::Read(char *data, size_t size)
{
	if(readData.size() > 0)//If there is some data from a previous read
	{
		istream is(&readData);
		size_t toRead = min(readData.size(), size);//How many bytes to read?
		is.read(data, toRead);
		data += toRead;
		size -= toRead;
		if(size == 0) return;//If read data was enough, just return
	}

	if(timeout != posix_time::seconds(0)) //If timeout is set, start timer
	{
		timer.expires_from_now(timeout);
		timer.async_wait(boost::bind(&TimeoutSerial::TimeoutExpired, this,
			asio::placeholders::error));
	}

	asio::async_read(port, asio::buffer(data, size), 
		boost::bind(
		&TimeoutSerial::ReadCompleted, this, asio::placeholders::error,
		asio::placeholders::bytes_transferred)
		);

	result = resultInProgress;
	bytesTransferred = 0;
	for(;;)
	{
		io.run_one();
		switch(result)
		{
		case resultSuccess:
			timer.cancel();
			return;
		case resultTimeoutExpired:
			port.cancel();
			throw(timeout_exception("Timeout expired"));
		case resultError:
			timer.cancel();
			port.cancel();
			throw(boost::system::system_error(boost::system::error_code(),
				"Error while reading"));
			//if resultInProgress remain in the loop
		default:
			break;
		}
	}
}

std::vector<char> TimeoutSerial::Read(size_t size)
{
	vector<char> result(size, '\0');	//Allocate a vector with the desired size
	Read(&result[0], size);	//Fill it with values
	return result;
}

std::string TimeoutSerial::ReadString(size_t size)
{
	string result(size, '\0');//Allocate a string with the desired size
	Read(&result[0], size);//Fill it with values
	return result;
}

std::string TimeoutSerial::ReadStringUntil(const std::string& delim)
{
	if(timeout != posix_time::seconds(0)) //If timeout is set, start timer
	{
		timer.expires_from_now(timeout);
		timer.async_wait(boost::bind(&TimeoutSerial::TimeoutExpired, this,
			asio::placeholders::error));
	}

	asio::async_read_until(port, readData, delim, boost::bind(
		&TimeoutSerial::ReadCompleted, this, asio::placeholders::error,
		asio::placeholders::bytes_transferred));

	result = resultInProgress;
	bytesTransferred = 0;
	for(;;)
	{
		io.run_one();
		switch(result)
		{
		case resultSuccess:
			{
				timer.cancel();
				bytesTransferred -= delim.size();//Don't count delim
				istream is(&readData);
				string result(bytesTransferred, '\0');//Alloc string
				is.read(&result[0], bytesTransferred);//Fill values
				is.ignore(delim.size());//Remove delimiter from stream
				return result;
			}
		case resultTimeoutExpired:
			port.cancel();
			throw(timeout_exception("Timeout expired"));
		case resultError:
			timer.cancel();
			port.cancel();
			throw(boost::system::system_error(boost::system::error_code(),
				"Error while reading"));
			//if resultInProgress remain in the loop
		default:
			break;
		}
	}
}

void TimeoutSerial::TimeoutExpired(const boost::system::error_code& error)
{
	if(result != resultInProgress) return;
	if(error != asio::error::operation_aborted)
	{
		result = resultTimeoutExpired;
	}
}

void TimeoutSerial::ReadCompleted(const boost::system::error_code& error,
								  const size_t bytesTransferred)
{
	if(error)
	{
		if(error != asio::error::operation_aborted)
		{
			result = resultError;
		}
	} 
	else 
	{
		if(result != resultInProgress) return;
		result = resultSuccess;
		this->bytesTransferred = bytesTransferred;
	}
}

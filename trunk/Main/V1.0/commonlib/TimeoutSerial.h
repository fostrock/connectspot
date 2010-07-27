//------------------------------------------------------------------------------------------
// File: <TimeoutSerial.h>
// Purpose: declare <a timeout serial based on boost::asio>
//
// @author <Yun Hua>
// @version 1.0 2010/03/03
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#ifndef _TIMEOUTSERIAL_H
#define	_TIMEOUTSERIAL_H

#include <stdexcept>
#include <boost/utility.hpp>
#include <boost/asio.hpp>


//Thrown if timeout occurs

class timeout_exception: public std::runtime_error
{
public:
	timeout_exception(const std::string& arg): runtime_error(arg) {}
};


//Serial port class, with timeout on read operations.

class TimeoutSerial: private boost::noncopyable
{
public:
	TimeoutSerial();
	~TimeoutSerial();


	// Opens a serial device. By default timeout is disabled.
	// @param <devname> serial device name, example "/dev/ttyS0" or "COM1"
	// @param <baud_rate> serial baud rate
	// @param <opt_parity> serial parity, default none
	// @param <opt_csize> serial character size, default 8bit
	// @param <opt_flow> serial flow control, default none
	// @param <opt_stop> serial stop bits, default 1
	// @throws boost::system::system_error if cannot open the serial device
	TimeoutSerial(const std::string& devname, unsigned int baud_rate,
		boost::asio::serial_port_base::parity opt_parity=
		boost::asio::serial_port_base::parity(
		boost::asio::serial_port_base::parity::none),
		boost::asio::serial_port_base::character_size opt_csize=
		boost::asio::serial_port_base::character_size(8),
		boost::asio::serial_port_base::flow_control opt_flow=
		boost::asio::serial_port_base::flow_control(
		boost::asio::serial_port_base::flow_control::none),
		boost::asio::serial_port_base::stop_bits opt_stop=
		boost::asio::serial_port_base::stop_bits(
		boost::asio::serial_port_base::stop_bits::one)
		);


	// Opens a serial device.
	// @param <devname> serial device name, example "/dev/ttyS0" or "COM1"
	// @param <baud_rate> serial baud rate
	// @param <opt_parity> serial parity, default none
	// @param <opt_csize> serial character size, default 8bit
	// @param <opt_flow> serial flow control, default none
	// @param <opt_stop> serial stop bits, default 1
	// @throws boost::system::system_error if cannot open the serial device  
	void Open(const std::string& devname, unsigned int baud_rate,
		boost::asio::serial_port_base::parity opt_parity=
		boost::asio::serial_port_base::parity(
		boost::asio::serial_port_base::parity::none),
		boost::asio::serial_port_base::character_size opt_csize=
		boost::asio::serial_port_base::character_size(8),
		boost::asio::serial_port_base::flow_control opt_flow=
		boost::asio::serial_port_base::flow_control(
		boost::asio::serial_port_base::flow_control::none),
		boost::asio::serial_port_base::stop_bits opt_stop=
		boost::asio::serial_port_base::stop_bits(
		boost::asio::serial_port_base::stop_bits::one)
		);


	// Whether the serial port is opened
	// @return true if serial device is open
	bool IsOpen() const;


	// Close the serial device
	// @throws boost::system::system_error if any error
	void Close();


	//  Set the timeout on read/write operations.
	//  To disable the timeout, call setTimeout(boost::posix_time::seconds(0));
	// @param <t> timeout duration 
	void SetTimeout(const boost::posix_time::time_duration& t);


	// Write data
	// @param data array of char to be sent through the serial device
	// @param size array size
	// @throws boost::system::system_error if any error
	void Write(const char *data, size_t size);


	// Write data
	// @param data to be sent through the serial device
	// @throws boost::system::system_error if any error  
	void Write(const std::vector<char>& data);

	// Write data
	// @param data to be sent through the serial device
	// @throws boost::system::system_error if any error  
	void Write(const std::vector<unsigned char>& data);


	// Write a string. Can be used to send ASCII data to the serial device.
	// To send binary data, use write()
	// @param s string to send
	// @throws boost::system::system_error if any error  
	void WriteString(const std::string& s);


	// Read some data, blocking
	// @param data array of char to be read through the serial device
	// @param size array size
	// @return number of character actually read 0<=return<=size
	// @throws boost::system::system_error if any error
	// @throws timeout_exception in case of timeout   
	void Read(char *data, size_t size);


	// Read some data, blocking
	// @param size how much data to read
	// @return the receive buffer. It is empty if no data is available
	// @throws boost::system::system_error if any error
	// @throws timeout_exception in case of timeout
	std::vector<char> Read(size_t size);


	// Read a string, blocking
	// Can only be used if the user is sure that the serial device will not
	// send binary data. For binary data read, use read()
	// The returned string is empty if no data has arrived
	// @param size how much data to read
	// @return a string with the received data.
	// @throws boost::system::system_error if any error
	// @throws timeout_exception in case of timeout 
	std::string ReadString(size_t size);

	// Read a line, blocking
	// Can only be used if the user is sure that the serial device will not
	// send binary data. For binary data read, use read()
	// The returned string is empty if the line delimiter has not yet arrived.
	// @param delimiter line delimiter, default="\n"
	// @return a string with the received data. The delimiter is removed from
	// the string.
	// @throws boost::system::system_error if any error
	// @throws timeout_exception in case of timeout
	std::string ReadStringUntil(const std::string& delim="\n"); 

private:

	// Callback called either when the read timeout is expired or canceled.
	// If called because timeout expired, sets result to resultTimeoutExpired    
	void TimeoutExpired(const boost::system::error_code& error);


	// Callback called either if a read complete or read error occurs
	// If called because of read complete, sets result to resultSuccess
	// If called because read error, sets result to resultError
	void ReadCompleted(const boost::system::error_code& error,
		const size_t bytesTransferred);

	// Clear the serial port states including the port states 
	// and the in-out data buffer. Call Win32 APIs directly.
	void ClearState();

	// Possible outcome of a read. Set by callbacks, read from main cod   
	enum ReadResult
	{
		resultInProgress,
		resultSuccess,
		resultError,
		resultTimeoutExpired
	};

private:
	boost::asio::io_service io; ///< Io service object
	boost::asio::serial_port port; ///< Serial port object
	boost::asio::deadline_timer timer; ///< Timer for timeout
	boost::posix_time::time_duration timeout; ///< Read/write timeout
	boost::asio::streambuf readData; ///< Holds eventual read but not consumed
	enum ReadResult result;  ///< Used by read with timeout
	size_t bytesTransferred; ///< Used by async read callback
};

#endif	///_TIMEOUTSERIAL_H 


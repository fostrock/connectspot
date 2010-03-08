//------------------------------------------------------------------------------------------
// File: <BCDConvert.h>
// Purpose: declare <Converter functions between BCD and decimal.>
//
// @author <Yun Hua>
// @version 1.0 2010/03/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#ifndef BCD_CONVERT_H__ 
#define BCD_CONVERT_H__

namespace CommonLib
{
	// Convert a BCD stream to a float. 
	// The begin iterator represents the lowest two digits.
	// @param <begin> the stream's begin iterator
	// @param <len> the stream's length
	// @param <digitNum> the number of digit. It can not exceed 6.
	// @return the converted unsigned integer
	float BCD2Float(const unsigned char* begin, unsigned short len, 
		unsigned short digitNum = 4);

	// Convert a BCD stream to a float. 
	// The begin iterator represents the highest two digits.
	// @param <begin> the stream's begin iterator
	// @param <len> the stream's length
	// @param <digitNum> the number of digit. It can not exceed 6.
	// @return the converted unsigned integer
	float BCD2FloatR(const unsigned char* begin, unsigned short len, 
		unsigned short digitNum = 4);

	// Convert a BCD stream to an integer. 
	// The begin iterator represents the lowest two digits.
	// @param <begin> the stream's begin iterator
	// @param <len> the stream's length
	// @return the converted unsigned integer
	unsigned int BCD2Int(const unsigned char* begin, unsigned short len);

	// Convert a BCD stream to an integer. 
	// The begin iterator represents the highest two digits
	// It is a Reverse function to BCD2Int
	// @param <begin> the stream's begin iterator
	// @param <len> the stream's length in bytes
	// @return the converted unsigned integer
	unsigned int BCD2IntR(const unsigned char* begin, unsigned short len);

	// Convert a decimal to BCD code. IF the input value larger than 99, the result will always be 0
	// @param <dec> a decimal code, it shall not larger than 99
	// @return a BCD code
	unsigned char Dec2BCD(unsigned char dec);
}



#endif
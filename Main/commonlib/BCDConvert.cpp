//------------------------------------------------------------------------------------------
// File: <BCDConvert.h>
// Purpose: declare <Converter functions between BCD and decimal.>
//
// @author <Yun Hua>
// @version 1.0 2010/03/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include "BCDConvert.h"
#include <cmath>
#include <stdexcept>

namespace CommonLib
{
	// Convert a BCD stream to a float. 
	float BCD2Float(const unsigned char* begin, unsigned short len, unsigned short digitNum)
	{
		_ASSERTE(len > 0 && digitNum < 7);
		if (0 == len || digitNum > 6)
		{
			throw std::invalid_argument("Empty BCD stream or the digit number exceeds 6");
		}

		float value = 0.0f;
		float base = 1.0f;
		float baseDigit = powf(0.1f, digitNum);
		bool isEven = (0 == (digitNum % 2)) ? true : false;
		const unsigned char* itStream = begin;
		unsigned int digitIndex = 0;

		if (digitNum > 0)
		{
			while ((digitIndex < digitNum / 2) && (len > 0))
			{
				value += baseDigit * ((*itStream) & 0x0f);
				baseDigit *= 10.0f;
				value += baseDigit * ((*itStream) >> 4);
				baseDigit *= 10.0f;
				itStream++;
				digitIndex++;
				len--;
			}
			if (!isEven)
			{
				value += baseDigit * ((*itStream) & 0x0f);
				value += base * ((*itStream) >> 4);
				base *= 10.0f;
				itStream++;
				len--;
			}
		}

		while (len--)
		{
			value += base * ((*itStream) & 0x0f);
			base *= 10.0f;
			value += base * ((*itStream) >> 4);
			base *= 10.0f;
			itStream++;
		}

		return value;
	}

	// Convert a BCD stream to a float. 
	float BCD2FloatR(const unsigned char* begin, unsigned short len, unsigned short digitNum)
	{
		_ASSERTE(len > 0 && digitNum < 7);
		if (0 == len || digitNum > 6)
		{
			throw std::invalid_argument("Empty BCD stream or the digit number exceeds 6");
		}

		float value = 0.0f;
		float base = 1.0f;
		float baseDigit = powf(0.1f, digitNum);
		bool isEven = (0 == (digitNum % 2)) ? true : false;
		const unsigned char* itStream = begin + len - 1;
		unsigned int digitIndex = 0;

		if (digitNum > 0)
		{
			while ((digitIndex < digitNum / 2) && (len > 0))
			{
				value += baseDigit * ((*itStream) & 0x0f);
				baseDigit *= 10.0f;
				value += baseDigit * ((*itStream) >> 4);
				baseDigit *= 10.0f;
				itStream--;
				digitIndex++;
				len--;
			}
			if (!isEven)
			{
				value += baseDigit * ((*itStream) & 0x0f);
				value += base * ((*itStream) >> 4);
				base *= 10.0f;
				itStream--;
				len--;
			}
		}

		while (len--)
		{
			value += base * ((*itStream) & 0x0f);
			base *= 10.0f;
			value += base * ((*itStream) >> 4);
			base *= 10.0f;
			itStream--;
		}

		return value;
	}

	// Convert a BCD stream to an integer. 
	unsigned int BCD2Int(const unsigned char* begin, unsigned short len)
	{
		_ASSERTE(len > 0 && len < 6);
		if (0 == len || len > 5)
		{
			throw std::invalid_argument("Empty BCD stream or the stream length is too long.");
		}

		unsigned int value = 0;
		unsigned int base = 1;

		const unsigned char* itStream = begin;
		while (len--)
		{
			value += base * ((*itStream) & 0x0f);
			base *= 10;
			value += base * ((*itStream) >> 4);
			base *= 10;
			itStream++;
		}
		return value;
	}

	// Convert a BCD stream to an integer. 
	unsigned int BCD2IntR(const unsigned char* begin, unsigned short len)
	{
		_ASSERTE(len > 0 && len < 6);
		if (0 == len || len > 5)
		{
			throw std::invalid_argument("Empty BCD stream or the stream length is too long.");
		}

		unsigned int value = 0;
		unsigned int base = 1;

		const unsigned char* itStream = begin + len - 1;
		while (len--)
		{
			value += base * ((*itStream) & 0x0f);
			base *= 10;
			value += base * ((*itStream) >> 4);
			base *= 10;
			itStream--;
		}

		return value;
	}

	// Convert a decimal to BCD code. IF the input value larger than 99, the result will always be 0
	unsigned char Dec2BCD(unsigned char dec)
	{
		_ASSERT(dec <= 99);
		if (dec > 99)
		{
			return 0;
		}
		return ((dec / 10) << 4) | (dec % 10);
	}

	// Convert a decimal integer to BCD code. 
	std::vector<unsigned char> Dec2BCD_R(unsigned int dec, unsigned short byteSize)
	{
		_ASSERTE(byteSize != 0);
		std::vector<unsigned char> vec(byteSize, 0x0);

		for (unsigned short i = 0; i < byteSize; i++)
		{
			vec.at(vec.size() - i - 1) = dec % 10;
			dec /= 10;
			vec.at(vec.size() - i - 1) |= ((dec % 10) << 4);
			dec /= 10;
		}

		return vec;
	}

	// Convert a decimal float to BCD code. 
	std::vector<unsigned char> Dec2BCD_R(float dec, unsigned short byteSize, 
		unsigned short digitNum)
	{
		_ASSERTE(byteSize != 0);
		std::vector<unsigned char> vec(byteSize, 0x0);
		long long d = dec * powf(10, digitNum);

		for (unsigned short i = 0; i < byteSize; i++)
		{
			vec.at(vec.size() - i - 1) = d % 10;
			d /= 10;
			vec.at(vec.size() - i - 1) |= ((d % 10) << 4);
			d /= 10;
		}

		return vec;
	}
}
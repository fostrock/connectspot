//------------------------------------------------------------------------------------------
// File: <zsserial_test.cpp>
// Purpose: unit test for <ZSSerial class>
//
// @author <Yun Hua>
// @version 1.0 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSSerial.h"

BOOST_AUTO_TEST_CASE(zsserial_constructors_test)
{
	ZSSerial zs("COM3", 9600);
	BOOST_CHECK(zs.IsOpened());
}

BOOST_AUTO_TEST_CASE(zsserial_calcBCDsum_test)
{
	std::vector<unsigned char> bcdVec;
	bcdVec.push_back(0x29);
	bcdVec.push_back(0x45);
	BOOST_CHECK_EQUAL(ZSSerial::CalculateBCDSum(bcdVec), 0x74);

	// the high part is discarded
	bcdVec.push_back(0x46);
	BOOST_CHECK_EQUAL(ZSSerial::CalculateBCDSum(bcdVec), 0x20);

	bcdVec.push_back(0x80);
	BOOST_CHECK_EQUAL(ZSSerial::CalculateBCDSum(bcdVec), 0x00);
}

BOOST_AUTO_TEST_CASE(zsserial_readdata_test)
{
	ZSSerial zs("COM3", 9600);
	zs.ReadData(ZSSerial::one);
	zs.ReadData(ZSSerial::two);
}
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
#include <algorithm>
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSSerial.h"

const std::string zs_protocol_file = "E:\\My Projects\\connectspot\\Main\\bin\\config\\zsdriver.xml";

class TestFixture {
public:
	TestFixture() : protocol(zs_protocol_file) 
	{ 
		bool isOK = protocol.Parse();
		if (!isOK)
		{
			throw new std::logic_error("parse protocol file failed");
		}
		BOOST_TEST_MESSAGE("setup fixture");
	}
	~TestFixture()
	{ 
		BOOST_TEST_MESSAGE("teardown fixture");
	}

public:
	ZSSerialProtocol protocol;
};

BOOST_FIXTURE_TEST_SUITE(zsserial_suite, TestFixture)

BOOST_AUTO_TEST_CASE(zsserial_constructors_test)
{
	ZSSerial zs("COM3", protocol);
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

BOOST_AUTO_TEST_CASE(zsserial_Dec2BCD_test)
{
	BOOST_CHECK(0x99 == ZSSerial::Dec2BCD(99));
	BOOST_CHECK(0x09 == ZSSerial::Dec2BCD(9));
}

BOOST_AUTO_TEST_CASE(zsserial_BCD2Int_test)
{
	unsigned char bcd[] = {0x21, 0x43, 0x65, 0x87};
	std::vector<unsigned char> a(bcd, bcd + 4);
	BOOST_CHECK_EQUAL(ZSSerial::BCD2Int(bcd, bcd + 4), 87654321);
}

BOOST_AUTO_TEST_CASE(zsserial_BCD2Float_test)
{
    unsigned char bcd[] = {0x21, 0x43, 0x65, 0x87};
	BOOST_CHECK_EQUAL(ZSSerial::BCD2Float(bcd, bcd + 4, 4), 8765.4321f);
	BOOST_CHECK_EQUAL(ZSSerial::BCD2Float(bcd, bcd + 4, 3), 87654.321f);
}

BOOST_AUTO_TEST_CASE(zsserial_MakeReadCmd_test)
{
	ZSSerial zs("COM3", protocol);
	char readCmd[] = {0xbb, 0x21, 0x04, 0x51, 0x76, 0xee};
	BOOST_CHECK(std::equal(readCmd, readCmd + 6, zs.MakeReadCmd(ZSSerial::one, 21).begin()));
}

BOOST_AUTO_TEST_CASE(zsserial_readdata_test)
{
	ZSSerial zs("COM3", protocol);
	zs.SetTimeout(boost::posix_time::seconds(3));
	std::vector<ZSDataItem> items = zs.ReadData(ZSSerial::one, 1);
	BOOST_CHECK_EQUAL(items.size(), 12);
}

BOOST_AUTO_TEST_SUITE_END()
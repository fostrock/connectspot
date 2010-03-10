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
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSSerial.h"
#include "commonlib/BCDConvert.h"

const std::string zs_protocol_file = "D:\\zsdriver.xml";

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

BOOST_AUTO_TEST_CASE(commonlib_Dec2BCD_test)
{
	BOOST_CHECK(0x99 == CommonLib::Dec2BCD(99));
	BOOST_CHECK(0x09 == CommonLib::Dec2BCD(9));

	unsigned int dec = 7654321;
	unsigned char bcd[] = {0x07, 0x65, 0x43, 0x21};
	std::vector<unsigned char> vec = CommonLib::Dec2BCD_R(dec, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(bcd, bcd + 4, vec.begin(), vec.end());

	double f = 76543.21f;
	unsigned char bcdFloat[] = {0x07, 0x65, 0x43, 0x21, 0x00};
	std::vector<unsigned char> vecF = CommonLib::Dec2BCD_R(f, 5, 4);

	// Because of the float storage format, we compare the first 4 bytes only. 
	BOOST_CHECK_EQUAL_COLLECTIONS(bcdFloat, bcdFloat + 4, vecF.begin(), vecF.begin() + 4);

	unsigned char bcdFloatII[] = {0x00, 0x00, 0x76, 0x54, 0x32};
	std::vector<unsigned char> vecFII = CommonLib::Dec2BCD_R(f, 5, 1);
	BOOST_CHECK_EQUAL_COLLECTIONS(bcdFloatII, bcdFloatII + 5, vecFII.begin(), vecFII.end());
}

BOOST_AUTO_TEST_CASE(commonlib_BCD2Int_test)
{
	unsigned char bcd[] = {0x21, 0x43, 0x65, 0x87};
	std::vector<unsigned char> vec(bcd, bcd + 4);
	BOOST_CHECK_EQUAL(CommonLib::BCD2Int(bcd, 4), 87654321);
	BOOST_CHECK_EQUAL(CommonLib::BCD2IntR(bcd, 4), 21436587);
}

BOOST_AUTO_TEST_CASE(commonlib_BCD2Float_test)
{
    unsigned char bcd[] = {0x21, 0x43, 0x65, 0x87};
	std::vector<unsigned char> vec(bcd, bcd + 4);
	BOOST_CHECK_CLOSE(CommonLib::BCD2Float(bcd, 4, 4), 8765.4321, 0.00001);
	BOOST_CHECK_CLOSE(CommonLib::BCD2Float(bcd, 4, 3), 87654.321, 0.00001);
	BOOST_CHECK_CLOSE(CommonLib::BCD2FloatR(bcd, 4, 4), 2143.6587, 0.00001);
	BOOST_CHECK_CLOSE(CommonLib::BCD2FloatR(bcd, 4, 3), 21436.587, 0.00001);
}

BOOST_AUTO_TEST_CASE(zsserial_MakeReadCmd_test)
{
	ZSSerial zs("COM3", protocol);
	unsigned char readCmd[] = {0xbb, 0x21, 0x04, 0x51, 0x76, 0xee};
	BOOST_CHECK(std::equal(readCmd, readCmd + 6, zs.MakeReadCmd(ZSSerial::one, 21).begin()));
}

BOOST_AUTO_TEST_CASE(zsserial_MakeWriteCmd_test)
{
	ZSSerial zs("COM3", protocol);
	unsigned char readCmd[] = 
	{0xbb, 0x21/*station*/, 0x09/*len*/, 0x60/*command*/, 0x21/*param*/, 
		0x98, 0x76, 0x54, 0x32, 0x71/*sum check*/, 0xee}; // PID比例系数
	boost::variant<unsigned int, double> val = 9876.5432;
	std::vector<unsigned char> cmdStream = zs.MakeWriteCmd(40, val, 21);
	BOOST_CHECK_EQUAL_COLLECTIONS(readCmd, readCmd + 11, cmdStream.begin(), cmdStream.end());

	unsigned char readCmdII[] = 
	{0xbb, 0x21/*station*/, 0x07/*len*/, 0x60/*command*/, 0x18/*param*/, 
		0x98, 0x76, 0x80/*sum check*/, 0xee}; // 控制方式字
	boost::variant<unsigned int, double> valII = 9876u;
	std::vector<unsigned char> cmdStreamII = zs.MakeWriteCmd(37, valII, 21);
	BOOST_CHECK_EQUAL_COLLECTIONS(readCmdII, readCmdII + 9, cmdStreamII.begin(), cmdStreamII.end());
}

BOOST_AUTO_TEST_CASE(zsserial_MakeCommonCmd_test)
{
	ZSSerial zs("COM3", protocol);
	unsigned char readCmd[] = {0xbb, 0x21, 0x04, 0x11, 0x36, 0xee}; // 自动校零操作
	BOOST_CHECK(std::equal(readCmd, readCmd + 6, zs.MakeCommonCmd(62, 21).begin()));
}

BOOST_AUTO_TEST_CASE(zsserial_readdata_test)
{
	ZSSerial zs("COM3", protocol);
	zs.SetTimeout(boost::posix_time::seconds(3));
	std::vector<ZSDataItem> items = zs.ReadData(ZSSerial::one, 1);
	BOOST_CHECK_EQUAL(items.size(), 12);
	std::cout << "*** read data group1 ***" << std::endl;
	for (std::size_t i = 0; i < items.size(); ++i)
	{
		std::cout << items.at(i).variant << std::endl;
	}

	items = zs.ReadData(ZSSerial::two, 1);
	BOOST_CHECK_EQUAL(items.size(), 22);
	std::cout << "*** read data group2 ***" << std::endl;
	for (std::size_t i = 0; i < items.size(); ++i)
	{
		std::cout << items.at(i).variant << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()
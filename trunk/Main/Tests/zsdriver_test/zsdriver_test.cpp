//------------------------------------------------------------------------------------------
// File: <zsdriver_test.cpp>
// Purpose: unit test for <Data access for ZS weight through RS-232.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/22
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//
#include "stdafx.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ZSDriver_test

#include <boost/test/unit_test.hpp>
#include "opcspot/ZSSerial.h"

class TestFixture {
public:
	TestFixture() : fixtureCount(0) { BOOST_TEST_MESSAGE("setup fixture"); }
	~TestFixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

public:
	int fixtureCount;
};

BOOST_AUTO_TEST_CASE(zsserial_constructors_test)
{
	ZSSerial zs("COM4", 9600);
	BOOST_CHECK(zs.IsOpened());
}

BOOST_FIXTURE_TEST_SUITE(suiteName, TestFixture)

BOOST_AUTO_TEST_CASE(test_case1) {
	BOOST_CHECK(fixtureCount == 1);
}

BOOST_AUTO_TEST_CASE(test_case2) {
	BOOST_CHECK_EQUAL(fixtureCount, 0);
}

BOOST_AUTO_TEST_SUITE_END()

//// zsdriver_test.cpp : Defines the entry point for the console application.
////
//
//
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


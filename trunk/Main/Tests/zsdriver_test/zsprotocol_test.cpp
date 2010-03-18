//------------------------------------------------------------------------------------------
// File: <zsprotocol_test.cpp>
// Purpose: unit test for <ZSSerialProtocol class>
//
// @author <Yun Hua>
// @version 1.0 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSSerialProtocol.h"

const std::string zs_protocol_file = "D:\\zsdriver.xml";

BOOST_AUTO_TEST_CASE(zsprotocol_constructors_test)
{
	ZSSerialProtocol protocol(zs_protocol_file);
}

BOOST_AUTO_TEST_CASE(zsprotocol_parse_test)
{
	ZSSerialProtocol protocol(zs_protocol_file);
	BOOST_CHECK_EQUAL(protocol.Parse(), true);

	// Port setting
	const std::vector<ZSSerialSetting>& settings = protocol.GetPortSetting();
	BOOST_CHECK_EQUAL(settings.size(), 2);
	BOOST_CHECK("COM1" == settings.at(0).devName);
	BOOST_CHECK_EQUAL(settings.at(0).parity, none);
	BOOST_CHECK_EQUAL(settings.at(0).csize, 8);
	BOOST_CHECK_EQUAL(settings.at(0).stopBits, one);
	const std::vector<std::pair<unsigned short, unsigned short> >& stations =
		settings.at(0).stations;
	BOOST_CHECK_EQUAL(stations.size(), 2);
	BOOST_CHECK_EQUAL(stations.at(0).first, 1);
	BOOST_CHECK_EQUAL(stations.at(0).second, 1);
	BOOST_CHECK_EQUAL(stations.at(1).first, 8);
	BOOST_CHECK_EQUAL(stations.at(1).second, 0);

    // Dataset
	const ZSSerialProtocol::DataSetDef& dataset = protocol.GetDataSetInfo();
	ZSSerialProtocol::DataSetDef::const_iterator it = dataset.find(1);
	BOOST_CHECK(it != dataset.end());
	BOOST_CHECK_EQUAL((*it).second.get<ZSSerialProtocol::ZS_DATA_TYPE_INDEX>(), true);
	BOOST_CHECK_EQUAL((*it).second.get<ZSSerialProtocol::ZS_DATA_LENGTH_INDEX>(), 4);
	BOOST_CHECK_EQUAL((*it).second.get<ZSSerialProtocol::ZS_DATA_ACCESS_INDEX>(), readwrite);
	BOOST_CHECK_EQUAL((*it).second.get<ZSSerialProtocol::ZS_DATA_NAME_INDEX>(), "流速设定量");

	// test read data cmd part
	const std::vector<ZSReadDataCmd>& readDataCmds = protocol.GetReadDataCmd();
	BOOST_CHECK(2 == readDataCmds.size());

	ZSReadDataCmd readDataCmd = readDataCmds.at(0);
	std::size_t findIndex = 0;
	for (; findIndex < readDataCmd.info.size(); ++findIndex)
	{
		if (7 == readDataCmd.info.at(findIndex).index)	// "当前累计总量" 
		{
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).length, 5);
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).offset, 18);
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).isFloat, true);
			break;
		}
	}
	BOOST_CHECK(findIndex != readDataCmd.info.size());

	findIndex = 0;
	readDataCmd = readDataCmds.at(1);
	for (; findIndex < readDataCmd.info.size(); ++findIndex)
	{
		if (38 == readDataCmd.info.at(findIndex).index)	// "缺料报警阀值" 
		{
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).length, 4);
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).offset, 46);
			BOOST_CHECK_EQUAL(readDataCmd.info.at(findIndex).isFloat, true);
			break;
		}
	}
	BOOST_CHECK(findIndex != readDataCmd.info.size());

	// test write data cmd part
	const ZSWriteDataCmd& writeDataCmd = protocol.GetWriteDataCmd();
	ZSWriteDataCmd::ParamDef::const_iterator itWrite = writeDataCmd.param.find(1);
	BOOST_CHECK(itWrite != writeDataCmd.param.end());
	BOOST_CHECK_EQUAL(itWrite->second, 1);

	itWrite = writeDataCmd.param.find(42);
	BOOST_CHECK(itWrite != writeDataCmd.param.end());
	BOOST_CHECK_EQUAL(itWrite->second, 23);

	// test common cmd part
	const ZSSerialProtocol::CommonCmdDef& commonCmd = protocol.GetCommonCmd();
	ZSSerialProtocol::CommonCmdDef::const_iterator itCmd = commonCmd.find(61);
	BOOST_CHECK(itCmd != commonCmd.end());
	BOOST_CHECK_EQUAL(itCmd->second, 0x10);

	itCmd = commonCmd.find(64);
	BOOST_CHECK(itCmd != commonCmd.end());
	BOOST_CHECK_EQUAL(itCmd->second, 0x13);
}
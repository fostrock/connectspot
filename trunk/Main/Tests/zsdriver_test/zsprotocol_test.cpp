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

const std::string zs_protocol_file = "E:\\My Projects\\connectspot\\Main\\bin\\config\\zsdriver.xml";

BOOST_AUTO_TEST_CASE(zsprotocol_constructors_test)
{
	//ZSSerialProtocol protocol(zs_protocol_file);
}

BOOST_AUTO_TEST_CASE(zsprotocol_parse_test)
{
	ZSSerialProtocol protocol(zs_protocol_file);
	BOOST_CHECK_EQUAL(protocol.Parse(), true);

	const ZSSerialProtocol::DataSetDef& dataset = protocol.GetDataSetInfo();
	ZSSerialProtocol::DataSetDef::const_iterator it = dataset.find(1);
	BOOST_CHECK(it != dataset.end());

	const std::vector<ZSReadDataCmd>& readDataCmd = protocol.GetReadDataCmd();
	BOOST_CHECK(2 == readDataCmd.size());
	ZSReadDataCmd::OffsetDef::const_iterator itRead = readDataCmd.at(1).offset.find(21);
	BOOST_CHECK(itRead != readDataCmd.at(1).offset.end());

	const ZSWriteDataCmd& writeDataCmd = protocol.GetWriteDataCmd();
	ZSWriteDataCmd::ParamDef::const_iterator itWrite = writeDataCmd.param.find(1);
	BOOST_CHECK(itWrite != writeDataCmd.param.end());

	const ZSSerialProtocol::CommonCmdDef& commonCmd = protocol.GetCommonCmd();
	ZSSerialProtocol::CommonCmdDef::const_iterator itCmd = commonCmd.find(61);
	BOOST_CHECK(itCmd != commonCmd.end());
}
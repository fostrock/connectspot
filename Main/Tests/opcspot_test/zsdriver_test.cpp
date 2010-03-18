#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSDriver.h"
#include <iostream>


BOOST_AUTO_TEST_CASE(zsdriver_methods_test) 
{
	BOOST_CHECK_EQUAL(true, ZSDriver::Init("d:\\zsdriver.xml"));

	std::vector<ZSDriver::TAG_DEF> tagDef = ZSDriver::GetTagDef();
	BOOST_CHECK_EQUAL(tagDef.size(), 38 * 4);
	for (std::size_t i = 0; i < tagDef.size(); i++)
	{
		ZSDriver::AssignTagIDIndexMap(tagDef.at(i).dataID + 1, tagDef.at(i).dataID);
	}

	ZSDriver::Destroy();
}
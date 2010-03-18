#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSDriver.h"


BOOST_AUTO_TEST_CASE(zsdriver_methods_test) 
{
	BOOST_CHECK_EQUAL(true, ZSDriver::Init("d:\\zsdriver.xml"));

	std::vector<ZSDriver::TAG_DEF> tagDef = ZSDriver::GetTagDef();
	BOOST_CHECK(tagDef.size() > 0);

	ZSDriver::Destroy();
}
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSDriver.h"


BOOST_AUTO_TEST_CASE(zsdriver_init_test) 
{
	BOOST_CHECK_EQUAL(true, ZSDriver::Init("d:\\zsdriver.xml"));
}
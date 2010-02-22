#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

class TestFixture {
public:
	TestFixture() : fixtureCount(0) { BOOST_TEST_MESSAGE("setup fixture"); }
	~TestFixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

public:
	int fixtureCount;
};

BOOST_FIXTURE_TEST_SUITE(suiteName, TestFixture)

BOOST_AUTO_TEST_CASE(test_case1) {
	BOOST_CHECK(fixtureCount == 1);
}

BOOST_AUTO_TEST_CASE(test_case2) {
	BOOST_CHECK_EQUAL(fixtureCount, 0);
}

BOOST_AUTO_TEST_SUITE_END()
#define BOOST_TEST_MODULE app_test_module

#include "../lib.h"
#include <boost/test/unit_test.hpp>
#include "ip_filter.h"

BOOST_AUTO_TEST_SUITE(app_test_suite)

BOOST_AUTO_TEST_CASE(test_version)
{
    BOOST_CHECK(version() > 0);
}

BOOST_AUTO_TEST_CASE(ip_filter_full_test)
{
    std::ifstream input{"../ip_filter.tsv"};

    auto ipPool = fill(&input);
    std::sort(ipPool.begin(), ipPool.end(), std::greater<Ip>());

    std::stringstream output;
    output << toString(ipPool);
    output << toString(filter(ipPool, 1));
    output << toString(filter(ipPool, 46, 70));
    output << toString(filter_any(ipPool, 46));

    std::ifstream testStream{ "../ip_filter.tst"};
    BOOST_CHECK(output.str() == toString(fill(&testStream)));

//    BOOST_CHECK(filter(ipPool, -1).size() <= 0);
//    BOOST_CHECK(filter(ipPool, 78).size() >= 2);
//    BOOST_CHECK(filter(ipPool, 78).size() >= 2);
//    BOOST_CHECK(filter(ipPool, 5, 8).size() == 6);
//    BOOST_CHECK(filter(ipPool, 8, 5).size() <= 0);
//    BOOST_CHECK(filter(ipPool, 46, 70, 113).size() >= 1);
//    BOOST_CHECK(filter(ipPool, 46, 70, 113, 73, 0, 1, 2, 3) != filter(ipPool, 46, 70, 113, 73));
}
BOOST_AUTO_TEST_SUITE_END()

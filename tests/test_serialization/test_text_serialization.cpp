#include <boost/test/unit_test.hpp>

#include "serialization/mime/text_serialization.hpp"

BOOST_AUTO_TEST_SUITE( test_text_serialization )

BOOST_AUTO_TEST_CASE( test_serialize_text )
{
    using namespace s2smtp::mime;
    using namespace s2smtp::serialization;

    std::stringstream s;

    serialize(s, text_t{"sometext"});

    BOOST_CHECK_EQUAL(s.str(), "=?UTF-8?B?c29tZXRleHQ=?=");
}

BOOST_AUTO_TEST_SUITE_END()

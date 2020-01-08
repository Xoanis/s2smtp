#include <boost/test/unit_test.hpp>

#include "s2smtp/mime/charset.hpp"

BOOST_AUTO_TEST_SUITE(test_charset)

BOOST_AUTO_TEST_CASE(test_charset_ctor_and_static) {
  using namespace s2smtp::mime;

  {
    charset_t cs("TEST");
    BOOST_CHECK_EQUAL(cs.to_string(), "TEST");
  }

  {
    charset_t cs;
    BOOST_CHECK_EQUAL(cs.to_string(), "UTF-8");
  }

  {
    charset_t::default_charset = charset_t("TEST");
    charset_t cs;
    BOOST_CHECK_EQUAL(cs.to_string(), "TEST");
  }

  {
    charset_t::default_charset = charsets::UTF_8;
    charset_t cs;
    BOOST_CHECK_EQUAL(cs.to_string(), "UTF-8");
  }
}

BOOST_AUTO_TEST_SUITE_END()

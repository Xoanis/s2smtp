#include <boost/test/unit_test.hpp>

#include "s2smtp/mime/text.hpp"

BOOST_AUTO_TEST_SUITE(test_text)

BOOST_AUTO_TEST_CASE(test_text_ctor) {
  using namespace s2smtp::mime;

  {
    text_t t;
    BOOST_CHECK_EQUAL(t.data, "");
    BOOST_CHECK(t.charset == charset_t::default_charset);
  }

  {
    text_t t("text", charsets::UTF_8);
    BOOST_CHECK_EQUAL(t.data, "text");
    BOOST_CHECK(t.charset == charsets::UTF_8);
  }

  {
    text_t t("text");
    BOOST_CHECK_EQUAL(t.data, "text");
    BOOST_CHECK(t.charset == charset_t::default_charset);
  }

  {
    text_t t(std::string("text"));
    BOOST_CHECK_EQUAL(t.data, "text");
    BOOST_CHECK(t.charset == charset_t::default_charset);
  }

  {
    std::string s = "text";
    text_t t(s);
    BOOST_CHECK_EQUAL(t.data, s);
    BOOST_CHECK(t.charset == charset_t::default_charset);
  }

  {
    text_t t("text", charsets::us_ascii);
    text_t t2(t);
    BOOST_CHECK_EQUAL(t2.data, "text");
    BOOST_CHECK(t2.charset == charsets::us_ascii);
    BOOST_CHECK(t2 == t);
  }

  {
    text_t t("text", charsets::us_ascii);
    text_t t2(std::move(t));
    BOOST_CHECK_EQUAL(t2.data, "text");
    BOOST_CHECK(t2.charset == charsets::us_ascii);
    BOOST_CHECK(t2 != t);
  }
}

BOOST_AUTO_TEST_CASE(test_text_move_and_assign_op) {
  using namespace s2smtp::mime;

  {
    text_t t;
    BOOST_CHECK_EQUAL(t.data, "");
    BOOST_CHECK(t.charset == charset_t::default_charset);

    std::string s = "test";
    t = s;

    BOOST_CHECK_EQUAL(t.data, s);
    BOOST_CHECK(t.charset == charset_t::default_charset);
  }

  {
    text_t t;
    std::string s = "test";
    t = std::move(s);

    BOOST_CHECK_EQUAL(t.data, "test");
    BOOST_CHECK_EQUAL(s, "");
  }

  {
    text_t t1, t2;
    t1 = "one";
    t2 = std::move(t1);
    BOOST_CHECK_EQUAL(t1.data, "");
    BOOST_CHECK_EQUAL(t2.data, "one");
  }
}

BOOST_AUTO_TEST_SUITE_END()

#include <boost/test/unit_test.hpp>

#include "parsing/smtp_parse.hpp"

BOOST_AUTO_TEST_SUITE(test_smtp_parse)

BOOST_AUTO_TEST_CASE(test_is_begin_of_multiline_reply) {
  using namespace s2smtp::parsing;
  BOOST_CHECK(is_begin_of_multiline_reply("250-foo.com greets bar.com\r\n"));
  BOOST_CHECK(is_begin_of_multiline_reply("250-foo\r\n"));
  BOOST_CHECK(!is_begin_of_multiline_reply("250 OK\r\n"));
  BOOST_CHECK(!is_begin_of_multiline_reply("test"));
  BOOST_CHECK(!is_begin_of_multiline_reply("259"));
}

BOOST_AUTO_TEST_CASE(test_is_end_of_multiline_reply) {
  using namespace s2smtp::parsing;
  BOOST_CHECK(is_end_of_multiline_reply("250 HELP\r\n"));
  BOOST_CHECK(is_end_of_multiline_reply("250 OK\r\n"));
  BOOST_CHECK(!is_end_of_multiline_reply("250-8BITMIME\r\n"));
  BOOST_CHECK(!is_end_of_multiline_reply("250-SIZE\r\n"));
  BOOST_CHECK(!is_end_of_multiline_reply("fs fd\r\n"));
  BOOST_CHECK(!is_end_of_multiline_reply("test\r\n"));
  BOOST_CHECK(is_end_of_multiline_reply("250\r\n"));
}

BOOST_AUTO_TEST_CASE(test_parse_server_reply) {
  using namespace s2smtp::parsing;
  using namespace s2smtp;

  {
    auto sr = parse_server_reply("250-foo.com greets bar.com\r\n"
                                 "250-8BITMIME\r\n"
                                 "250-SIZE\r\n"
                                 "250-DSN\r\n"
                                 "250 HELP\r\n");
    BOOST_CHECK_EQUAL(sr.code, 250);
    BOOST_CHECK_EQUAL(sr.text_lines[0], "foo.com greets bar.com");
    BOOST_CHECK_EQUAL(sr.text_lines[1], "8BITMIME");
    BOOST_CHECK_EQUAL(sr.text_lines[2], "SIZE");
    BOOST_CHECK_EQUAL(sr.text_lines[3], "DSN");
    BOOST_CHECK_EQUAL(sr.text_lines[4], "HELP");
    BOOST_CHECK_EQUAL(sr.text(), "foo.com greets bar.com\n"
                                 "8BITMIME\n"
                                 "SIZE\n"
                                 "DSN\n"
                                 "HELP");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = parse_server_reply(
        "221 foo.com Service closing transmission channel\r\n");
    BOOST_CHECK_EQUAL(sr.code, 221);
    BOOST_CHECK_EQUAL(sr.text(),
                      "foo.com Service closing transmission channel");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::connections);
  }

  {
    auto sr =
        parse_server_reply("354 Start mail input; end with <CRLF>.<CRLF>\r\n");
    BOOST_CHECK_EQUAL(sr.code, 354);
    BOOST_CHECK_EQUAL(sr.text(), "Start mail input; end with <CRLF>.<CRLF>");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_intermediate);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = parse_server_reply("250 Mark Crispin <Admin.MRC@foo.com>\r\n");
    BOOST_CHECK_EQUAL(sr.code, 250);
    BOOST_CHECK_EQUAL(sr.text(), "Mark Crispin <Admin.MRC@foo.com>");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = parse_server_reply("550 Access Denied to You\r\n");
    BOOST_CHECK_EQUAL(sr.code, 550);
    BOOST_CHECK_EQUAL(sr.text(), "Access Denied to You");
    BOOST_CHECK(sr.kind == server_reply::kind_t::permanent_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  BOOST_CHECK_THROW(parse_server_reply(""), smtp_error);
  BOOST_CHECK_THROW(parse_server_reply("\r\n"), smtp_error);
  BOOST_CHECK_THROW(parse_server_reply("150\r\n"), smtp_error);
  BOOST_CHECK_THROW(parse_server_reply("250nospace\r\n"), smtp_error);
  BOOST_CHECK_THROW(parse_server_reply("001 wrong code\r\n"), smtp_error);
  BOOST_CHECK_THROW(
      parse_server_reply("250-begin of multiline, but no end\r\n"),
      smtp_error);
}

BOOST_AUTO_TEST_SUITE_END()

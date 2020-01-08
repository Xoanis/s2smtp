#include "s2smtp/smtp/server_reply.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_server_reply)

BOOST_AUTO_TEST_CASE(test_server_reply_ctor) {
  using namespace s2smtp;

  {
    auto sr = server_reply(250, {"test message", "test message"});
    BOOST_CHECK_EQUAL(sr.code, 250);
    BOOST_CHECK_EQUAL(sr.text(), "test message\ntest message");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = server_reply(211, {"System status, or system help reply"});
    BOOST_CHECK_EQUAL(sr.code, 211);
    BOOST_CHECK_EQUAL(sr.text(), "System status, or system help reply");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::information);
  }

  {
    auto sr = server_reply(214, {"Help message"});
    BOOST_CHECK_EQUAL(sr.code, 214);
    BOOST_CHECK_EQUAL(sr.text(), "Help message");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::information);
  }

  {
    auto sr = server_reply(220, {"<domain> Service ready"});
    BOOST_CHECK_EQUAL(sr.code, 220);
    BOOST_CHECK_EQUAL(sr.text(), "<domain> Service ready");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::connections);
  }

  {
    auto sr =
        server_reply(221, {"<domain> Service closing transmission channel"});
    BOOST_CHECK_EQUAL(sr.code, 221);
    BOOST_CHECK_EQUAL(sr.text(),
                      "<domain> Service closing transmission channel");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::connections);
  }

  {
    auto sr = server_reply(250, {"Requested mail action okay, completed"});
    BOOST_CHECK_EQUAL(sr.code, 250);
    BOOST_CHECK_EQUAL(sr.text(), "Requested mail action okay, completed");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = server_reply(354, {"Start mail input; end with <CRLF>.<CRLF>"});
    BOOST_CHECK_EQUAL(sr.code, 354);
    BOOST_CHECK_EQUAL(sr.text(), "Start mail input; end with <CRLF>.<CRLF>");
    BOOST_CHECK(sr.kind == server_reply::kind_t::positive_intermediate);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = server_reply(
        421, {"<domain> Service not available, closing transmission channel"});
    BOOST_CHECK_EQUAL(sr.code, 421);
    BOOST_CHECK_EQUAL(
        sr.text(),
        "<domain> Service not available, closing transmission channel");
    BOOST_CHECK(sr.kind == server_reply::kind_t::transient_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::connections);
  }

  {
    auto sr = server_reply(
        450, {"Requested mail action not taken: mailbox unavailable"});
    BOOST_CHECK_EQUAL(sr.code, 450);
    BOOST_CHECK_EQUAL(sr.text(),
                      "Requested mail action not taken: mailbox unavailable");
    BOOST_CHECK(sr.kind == server_reply::kind_t::transient_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = server_reply(
        451, {"Requested action aborted: local error in processing"});
    BOOST_CHECK_EQUAL(sr.code, 451);
    BOOST_CHECK_EQUAL(sr.text(),
                      "Requested action aborted: local error in processing");
    BOOST_CHECK(sr.kind == server_reply::kind_t::transient_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }

  {
    auto sr = server_reply(500, {"Syntax error, command unrecognized"});
    BOOST_CHECK_EQUAL(sr.code, 500);
    BOOST_CHECK_EQUAL(sr.text(), "Syntax error, command unrecognized");
    BOOST_CHECK(sr.kind == server_reply::kind_t::permanent_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::syntax);
  }

  {
    auto sr = server_reply(
        555,
        {"MAIL FROM/RCPT TO parameters not recognized or not implemented"});
    BOOST_CHECK_EQUAL(sr.code, 555);
    BOOST_CHECK_EQUAL(
        sr.text(),
        "MAIL FROM/RCPT TO parameters not recognized or not implemented");
    BOOST_CHECK(sr.kind == server_reply::kind_t::permanent_negative_completion);
    BOOST_CHECK(sr.category == server_reply::category_t::mail_system);
  }
}

BOOST_AUTO_TEST_CASE(test_server_reply_trow) {
  using namespace s2smtp;

  BOOST_CHECK_THROW(server_reply(545, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(575, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(655, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(155, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(1, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(22, {""}), smtp_error);
  BOOST_CHECK_THROW(server_reply(1155, {""}), smtp_error);
}

BOOST_AUTO_TEST_SUITE_END()

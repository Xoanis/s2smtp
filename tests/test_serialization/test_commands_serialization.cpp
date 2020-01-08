#include <boost/test/unit_test.hpp>

#include "serialization/mime/message_serialization.hpp"
#include "serialization/smtp/commands_serialization.hpp"


BOOST_AUTO_TEST_SUITE(test_commands_serialization)

BOOST_AUTO_TEST_CASE(test_serialize_helo) {
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_helo("domain"), "HELO domain\r\n");
}
BOOST_AUTO_TEST_CASE(test_serialize_ehlo) {
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_ehlo("domain"), "EHLO domain\r\n");
}

BOOST_AUTO_TEST_CASE(test_serialize_mail_from) {
  using namespace s2smtp;
  using namespace s2smtp::mime;
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_mail_from("loc@domain.s"),
                    "MAIL FROM:<loc@domain.s>\r\n");
  BOOST_CHECK_THROW(serialize_mail_from("locdomain.s"), mime_error);
  BOOST_CHECK_THROW(serialize_mail_from(""), mime_error);
  BOOST_CHECK_THROW(serialize_mail_from("loc@domain.s\r\n"), mime_error);
}
BOOST_AUTO_TEST_CASE(test_serialize_rcpt_to) {
  using namespace s2smtp;
  using namespace s2smtp::mime;
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_rcpt_to("loc@domain.s"),
                    "RCPT TO:<loc@domain.s>\r\n");
  BOOST_CHECK_THROW(serialize_rcpt_to("locdomain.s"), mime_error);
  BOOST_CHECK_THROW(serialize_rcpt_to(""), mime_error);
  BOOST_CHECK_THROW(serialize_rcpt_to("loc@domain.s\r\n"), mime_error);
}
BOOST_AUTO_TEST_CASE(test_serialize_data) {
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_data(), "DATA\r\n");
}
BOOST_AUTO_TEST_CASE(test_serialize_data_content) {
  using namespace s2smtp::mime;
  using namespace s2smtp::serialization;

  message_t msg("test@t.u");
  msg.header.to.emplace_back(mailbox_t("rcpt@dom.d"));
  msg.text.content = "test msg\r\ntestline\r\n";

  auto sm = serialize(msg);

  BOOST_CHECK_EQUAL(serialize_data_content(msg), sm + "\r\n.\r\n");
}
BOOST_AUTO_TEST_CASE(test_serialize_quit) {
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_quit(), "QUIT\r\n");
}
BOOST_AUTO_TEST_CASE(test_starttls) {
  using namespace s2smtp::serialization;
  BOOST_CHECK_EQUAL(serialize_starttls(), "STARTTLS\r\n");
}

BOOST_AUTO_TEST_SUITE_END()

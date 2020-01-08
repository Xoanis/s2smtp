#include <boost/test/unit_test.hpp>

#include "s2smtp/mime/message.hpp"

BOOST_AUTO_TEST_SUITE(test_message)

BOOST_AUTO_TEST_CASE(test_header_ctor) {
  using namespace s2smtp::mime;

  {
    header_t h("test@test.test");
    BOOST_CHECK(h.from == mailbox_t("test@test.test"));
    BOOST_CHECK(h.to.empty());
    BOOST_CHECK(h.cc.empty());
    BOOST_CHECK(h.bcc.empty());
    BOOST_CHECK(h.reply_to.empty());
    BOOST_CHECK(h.subject.data.empty());
  }

  {
    std::tm t;
    t.tm_hour = 1;
    t.tm_mday = 31;

    header_t h("test@test.test", t);
    BOOST_CHECK_EQUAL(h.date.tm_hour, 1);
    BOOST_CHECK_EQUAL(h.date.tm_mday, 31);
  }

  {
    BOOST_CHECK(header_t({"test", "test.test", ""}).from ==
                mailbox_t("test@test.test"));
    BOOST_CHECK(header_t({"test", "test.test", "ts"}).from ==
                mailbox_t("test@test.test", "ts"));
  }

  {
    mailbox_t mb("test@test.t", "name");
    header_t h(mb);
    BOOST_CHECK(h.from == mb);
  }

  {
    mailbox_t mb("test@test.t", "name");
    header_t h(std::move(mb));
    BOOST_CHECK(h.from != mb);
  }
}

BOOST_AUTO_TEST_CASE(text_ctor) {
  using namespace s2smtp::mime;

  text_part_t tp;
  BOOST_CHECK(tp.text_content_type == text_content_types::plain);
  BOOST_CHECK(tp.content == text_t{});
}

BOOST_AUTO_TEST_CASE(message_ctor) {
  using namespace s2smtp::mime;

  charset_t::default_charset = charsets::us_ascii;

  message_t msg("test@t.t");
  BOOST_CHECK_EQUAL(msg.header.from.local_part(), "test");
  BOOST_CHECK_EQUAL(msg.header.from.domain(), "t.t");
  BOOST_CHECK_EQUAL(msg.header.from.display_name().data, "");
  BOOST_CHECK(msg.text.text_content_type == text_content_types::plain);
  BOOST_CHECK_EQUAL(msg.text.content.data, "");

  text_part_t tp{text_content_types::plain, ""};
  tp.text_content_type = text_content_types::html;
  tp.content = "test";
  msg.text = std::move(tp);

  BOOST_CHECK_EQUAL(tp.content.data, "");
  BOOST_CHECK(msg.text.content == text_t("test"));
  charset_t::default_charset = charsets::UTF_8;
  BOOST_CHECK(msg.text.content != text_t("test"));
}

BOOST_AUTO_TEST_SUITE_END()

#include <boost/test/unit_test.hpp>
#include <serialization/mime/address_serialization.hpp>

BOOST_AUTO_TEST_SUITE(test_address_serialization)

BOOST_AUTO_TEST_CASE(test_serialize_mailbox) {
  using namespace s2smtp::serialization;
  using namespace s2smtp::mime;
  {
    std::stringstream s;
    serialize(s, mailbox_t("loc", "dom.s", "name"));
    BOOST_CHECK_EQUAL(s.str(), "=?UTF-8?B?bmFtZQ==?= <loc@dom.s>");
  }

  {
    std::stringstream s;
    serialize(s, mailbox_t("loc", "dom.s", text_t("name", charsets::us_ascii)));
    BOOST_CHECK_EQUAL(s.str(), "=?us-ascii?B?bmFtZQ==?= <loc@dom.s>");
  }

  {
    std::stringstream s;
    serialize(
        s, mailbox_t("some", "slow.su", text_t("name", charsets::iso_8859_1)));
    BOOST_CHECK_EQUAL(s.str(), "=?ISO-8859-1?B?bmFtZQ==?= <some@slow.su>");
  }

  {
    charset_t::default_charset = charset_t("test");
    std::stringstream s;
    serialize(s, mailbox_t("loc", "dom.s", "name"));
    charset_t::default_charset = charsets::UTF_8;
    BOOST_CHECK_EQUAL(s.str(), "=?test?B?bmFtZQ==?= <loc@dom.s>");
  }

  {
    std::stringstream s;
    serialize(s, mailbox_t("loc@dom.s"));
    BOOST_CHECK_EQUAL(s.str(), "loc@dom.s");
  }
}

BOOST_AUTO_TEST_CASE(test_serialize_mailbox_list) {
  using namespace s2smtp::serialization;
  using namespace s2smtp::mime;

  {
    std::stringstream s;
    serialize(s, mailbox_list_t{{"s@s.s"}, {"r@r.s", "name"}});
    BOOST_CHECK_EQUAL(s.str(), "s@s.s, =?UTF-8?B?bmFtZQ==?= <r@r.s>");
  }
}

BOOST_AUTO_TEST_CASE(test_serialize_group) {
  using namespace s2smtp::serialization;
  using namespace s2smtp::mime;

  {
    std::stringstream s;
    serialize(s, group_t{text_t{"group1"},
                         {mailbox_t{"s@s.s"}, mailbox_t{"r@r.s", "name"}}});
    BOOST_CHECK_EQUAL(
        s.str(), "=?UTF-8?B?Z3JvdXAx?=: s@s.s, =?UTF-8?B?bmFtZQ==?= <r@r.s>;");
  }
}

BOOST_AUTO_TEST_CASE(test_serialize_address) {
  using namespace s2smtp::serialization;
  using namespace s2smtp::mime;

  {
    std::stringstream s;
    serialize(s, address_t{mailbox_t{"s@s.s"}});
    BOOST_CHECK_EQUAL(s.str(), "s@s.s");
  }

  {
    std::stringstream s;
    serialize(s, address_t{group_t{text_t{"group1"}, {{"s@s.s"}, {"r@r.s", "name"}}}});
    BOOST_CHECK_EQUAL(
        s.str(), "=?UTF-8?B?Z3JvdXAx?=: s@s.s, =?UTF-8?B?bmFtZQ==?= <r@r.s>;");
  }
}

BOOST_AUTO_TEST_CASE(test_serialize_address_list) {
  using namespace s2smtp::serialization;
  using namespace s2smtp::mime;

  {
    std::stringstream s;
    address_list_t al = {mailbox_t{"wot@ls.u"},
                         group_t{"group1", {{"s@s.s"}, {"r@r.s", "name"}}}};
    serialize(s, al);
    BOOST_CHECK_EQUAL(
        s.str(),
        "wot@ls.u, =?UTF-8?B?Z3JvdXAx?=: s@s.s, =?UTF-8?B?bmFtZQ==?= <r@r.s>;");
  }
}

BOOST_AUTO_TEST_SUITE_END()

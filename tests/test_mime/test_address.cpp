#include <boost/test/unit_test.hpp>

#include "s2smtp/mime/address.hpp"
#include "s2smtp/mime/mime_error.hpp"
#include <ostream>


std::ostream &operator<<(std::ostream &os, const s2smtp::mime::mailbox_t &m) {
  return os << m.to_string()
            << "(charset: " << m.display_name().charset.to_string() << ")";
}

namespace boost {
namespace test_tools {
template <> struct tt_detail::print_log_value<s2smtp::mime::mailbox_t> {
  void operator()(std::ostream &os, const s2smtp::mime::mailbox_t &m) {
    ::operator<<(os, m);
  }
};
} // namespace test_tools
} // namespace boost

std::ostream &operator<<(std::ostream &os, const s2smtp::mime::text_t &t) {
  return os << "<" << t.data << "> (charset: " << t.charset.to_string() << ")";
}

namespace boost {
namespace test_tools {
template <> struct tt_detail::print_log_value<s2smtp::mime::text_t> {
  void operator()(std::ostream &os, const s2smtp::mime::text_t &t) {
    ::operator<<(os, t);
  }
};
} // namespace test_tools
} // namespace boost

BOOST_AUTO_TEST_SUITE(test_address)

BOOST_AUTO_TEST_CASE(test_mailbox_ctors) {
  using namespace s2smtp::mime;
  using namespace s2smtp;

  {
    mailbox_t m("test_loc", "test_dom", "test_name");
    BOOST_TEST_CHECK(m.local_part() == "test_loc");
    BOOST_TEST_CHECK(m.domain() == "test_dom");
    BOOST_TEST_CHECK(m.display_name().data == "test_name");
  }

  {
    mailbox_t m("test_loc", "test_dom", "test_name");
    BOOST_TEST_CHECK(m.local_part() == "test_loc");
    BOOST_TEST_CHECK(m.domain() == "test_dom");
    BOOST_TEST_CHECK(m.display_name().data == "test_name");

    auto m2 = std::move(m);
    BOOST_TEST_CHECK(m2.local_part() == "test_loc");
    BOOST_TEST_CHECK(m2.domain() == "test_dom");
    BOOST_TEST_CHECK(m2.display_name().data == "test_name");
    BOOST_TEST_CHECK(m.local_part().size() == 0);
    BOOST_TEST_CHECK(m.domain().size() == 0);
    BOOST_TEST_CHECK(m.display_name().data.size() == 0);
  }

  {
    BOOST_CHECK_THROW(mailbox_t("", "test_dom", "test_name"), mime_error);
    BOOST_CHECK_THROW(mailbox_t("test_loc", "", "test_name"), mime_error);
    BOOST_CHECK_THROW(mailbox_t("test_:loc", "test_dom", "test_name"),
                      mime_error);
    BOOST_CHECK_THROW(mailbox_t("test\n_loc", "test_dom", "test_name"),
                      mime_error);
    BOOST_CHECK_THROW(mailbox_t("test_loc", "test<_dom", "test_name"),
                      mime_error);
  }

  {
    mailbox_t m("test_loc@test_dom.test", "test_name");
    BOOST_TEST_CHECK(m.local_part() == "test_loc");
    BOOST_TEST_CHECK(m.domain() == "test_dom.test");
    BOOST_TEST_CHECK(m.display_name().data == "test_name");
  }

  {
    mailbox_t m("test_loctest_dom.tes@t", "test_name");
    BOOST_TEST_CHECK(m.local_part() == "test_loctest_dom.tes");
    BOOST_TEST_CHECK(m.domain() == "t");
    BOOST_TEST_CHECK(m.display_name().data == "test_name");
  }

  {
    BOOST_CHECK_THROW(mailbox_t("test_dom", "test_name"), mime_error);
    BOOST_CHECK_THROW(mailbox_t("@test_loc", "test_name"), mime_error);
    BOOST_CHECK_THROW(mailbox_t("test_:loc@g.r", "test_name"), mime_error);
    BOOST_CHECK_THROW(mailbox_t("tes@t\n_loc", "test_dom", "test_name"),
                      mime_error);
    BOOST_CHECK_THROW(mailbox_t("@", "test_name"), mime_error);
  }

  {
    mailbox_t m("test_loc@test_dom.test");
    BOOST_TEST_CHECK(m.local_part() == "test_loc");
    BOOST_TEST_CHECK(m.domain() == "test_dom.test");
    BOOST_TEST_CHECK(m.display_name().data == "");
  }

  {
    mailbox_t m1("test_loc@test_dom.test", text_t("test", charsets::UTF_8));
    mailbox_t m2("test_loc@test_dom.test", text_t("test", charsets::us_ascii));
    BOOST_CHECK_NE(m1, m2);
  }

  {
    mailbox_t m1("test_loc@test_dom.test", {"test", charsets::UTF_8});
    mailbox_t m2("test_loc@test_dom.test", {"test", charsets::us_ascii});
    BOOST_CHECK_NE(m1, m2);
  }

  {
    auto tmp = charset_t::default_charset;
    mailbox_t m1("test_loc@test_dom.test", "test");
    charset_t::default_charset = charsets::us_ascii;
    mailbox_t m2("test_loc@test_dom.test", "test");
    charset_t::default_charset = tmp;
    BOOST_CHECK_NE(m1, m2);
  }
}

BOOST_AUTO_TEST_CASE(test_mailbox_list) {
  using namespace s2smtp::mime;
  {
    mailbox_t m("test_loc", "test_dom", "1");
    mailbox_t m1("test_loc@test_dom", "2");
    mailbox_t m2("test_loc@test_dom");

    mailbox_list_t ml = {m, m1, m2};
    BOOST_TEST_REQUIRE(ml.size() == 3);
    BOOST_CHECK_EQUAL(ml[0], m);
    BOOST_CHECK_EQUAL(ml[1], m1);
    BOOST_CHECK_EQUAL(ml[2], m2);

    auto ml1 = std::move(ml);
    BOOST_TEST_REQUIRE(ml1.size() == 3);
    BOOST_CHECK_EQUAL(ml.size(), 0);

    mailbox_t m3("test_loc@test_dom");
    ml1.push_back(std::move(m3));
    BOOST_TEST_REQUIRE(ml1.size() == 4);
    BOOST_CHECK_EQUAL(m3.local_part().size(), 0);
    BOOST_CHECK_EQUAL(m3.domain().size(), 0);
    BOOST_CHECK_EQUAL(ml1[3].local_part(), "test_loc");
  }

  {
    mailbox_list_t ml = {mailbox_t("test_loc", "test_dom", "1"),
                         mailbox_t("test_loc@test_dom", "2")};

    BOOST_TEST_REQUIRE(ml.size() == 2);
  }

  {
    mailbox_list_t ml;
    ml.emplace_back("test_loc", "test_dom", "1");
    ml.emplace_back("localpart@domain.test", "name");
    ml.emplace_back("localpart@domain.test");
    BOOST_TEST_REQUIRE(ml.size() == 3);
    BOOST_CHECK_EQUAL(ml[0].local_part(), "test_loc");
    BOOST_CHECK_EQUAL(ml[0].domain(), "test_dom");
    BOOST_CHECK_EQUAL(ml[0].display_name(), text_t{"1"});
    BOOST_CHECK_EQUAL(ml[1].local_part(), "localpart");
    BOOST_CHECK_EQUAL(ml[1].domain(), "domain.test");
    BOOST_CHECK_EQUAL(ml[1].display_name(), text_t{"name"});
    BOOST_CHECK_EQUAL(ml[2].local_part(), "localpart");
    BOOST_CHECK_EQUAL(ml[2].domain(), "domain.test");
    BOOST_CHECK_EQUAL(ml[2].display_name(), text_t{""});
    ml.clear();
    BOOST_TEST_REQUIRE(ml.size() == 0);
  }
}

BOOST_AUTO_TEST_SUITE_END()

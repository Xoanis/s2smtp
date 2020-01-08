#include <boost/test/unit_test.hpp>

#include "s2smtp/mime/attachment.hpp"
#include <sstream>

std::ostream &operator<<(std::ostream &os,
                         const s2smtp::mime::attachment_info_t &ai) {
  return os << "(name: " << ai.name << ", content_type: " << ai.content_type
            << ")";
}

namespace boost {
namespace test_tools {
template <> struct tt_detail::print_log_value<s2smtp::mime::attachment_info_t> {
  void operator()(std::ostream &os, const s2smtp::mime::attachment_info_t &ai) {
    ::operator<<(os, ai);
  }
};
} // namespace test_tools
} // namespace boost

BOOST_AUTO_TEST_SUITE(test_attachment)

BOOST_AUTO_TEST_CASE(test_attachment_info_ctors) {
  using namespace s2smtp::mime;
  {
    attachment_info_t ai("a1", "ct1");
    BOOST_CHECK_EQUAL(ai.name, "a1");
    BOOST_CHECK_EQUAL(ai.content_type, "ct1");
  }

  {
    attachment_info_t ai("a1");
    BOOST_CHECK_EQUAL(ai.name, "a1");
    BOOST_CHECK_EQUAL(ai.content_type, attachment_info_t::default_content_type);
  }
}

BOOST_AUTO_TEST_CASE(test_attachment_ctors) {
  using namespace s2smtp;
  using namespace s2smtp::mime;
  {
    attachment_t::octetvector_t v = {10, 12, 14, 15, 16, 17};
    std::stringstream s;
    for (auto c : v)
      s << c;

    attachment_info_t ai("a1", "ct1");
    attachment_t a(ai, s);
    BOOST_CHECK_EQUAL(a.info(), ai);
    BOOST_CHECK(a.content() == v);
  }

  {
    attachment_t::octetvector_t v = {10, 12, 14, 15, 16, 17};
    std::stringstream s;
    for (auto c : v)
      s << c;

    attachment_info_t ai("a1", "ct1");
    attachment_t a(ai, std::stringstream(std::move(s)));
    BOOST_CHECK_EQUAL(a.info(), ai);
    BOOST_CHECK(a.content() == v);
  }

  {
    std::stringstream s("some_data");
    s.setstate(std::ios_base::failbit);
    BOOST_CHECK_THROW(attachment_t("", s), mime_error);
  }

  {
    std::stringstream s("some_data");
    s.setstate(std::ios_base::badbit);
    BOOST_CHECK_THROW(attachment_t("", s), mime_error);
  }
}

BOOST_AUTO_TEST_SUITE_END()

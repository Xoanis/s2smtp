#include <boost/test/unit_test.hpp>

#include "serialization/mime/header_serialization.hpp"
#include "serialization/mime/message_serialization.hpp"

BOOST_AUTO_TEST_SUITE(test_message_serialization)

BOOST_AUTO_TEST_CASE(test_header_serialization)
{
    using namespace s2smtp::mime;
    using namespace s2smtp::serialization;
    std::tm t = get_local_time();

    auto get_date_string = [&t] () {
        std::stringstream s;
        serialize(s, t);
        return s.str();
    };

    {
        std::stringstream s;

        header_t h(mailbox_t("a@a.a"), t);
        h.bcc.emplace_back(mailbox_t("bcc1@bcc.bcc"));
        h.bcc.emplace_back(mailbox_t("bcc2@bcc.bcc"));
        h.cc.emplace_back(mailbox_t("cc1@cc.cc"));
        h.cc.emplace_back(mailbox_t("cc2@cc.cc"));
        h.reply_to.emplace_back(mailbox_t("reply@u"));
        h.subject = "test";
        h.to.emplace_back(mailbox_t("to@me.i.a", "Rcpt"));

        serialize(s, h);

        BOOST_CHECK_EQUAL(s.str(),
                          "MIME-Version: 1.0\r\n"
                          "From: a@a.a\r\n"
                          "Reply-To: reply@u\r\n"
                          "To: =?UTF-8?B?UmNwdA==?= <to@me.i.a>\r\n"
                          "Cc: cc1@cc.cc, cc2@cc.cc\r\n"
                          "Date: " + get_date_string() + "\r\n"
                          "Subject: =?UTF-8?B?dGVzdA==?=\r\n");
    }

    {
        std::stringstream s;

        header_t h(mailbox_t("a@a.a"), t);
        h.bcc.emplace_back(mailbox_t("bcc1@bcc.bcc"));
        h.bcc.emplace_back(mailbox_t("bcc2@bcc.bcc"));
        h.cc.emplace_back(mailbox_t("cc1@cc.cc"));
        h.cc.emplace_back(mailbox_t("cc2@cc.cc"));
        h.reply_to.emplace_back(mailbox_t("reply@u"));
        h.subject = "";
        h.to.emplace_back(mailbox_t("to@me.i.a", "Rcpt"));

        serialize(s, h);

        BOOST_CHECK_EQUAL(s.str(),
                          "MIME-Version: 1.0\r\n"
                          "From: a@a.a\r\n"
                          "Reply-To: reply@u\r\n"
                          "To: =?UTF-8?B?UmNwdA==?= <to@me.i.a>\r\n"
                          "Cc: cc1@cc.cc, cc2@cc.cc\r\n"
                          "Date: " + get_date_string() + "\r\n"
                          "Subject: \r\n");
    }
}

BOOST_AUTO_TEST_CASE(test_message_serialization)
{
    using namespace s2smtp::mime;
    using namespace s2smtp::serialization;
    std::tm t = get_local_time();

    auto get_date_string = [&t] () {
        std::stringstream s;
        serialize(s, t);
        return s.str();
    };

    {
        message_t msg(mailbox_t("a@a.a"), t);
        msg.header.bcc.emplace_back(mailbox_t("bcc1@bcc.bcc"));
        msg.header.bcc.emplace_back(mailbox_t("bcc2@bcc.bcc"));
        msg.header.cc.emplace_back(mailbox_t("cc1@cc.cc"));
        msg.header.cc.emplace_back(mailbox_t("cc2@cc.cc"));
        msg.header.reply_to.emplace_back(mailbox_t("reply@u"));
        msg.header.subject = "test";
        msg.header.to.emplace_back(mailbox_t("to@me.i.a", "Rcpt"));

        auto str = serialize(msg);

        BOOST_CHECK_EQUAL(str,
                          "MIME-Version: 1.0\r\n"
                          "From: a@a.a\r\n"
                          "Reply-To: reply@u\r\n"
                          "To: =?UTF-8?B?UmNwdA==?= <to@me.i.a>\r\n"
                          "Cc: cc1@cc.cc, cc2@cc.cc\r\n"
                          "Date: " + get_date_string() + "\r\n"
                          "Subject: =?UTF-8?B?dGVzdA==?=\r\n");
    }

    using namespace s2smtp::mime;
    using namespace s2smtp::serialization;
    {
        message_t msg(mailbox_t("a@a.a"), t);
        msg.header.bcc.emplace_back(mailbox_t("bcc1@bcc.bcc"));
        msg.header.cc.emplace_back(mailbox_t("cc1@cc.cc"));
        msg.header.subject = "test";
        msg.header.to.emplace_back(mailbox_t("to@me.i.a", "Rcpt"));
        msg.text.content = "test";

        auto str = serialize(msg);

        BOOST_CHECK_EQUAL(str,
                          "MIME-Version: 1.0\r\n"
                          "From: a@a.a\r\n"
                          "To: =?UTF-8?B?UmNwdA==?= <to@me.i.a>\r\n"
                          "Cc: cc1@cc.cc\r\n"
                          "Date: " + get_date_string() + "\r\n"
                          "Subject: =?UTF-8?B?dGVzdA==?=\r\n"
                          "Content-type: text/plain; charset=\"UTF-8\"\r\n"
                          "Content-transfer-encoding: base64\r\n"
                          "Content-Disposition: inline\r\n"
                          "\r\ndGVzdA==");
    }
}

BOOST_AUTO_TEST_SUITE_END()
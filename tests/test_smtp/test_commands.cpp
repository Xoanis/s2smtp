#include "s2smtp/network/base_channel.hpp"
#include "smtp/commands.hpp"
#include <boost/test/unit_test.hpp>
#include <chrono>
#include <string>


class mock_channel_t : public s2smtp::network::base_channel_t {
public:
  void write(const std::string &data, s2smtp::network::duration_t timeout =
                                          std::chrono::seconds(0)) override {
    (void)timeout;
    in << data;
  }
  std::string read_until(std::string_view delim,
                         s2smtp::network::duration_t timeout) override {
    (void)timeout;
    std::string res;
    char ldc = *(delim.rbegin());
    size_t delim_size = delim.size(), res_size;
    do {
      std::string temp;
      if (!std::getline(out, temp, ldc))
        throw std::runtime_error("empty stream");
      res += temp + ldc;
      res_size = res.size();
    } while ((res_size < delim_size) ||
             (res.substr(res_size - delim_size, delim_size) != delim));
    return res;
  }
  void upgrade_to_secure(s2smtp::network::ssl_protocol_t ssl_protocol,
                         s2smtp::network::duration_t timeout) override {
    (void)ssl_protocol;
    (void)timeout;
    handshaked = true;
  }

  void clear() {
    out.str("");
    in.str("");
    handshaked = false;
  }

  std::stringstream out;
  std::stringstream in;
  bool handshaked = false;
};

BOOST_AUTO_TEST_SUITE(test_commands)

BOOST_AUTO_TEST_CASE(test_receive_greeting) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  mock_channel_t mock;

  {
    mock.out << "220 ISIF.USC.EDU Service ready\r\n";
    auto reply = command::receive_greeting(mock, 0s);

    BOOST_CHECK_EQUAL(reply.code, 220);
    BOOST_CHECK_EQUAL(reply.text(), "ISIF.USC.EDU Service ready");
  }
  {
    mock.out << "220 mail.example.com SuperSMTP v 6.1.2 Service ready\r\n";
    auto reply = command::receive_greeting(mock, 0s);

    BOOST_CHECK_EQUAL(reply.code, 220);
    BOOST_CHECK_EQUAL(reply.text(),
                      "mail.example.com SuperSMTP v 6.1.2 Service ready");
  }
  {
    mock.out << "220 [10.0.0.1] Clueless host service ready\r\n";
    auto reply = command::receive_greeting(mock, 0s);

    BOOST_CHECK_EQUAL(reply.code, 220);
    BOOST_CHECK_EQUAL(reply.text(), "[10.0.0.1] Clueless host service ready");
  }
  {
    mock.out << "554\r\n";
    auto reply = command::receive_greeting(mock, 0s);

    BOOST_CHECK_EQUAL(reply.code, 554);
    BOOST_CHECK_EQUAL(reply.text(), "");
  }
}

BOOST_AUTO_TEST_CASE(test_command_helo) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  mock_channel_t mock;

  {
    std::string test_domain = "USC-ISIF.ARPA";

    mock.out << "250 BBN-UNIX.ARPA\r\n";
    auto reply = command::helo(mock, test_domain).execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "HELO USC-ISIF.ARPA\r\n");
    BOOST_CHECK_EQUAL(reply.code, 250);
    BOOST_CHECK_EQUAL(reply.text(), "BBN-UNIX.ARPA");
  }
}

BOOST_AUTO_TEST_CASE(test_command_ehlo) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  mock_channel_t mock;

  {
    std::string test_domain = "bar.com";

    mock.out << "250-foo.com greets bar.com\r\n";
    mock.out << "250-8BITMIME\r\n";
    mock.out << "250-SIZE\r\n";
    mock.out << "250-DSN\r\n";
    mock.out << "250 HELP\r\n";

    auto reply = command::ehlo(mock, test_domain).execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "EHLO bar.com\r\n");
    BOOST_CHECK_EQUAL(reply.code, 250);
    BOOST_CHECK_EQUAL(reply.text(), "foo.com greets bar.com\n"
                                    "8BITMIME\n"
                                    "SIZE\n"
                                    "DSN\n"
                                    "HELP");
  }
}

BOOST_AUTO_TEST_CASE(test_command_mail_from) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  using namespace s2smtp::mime;
  mock_channel_t mock;

  {
    mock.out << "250 OK\r\n";

    auto reply = command::mail_from(mock, mailbox_t("user", "mail.com", "user"))
                     .execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "MAIL FROM:<user@mail.com>\r\n");
    BOOST_CHECK_EQUAL(reply.code, 250);
    BOOST_CHECK_EQUAL(reply.text(), "OK");
    mock.clear();
  }

  {
    mock.out << "451\r\n";

    auto reply = command::mail_from(mock, mailbox_t("user@mail.com", "user"))
                     .execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "MAIL FROM:<user@mail.com>\r\n");
    BOOST_CHECK_EQUAL(reply.code, 451);
    BOOST_CHECK_EQUAL(reply.text(), "");
    mock.clear();
  }
}

BOOST_AUTO_TEST_CASE(test_command_rcpt_to) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  using namespace s2smtp::mime;
  mock_channel_t mock;

  {
    mock.out << "250 OK\r\n";

    auto reply = command::rcpt_to(mock, mailbox_t("user", "mail.com", "user"))
                     .execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "RCPT TO:<user@mail.com>\r\n");
    BOOST_CHECK_EQUAL(reply.code, 250);
    BOOST_CHECK_EQUAL(reply.text(), "OK");
    mock.clear();
  }

  {
    mock.out << "451\r\n";

    auto reply =
        command::rcpt_to(mock, mailbox_t("user@mail.com", "user")).execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "RCPT TO:<user@mail.com>\r\n");
    BOOST_CHECK_EQUAL(reply.code, 451);
    BOOST_CHECK_EQUAL(reply.text(), "");
    mock.clear();
  }
}
BOOST_AUTO_TEST_CASE(test_command_quit) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  using namespace s2smtp::mime;
  mock_channel_t mock;

  {
    mock.out << "221 foo.com Service closing transmission channel\r\n";

    auto reply = command::quit(mock).execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "QUIT\r\n");
    BOOST_CHECK_EQUAL(reply.code, 221);
    BOOST_CHECK_EQUAL(reply.text(),
                      "foo.com Service closing transmission channel");
  }
}

BOOST_AUTO_TEST_CASE(test_command_starttls) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  using namespace s2smtp::mime;
  mock_channel_t mock;

  {
    mock.out << "220 Go ahead\r\n";

    auto reply = command::starttls(mock, s2smtp::network::ssl_protocol_t::e_tls)
                     .execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "STARTTLS\r\n");
    BOOST_CHECK(mock.handshaked);
    BOOST_CHECK_EQUAL(reply.code, 220);
    BOOST_CHECK_EQUAL(reply.text(), "Go ahead");
    mock.clear();
  }

  {
    mock.out << "501 Syntax error (no parameters allowed)\r\n";

    auto reply = command::starttls(mock, s2smtp::network::ssl_protocol_t::e_tls)
                     .execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "STARTTLS\r\n");
    BOOST_CHECK(mock.handshaked == false);
    BOOST_CHECK_EQUAL(reply.code, 501);
    BOOST_CHECK_EQUAL(reply.text(), "Syntax error (no parameters allowed)");
    mock.clear();
  }
}

BOOST_AUTO_TEST_CASE(test_command_auth_login) {
  using namespace std::chrono_literals;
  using namespace s2smtp;
  using namespace s2smtp::mime;
  mock_channel_t mock;

  {
    mock.out << "334 VXNlciBOYW1lAA==\r\n";
    mock.out << "334 UGFzc3dvcmQA\r\n";
    mock.out << "235 Authentication successful.\r\n";

    auto reply =
        command::auth_login(mock, "tim", "tanstaaftanstaaf").execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "AUTH LOGIN\r\n"
                                     "dGlt\r\n"
                                     "dGFuc3RhYWZ0YW5zdGFhZg==\r\n");
    BOOST_CHECK_EQUAL(reply.code, 235);
    BOOST_CHECK_EQUAL(reply.text(), "Authentication successful.");
    mock.clear();
  }

  {
    mock.out << "334 VXNlciBOYW1lAA==\r\n";
    mock.out << "454 4.7.0  Temporary authentication failure\r\n";

    auto reply =
        command::auth_login(mock, "tim", "tanstaaftanstaaf").execute(0s);

    BOOST_CHECK_EQUAL(mock.in.str(), "AUTH LOGIN\r\n"
                                     "dGlt\r\n");
    BOOST_CHECK_EQUAL(reply.code, 454);
    BOOST_CHECK_EQUAL(reply.text(), "4.7.0  Temporary authentication failure");
    mock.clear();
  }
}

BOOST_AUTO_TEST_SUITE_END()

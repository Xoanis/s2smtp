// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef PRIVATE_CHANNEL_HPP
#define PRIVATE_CHANNEL_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/lexical_cast.hpp>

#include "s2smtp/network/network_error.hpp"

namespace s2smtp::network {

using namespace boost::asio;
using duration_t = std::chrono::steady_clock::duration;

static io_context ctx;

class private_channel_t {
public:
  private_channel_t(const std::string &domain, uint16_t port,
                    duration_t connect_timeout)
      : socket_(ctx) {
    auto endpoints = ip::tcp::resolver(ctx).resolve(
        domain, boost::lexical_cast<std::string>(port));

    boost::system::error_code error;
    boost::asio::async_connect(
        socket_, endpoints,
        [&error](const boost::system::error_code &result_error,
                 const ip::tcp::endpoint &) { error = result_error; });

    run(connect_timeout);

    if (error)
      throw network_error(error);
  }

  private_channel_t(private_channel_t &&rh)
      : socket_(std::move(rh.socket_)),
        input_buffer_(std::move(rh.input_buffer_)) {}

  virtual ~private_channel_t() {
    try {
      if (socket_.is_open())
        socket_.close();
    } catch (...) {
    }
  }

  virtual void write(const std::string &data, duration_t timeout) {
    write_(data, timeout, socket_);
  }

  virtual std::string read_until(std::string_view delim, duration_t timeout) {
    return read_until_(delim, timeout, socket_);
  }

protected:
  template <typename StreamSocket>
  std::string read_until_(std::string_view delim, duration_t timeout,
                          StreamSocket &s) {
    boost::system::error_code error;
    std::size_t n = 0;
    boost::asio::async_read_until(
        s, boost::asio::dynamic_buffer(input_buffer_), delim,
        [&error, &n](const boost::system::error_code &result_error,
                     std::size_t result_n) {
          error = result_error;
          n = result_n;
        });

    run(timeout);

    if (error)
      throw network_error(error);

    std::string chunk(input_buffer_.substr(0, n));
    input_buffer_.erase(0, n);
    return chunk;
  }

  template <typename StreamSocket>
  void write_(const std::string &data, duration_t timeout, StreamSocket &s) {
    boost::system::error_code error;
    boost::asio::async_write(
        s, boost::asio::buffer(data),
        [&error](const boost::system::error_code &result_error, std::size_t) {
          error = result_error;
        });

    run(timeout);

    if (error)
      throw network_error(error);
  }

  void run(duration_t timeout) {
    ctx.restart();
    ctx.run_for(timeout);
    if (!ctx.stopped()) {
      socket_.close();
      ctx.run();
    }
  }

protected:
  ip::tcp::socket socket_;
  std::string input_buffer_;
};

using ssl_asio_socket_t = boost::asio::ssl::stream<ip::tcp::socket &>;

class private_secure_channel_t : public private_channel_t {
public:
  private_secure_channel_t(const std::string &domain, uint16_t port,
                           ssl_protocol_t ssl_protocol, duration_t timeout)
      : private_channel_t(domain, port, timeout),
        ssl_ctx_(static_cast<boost::asio::ssl::context::method>(ssl_protocol)),
        ssl_socket_(socket_, ssl_ctx_) {
    handshake_(timeout);
  }

  private_secure_channel_t(private_channel_t &&to_move,
                           ssl_protocol_t ssl_protocol, duration_t timeout)
      : private_channel_t(std::move(to_move)),
        ssl_ctx_(static_cast<boost::asio::ssl::context::method>(ssl_protocol)),
        ssl_socket_(socket_, ssl_ctx_) {
    handshake_(timeout);
  }

  void write(const std::string &data, duration_t timeout) override {
    write_(data, timeout, ssl_socket_);
  }

  std::string read_until(std::string_view delim, duration_t timeout) override {
    return read_until_(delim, timeout, ssl_socket_);
  }

private:
  void handshake_(duration_t timeout) {
    boost::system::error_code error;
    ssl_ctx_.set_verify_mode(boost::asio::ssl::verify_none);
    ssl_socket_.async_handshake(
        boost::asio::ssl::stream_base::client,
        [&error](const boost::system::error_code &result_error) {
          error = result_error;
        });
    run(timeout);

    if (error)
      throw network_error(error);
  }

  boost::asio::ssl::context ssl_ctx_;
  ssl_asio_socket_t ssl_socket_;
};

} // namespace s2smtp::network

#endif // PRIVATE_CHANNEL_HPP
// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "smtp_parse.hpp"

#include <string>
#include <vector>

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/x3.hpp>

#include "s2smtp/smtp/smtp_error.hpp"

namespace s2smtp {
namespace parsing {
namespace ast {

struct reply_line_t {
  std::string code;
  std::string text;
};

using reply_line_list_t = std::vector<reply_line_t>;

struct server_reply_t {
  reply_line_list_t optional_lines;
  reply_line_t required_line;
};

} // namespace ast
} // namespace parsing
} // namespace s2smtp

BOOST_FUSION_ADAPT_STRUCT(s2smtp::parsing::ast::reply_line_t, code, text)
BOOST_FUSION_ADAPT_STRUCT(s2smtp::parsing::ast::server_reply_t, optional_lines,
                          required_line)

namespace s2smtp {
namespace parsing {
namespace parsers {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::char_;
using x3::lit;
using x3::repeat;

auto const crlf = lit("\r\n");
auto const sp = lit(' ');

auto const reply_code = x3::rule<class reply_code, std::string>() =
    char_("2-5") >> char_("0-5") >> char_("0-9");

auto const textstring = x3::rule<class textstring, std::string>() =
    +(char_('\t') | char_("\x20-\x7e"));

auto const multiline_signature =
    x3::rule<class multiline_signature, ast::reply_line_t>() =
        reply_code >> "-" >> -textstring >> crlf;

auto const optional_part =
    x3::rule<class optional_part, ast::reply_line_list_t>() =
        *multiline_signature;

auto const singleline_or_end_signature =
    x3::rule<class singleline_or_end_signature, ast::reply_line_t>() =
        reply_code >> -(sp >> textstring) >> crlf;

auto const server_reply = x3::rule<class server_reply, ast::server_reply_t>() =
    optional_part >> singleline_or_end_signature;
} // namespace parsers

using boost::spirit::x3::parse;

server_reply convert_from_ast(ast::server_reply_t ast) {
  auto code = boost::lexical_cast<uint16_t>(ast.required_line.code);
  std::vector<std::string> text_lines;
  for (auto &item : ast.optional_lines) {
    if (code != boost::lexical_cast<uint16_t>(item.code))
      throw smtp_error("Wrong multiline reply format");
    text_lines.push_back(std::move(item.text));
  }
  text_lines.push_back(std::move(ast.required_line.text));
  return server_reply(boost::lexical_cast<uint16_t>(code),
                      std::move(text_lines));
}

bool is_begin_of_multiline_reply(const std::string &data) {
  auto it = data.begin();
  auto end = data.end();
  return parse(it, end, parsers::multiline_signature);
}

bool is_end_of_multiline_reply(const std::string &data) {
  auto it = data.begin();
  auto end = data.end();
  return parse(it, end, parsers::singleline_or_end_signature);
}

server_reply parse_server_reply(const std::string &data) {
  ast::server_reply_t ast;
  auto it = data.begin();
  auto end = data.end();

  if (!parse(it, end, parsers::server_reply, ast))
    throw smtp_error("can't parse -> \"" + std::string(it, end) + '\"');

  return convert_from_ast(ast);
}

} // namespace parsing
} // namespace s2smtp

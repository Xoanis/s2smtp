// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/mime/attachment.hpp"

#if ( defined(__GNUC__) && !defined(__llvm__) && (__GNUC___ < 8) )
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif
#include <fstream>
#include <iterator>

namespace s2smtp::mime {

const std::string attachment_info_t::default_content_type =
    "application/octet-stream";

void check_rdstate(const attachment_info_t &info, const std::istream &stream) {
  if (stream.rdstate())
    throw s2smtp::mime_error("File \"" + info.name +
                             "\": Stream reading error");
}

bool attachment_info_t::operator==(const attachment_info_t &rh) const {
  return (name == rh.name) && (content_type == rh.content_type);
}

attachment_info_t::attachment_info_t(std::string name, std::string content_type)
    : name(std::move(name)), content_type(std::move(content_type)) {}

attachment_info_t::attachment_info_t(std::string name)
    : attachment_info_t(std::move(name), default_content_type) {}

attachment_t::attachment_t(attachment_info_t info, std::istream &stream)
    : info_(std::move(info)), content_(std::istreambuf_iterator<char>(stream),
                                       std::istreambuf_iterator<char>()) {
  check_rdstate(info_, stream);
}

attachment_t::attachment_t(attachment_info_t info, std::istream &&stream)
    : info_(std::move(info)), content_(std::istreambuf_iterator<char>(stream),
                                       std::istreambuf_iterator<char>()) {
  check_rdstate(info_, stream);
}

attachment_t::attachment_t(const std::string &attach_name, std::istream &stream)
    : attachment_t(attachment_info_t(attach_name), stream) {}

attachment_t::attachment_t(const std::string &attach_name,
                           std::istream &&stream)
    : attachment_t(attachment_info_t(attach_name), stream) {}

std::ifstream open_fstream(const std::string &file_path) {
  std::ifstream fstream(file_path, std::ios_base::binary);
  if (!fstream)
    throw mime_error("Failed to open file \"" + file_path + "\"");
  return fstream;
}

attachment_t::attachment_t(const std::string &file_path)
    : attachment_t(fs::path(file_path).filename().string(),
                   open_fstream(file_path)) {}

} // namespace s2smtp::mime

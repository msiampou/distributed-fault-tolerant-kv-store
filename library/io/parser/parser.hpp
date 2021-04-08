#ifndef _LIBRARY_IO_PARSER_PARSER_HPP_
#define _LIBRARY_IO_PARSER_PARSER_HPP_

#include <string>
#include <vector>
#include <queue>

namespace io {
  std::vector<std::string> file_parser(std::string filename);
  std::queue<std::string> extract_keys(std::string json);
  std::vector<std::string> split_keypath(std::string keypath);
  std::string construct_keypath(std::vector<std::string> container);
  std::vector<std::vector<std::string>> json_parser(std::string json, std::queue<std::string>& q);
  std::string json_manipulation(std::string json);
}

#endif
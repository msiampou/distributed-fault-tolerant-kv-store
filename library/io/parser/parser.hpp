#ifndef _LIBRARY_IO_PARSER_PARSER_HPP_
#define _LIBRARY_IO_PARSER_PARSER_HPP_

#include <string>
#include <vector>
#include <queue>

namespace io {
  /// Parses the data file.
  std::vector<std::string> file_parser(std::string filename);
  /// Parses the server file.
  void file_parser(std::vector<std::uint32_t>& ports, std::vector<std::string>& addresses, std::string filename);
  /// Parses json input and stores values in a queue of strings.
  std::queue<std::string> extract_keys(std::string json);
  /// Used by Query command to split keypath to a vector of strings.
  std::vector<std::string> split_keypath(std::string keypath);
  /// Used by GET / Query command to construct string keypath from a vector of strings.
  std::string construct_keypath(std::vector<std::string> container);
  /// Parses json input and constructs key-value keypaths to latter insert them into trie.
  std::vector<std::vector<std::string>> json_parser(std::string json, std::queue<std::string>& q);
  /// Manipulates json input in order to latter parse it.
  std::string json_manipulation(std::string json);
} // namespace io

#endif // _LIBRARY_IO_PARSER_PARSER_HPP_
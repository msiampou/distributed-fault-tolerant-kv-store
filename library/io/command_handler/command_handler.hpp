#ifndef _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
#define _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_

#include <string>
#include <vector>

#include "../../structures/trie.hpp"

namespace io {
  std::string handle_request(std::string key, trie<std::string>& dict);
  std::string put_request(std::string key, trie<std::string>& dict);
  std::string get_request(std::string key, trie<std::string>& dict);
  std::string query_request(std::string key, trie<std::string>& dict);
  std::string delete_request(std::string key, trie<std::string>& dict);
}

#endif // _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
#ifndef _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
#define _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_

#include <string>
#include <vector>

#include "../../structures/trie.hpp"

namespace io {
  /// Handles requests from broker
  std::string handle_request(std::string key, trie<std::string>& dict);
  /// Implementation of PUT request. Inserts data into trie.
  std::string put_request(std::string key, trie<std::string>& dict);
  /// Implementation of GET request. Returns the values of a high level key.
  std::string get_request(std::string key, trie<std::string>& dict);
  /// Implementation of QUERY request. Returns the values of a specific key.
  std::string query_request(std::string key, trie<std::string>& dict);
  /// Implementation of DELETE request. Deletes a high level key.
  std::string delete_request(std::string key, trie<std::string>& dict);
} // namespace io

#endif // _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
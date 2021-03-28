#ifndef _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
#define _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_

#include <string>
#include <vector>

namespace io {

  //template <typename Key>
  std::string handle_request(std::string key);

  //template <typename Key>
  std::string put_request(std::string key);
  
  //template <typename Key>
  std::string get_request(std::string key);

  //template <typename Key>
  std::string query_request(std::string key);

  //template <typename Key>
  std::string delete_request(std::string key);
}

#endif // _LIBRARY_IO_COMMAND_HANDLER_COMMAND_HANDLER_HPP_
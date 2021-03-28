#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../../../library/io/command_handler/command_handler.hpp"

namespace io {

  // TODO: switch case with string
  std::string handle_request(std::string key) {
    switch(key[0]) {
      case 'P' :
        return put_request(key);
        break;
      case 'G' :
        return get_request(key);
        break;
      case 'Q' :
        return query_request(key);
        break;
      case 'D' :
        return delete_request(key);
        break;
      case 'E' :
        return "e";
        break;
      default :
        throw "Unknown command";
    }
  }
  
  std::string put_request(std::string key) {
    return "ΟΚ";
  }
  
  std::string get_request(std::string key) {
    return "OK";
  }

  std::string query_request(std::string key) {
    return "OK";
  }

  std::string delete_request(std::string key) {
    return "ΟΚ";
  }
  
} // namespace io
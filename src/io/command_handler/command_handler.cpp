#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

#include "../../../library/io/io.hpp"
#include "../../../library/structures/trie.hpp"

namespace io {
  /// Handles requests, depending on the first letter of the command. Throws
  /// an error in case of unknown command.
  std::string handle_request(std::string key, trie<std::string>& dict) {
    switch(key[0]) {
      case 'P' :
        return put_request(key, dict);
        break;
      case 'G' :
        return get_request(key, dict);
        break;
      case 'Q' :
        return query_request(key, dict);
        break;
      case 'D' :
        return delete_request(key, dict);
        break;
      case 'E' :
        return "e";
        break;
      default :
        return "Unknown command" ;
    }
  }
  
  /// Implementation of PUT request. Inserts data into trie.
  std::string put_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 4);
    std::string json_str = io::json_manipulation(key);
    auto attr =  io::extract_keys(json_str);
    auto keypaths = io::json_parser(json_str, attr);

    bool ok = true;
    for(auto& path:keypaths) {
      std::vector<std::string>::const_iterator first = path.begin();
      std::vector<std::string>::const_iterator last = path.end()-1;
      std::vector<std::string> dup(first, last);
      if (dict.query(dup).empty()) {
        ok = ok && dict.insert(path);
      }
    }
    return ok ? "ΟΚ":"ERROR";
  }

  /// Implementation of GET request. Returns the values of a high level key.  
  std::string get_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 4);
    auto key_list = dict.get(key);
    return key_list.empty() ? "NOT FOUND":io::construct_keypath(key_list);    
  }

  /// Implementation of QUERY request. Returns the values of a specific key.
  std::string query_request(std::string keypath, trie<std::string>& dict) {
    keypath.erase(0, 6);
    std::vector<std::string> key_list;
    if (keypath.find(".") != std::string::npos) {
      auto branch = io::split_keypath(keypath);
      key_list = dict.query(branch);
    } else {
      key_list = dict.query(keypath);
    }
    return key_list.empty() ? "NOT FOUND":io::construct_keypath(key_list); 
  }

  /// Implementation of DELETE request. Deletes a high level key.
  std::string delete_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 7);
    bool ok = dict.del(key);
    return ok ? "ΟΚ":"ERROR";
  }
  
} // namespace io
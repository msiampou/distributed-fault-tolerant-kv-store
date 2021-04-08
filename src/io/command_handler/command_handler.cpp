#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

#include "../../../library/io/io.hpp"
#include "../../../library/structures/trie.hpp"

namespace io {

  // TODO: switch case with string
  std::string handle_request(std::string key, trie<std::string>& dict) {
    std::cout << key << std::endl;
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
        throw "Unknown command";
    }
  }
  
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
  
  std::string get_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 4);
    auto key_list = dict.get(key);
    if (key_list.empty()) {
      return "NOT FOUND";
    } else {
      return io::construct_keypath(key_list);
    }    
  }

  std::string query_request(std::string keypath, trie<std::string>& dict) {
    keypath.erase(0, 6);
    if (keypath.find(".") != std::string::npos) {
      auto branch = io::split_keypath(keypath);
      auto ret_val = dict.query(branch);
      return (!ret_val.empty()) ? ret_val:"NOT FOUND";
    } else {
      auto key_list = dict.query(keypath);
      if (key_list.empty()) {
        return "NOT FOUND";
      } else {
        return io::construct_keypath(key_list);
      }
    } 
  }

  std::string delete_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 7);
    bool ok = dict.del(key);
    return ok ? "ΟΚ":"ERROR";
  }
  
} // namespace io
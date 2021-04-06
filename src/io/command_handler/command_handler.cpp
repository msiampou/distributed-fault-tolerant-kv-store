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
      ok = ok && dict.insert(path);
    }
    dict.print();
    return ok ? "ΟΚ":"ERROR";
  }
  
  std::string get_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 4);
    return dict.get(key);
  }

  std::string query_request(std::string keypath, trie<std::string>& dict) {
    keypath.erase(0, 6);
    if (keypath.find(".") != std::string::npos) {
      auto branch = io::split_keypath(keypath);
      return dict.query(branch);
    } else {
      return dict.query(keypath);
    } 
  }

  std::string delete_request(std::string key, trie<std::string>& dict) {
    key.erase(0, 7);
    bool ok = dict.del(key);
    return ok ? "ΟΚ":"ERROR";
  }
  
} // namespace io
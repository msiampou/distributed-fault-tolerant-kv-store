#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>

#include "../../../library/io/io.hpp"

namespace io {
  /// Parses the data file.
  std::vector<std::string> file_parser(std::string filename) {
    std::vector<std::string> container;
    std::ifstream f(filename);
    assert(f.is_open() && "Couln't open file");
    std::string line;
    while (getline(f,line)) {
      container.push_back(line);
    }
    f.close();
    return container;
  }

  /// Parses the server file. File contains servers' ports and addresses.
  void file_parser(std::vector<std::uint32_t>& ports,
                  std::vector<std::string>& addresses, std::string filename) {
    std::ifstream f(filename);
    assert(f.is_open() && "Couln't open file");
    std::string line;
    while (getline(f,line)) {
      auto first_token = line.substr(0, line.find(' '));
      auto second_token = line.substr(line.find(' ')+1, line.length());
      addresses.push_back(first_token);
      ports.push_back(static_cast<uint32_t>(std::stoul(second_token)));
    }
    f.close();
  }

  /// Parses json input and stores values in a queue of strings.
  std::queue<std::string> extract_keys(std::string json) {
    std::string const delims {" :;{}"};
    std::queue<std::string> q;
    size_t beg, pos = 0;
    while ((beg = json.find_first_not_of(delims, pos)) != std::string::npos) {
      pos = json.find_first_of(delims, beg + 1);
      q.push(json.substr(beg, pos - beg));
    }
    return q;
  }

  /// Manipulates json input in order to latter parse it.
  std::string json_manipulation(std::string json) {
    json += ';';
    size_t index = 0;
    while (true) {
      // locate the substring to replace
      index = json.find("{}", index);
      if (index == std::string::npos) {
        break;
      }
      // make the replacement
      json.replace(index, 2, "/0");
      // advance index forward so the next iteration doesn't pick it up as well
      index += 2;
    }
    return json;
  }

  /// Parses json input and constructs key-value keypaths to latter insert them
  /// into trie.
  std::vector<std::vector<std::string>> json_parser(std::string json,
                                                  std::queue<std::string>& q) {
    std::vector<std::vector<std::string>> keypaths;
    std::stack<std::string> s;
    int lvl = 0;
    for (char const& c: json) {
      if (c == ':') {
        std::string val = q.front();
        q.pop();
        s.push(val);
        // entering a deeper level of nesting
      } else if (c == '{') {
        lvl +=1;
        // exiting a level of nesting
      } else if (c == '}') {
        lvl-=1;
        // read a value
      } else if (c == ';') {
        std::string val = q.front();
        q.pop();
        s.push(val);
        std::stack<std::string> temp = s;
        std::vector<std::string> v;
        // construct a path for each high level key
        while(!temp.empty()) {
          v.insert(v.begin(), temp.top());
          temp.pop();
        }
        keypaths.push_back(v);
        while(s.size() != lvl) {
          s.pop();
        }
      }
    }
    return keypaths;
  }

  /// Used by Query command to split keypath to a vector of strings.
  std::vector<std::string> split_keypath(std::string keypath) {
    std::string const delims {"."};
    std::vector<std::string> vec;
    size_t beg, pos = 0;
    while ((beg = keypath.find_first_not_of(delims, pos)) != std::string::npos) {
      pos = keypath.find_first_of(delims, beg + 1);
      vec.push_back(keypath.substr(beg, pos - beg));
    }
    return vec;
  }

  /// Used by GET / Query command to construct string keypath from a vector of
  /// strings.
  std::string construct_keypath(std::vector<std::string> key_list) {
    std::string keypath = "";
    for(auto& key:key_list) {
      if (key.empty()) {
        keypath += "\n";
      } else {
        keypath += key;
        keypath += " ";
      }
    }
    return keypath;
  }
} // namespace io
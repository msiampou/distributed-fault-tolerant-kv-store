#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#include "../../../library/io/parser/parser.hpp"

namespace io {
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
} // namespace io
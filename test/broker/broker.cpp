#include <iostream>
#include <vector>

#include "../../library/network/broker/client.hpp"
#include "../../library/io/io.hpp"

int main (void) {
  std::vector<int32_t> ports{8888, 8081};
  std::vector<std::string> addr{"localhost", "localhost"};
  client C(ports, addr, 2);
  auto container = io::file_parser("../../data/testfile.txt");
  std::cout << container.size() << std::endl;
  C.send_data(container);
  C.run();

  return 0;
}
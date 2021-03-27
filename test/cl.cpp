#include <iostream>
#include <vector>

#include "../library/broker/client.hpp"

int main (void) {
  std::vector<int32_t> ports{8888, 8081};
  std::vector<std::string> addr{"localhost", "localhost"};
  client C(ports, addr, 2);

  C.run();

  return 0;
}
#include <iostream>

#include "../../library/network/server/server.hpp"

int main (void) {
  std::int32_t p;
  std::cout << "Enter Port: ";
  std::cin >> p;
  server S(p);

  S.run();

  return 0;
}
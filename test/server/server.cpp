#include <iostream>

#include "../../library/network/server/server.hpp"

int main (int argc, char* argv[]) {

  /// Check the number of the input parameters.
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << "-a IP -p port" << std::endl;
    return 1;
  }
  std::uint32_t p;
  std::string IP;
  for (std::size_t i=1; i<argc; ++i) {
    if (std::string(argv[i]) == "-a") {
      IP = (i+1 < argc) ? std::string(argv[++i]):"";
    } else if (std::string(argv[i]) == "-p") {
      p = (i+1 < argc) ? static_cast<uint32_t>(std::stoul(std::string(argv[++i]))):0;
    } else {
      std::cerr << "Usage: " << argv[0] << "-a IP -p port" << std::endl;
      return 1;
    }
  }
  
  /// Check the validity of the values.
  if (IP == "" || p == 0) {
    std::cerr << "Usage: " << argv[0] << "-a IP -p port" << std::endl;
    return 1;
  }

  /// Run server.
  server S(p,IP);
  S.run();

  return 0;
}
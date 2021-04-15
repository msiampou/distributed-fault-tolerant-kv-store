#include <iostream>
#include <vector>

#include "../../library/network/broker/client.hpp"
#include "../../library/io/io.hpp"

int main (int argc, char* argv[]) {
  /// Check the number of the input parameters.
  if (argc < 7) {
    std::cerr << "Usage: " << argv[0] << "-s serverFile.txt -i datatToIndex.txt -k dublicationFactor" << std::endl;
    return 1;
  }

  /// Parse input parameters.
  std::string server_file, data_file;
  std::int32_t k; //dublication factor
  for (std::size_t i=1; i<argc; ++i) {
    if (std::string(argv[i]) == "-s") {
      server_file = (i+1 < argc) ? std::string(argv[++i]):"";
    } else if (std::string(argv[i]) == "-i") {
      data_file = (i+1 < argc) ? std::string(argv[++i]):"";
    } else if (std::string(argv[i]) == "-k") {
      k = (i+1 < argc) ? atoi(argv[++i]):-1;
    } else {
      std::cerr << "Usage: " << argv[0] << "-s serverFile.txt -i datatToIndex.txt -k dublicationFactor" << std::endl;
      return 1;
    }
  }

  /// Check the validity of the values.
  if (server_file == "" || data_file == "" || k == -1) {
    std::cerr << "Usage: " << argv[0] << "-s serverFile.txt -i datatToIndex.txt -k dublicationFactor" << std::endl;
    return 1;
  }

  std::vector<std::uint32_t> ports; 
  std::vector<std::string> addr; 
  /// Parse input files.
  auto container = io::file_parser(data_file);
  io::file_parser(ports, addr, server_file);
  /// Run broker.
  client C(ports, addr, ports.size());
  if (k > ports.size()) {
    std::cerr << "k must be less than number of servers" << std::endl;
    return 1;
  }
  bool ok = C.send_data(container, k);
  if (ok) {
    std::cout << "Data sent successfully" << std::endl;
  } else {
    std::cout << "Less than " << k << " servers responded.. \n Cannot guarantee the correctness of the next commands \n" << std::endl;
  }
  C.run(k);

  return 0;
}
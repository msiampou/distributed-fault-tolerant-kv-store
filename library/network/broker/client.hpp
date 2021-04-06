#ifndef _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
#define _LIBRARY_NETWORK_BROKER_CLIENT_HPP_

#include <iostream>
#include <string>

#include "../socket/socket.hpp"

class client {
  public:
  template <typename Ports, typename IPs>
  explicit client(Ports& no_ports, IPs& addresses, std::int32_t max_clients) 
          : num_clients(max_clients), sockets(max_clients), new_connections(max_clients) {
    for(std::int32_t i=0; i<num_clients; ++i) {
      sockets[i].create(no_ports[i], addresses[i]);
      new_connections[i] = sockets[i].connect();
    }
  }

  ~client() = default;

  template <typename Container>
  bool send_data(Container& data) {
    bool ok = true;
    for(auto& key:data) {
      for(std::int32_t i=0; i<num_clients; ++i) {     
        sockets[i].send_request("PUT " + key);
        auto result = sockets[i].recv_result();
        std::cout << result << std::endl;
      }
    }
    return ok;
  }

  bool run() {
    bool ok = true;
    while(1) {
      std::string buffer;
      std::getline(std::cin, buffer);
      //std::cin >> buffer;
      for(std::int32_t i=0; i<num_clients; ++i) {     
        sockets[i].send_request(buffer);
        auto result = sockets[i].recv_result();
        std::cout << result << std::endl;
      }
      if (buffer == "E") {
        break;
      }
    }
    return ok;
  }

  private:
    std::int32_t num_clients;
    std::vector<io::socket> sockets;
    std::vector<std::int32_t> new_connections;
};

#endif // _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
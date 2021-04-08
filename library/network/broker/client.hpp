#ifndef _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
#define _LIBRARY_NETWORK_BROKER_CLIENT_HPP_

#include <iostream>
#include <string>
#include <random>

#include "../socket/socket.hpp"

class client {
  public:
  template <typename Ports, typename IPs>
  explicit client(Ports& no_ports, IPs& addresses, std::uint32_t max_clients) 
          : num_clients(max_clients), sockets(max_clients), new_connections(max_clients) {
    for(std::uint32_t i=0; i<num_clients; ++i) {
      sockets[i].create(no_ports[i], addresses[i]);
      new_connections[i] = sockets[i].connect();
    }
  }

  ~client() = default;

  template <typename Container>
  bool send_data(Container& data, std::uint32_t k) {
    bool ok = true;
    for(auto& key:data) {
      for(std::uint32_t srv=0; srv<k; ++srv) {
        std::uint32_t srv_idx = rand()%num_clients;
        sockets[srv_idx].send_request("PUT " + key);
        auto result = sockets[srv_idx].recv_result();
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
      for(std::uint32_t i=0; i<num_clients; ++i) {     
        sockets[i].send_request(buffer);
        auto result = sockets[i].recv_result();
        std::cout << "Received from " << i << " server:" << std::endl << std::endl;
        std::cout << result << std::endl << std::endl;
      }
      if (buffer == "E") {
        break;
      }
    }
    return ok;
  }

  private:
    std::uint32_t num_clients;
    std::vector<io::socket> sockets;
    std::vector<std::int32_t> new_connections;
};

#endif // _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
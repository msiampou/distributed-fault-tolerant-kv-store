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

  void run() {
    while(1) {
      std::string buffer;
      std::cin >> buffer;
      for(std::int32_t i=0; i<num_clients; ++i) {     
        auto msg_bytes = sockets[i].write(buffer);
        msg_bytes = sockets[i].read();
      }
      if (buffer == "q") {
        break;
      }
    }
  }

  private:
    std::int32_t num_clients;
    std::vector<io::socket> sockets;
    std::vector<std::int32_t> new_connections;
};

#endif // _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
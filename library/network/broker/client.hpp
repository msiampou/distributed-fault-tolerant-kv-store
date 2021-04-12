#ifndef _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
#define _LIBRARY_NETWORK_BROKER_CLIENT_HPP_

#include <iostream>
#include <string>
#include <random>

#include "../socket/socket.hpp"

/// Client class represents the broker. Broker connects to the available
/// servers and communicates with them through sockets. Broker is responsible
/// for sending specific requests to the connected servers.
class client {
  public:
    /// 'client' constructor. Receives as arguments a port number and an ip
    /// address for each server, and the total number of servers.
    template <typename Ports, typename IPs>
    explicit client(Ports& no_ports, IPs& addresses, std::uint32_t max_servers) 
            : num_servers(max_servers), sockets(max_servers),
              new_connections(max_servers) {
      // creates a socket for each server and connects. 'new_connections array
      // holds the socket's fd for each server connection
      for(std::uint32_t i=0; i<num_servers; ++i) {
        sockets[i].create(no_ports[i], addresses[i]);
        new_connections[i] = sockets[i].connect();
      }
    }

    /// 'client' default destructor.
    ~client() = default;

    /// Sends data to servers to index them. Takes as arguments a container and
    /// 'data' and a replication factor 'k'.
    template <typename Container>
    bool send_data(Container& data, std::int32_t k) {
      bool ok = true;
      std::int32_t total_active = 0;
      for(auto& key:data) {
        // send each data to k random servers
        for(std::int32_t srv=0; srv<k; ++srv) {
          std::uint32_t srv_idx = rand()%num_servers;
          // send put request to 'srv_idx' server
          bool active = sockets[srv_idx].send_request("PUT " + key);
          total_active += (active) ? 1:(-1);
          // receive results from 'srv_idx' server
          auto result = sockets[srv_idx].recv_result();
          // check if all data were inserted successfullly
          if (active) {
            std::cout << "SERVER " << srv_idx << ": " << result << std::endl;
          } else {
            std::cout << "SERVER " << srv_idx << ": NOT RESPONDED" << std::endl;
          }
          ok = ok && (result != "ERROR") ? true:false;
        }
        if (total_active < k) {
          std::cout << "WARNING MORE THAN K SERVERS SEEM TO BE DOWN.." << std::endl;
          ok = ok && false;
        }
      }
      return ok;
    }

    /// Broker runs until it receives 'E' from the user.
    bool run() {
      bool ok = true;
      while(1) {
        std::string buffer;
        // get command from user
        std::getline(std::cin, buffer);
        // send command to all connected servers
        for(std::uint32_t i=0; i<num_servers; ++i) {
          sockets[i].send_request(buffer);
          auto result = sockets[i].recv_result();
          std::cout << "Received from " << "server:" <<  i << std::endl << std::endl;
          std::cout << result << std::endl << std::endl;
        }
        // user wants to terminate the proccess.
        if (buffer == "E") {
          break;
        }
      }
      return ok;
    }

  private:
    /// Total number of servers to connect to.
    std::uint32_t num_servers;
    /// Socket's vector stores a socket class for each connection.
    std::vector<io::socket> sockets;
    /// Holds the fd of each connection.
    std::vector<std::int32_t> new_connections;
};

#endif // _LIBRARY_NETWORK_BROKER_CLIENT_HPP_
#ifndef _LIBRARY_NETWORK_SOCKET_SOCKET_HPP_
#define _LIBRARY_NETWORK_SOCKET_SOCKET_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <netdb.h>
#include <cassert>
#include <vector>
#include <unistd.h> 
#include <cstring>

#include "../../io/command_handler/command_handler.hpp"
#include "../../structures/trie.hpp"

namespace io {

/// Class 'socket' implements the creation of a TCP socket, that is used for
/// the communication between the broker and a server.
class socket {
  public:
    /// 'socket' uses default constructor.
    socket() = default;

    /// Creates socket using a local IP address.
    template <typename Port>
    void create(Port& port) {
      socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
      assert(socket_fd >= 0 && "Creation of socket failed");

      int opt = 1;
      std::int32_t err = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt), sizeof(opt));
      assert(err >= 0);

      bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(port);
    }

    /// Creates socket using a specific IP address.
    template <typename Port, typename IP>
    void create(Port& port, IP& addr) {
      socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
      assert(socket_fd >= 0 && "Creation of socket failed");

      struct hostent* host = gethostbyname(addr.c_str());
      assert(host && "gethostbyname failed");

      bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy(reinterpret_cast<char*>(host->h_addr), reinterpret_cast<char*>(&serv_addr.sin_addr.s_addr), host->h_length);
      serv_addr.sin_port = htons(port);
    }

    /// Binds socket to a specific address.
    void bind() {
      auto err = ::bind(socket_fd, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr));
      assert(err >= 0 && "Bind failed");
    }

    /// Listens to a specific port.
    void listen() {
      auto err = ::listen(socket_fd, 5);
      assert(err >= 0);
    }

    /// Accepts connection.
    std::int32_t accept() {
      socklen_t clilen = sizeof(cli_addr);
      return ::accept(socket_fd, reinterpret_cast<struct sockaddr*>(&cli_addr), &clilen);
    }

    /// Connects to socket.
    std::int32_t connect() {
      socklen_t addrlen = sizeof(serv_addr);
      return ::connect(socket_fd, reinterpret_cast<struct sockaddr*>(&serv_addr), addrlen);
    }

    /// Reads results from socket.
    /// This method is called by brocker to read results from socket.
    std::string recv_result() {
      std::string result = "";
      auto msg_bytes = 0;
      // splits received data into chunks of 1023 bytes
      while(1) {
        char buffer[1024];
        bzero(buffer, 1024);
        msg_bytes = ::read(socket_fd, buffer, 1023); 
        result += std::string(buffer);
        // continue reading until bytes read are less than chunk size
        if (msg_bytes < 1023) {
          break;
        }
      }
      return result;
    }

    /// Reads requests from socket.
    /// This method is called by server to receive broker's requests from socket.
    std::string recv_request(std::int32_t connection_fd, trie<std::string>& dict) {
      std::string key = "";
      auto msg_bytes = 0;
      // splits received data into chunks of 1023 bytes
      while(1) {
        char buffer[1024];
        bzero(buffer, 1023);
        msg_bytes = ::read(connection_fd, buffer, 1023);
        std::cout << buffer << std::endl;
        std::string buf(buffer);
        key += std::string(buf);
        // continue reading until bytes read are less than chunk size
        if (msg_bytes < 1023 || buffer[msg_bytes-1] == '$') {
          break;
        }
      }
      // handles request
      key.pop_back();
      std::string result = io::handle_request(key, dict);
      return result;
    }

    /// Writes results to socket.
    /// This method is called by server to send results to broker.
    void send_result(std::int32_t connection_fd, std::string message) {
      auto msg_bytes = ::write(connection_fd, message.c_str(), strlen(message.c_str()));
    }

    /// Writes results to socket.
    /// This method is called by broker to requests to server.
    bool send_request(std::string message) {
      message += "$";  
      auto msg_bytes = ::send(socket_fd, message.c_str(), strlen(message.c_str()), MSG_NOSIGNAL);
      return (msg_bytes <= 0) ? false:true;
    }

    /// 'socket' destructor.
    ~socket() {
      close(socket_fd);
    }

  private:
    /// The file descriptor of socket.
    std::int32_t socket_fd;
    /// Server and client socket addresses.
    struct sockaddr_in serv_addr, cli_addr;
};

} // namespace io

#endif // _LIBRARY_NETWORK_SOCKET_SOCKET_HPP_
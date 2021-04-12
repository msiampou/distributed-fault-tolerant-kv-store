#ifndef _LIBRARY_NETWORK_SERVER_SERVER_HPP_
#define _LIBRARY_NETWORK_SERVER_SERVER_HPP_

#include <string>

#include "../socket/socket.hpp"
#include "../../structures/trie.hpp"

/// Server class creates a TCP connection to the available IP and port using
/// sockets. Server is responsible for indexing the received and send query
/// responses back to the broker. Server uses a Trie structure for indexing.
class server {
  public:
  /// 'server' constructor creates a socket, binds it to the 'ip' address, that
  // afterwards listens to the 'port'.
  template <typename Port, typename IP>
  explicit server(Port& port, IP& ip) {
    socket.create(port, ip);
    socket.bind();
    socket.listen();
  }
  
  /// 'server' default destructor.
  ~server() = default;

  /// Server runs until it receives 'e' from the broker. During run, server
  /// creates a trie to index data. The root of Trie consists of a dummy node
  /// with value "+".
  void run() {
    // create a dummy root node
    std::string root_val = "+";
    trie<std::string> dict(root_val);
    // accept connection to socket
    std::int32_t new_connection = socket.accept();
    assert(new_connection >= 0);
    while(1) {
      // receive request from broker
      auto req = socket.recv_request(new_connection, dict);
      // when "e" is received, connection closes
      if (req == "e") {
        socket.send_result(new_connection, "Closing connection..");
        break;
      }
      // send results to broker
      socket.send_result(new_connection, req);
    }
    // close socket
    close(new_connection);
  }
  
  private:
    // 'socket' is used for the communication between the broker and the
    // server.
    io::socket socket;
};

#endif // _LIBRARY_NETWORK_SERVER_SERVER_HPP_
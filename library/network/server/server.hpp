#ifndef _LIBRARY_NETWORK_SERVER_SERVER_HPP_
#define _LIBRARY_NETWORK_SERVER_SERVER_HPP_

#include <string>

#include "../socket/socket.hpp"

class server {
  public:
  template <typename Port>
  explicit server(Port port) {
    socket.create(port);
    socket.bind();
    socket.listen();
  }

  ~server() = default;

  void run() {
    std::int32_t new_connection = socket.accept();
    assert(new_connection >= 0);
    while(1) {
      auto result = socket.recv_request(new_connection);
      if (result == "e") {
        break;
      }
      socket.send_result(new_connection, result);
    }
    close(new_connection);
  }
  
  private:
    io::socket socket;
};

#endif // _LIBRARY_NETWORK_SERVER_SERVER_HPP_
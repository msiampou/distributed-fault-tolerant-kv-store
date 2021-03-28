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
      auto msg_bytes = socket.read(new_connection);
      msg_bytes = socket.write(new_connection, "I got your message");
      if (msg_bytes == 1) {
        break;
      }
    }
    close(new_connection);
  }
  
  private:
    io::socket socket;

}; // class server

#endif // _LIBRARY_NETWORK_SERVER_SERVER_HPP_
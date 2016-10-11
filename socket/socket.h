#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

namespace sock_test {

void error(const char *msg) {
  perror(msg);
  exit(1);
}

class Socket {
public:
  Socket(const char *host, unsigned int port) {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) { error("ERROR opening socket."); }

    hostent *server = gethostbyname(host);
    if (server == NULL) { error("ERROR no such host."); }
    sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      error("ERROR connecting.");
    }
  }
  Socket(int sockfd) : sockfd_(sockfd) {}
  ~Socket() { close(sockfd_); }

  int write(const char *buf, int len) {
    return ::write(sockfd_, buf, len);
  }

  int read(char *buf, int len) {
    bzero(buf, len);
    return ::read(sockfd_, buf, len);
  }

private:
  int sockfd_;
};

} // namespace sock_test

#endif

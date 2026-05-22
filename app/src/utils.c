#ifdef DEBUG
#include <errno.h>
#include <stdio.h>
#include <string.h>
#endif

#include <netdb.h>
#include <sys/socket.h>

int _socket(int domain, int type, int protocol) {
  int fd = socket(domain, type, protocol);

  if (fd < 0) {
#ifdef DEBUG
    fprintf(stderr, "getaddrinfo: %s\n", strerror(errno));
#endif

    return -1;
  }
  return fd;
}

int _getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res) {
  int ret = getaddrinfo(node, service, hints, res);

  if (ret != 0) {
#ifdef DEBUG
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
#endif

    return 1;
  }
  return 0;
}

int _getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host,
                 socklen_t hostlen, char *serv, socklen_t servlen, int flags) {
  int ret = getnameinfo(addr, addrlen, host, hostlen, serv, servlen, flags);

  if (ret != 0) {
#ifdef DEBUG
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
#endif

    return 1;
  }
  return 0;
}

/*  Checksum function implemented following this document:
    RFC 1071: Computing the internet checksum
    https://www.rfc-editor.org/info/rfc1071/
*/
uint16_t _checksum(void *addr, size_t len) {
  uint16_t *word = addr;
  uint32_t result = 0;
  for (size_t i = 0; i < len / sizeof(uint16_t); i++) {
    result += *(word + i);
  }
  if (len % 2 == 1) {
    result += *((uint8_t *)addr + len - 1);
  }
  result = (result >> 16) + (result & 0xffff);
  // Carry the previous add.
  result = (result >> 16) + (result & 0xffff);
  return ~result;
}

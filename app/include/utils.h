#ifndef UTILS_H
#define UTILS_H

#include <netdb.h>

#include <stdint.h>   // uint16_t
#include <stddef.h>   // size_t

int _socket(int domain, int type, int protocol);

int _getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res);

int _getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host,
                 socklen_t hostlen, char *serv, socklen_t servlen, int flags);

uint16_t _checksum(void *addr, size_t len);

#endif

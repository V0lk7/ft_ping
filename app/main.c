// #include <netinet/in.h>
// #include <sys/socket.h>

// #include <stdio.h>

int main(void) {
  // const char *str = "8.8.8.8";
  // int raw_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

  // if (raw_socket == -1) {
  //   printf("Error - Socket");
  // }
  return 0;
}

// #define BUFSIZE 1500
// /* globals */
// char sendbuf[BUFSIZE];
// int datalen; /* #bytes of data following ICMP header */
// char *host;
// int nsent; /* add 1 for each sendto() */
// pid_t pid; /* our PID */
// int sockfd;
// int verbose;

// Create socket
// uint8_t -> unsigned char -> 8bits
// uint16_t -> unsigned short -> 16bits
// uint32_t -> unsigned int -> 32bits
// uint64_t -> unsigned long long -> 64bits

// Echo or Echo Reply Message
//  0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Type      |     Code      |          Checksum             |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |           Identifier          |        Sequence Number        |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Data ...
//    +-+-+-+-+-

// Addresses
// Address of the source of the ech message
// will become the address of destination of the reply
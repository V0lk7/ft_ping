//================================================================================
// You will have to manage a simple IPv4 (address/hostname) as parameters

// You will have to manage FQDN without doing the DNS resolution in the packet
// return

// You are allowed to use all the functions of the printf family

//  -v -?

// Additional -f -l -n -w -W -p -r -s -T --ttl --ip-timestamp flags...

// the flags -V, –usage, –echo are not considered as bonus

// Of course two flags corresponding to the same feature (eg: -t and
// –type) are not considered as two bonuses
//================================================================================

// #include <netinet/in.h>
// #include <sys/socket.h>

// #include <stdio.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <netinet/ip_icmp.h>

// #include <string.h>
// #include <sys/socket.h>
// #include <sys/uio.h>

// #include <stdalign.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ft_ping.h"
#include "icmp_utils.h"
#include "ip_utils.h"
// #include "utils.h"

static uint8_t s_spacket[STD_IP_PACKET_LEN] = {0};
static uint8_t s_rpacket[STD_IP_PACKET_LEN] = {0};

static uint8_t *s_ptr_spacket = s_spacket;
static uint8_t *s_ptr_rpacket = s_rpacket;

static volatile int s_quit = 1;        // trigger the exit of the program
static volatile int s_send_packet = 0; // trigger the send of a new icmp request

// static functions
static void set_quit_handler();
static void set_alarm_handler();
void time_to_send_packet();

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "ping: missing host operand\nTry 'ping --help' or 'ping "
                    "--usage' for more information.\n");
    return 1;
  }

  set_quit_handler();
  set_alarm_handler();

  if (create_packet(s_spacket, argv[1]) == 1) {
    return 1;
  }

  (void)argv;
  (void)s_ptr_rpacket;
  (void)s_ptr_spacket;

  time_to_send_packet();
  while (s_quit) {
    if (s_send_packet) {
      printf("Sending packet!\n");
      s_send_packet = 0;
    }
    usleep(50);
  }
}

void quit_handler(int signo, siginfo_t *info, void *context) {
  (void)signo;
  (void)info;
  (void)context;
  s_quit = 0;
}

static void set_quit_handler() {
  struct sigaction act = {0};
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = &quit_handler;
  if (sigaction(SIGQUIT, &act, NULL) == -1 ||
      sigaction(SIGINT, &act, NULL) == -1) {
#ifdef DEBUG
    perror("[error] set_quit_handler - ernno");
#endif
    exit(EXIT_FAILURE);
  }
}

void time_to_send_packet() {
  s_send_packet = 1;
  errno = 0;
  (void)alarm(1);
  if (errno != 0)
    s_quit = 0;
}

static void set_alarm_handler() {
  struct sigaction act = {0};
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = &time_to_send_packet;
  if (sigaction(SIGALRM, &act, NULL) == -1) {
#ifdef DEBUG
    perror("[error] set_alarm_handler - ernno");
#endif
    exit(EXIT_FAILURE);
  }
}

// struct sockaddr *get_ipv4_node(struct addrinfo *list);

// int main(int argc, char **argv) {
//   (void)argc;
//   (void)argv;
//   printf("sizeof(struct icmphdr) = %zu\n", sizeof(struct icmphdr));
//   printf("alignof(struct icmphdr) = %zu\n", alignof(struct icmphdr));
//   return 0;

//   struct addrinfo hints;
//   struct addrinfo *res;
//   char host[NI_MAXHOST], serv[NI_MAXSERV];

//   memset(&hints, 0, sizeof(struct addrinfo));
//   memset(host, 0, sizeof(host));
//   memset(serv, 0, sizeof(serv));

//   hints.ai_family = AF_INET;
//   hints.ai_socktype = SOCK_RAW;

//   if (argc != 2) {
//     fprintf(stderr, "error: %s\n", argv[0]);
//     return 1;
//   }

//   int ret = _getaddrinfo(argv[1], NULL, &hints, &res);
//   if (ret != 0) {
//     perror("Getaddrinfo");
//     return 1;
//   }

//   // TODO check if res has good family, good type and good protocole
//   struct sockaddr_in sock_copy;
//   memcpy(&sock_copy, res->ai_addr, sizeof(struct sockaddr_in));
//   freeaddrinfo(res);

//   struct msghdr msg_hdr;
//   memset(&msg_hdr, 0, sizeof(msg_hdr));

//   struct iovec iov;
//   memset(&iov, 0, sizeof(iov));

//   msg_hdr.msg_name = &sock_copy;
//   msg_hdr.msg_namelen = sizeof(sock_copy);

//   char packet[9];
//   memset(packet, 0, sizeof(packet));

//   // icmphdr = 8bytes
//   struct icmphdr *icmp_hdr = (struct icmphdr *)packet;

//   icmp_hdr->type = ICMP_ECHO;
//   icmp_hdr->code = 0;
//   icmp_hdr->checksum = 0;
//   icmp_hdr->un.echo.id = htons(getpid());
//   icmp_hdr->un.echo.sequence = 0;

//   memset(packet + sizeof(struct icmphdr), 'A',
//          sizeof(packet) - sizeof(struct icmphdr));
//   icmp_hdr->checksum = _checksum(packet, sizeof(packet));

//   iov.iov_base = packet;
//   iov.iov_len = sizeof(packet);

//   msg_hdr.msg_iov = &iov;
//   msg_hdr.msg_iovlen = 1;

//   int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

//   if (sockfd < 0) {
//     perror("socket");
//     return 1;
//   }

//   sendmsg(sockfd, &msg_hdr, 0);

//   char buffer[1024];
//   struct iovec m_iov;

//   m_iov.iov_base = buffer;
//   m_iov.iov_len = sizeof(buffer);

//   struct sockaddr_storage addr;

//   struct msghdr msg;
//   memset(&msg, 0, sizeof(msg));

//   msg.msg_name = &addr;
//   msg.msg_namelen = sizeof(addr);

//   msg.msg_iov = &m_iov;
//   msg.msg_iovlen = 1;
//   ssize_t n = recvmsg(sockfd, &msg, 0);

//   if (n < 0) {
//     perror("recvmsg");
//     return 1;
//   }

//   struct iphdr *ip = (struct iphdr *)buffer;
//   int ip_header_len = ip->ihl * 4;

//   struct icmphdr *icmp = (struct icmphdr *)(buffer + ip_header_len);

//   if (icmp->type == ICMP_ECHOREPLY) {
//     printf("Got reply!\n");

//     printf("id=%d\n", ntohs(icmp->un.echo.id));
//     printf("seq=%d\n", ntohs(icmp->un.echo.sequence));
//   }

//   char ipstr[INET_ADDRSTRLEN];

//   struct sockaddr_in *sin = (struct sockaddr_in *)&addr;
//   inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr));

//   printf("From: %s\n", ipstr);

//   // for (tmp = res; tmp != NULL; tmp = tmp->ai_next) {
//   //   Getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), serv,
//   //               sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

//   //   // puts(host);
//   // }
//   return 0;
// }

// struct sockaddr *get_ipv4_node(struct addrinfo *list) {
//   struct sockaddr *node = NULL;
//   struct addrinfo *tmp = list;

//   while (tmp) {
//     fprintf(stdout, "Family: %i\tType: %i\tProtocole: %i\n", tmp->ai_family,
//             tmp->ai_socktype, tmp->ai_protocol);
//     tmp = tmp->ai_next;
//   }
//   return node;
// }

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
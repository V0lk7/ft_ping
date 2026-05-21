#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

#include <getopt.h>

#define OPT_ECHO (1 << 0)
#define OPT_ADDRESS (1 << 1)
#define OPT_CORRUPT (1 << 2)
#define OPT_LOSS (1 << 3)
#define OPT_TIMESTAMP (1 << 4)

// Define long options
static struct option long_opts[] = {
    {"echo", 0, 0, 'e'},      {"address", 0, 0, 'a'},
    {"corrupt", 0, 0, 'c'},   {"loss", 0, 0, 'l'},
    {"timestamp", 0, 0, 't'}, {0, 0, 0, 0} // must end with this
};

static volatile int _quit = 0;

static char _buffer[2048] = {0};
static size_t buflen = sizeof(_buffer);
static int _opts = 0;

void set_signal_handler();
int get_options(int argc, char **argv);
void handle_response(int sock, struct msghdr *msg);

void _readmsg(int sock, struct msghdr *msg);
void reset_msg_struct(struct msghdr *msg);

int main(int argc, char **argv) {

  set_signal_handler();

  _opts = get_options(argc, argv);

  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  if (sock < 0) {
    perror("[error] socket | ernno - ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sender;
  struct iovec iov = {.iov_base = _buffer, .iov_len = sizeof(_buffer)};
  struct msghdr msg = {
      .msg_name = &sender,
      .msg_namelen = sizeof(sender),
      .msg_iov = &iov,
      .msg_iovlen = 1,
  };

  while (_quit == 0) {
    reset_msg_struct(&msg);
    _readmsg(sock, &msg);
    handle_response(sock, &msg);
  }

  close(sock);
  return 0;
}

void signal_handler(int signo, siginfo_t *info, void *context) {
  (void)signo;
  (void)info;
  (void)context;
  _quit = 1;
}

void set_signal_handler() {
  struct sigaction act = {0};
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = &signal_handler;
  if (sigaction(SIGQUIT, &act, NULL) == -1 ||
      sigaction(SIGINT, &act, NULL) == -1) {
    perror("[error] set_signal_handler - ernno");
    exit(EXIT_FAILURE);
  }
}

int get_options(int argc, char **argv) {
  int opts = 0;
  int opt;

  if (argc == 1) {
    opts |= OPT_ECHO;
  } else {
    while ((opt = getopt_long(argc, argv, "eaclt", long_opts, NULL)) != -1) {
      switch (opt) {
      case 'e':
        opts |= OPT_ECHO;
        break;
      case 'a':
        opts |= OPT_ADDRESS;
        break;
      case 'c':
        opts |= OPT_CORRUPT;
        break;
      case 'l':
        opts |= OPT_LOSS;
        break;
        break;
      case 't':
        opts |= OPT_TIMESTAMP;
        break;
      }
    }
  }
  return opts;
}

void reset_msg_struct(struct msghdr *msg) {
  memset(msg->msg_name, 0, msg->msg_namelen);
  memset(_buffer, 0, buflen);

  msg->msg_iov->iov_base = _buffer;
  msg->msg_iov->iov_len = sizeof(_buffer);
}

void _readmsg(int sock, struct msghdr *msg) {

  ssize_t n = recvmsg(sock, msg, 0);

  if (n < 0) {
    perror("[error] _readmsg - errno");
    close(sock);
    exit(EXIT_FAILURE);
  }
}

void send_echo_reply(struct msghdr *);
void send_address_reply(struct msghdr *);
void send_corrupt_reply(struct msghdr *);
void send_timestamp_reply(struct msghdr *);

uint16_t checksum(void *addr, size_t len);

void handle_response(int sock, struct msghdr *msg) {

  struct ip *ip = (struct ip *)_buffer;
  struct icmphdr *icmp =
      (struct icmphdr *)(_buffer +
                         (ip->ip_hl * 4)); // 4 -> Bytes (32 bits word)
  // struct in_addr addr = ((struct sockaddr_in *)msg->msg_name)->sin_addr;

  // printf("ICMP type=%d from %s\n", icmp->type, inet_ntoa(addr));

  if (icmp->type == ICMP_ECHO && (_opts & OPT_ECHO))
    send_echo_reply(msg);
  else if (icmp->type == ICMP_TIMESTAMP && (_opts & OPT_TIMESTAMP))
    send_timestamp_reply(msg);
  else if (icmp->type == ICMP_INFO_REQUEST && (_opts & OPT_ADDRESS))
    send_address_reply(msg);
  else
    return; // tout le reste on ignore

  if (sendmsg(sock, msg, 0) < 0) {
    perror("[error] handle_response - errno");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Msg sendt\n");
}

void send_echo_reply(struct msghdr *msg) {
  struct ip *ip = (struct ip *)_buffer;
  struct icmphdr *icmp =
      (struct icmphdr *)(_buffer +
                         (ip->ip_hl * 4)); // 4 -> Bytes (32 bits word)

  icmp->type = 0;
  icmp->checksum = 0;
  size_t icmp_len = ntohs(ip->ip_len) - (ip->ip_hl * 4);

  icmp->checksum = checksum(icmp, icmp_len);
  msg->msg_iov->iov_base = icmp;
  msg->msg_iov->iov_len = icmp_len;

  printf("send_echo_reply - Do nothing for now\n");
  return;
}

void send_address_reply(struct msghdr *msg) {
  (void)msg;
  printf("send_address_reply - Do nothing for now\n");
  return;
}

void send_corrupt_reply(struct msghdr *msg) {
  (void)msg;
  printf("send_corrupt_reply - Do nothing for now\n");
  return;
}

void send_timestamp_reply(struct msghdr *msg) {
  (void)msg;
  printf("send_timestamp_reply - Do nothing for now\n");
  return;
}

uint16_t checksum(void *addr, size_t len) {
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
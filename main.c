#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

static volatile int i = 0;

void handler(int signo, siginfo_t *info, void *context) {
  (void)signo;
  (void)info;
  (void)context;
  i = 1;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  struct sigaction act = {0};
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = &handler;
  if (sigaction(SIGQUIT, &act, NULL) == -1 ||
      sigaction(SIGINT, &act, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  char buffer[4096];

  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  struct sockaddr_in sender;
  struct iovec iov = {.iov_base = buffer, .iov_len = sizeof(buffer)};
  struct msghdr msg = {
      .msg_name = &sender,
      .msg_namelen = sizeof(sender),
      .msg_iov = &iov,
      .msg_iovlen = 1,
  };

  while (i == 0) {
    ssize_t n = recvmsg(sockfd, &msg, 0);

    if (n < 0) {
      perror("recvmsg");
      break;
    }
    struct iphdr *ip = (struct iphdr *)buffer;
    struct icmphdr *icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));
    printf("ICMP type=%d from %s\n", icmp->type, inet_ntoa(sender.sin_addr));
  }
}
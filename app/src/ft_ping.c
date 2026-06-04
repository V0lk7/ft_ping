#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include <sys/time.h>

#include "ft_ping.h"
#include "icmp_utils.h"
// #include "ip_utils.h"
#include "ip_utils.h"
#include "utils.h"

static uint8_t s_std_payload[STD_ICMP_PAYLOAD_LEN] = {0};
static int s_payload_filled = 0;

static struct sockaddr *get_ipv4(struct addrinfo *list);
static void print_packet(uint8_t *packet, size_t size);

void fill_buffer(uint8_t *buf, size_t len);

int create_packet(uint8_t *packet, char *str) {
  struct icmp_infos icmp;
  struct ip_infos ip;

  struct addrinfo hints = {0};
  struct addrinfo *res;

  struct timeval time;
  (void)gettimeofday(&time, NULL);
  memcpy(s_std_payload, &time, sizeof(time));

  fill_buffer(s_std_payload + sizeof(time),
              STD_ICMP_PAYLOAD_LEN - sizeof(time));
  icmp.payload = s_std_payload;
  icmp.payload_size = STD_ICMP_PAYLOAD_LEN;
  icmp.type = ICMP_ECHO;
  icmp.code = 0;

  if (icmp_create_packet(packet + IP_HDR_LEN,
                         ICMP_HEADER_SIZE + STD_ICMP_PAYLOAD_LEN, &icmp) == 1) {
    fprintf(stderr, "Merdeuh - 1");
    return 1;
  }

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int ret = _getaddrinfo(str, NULL, &hints, &res);
  if (ret == 1) {
    return 1;
  }
  (void)get_ipv4(res);
  ip.dest = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
  if (inet_pton(AF_INET, "10.0.2.15", &(ip.src)) < 0) {
    fprintf(stderr, "Merdeuh - 2");
    return 1;
  }
  ip.tos = 0;
  ip.ttl = 64;
  ip.protocol = IPPROTO_ICMP;
  ip.payload = packet + IP_HDR_LEN;
  ip.payload_size = ICMP_HEADER_SIZE + STD_ICMP_PAYLOAD_LEN;

  if (ipv4_create_packet(packet, STD_IP_PACKET_LEN, &ip) == 1) {
    fprintf(stderr, "Merdeuh - 3");
    return 1;
  }
  print_packet(packet, STD_IP_PACKET_LEN);
  return 0;
}

static void print_packet(uint8_t *packet, size_t size) {
  for (size_t i = 0; i + 1 < size; i += 2) {
    printf("%02X%02X ", packet[i], packet[i + 1]);
    if (((i / 2) + 1) % 8 == 0)
      printf("\n");
  }
  if ((size / 2) % 8 != 0)
    printf("\n");
}

static struct sockaddr *get_ipv4(struct addrinfo *list) {
  struct sockaddr *node = NULL;
  struct addrinfo *tmp = list;

  while (tmp) {
    fprintf(stdout, "Family: %i\tType: %i\tProtocole: %i\n", tmp->ai_family,
            tmp->ai_socktype, tmp->ai_protocol);
    tmp = tmp->ai_next;
  }
  return node;
}

void fill_buffer(uint8_t *buf, size_t len) {
  for (size_t i = 0; i < len; i++)
    buf[i] = (uint8_t)(i % 256);
  s_payload_filled = 1;
}
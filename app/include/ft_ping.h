#ifndef FT_PING_H
#define FT_PING_H

#include <stdint.h>

#define STD_ICMP_PAYLOAD_LEN 56
#define STD_IP_PACKET_LEN                                                      \
  IP_HDR_LEN + ICMP_HEADER_SIZE + STD_ICMP_PAYLOAD_LEN // 84Bytes

int create_packet(uint8_t *packet, char *argv);

#endif
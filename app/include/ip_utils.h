#ifndef IP_UTILS_H
#define IP_UTILS_H

#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

#define IP_HDR_LEN 20
#define IP_VHL_OFFSET 0
#define IP_TOS_OFFSET 1
#define IP_LEN_OFFSET 2
#define IP_ID_OFFSET 4
#define IP_OFF_OFFSET 6
#define IP_TTL_OFFSET 8
#define IP_PROTO_OFFSET 9
#define IP_CHECKSUM_OFFSET 10
#define IP_SRC_OFFSET 12
#define IP_DEST_OFFSET 16

struct ip_infos {
    uint8_t tos;
    uint8_t ttl;
    uint8_t protocol;

    struct in_addr src;
    struct in_addr dest;

    uint8_t *payload;
    size_t payload_size;
};

int ipv4_create_packet(uint8_t *packet, size_t packet_size, const struct ip_infos *infos);

#endif
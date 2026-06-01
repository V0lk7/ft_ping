#ifndef ICMP_UTILS_H
#define ICMP_UTILS_H

#include <stddef.h>
#include <stdint.h>

#define ICMP_HEADER_SIZE 8
#define ICMP_OFFSET_TYPE 0
#define ICMP_OFFSET_CODE 1
#define ICMP_OFFSET_CHECKSUM 2
#define ICMP_OFFSET_IDENTIFIER 4
#define ICMP_OFFSET_SEQUENCE_NUMBER 6

struct icmp_infos {
    uint8_t type;
    uint8_t code;

    uint8_t *payload;
    size_t payload_size;
};

int icmp_create_packet(uint8_t *packet, size_t packet_size, const struct icmp_infos *infos);

#endif
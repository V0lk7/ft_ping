#ifndef IP_UTILS_H
#define IP_UTILS_H

#include <stddef.h>
#include <stdint.h>

// struct icmp_infos {
//     uint8_t type;
//     uint8_t code;

//     uint8_t *payload;
//     size_t payload_size;
// };

int ip_create_packet(uint8_t *packet, size_t packet_size, const struct icmp_infos *infos);

#endif
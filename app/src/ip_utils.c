#include "ip_utils.h"
#include "utils.h"

#include <netinet/ip.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int ipv4_create_packet(uint8_t *packet, size_t packet_size,
                       const struct ip_infos *infos) {
  if (packet_size < IP_HDR_LEN + infos->payload_size)
    return 1;

  static uint8_t version = 4;
  static uint8_t ihl = 5;

  packet[IP_VHL_OFFSET] = (version << 4) | (ihl & 0x0F);
  packet[IP_TOS_OFFSET] = infos->tos;

  uint16_t total_len = htons(IP_HDR_LEN + (uint16_t)infos->payload_size);
  memcpy(packet + IP_LEN_OFFSET, &total_len, sizeof(uint16_t));

  uint16_t id = htons(getpid());
  memcpy(packet + IP_ID_OFFSET, &id, sizeof(uint16_t));

  uint16_t frag_off = htons(IP_DF); // 0x4000 DF don't fragment
  memcpy(packet + IP_OFF_OFFSET, &frag_off, sizeof(uint16_t));

  packet[IP_TTL_OFFSET] = infos->ttl;
  packet[IP_PROTO_OFFSET] = infos->protocol;

  uint16_t checksum = 0;
  memcpy(packet + IP_CHECKSUM_OFFSET, &checksum, sizeof(uint16_t));

  // packet[IP_SRC_OFFSET] = infos->src.s_addr;
  memcpy(packet + IP_SRC_OFFSET, &infos->src.s_addr, sizeof(infos->src.s_addr));
  // packet[IP_DEST_OFFSET] = infos->dest.s_addr;
  memcpy(packet + IP_DEST_OFFSET, &infos->dest.s_addr,
         sizeof(infos->dest.s_addr));

  packet[IP_CHECKSUM_OFFSET] = _checksum(packet, IP_HDR_LEN);

  memcpy(packet + IP_HDR_LEN, infos->payload, infos->payload_size);

  return 0;
}

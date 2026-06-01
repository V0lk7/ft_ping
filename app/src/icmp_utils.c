#include "icmp_utils.h"
#include "utils.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

int icmp_create_packet(uint8_t *packet, size_t packet_size,
                       const struct icmp_infos *infos) {

  if (packet_size < (ICMP_HEADER_SIZE + infos->payload_size)) {
    return 1;
  }
  uint16_t id = htons(getpid());
  uint16_t seq = htons(0);

  packet[ICMP_OFFSET_TYPE] = infos->type;
  packet[ICMP_OFFSET_CODE] = infos->code;
  packet[ICMP_OFFSET_CHECKSUM] = 0;

  memcpy(packet + ICMP_OFFSET_IDENTIFIER, &id, sizeof(uint16_t));
  memcpy(packet + ICMP_OFFSET_SEQUENCE_NUMBER, &seq, sizeof(uint16_t));

  memcpy(packet + ICMP_HEADER_SIZE, infos->payload, infos->payload_size);

  uint16_t checksum = _checksum(packet, ICMP_HEADER_SIZE + infos->payload_size);
  memcpy(packet + ICMP_OFFSET_CHECKSUM, &checksum, sizeof(uint16_t));

  return 0;
}

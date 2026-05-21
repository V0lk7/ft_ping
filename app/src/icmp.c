#include "icmp.h"

#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static uint16_t checksum(void *addr, size_t len);

int icmp_create_packet(uint8_t *packet, size_t packet_size,
                       const struct icmp_infos *infos) {

  // Check if packet can contain the icmp header and the payload
  static size_t s_icmphdr_size = sizeof(struct icmphdr);

  if (packet_size < (s_icmphdr_size + infos->payload_size)) {
    return 1;
  }

  struct icmphdr header;

  header.type = infos->type;
  header.code = infos->code;
  header.checksum = 0;
  header.un.echo.id = htons(getpid());
  header.un.echo.sequence = 0;

  (void)memcpy(packet, &header, s_icmphdr_size);
  (void)memcpy(packet + s_icmphdr_size, infos->payload, infos->payload_size);

  ((struct icmphdr *)packet)->checksum =
      checksum(packet, s_icmphdr_size + infos->payload_size);

  return 0;
}

static uint16_t checksum(void *addr, size_t len) {
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

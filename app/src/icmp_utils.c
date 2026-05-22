#include "icmp_utils.h"
#include "utils.h"

#include <netinet/ip_icmp.h>
#include <string.h>
#include <unistd.h>

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
      _checksum(packet, s_icmphdr_size + infos->payload_size);

  return 0;
}

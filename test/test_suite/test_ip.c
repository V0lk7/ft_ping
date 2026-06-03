#include "greatest.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "ip_utils.h"
#include "utils.h"

/* ── Offsets and constants ── */
// #define IP_HDR_LEN 20
// #define IP_VHL_OFFSET 0
// #define IP_TOS_OFFSET 1
// #define IP_LEN_OFFSET 2
// #define IP_ID_OFFSET 4
// #define IP_OFF_OFFSET 6
// #define IP_TTL_OFFSET 8
// #define IP_PROTO_OFFSET 9
// #define IP_CHECKSUM_OFFSET 10
// #define IP_SRC_OFFSET 12
// #define IP_DEST_OFFSET 16
#define IP_DF 0x4000

/* ── Helpers ── */
static uint16_t read_be16(const uint8_t *p) {
  return (uint16_t)((p[0] << 8) | p[1]);
}

static struct ip_infos make_infos(const uint8_t *payload, size_t payload_size) {
  struct ip_infos infos = {0};
  infos.payload = (uint8_t *)payload;
  infos.payload_size = payload_size;
  infos.tos = 0x10;
  infos.ttl = 64;
  infos.protocol = 17; /* UDP */

  struct in_addr tmp;
  inet_pton(AF_INET, "192.168.1.1", &tmp);
  infos.src = tmp;
  inet_pton(AF_INET, "10.0.0.1", &tmp);
  infos.dest = tmp;
  return infos;
}

/* ── Tests ── */

TEST test_return_0_on_success(void) {
  static const uint8_t data[32] = {0};
  uint8_t pkt[IP_HDR_LEN + sizeof(data)] = {0};
  struct ip_infos infos = make_infos(data, sizeof(data));
  ASSERT_EQ(0, ipv4_create_packet(pkt, sizeof(pkt), &infos));
  PASS();
}

TEST test_return_1_if_buffer_too_small(void) {
  static const uint8_t data[16] = {0};
  uint8_t pkt[IP_HDR_LEN + 8] = {0}; /* needs 36 bytes, only 28 given */
  struct ip_infos infos = make_infos(data, sizeof(data));
  ASSERT_EQ(1, ipv4_create_packet(pkt, sizeof(pkt), &infos));
  PASS();
}

TEST test_header_type_and_code(void) {
  static const uint8_t data[16] = {0};
  uint8_t pkt[IP_HDR_LEN + sizeof(data)] = {0};
  struct ip_infos infos = make_infos(data, sizeof(data));
  ipv4_create_packet(pkt, sizeof(pkt), &infos);

  /* VHL: version=4, IHL=5 → 0x45 */
  ASSERT_EQ_FMT(0x45, pkt[IP_VHL_OFFSET], "0x%02x");

  /* TOS */
  ASSERT_EQ_FMT(infos.tos, pkt[IP_TOS_OFFSET], "0x%02x");

  /* TTL */
  ASSERT_EQ_FMT(infos.ttl, pkt[IP_TTL_OFFSET], "%u");

  /* Protocol */
  ASSERT_EQ_FMT(infos.protocol, pkt[IP_PROTO_OFFSET], "%u");

  /* Total length (big-endian) */
  uint16_t expected_len = IP_HDR_LEN + (uint16_t)infos.payload_size;
  uint16_t actual_len = 0;
  memcpy(&actual_len, pkt + IP_LEN_OFFSET, sizeof(uint16_t));
  actual_len = ntohs(actual_len);
  //   ASSERT_EQ_FMT(expected_len, read_be16(pkt + IP_LEN_OFFSET), "%u");
  ASSERT_EQ_FMT(expected_len, actual_len, "%u");

  /* DF flag set, no fragment offset */
  ASSERT_EQ_FMT(IP_DF, read_be16(pkt + IP_OFF_OFFSET), "0x%04x");

  /* Source and destination addresses */
  ASSERT_EQ_FMT(infos.src.s_addr, *(uint32_t *)(pkt + IP_SRC_OFFSET), "%u");
  ASSERT_EQ_FMT(infos.dest.s_addr, *(uint32_t *)(pkt + IP_DEST_OFFSET), "%u");

  PASS();
}

TEST test_payload_written_after_header(void) {
  static const uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xEF,
                                    0xCA, 0xFE, 0x00, 0xFF};
  uint8_t pkt[IP_HDR_LEN + sizeof(payload)] = {0};
  struct ip_infos infos = make_infos(payload, sizeof(payload));

  ipv4_create_packet(pkt, sizeof(pkt), &infos);

  ASSERT_MEM_EQ(payload, pkt + IP_HDR_LEN, sizeof(payload));
  PASS();
}

TEST test_exact_buffer_size(void) {
  static const uint8_t data[4] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t pkt[IP_HDR_LEN + sizeof(data)];
  struct ip_infos infos = make_infos(data, sizeof(data));

  /* packet_size == IP_HDR_LEN + payload_size is the minimum allowed */
  ASSERT_EQm("Exact-size buffer must return 0", 0,
             ipv4_create_packet(pkt, sizeof(pkt), &infos));

  /* Payload must still land correctly at the exact boundary */
  ASSERT_MEM_EQ(data, pkt + IP_HDR_LEN, sizeof(data));
  PASS();
}

TEST test_empty_payload(void) {
  uint8_t pkt[IP_HDR_LEN] = {0};
  struct ip_infos infos = make_infos(NULL, 0);

  ASSERT_EQm("Zero-payload packet must return 0", 0,
             ipv4_create_packet(pkt, sizeof(pkt), &infos));

  uint16_t total_len = read_be16(pkt + IP_LEN_OFFSET);
  ASSERT_EQ_FMT(IP_HDR_LEN, total_len, "%u");
  PASS();
}

/* ── Suite ── */
SUITE(ip_suite) {
  RUN_TEST(test_return_0_on_success);
  RUN_TEST(test_return_1_if_buffer_too_small);
  RUN_TEST(test_header_type_and_code);
  RUN_TEST(test_payload_written_after_header);
  RUN_TEST(test_exact_buffer_size);
  RUN_TEST(test_empty_payload);
}

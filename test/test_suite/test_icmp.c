#include "greatest.h"
#include "icmp.h"
#include <netinet/ip_icmp.h>
#include <stdint.h>

/* Recalcule le checksum pour vérification */
static uint16_t verify_checksum(void *addr, size_t len) {
  uint16_t *word = addr;
  uint32_t result = 0;
  for (size_t i = 0; i < len / sizeof(uint16_t); i++)
    result += *(word + i);
  if (len % 2 == 1)
    result += *((uint8_t *)addr + len - 1);
  result = (result >> 16) + (result & 0xffff);
  result = (result >> 16) + (result & 0xffff);
  return ~result;
}

TEST test_return_0_on_success(void) {
  uint8_t packet[64] = {0};
  uint8_t payload[] = "hello";
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  int ret = icmp_create_packet(packet, sizeof(packet), &infos);
  ASSERT_EQm("should return 0 on success", 0, ret);
  PASS();
}

TEST test_return_1_if_buffer_too_small(void) {
  uint8_t packet[4] = {0}; /* trop petit pour header + payload */
  uint8_t payload[] = "hello";
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  int ret = icmp_create_packet(packet, sizeof(packet), &infos);
  ASSERT_EQm("should return 1 if buffer too small", 1, ret);
  PASS();
}

TEST test_header_type_and_code(void) {
  uint8_t packet[64] = {0};
  uint8_t payload[] = "test";
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 42,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  icmp_create_packet(packet, sizeof(packet), &infos);

  struct icmphdr *hdr = (struct icmphdr *)packet;
  ASSERT_EQ_FMT(ICMP_ECHO, hdr->type, "%d");
  ASSERT_EQ_FMT(42, hdr->code, "%d");
  PASS();
}

TEST test_payload_written_after_header(void) {
  uint8_t packet[64] = {0};
  uint8_t payload[] = "hello";
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  icmp_create_packet(packet, sizeof(packet), &infos);

  uint8_t *written_payload = packet + sizeof(struct icmphdr);
  ASSERT_MEM_EQ(payload, written_payload, sizeof(payload));
  PASS();
}

TEST test_checksum_is_valid(void) {
  uint8_t packet[64] = {0};
  uint8_t payload[] = "hello";
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  icmp_create_packet(packet, sizeof(packet), &infos);

  /*
   * Un checksum valide signifie que recalculer sur le paquet entier
   * (checksum inclus) donne 0.
   */
  size_t total = sizeof(struct icmphdr) + sizeof(payload);
  uint16_t result = verify_checksum(packet, total);
  ASSERT_EQ_FMT(0, result, "%04x");
  PASS();
}

TEST test_exact_buffer_size(void) {
  uint8_t payload[] = "hi";
  size_t total = sizeof(struct icmphdr) + sizeof(payload);
  uint8_t packet[total];
  memset(packet, 0, total);

  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = payload,
      .payload_size = sizeof(payload),
  };

  int ret = icmp_create_packet(packet, total, &infos);
  ASSERT_EQm("exact size buffer should succeed", 0, ret);
  PASS();
}

TEST test_empty_payload(void) {
  uint8_t packet[sizeof(struct icmphdr)] = {0};
  struct icmp_infos infos = {
      .type = ICMP_ECHO,
      .code = 0,
      .payload = NULL,
      .payload_size = 0,
  };

  int ret = icmp_create_packet(packet, sizeof(packet), &infos);
  ASSERT_EQ(0, ret);
  PASS();
}

SUITE(icmp_suite) {
  RUN_TEST(test_return_0_on_success);
  RUN_TEST(test_return_1_if_buffer_too_small);
  RUN_TEST(test_header_type_and_code);
  RUN_TEST(test_payload_written_after_header);
  RUN_TEST(test_checksum_is_valid);
  RUN_TEST(test_exact_buffer_size);
  RUN_TEST(test_empty_payload);
}

// ASSERT(COND)
// ASSERT_FALSE(COND)

// Toutes les assertions ont une variante avec un suffix 'm' pour ajouter
// un message
// ASSERT_EQm("test should be...", EXP, GOT)
// ASSERT_EQ(EXP, GOT)
// ASSERT_NEQ(EXP, GOT)
// ASSERT_GT(EXP, GOT)
// ASSERT_GTE(EXP, GOT)
// ASSERT_LT(EXP, GOT)
// ASSERT_LTE(EXP, GOT)
// ASSERT_EQ_FMT(EXP, GOT, FMT) -> ASSERT_EQ_FMT(42, result, "%d")

// ASSERT_IN_RANGE(EXP, GOT, TOL) -> Vérifie que GOT est dans EXP ± TOL

// ASSERT_STR_EQ(EXP, GOT)
// ASSERT_STRN_EQ(EXP, GOT, SIZE)
// ASSERT_MEM_EQ(EXP, GOT, SIZE) -> Compare SIZE octets, affiche un
// hexdump en cas d'échec

// ASSERT_EQUAL_T(EXP, GOT, TYPE_INFO, UDATA)
// ->  Comparaison via callback TYPE_INFO->equal,
//     avec affichage custom via TYPE_INFO->print

// ASSERT_ENUM_EQ(EXP, GOT, ENUM_STR_FUN)
// ->  ENUM_STR_FUN)Comme ASSERT_EQ mais convertit
//     les valeurs en string via ENUM_STR_FUN
//     pour le message d'erreur

// PASS()Marque le test comme réussi et retourne
// FAIL()Marque le test comme échoué et retourne
// SKIP()Marque le test comme ignoré et retourne
// https://github.com/silentbicycle/greatest

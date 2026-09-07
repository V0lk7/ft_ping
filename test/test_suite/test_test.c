#include "greatest.h"
#include "tmp.h"
// #include "..."

// static void setup(void *data)
// static void teardown (void *data)

TEST    test_add(void) {
    ASSERT_EQ(5, add(3, 2));
    PASS();
}


SUITE (test_tmp)
{
    // SET_SETUP(setup, NULL);
    // SET_TEARDOWN(teardown, NULL);
    RUN_TEST(test_add);
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
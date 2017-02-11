#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "signals_util.h"

START_TEST(verify_catch_sigsegv)
{
  reset_signal_counters();

  HIBES_CATCH(SIGSEGV);

  TRY {
    int* bla = 0;

    bla[0] = 0;
  }

  HIBES_NO_CATCH(SIGSEGV);

  ck_assert(getCounterSigSegV() == 1);

  reset_signal_counters();
}
END_TEST

Suite* signal_suite() {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Signal");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, verify_catch_sigsegv);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = signal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

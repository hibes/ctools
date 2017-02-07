#include <check.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// My strings library
#include "strings_util.h"

char** buffer = 0;
long i;
int j;

char* validator[] = {
  "There",
  "once",
  "was",
  "a",
  "potato."
};

char* strings_to_split[] = {
  "There once was a potato.",
  " There once was a potato.",
  " There once was a potato. ",
  "There once was a potato."
};

int strings_to_split_length = 4;

START_TEST(splitstring_single_character_delim) {
  for (j = 0; j < strings_to_split_length; ++j) {
    buffer = malloc(sizeof(char**)*  1);

    if (split_string(strings_to_split[j], " ", &buffer) == 0) {
      ck_assert(buffer != 0);
      for (i = 0; buffer[i] != 0; ++i) {
        ckassert(strcmp(validator[i], buffer[i]) == 0);

        free(buffer[i]);
        buffer[i] = 0;
      }

      free(buffer);
      buffer = 0;
    }
  }
}
END_TEST

START_TEST(splitstring_multi_character_delim) {
  buffer = malloc(sizeof(char**)*  1);

  if (split_string("There| |once| |was| |a| |potato.", "| |", &buffer) == 0) {
    ck_assert(buffer != 0);
    for (i = 0; buffer[i] != 0; ++i) {
      ckassert(strcmp(validator[i], buffer[i]) == 0);

      free(buffer[i]);
      buffer[i] = 0;
    }

    free(buffer);
    buffer = 0;
  }
}
END_TEST

Suite* splitstring_suite() {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Splitstring");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, splitstring_single_character_delim);
  tcase_add_test(tc_core, splitstring_multi_character_delim);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = splitstring_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

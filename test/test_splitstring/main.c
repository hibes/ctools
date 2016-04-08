// C Standard Libraries
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// My strings library
#include <strings.h>

// Returns 
// * 0:         Success
// * Non-zero:  Failure
int main(int argc, char** argv) {
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

  // Single character delimiter
  for (j = 0; j < strings_to_split_length; ++j) {
    buffer = malloc(sizeof(char**) * 1);

    if (split_string(strings_to_split[j], " ", &buffer) == 0) {
      if (buffer == 0) {
        return 1;
      }

      for (i = 0; buffer[i] != 0; ++i) {
        if (strcmp(validator[i], buffer[i]) != 0) {
          fprintf(stderr, "FAIL validator[0]=%s buffer[0]={%s}\n", validator[i], buffer[i]);
        } else {
          fprintf(stderr, "PASS buffer[0]=%s\n", buffer[i]);
        }

        free(buffer[i]);
        buffer[i] = 0;
      }

      free(buffer);
      buffer = 0;
    }
  }

  // Multiple character delimiter
  buffer = malloc(sizeof(char**) * 1);

  if (split_string("There| |once| |was| |a| |potato.", "| |", &buffer) == 0) {
    if (buffer == 0) {
      return 1;
    }

    for (i = 0; buffer[i] != 0; ++i) {
      if (strcmp(validator[i], buffer[i]) != 0) {
        fprintf(stderr, "FAIL validator[0]=%s buffer[0]={%s}\n", validator[i], buffer[i]);
      } else {
        fprintf(stderr, "PASS buffer[0]=%s\n", buffer[i]);
      }

      free(buffer[i]);
      buffer[i] = 0;
    }

    free(buffer);
    buffer = 0;
  }

  return 0;
}

// C Standard Libraries
#include <string.h> // stdlib C strings
#include <stdlib.h>
#include <stdio.h>

// My strings library
#include "strings.h"

char replace(char** string, char needle, char replacement) {
  int i;

  if (string == 0 || *string == 0) {
    return 1;
  }

  for (i = 0; i < strlen(*string); ++i) {
    //fprintf(stderr, "Dereferencing the string %s %d?\n", *string + i, strlen(*string));
    if (*string[i] == needle) {
      *string[i] = replacement;
    }
  }

  return 0;
}

char split_string(char* string, char* delimiter, char*** buffer_arg) {
  long string_length = strlen(string);
  long delimiter_length = strlen(delimiter);
  long i;
  long last_index = 0;
  int buffer_count = 0;

  char** buffer;

  // Allocate enough chars to have the last one be null to indicate the end of
  //  the list
  buffer = malloc((sizeof(char*) * buffer_count) + 1);
  buffer[buffer_count] = 0;

  long this_string_length = 0;

  for (i = 0; i < string_length - delimiter_length + 1; ++i) {
    if (strncmp(&string[i], delimiter, delimiter_length) == 0) {
      this_string_length = i - last_index;

      if (this_string_length > 0) {
        ++buffer_count;
        buffer = realloc(buffer, (sizeof(char**) * buffer_count) + 1);
        buffer[buffer_count] = 0;

        buffer[buffer_count - 1] = malloc((sizeof(char) * this_string_length) + 1);

        strncpy(buffer[buffer_count - 1], &string[last_index], this_string_length);
        buffer[buffer_count - 1][this_string_length] = '\0';
      }

      last_index = i + delimiter_length;
      i = last_index;
    } else if (i + 1 == string_length - delimiter_length + 1) {
      this_string_length = string_length - last_index;

      if (this_string_length > 0) {
        ++buffer_count;
        buffer = realloc(buffer, (sizeof(char**) * buffer_count) + 1);
        buffer[buffer_count] = 0;

        buffer[buffer_count - 1] = malloc((sizeof(char) * this_string_length) + 1);

        strncpy(buffer[buffer_count - 1], &string[last_index], this_string_length);
        buffer[buffer_count - 1][this_string_length] = '\0';
      }
    }
  }

  buffer_arg[0] = buffer;

  return 0;
}

char read_all_stdin(char** string, long max_length) {
  int chunk_len = 256;
  char chunk_buffer[chunk_len + 1];

  int bytes_read              = 0;

  long total_bytes_read       = 0;
  long total_bytes_written    = 0;
  long total_bytes_allocated  = 0;

  if (max_length == 0) {
    max_length = 50 * 1024 * 1024;
  }

  *string = malloc(chunk_len);

  total_bytes_allocated = chunk_len;

  bytes_read = fread(chunk_buffer, sizeof(char), chunk_len, stdin);

  while (bytes_read > 0) {
    total_bytes_read += bytes_read;

    if (total_bytes_read > max_length) {
      fprintf(stderr, "TOO BIG!\n");
      return 1;
    }

    if (total_bytes_read > total_bytes_allocated) {
      // Double the allocation up to max_length
      total_bytes_allocated *= 2;

      if (total_bytes_allocated > max_length) {
        total_bytes_allocated = max_length;
      }

      // Reallocate the new section
      *string = realloc(*string, total_bytes_allocated + 1);
    }

    if (*string == 0) {
      fprintf(stderr, "Failed to realloc\n");

      return 1;
    }

    memcpy(*string + total_bytes_written, chunk_buffer, bytes_read);
    total_bytes_written += bytes_read;

    *(*string + total_bytes_written) = '\0';

    bytes_read = fread(chunk_buffer, sizeof(char), chunk_len, stdin);
  }

  return 0;
}


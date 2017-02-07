// C Standard Libraries
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

// My strings library
#include <strings.h>

void usage();

// Takes in a string of characters from stdin, using a (currently hard-coded)
//  delimiter, and outputs a stream of characters separated by the
//  RS (record separator) 0x1E character code
//  characters delimited
// Returns
// * 0:         Success
// * Non-zero:  Failure
int main(int argc, char** argv) {
  char* ibuffer = 0;
  char** obuffer = 0;
  long i;

  int option_index = 0;
  int c;

  char* delimiter = 0;

  static struct option long_options[] = {
    {"help",  no_argument,        0,  'h'},
    {"delim", required_argument,  0,  'd'},
    {0,       0,                  0,  0}
  };

  while ((c = getopt_long(argc, argv, "hd:", long_options, &option_index)) != -1) {
    switch (c) {
      case 'h':
        usage(argv);

        return 1;
      case 'd':
        delimiter = optarg;

        break;
      default:
        usage(argv);

        break;
    }
  }

  if (argc == 1 || delimiter == 0) {
    usage();

    return 1;
  }

  ibuffer = malloc(sizeof(char*) * 1);

  if (read_all_stdin(&ibuffer, 0) || ibuffer == 0) {
    fprintf(stderr, "Failed read_all_stdin\n");

    return 1;
  }

  obuffer = malloc(sizeof(char**) * 1);

  if (split_string(ibuffer, delimiter, &obuffer) == 0) {
        //"<!-- BEGIN TEMPLATE: uwp_post_tracker_bit -->", &obuffer) == 0) {
    if (obuffer == 0) {
      return 1;
    }

    for (i = 0; obuffer[i] != 0; ++i) {
      fprintf(stdout, "%s%c", obuffer[i], 0x1E);

      free(obuffer[i]);
      obuffer[i] = 0;
    }

    free(obuffer);
    obuffer = 0;
  }

  return 0;
}

void usage(char** argv) {
  fprintf(stderr,
      "%s: Usage\n\n", argv[0]);
  fprintf(stderr,
      "\t--delim=$ARG\t-d $ARG \tSpecify delimiter argument (required)\n");
  fprintf(stderr,
      "\t--help      \t-h      \tPrint this help\n");
}

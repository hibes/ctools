#ifndef TOOLS_STRINGS_H
#define TOOLS_STRINGS_H

// Takes in a string to split, a delimiter to split by, and stores the result in
//  the buffer parameter
//
// Example usage:
//
// char** buffer = malloc(sizeof(char**) * 1);
// split_string("There once was a potato.", " ", &buffer);
//
// The contents of buffer will be an array of char*s pointing to:
// *  There
// *  once
// *  was
// *  a
// *  potato.
//
// It's a char*** to simulate pass by reference in C.
//
// Returns
//  0:        Success
//  Non-zero: Failure
char split_string(char* string, char* delimiter, char*** buffer_arg);

// Reads all available data from stdin and stores it in string up to max_length
//
// This will malloc for you.
//
// Example usage:
//
// char* string;
//
// read_all_stdin(&string, 65535);
//
// Returns
//  0:        Success
//  Non-zero: Failure
char read_all_stdin(char** string, long max_length);

#endif

# Henry Brown
# 2015

CC=gcc
RM=rm
MKDIR=mkdir

#CFLAGS=-Wall -g
CFLAGS=-Wall

BUILD_DIR=build
EXE=
EXE_OUT=$(BUILD_DIR)
OBJ_OUT=$(BUILD_DIR)/obj
TESTS_OUT=$(BUILD_DIR)/tests
HOME_BIN=~/bin
LIB=.so
SRC=src
STATIC=-static

TEST_SCRIPT=test/run_tests.sh
TEST_SCRIPT=echo Make a test script!

# .c files
STRINGS_C=$(SRC)/strings/strings.c
SPLIT_STRING_C=$(SRC)/strings/split_string/main.c
DEVTRACKER_C=$(SRC)/ac/devtracker/main.c

# .o files
STRINGS_O=$(OBJ_OUT)/strings.o

# $(EXE) files
SPLIT_STRING_BIN=$(EXE_OUT)/split_string$(EXE)
DEVTRACKER_BIN=$(EXE_OUT)/devtracker$(EXE)

DEP=mkdirs

INCLUDE_DIRS=-I src/strings/

all: $(DEP) bins run_tests clean_objs

mkdirs:
	$(MKDIR) -p $(BUILD_DIR)
	$(MKDIR) -p $(EXE_OUT)
	$(MKDIR) -p $(OBJ_OUT)
	$(MKDIR) -p $(TESTS_OUT)
	$(MKDIR) -p $(HOME_BIN)

bins: $(DEP) lib_strings split_string devtracker

split_string: $(DEP) strings.o
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(STRINGS_O) $(SPLIT_STRING_C) \
		-o $(SPLIT_STRING_BIN)

lib_strings: $(DEP) strings.o
	ar rcs build/lib_strings$(LIB) $(STRINGS_O)

devtracker: $(DEP)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(STRINGS_O) $(DEVTRACKER_C) \
		-L/usr/lib -ltidy -lcurl.dll -o $(DEVTRACKER_BIN)

strings.o: $(DEP)
	$(CC) $(CFLAGS) $(STATIC) -c $(STRINGS_C) -o $(STRINGS_O)

tests: $(DEP) bins

run_tests: $(DEP) tests
	EXE=$(EXE) TESTS_OUT=$(TESTS_OUT) $(TEST_SCRIPT)

install: $(DEP) bins
	find $(shell pwd)/$(EXE_OUT) -maxdepth 1 -type f -executable \
		-exec cp {} $(HOME_BIN)/ \;

clean_objs:
	@ $(RM) -rf $(OBJ_OUT)

clean:
	$(RM) -rf $(BUILD_DIR)/*

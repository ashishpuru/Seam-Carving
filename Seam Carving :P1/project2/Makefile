BIN_NAME    := carve
TESTER_NAME := testrunner

BIN_FILES    := src/argparser.c src/energy.c src/image.c src/main.c src/indexing.c
TESTER_FILES := src/argparser.c src/energy.c src/image.c src/indexing.c src/unit_tests.c src/test_main.c
HEADERS      := $(wildcard src/*.h)

TEST_SCRIPT := test/run_tests.py
Q ?= @

DEBUG   := -O0 -g -fsanitize=address -fsanitize=undefined
OPT     := -O3

CFLAGS  += -I src -Wall -Wextra -pedantic
LDFLAGS +=

.PHONY: all check clean

all: bin/$(BIN_NAME)_opt bin/$(BIN_NAME)_debug bin/$(TESTER_NAME)

bin/$(BIN_NAME)_opt: $(patsubst src/%.c, build/%.opt.o, $(BIN_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(OPT) $+ $(LDFLAGS)

bin/$(BIN_NAME)_debug: $(patsubst src/%.c, build/%.debug.o, $(BIN_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

bin/$(TESTER_NAME): $(patsubst src/%.c, build/%.debug.o, $(TESTER_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

build/%.opt.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(OPT) $<

build/%.debug.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(DEBUG) $<

check: all
	@echo "===> CHECK"
	$(Q)$(TEST_SCRIPT)

clean:
	@echo "===> CLEAN"
	$(Q)rm -rf bin build
extract_imgs.tgz:
	@make -C test/data > /dev/null

all: extract_imgs.tgz

extract_refs.tgz:
	@make -C test/ref_output > /dev/null

all: extract_refs.tgz


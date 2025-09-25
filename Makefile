NAME=alloc
LIB_NAME=lib$(NAME)

SRC=src
TARGET=target
TEST_DIR=tests
MACRO_SCRIPTS=macro_scripts


CC=gcc
AR=ar
CFLAGS=-Wall -g





all: build

alloc.o: $(SRC)/alloc.h $(SRC)/alloc.c
	$(CC) $(CFLAGS) -c $(SRC)/alloc.c

build: alloc.o
	mkdir -p $(TARGET)
	$(AR) -rcs $(LIB_NAME).a *.o

b: build



test: build $(TEST_DIR)/*.c
	mkdir -p $(TARGET)/tests
	for f in $(wildcard $(TEST_DIR)/*.c); do \
		filename=$$(basename $$f); \
		name=$${filename%.*}; \
		$(CC) $(CFLAGS) $$f -L. -l$(NAME) -lcore -o $(TARGET)/tests/$$name; \
		$(TARGET)/tests/$$name; \
	done


clean:
	rm *.o *.a
	rm -rf $(TARGET)/*











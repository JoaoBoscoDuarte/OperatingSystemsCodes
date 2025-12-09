CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lm
BIN_DIR = ../bin

all: $(BIN_DIR)/single_thread $(BIN_DIR)/multi_thread $(BIN_DIR)/single_process $(BIN_DIR)/multi_process

$(BIN_DIR)/single_thread: main_single_thread.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/multi_thread: main_multi_thread.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/single_process: main_single_process.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/multi_process: main_multi_process.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/* *.o
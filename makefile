CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iglobals -Iutilities

# Directories
SRV_DIR := server
CLI_DIR := client
UTIL_DIR := utilities
GLB_DIR := globals

# Source files
UTIL_SRC := $(wildcard $(UTIL_DIR)/*.cpp)
SERVER_SRC := $(SRV_DIR)/server.cpp $(UTIL_SRC)
CLIENT_SRC := $(CLI_DIR)/client.cpp $(UTIL_SRC)

# Output binaries
SERVER_BIN := server_exe
CLIENT_BIN := client_exe

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Cleanup
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all clean
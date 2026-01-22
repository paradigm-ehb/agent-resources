# Compilers
CXX := g++
CC  := gcc

# Paths
SRC_DIR   := .
GEN_DIR   := gen
BUILD_DIR := build

TARGET := $(BUILD_DIR)/agent

# Includes
INCLUDES := \
	-I$(SRC_DIR) \
	-I$(GEN_DIR)

INCLUDES += $(shell pkg-config --cflags grpc++)
LDFLAGS  += $(shell pkg-config --libs grpc++)

# Compiler flags
CXXFLAGS := \
	-std=c++17 \
	-g    \
	-Wall \
	-Wextra \
	-Wno-unused-function \
	-lgrpc++_reflection \
	$(INCLUDES)

CFLAGS := \
	-std=c99 \
	-g \
	-wall \
	-Wextra \
	-Wno-unused-function \
	$(INCLUDES)

# Linker flags
LDFLAGS := \
	-pthread \
	-lgrpc++ \
	-lgrpc \
	-lprotobuf \
	-labsl_base \
	-labsl_synchronization \
	-labsl_strings \
	-ldbus-1 \
	-lsystemd \
	-D_POSIX_C_SOURCE=200809L \
  -lssl \
	-lcrypto \

# Sources
# Unity build OR list files explicitly
SOURCES := \
	unity.cc


# Rules
all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

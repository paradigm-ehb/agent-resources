# Compiler
CXX := g++

# Paths
SRC_DIR   := .
GEN_DIR   := grpc/generated
BUILD_DIR := build

TARGET := $(BUILD_DIR)/agent

# Includes
INCLUDES := \
	-I$(SRC_DIR) \
	-I$(GEN_DIR)

INCLUDES += $(shell pkg-config --cflags grpc++)

# Compiler flags (C++ only)
CXXFLAGS := \
	-std=c++17 \
	-g \
	-O0 \
	-fno-omit-frame-pointer \
	-Wall \
	-Wextra \
	-Wno-unused-function \
	$(INCLUDES)

# Linker flags
LDFLAGS := \
	$(shell pkg-config --libs grpc++) \
	-pthread \
	-lgrpc++ \
	-lgrpc \
	-lprotobuf \
	-labsl_base \
	-labsl_synchronization \
	-labsl_strings \
	-ldbus-1 \
	-lsystemd \
	-lssl \
	-lcrypto \
	-lgrpc++_reflection

# Sources (unity build)
SOURCES := \
	unity.cc

# Rules
all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

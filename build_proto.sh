#!/bin/bash

set -euo pipefail

# Protobuf / gRPC code generation

# Directories
PROTO_DIR="grpc/proto"
OUT_DIR="grpc/generated"

# Locate protoc (FORCE system version, ignore ~/.local/bin)
PROTOC="/usr/bin/protoc"  # Explicitly use system protoc
if [ ! -x "$PROTOC" ]; then
  echo "error: system protoc not found at $PROTOC"
  echo "install with: sudo dnf install protobuf-compiler"
  exit 1
fi

echo "Using protoc: $PROTOC"
$PROTOC --version

# Locate grpc_cpp_plugin (system)
GRPC_CPP_PLUGIN="$(command -v grpc_cpp_plugin || true)"
if [ -z "$GRPC_CPP_PLUGIN" ]; then
  echo "error: failed to build protobufs, grpc_cpp_plugin not found"
  exit 1
fi
echo "Using grpc_cpp_plugin: $GRPC_CPP_PLUGIN"

# Validate proto directory
if [ ! -d "$PROTO_DIR" ]; then
  echo "error: proto directory '$PROTO_DIR' does not exist"
  exit 1
fi

# Clean output directory
if [ -d "$OUT_DIR" ]; then
  echo "Removing existing '$OUT_DIR/'"
  rm -rf "$OUT_DIR"
fi
mkdir -p "$OUT_DIR"

# Find .proto files
PROTO_FILES="$(find "$PROTO_DIR" -type f -name '*.proto')"
if [ -z "$PROTO_FILES" ]; then
  echo "No .proto files found in '$PROTO_DIR'"
  exit 0
fi

# Generate code
for proto in $PROTO_FILES; do
  echo "Compiling: $proto"
  "$PROTOC" \
    -I "$PROTO_DIR" \
    --cpp_out="$OUT_DIR" \
    --grpc_out="$OUT_DIR" \
    --plugin=protoc-gen-grpc="$GRPC_CPP_PLUGIN" \
    "$proto"
done

# Done
echo ""
echo "Protobuf / gRPC generation complete"
echo "Output directory: $OUT_DIR/"

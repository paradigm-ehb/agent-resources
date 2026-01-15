#!/bin/bash
set -eu

pushd "$(dirname "$0")" >/dev/null

# Compilers
CC=clang
CXX=clang++

# Paths
ROOT=/home/nasr/paradigm/agent/pkg/agent-resources

# gRPC flags
grpc_c_flags="$(pkg-config --cflags grpc++)"
grpc_lib_flags="$(pkg-config --libs grpc++)"

# Common flags (CRITICAL)
CFLAGS="-I$ROOT -Wall"
CXXFLAGS="-I$ROOT -Wall $grpc_c_flags"

LDFLAGS="-lm -pthread $grpc_lib_flags -lprotobuf -lssl -lcrypto -lz"

# Build dir
mkdir -p build
pushd build >/dev/null

echo "[build] compiling C sources"
$CC -std=c17 -c $CFLAGS ../base/base.c
$CC -std=c17 -c $CFLAGS ../base/base_arena.c
$CC -std=c17 -c $CFLAGS ../libvm/qc.c
$CC -std=c17 -c $CFLAGS ../libpkg/configure.c

echo "[build] compiling C++ sources"
$CXX -std=c++23 -c $CXXFLAGS ../agent/agent_core.cc
$CXX -std=c++23 -c $CXXFLAGS ../libgrpc/grpc.cc
$CXX -std=c++23 -c $CXXFLAGS ../libnet/ports.cc
$CXX -std=c++23 -c $CXXFLAGS ../libpkg/arch_pac.cc
$CXX -std=c++23 -c $CXXFLAGS ../libpkg/fedora_dnf.cc
$CXX -std=c++23 -c $CXXFLAGS ../libpkg/pacman.cc
$CXX -std=c++23 -c $CXXFLAGS ../libres/resources.cc

echo "[build] compiling protobuf / grpc sources"
find ../gen -name '*.pb.cc' -o -name '*.grpc.pb.cc' | while read -r f; do
  $CXX -std=c++23 -c $CXXFLAGS "$f"
done

echo "[link] agent"
$CXX *.o $LDFLAGS -o agent

echo "[run] ./agent"
./agent

popd >/dev/null
popd >/dev/null

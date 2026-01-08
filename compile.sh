#!/bin/sh
set -eu

CC=cc
SRC=resources_main.c
OUT_DIR=build
OUT_BIN=$OUT_DIR/agent_resources

CFLAGS="
-std=c99
-Wall
-Wextra
-Wpedantic
-Wshadow
-Wconversion
-Wundef
-Wpointer-arith
-Wcast-align
-Wcast-qual
-Wwrite-strings
-Wformat=2
-D_POSIX_C_SOURCE=200809L
"

mkdir -p "$OUT_DIR"

echo "Compiling binary..."
$CC $CFLAGS "$SRC" -o "$OUT_BIN"

echo "Done:"
echo "  $OUT_BIN"

./$OUT_BIN

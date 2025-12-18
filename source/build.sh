#!/bin/sh

set -xe

CC=clang
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

SOURCE="$SCRIPT_DIR/main.c"
OUT="$SCRIPT_DIR/main"

$CC --std=c99 -g "$SOURCE"  -o "$OUT"

./source/main

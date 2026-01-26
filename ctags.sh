#!/usr/bin/env bash
set -euo pipefail

ctags -R \
  --languages=C,C++ \
  --exclude=.git \
  --exclude=build \
  --exclude=gen \
  --exclude=vendor \
  --exclude=node_modules \
  --fields=+ailmnS \
  --extras=+q \
  .

echo "Tags generated successfully"

#!/usr/bin/env sh
fd -t f -e c -e cc -e cpp -e h . \
  -E grpc/generated \
  -E build \
  -E temp \
  -0 | xargs -0 -r clang-format -i --

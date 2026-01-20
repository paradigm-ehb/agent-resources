#!/bin/bash
CC=clang++
OUT=temp
FLAGS="-I."

if [ ! -d "./$OUT" ]; then
  echo "building out directory for tests"
  mkdir ./$OUT
fi

for file in ./tests/*.cc; do
  if [ ! -e "$file" ]; then
    echo -e "\e[31mno .cc files fount\e[0m"
    break
  fi

  BASE=$(basename "$file" .cc)

  $CC "$file" $FLAGS -o "./$OUT/$BASE"

  if [ $? -eq 0 ]; then
    "./$OUT/$BASE"
  fi
done

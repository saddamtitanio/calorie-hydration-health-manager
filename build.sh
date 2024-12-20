#!/bin/bash
CC=gcc
CFLAGS="-Wall -g"
OUTPUT="main"

SOURCES=(
  "main.c"
  "./utils/glass.c"
  "./utils/menu.c"
  "./utils/utils.c"
  "./db/db_directory.c"
  "./controller/usersController.c"
  "./controller/healthController.c"
)

echo "Compiling..."
$CC "${SOURCES[@]}" $CFLAGS -o $OUTPUT

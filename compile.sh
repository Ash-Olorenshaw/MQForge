#!/bin/bash

FLAGS=""
FILES="
utils/printer.c
utils/string.c
file/utils.c
file/orderer.c
file/importer.c
globals.c
cJSON.c
config.c
compiler.c
sys_interactions_linux.c"

if [[ $1 == "debug" ]]; then
	FLAGS="-g -fsanitize=address -fno-omit-frame-pointer "
fi

gcc $FLAGS main.c $FILES -o out

# ./out "$@"
./out '/mnt/D-Files/langs/MetaQuotes/DadProj/'

echo "Build complete: 'out'"

rm ./out


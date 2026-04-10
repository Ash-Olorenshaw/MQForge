#!/bin/bash

FLAGS=""
FILES="
utils/printer.c
globals.c
utils.c
basic_json_parse.c
file_utils.c
file_orderer.c
file_importer.c
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


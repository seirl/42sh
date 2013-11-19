#!/bin/bash

set -e

VERY_RANDOM=$RANDOM
DOT_FILE=ast_$VERY_RANDOM.dot
PNG_FILE=ast_$VERY_RANDOM.png

./test_ast_printer "$1" > $DOT_FILE
dot -Tpng $DOT_FILE > $PNG_FILE
xdg-open $PNG_FILE
rm $DOT_FILE $PNG_FILE

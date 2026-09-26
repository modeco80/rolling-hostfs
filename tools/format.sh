#!/bin/bash
# Reformat source tree

SOURCE_DIRS=(
 include/
 src/
)

find ${SOURCE_DIRS[@]} -type f | rg -e "(\.cpp|\.hpp|\.h)" | xargs -i -- clang-format -i "{}"

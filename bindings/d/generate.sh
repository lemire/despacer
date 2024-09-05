#!/bin/bash

set -ve

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# cd "$SCRIPT_DIR/src/despacer/libc" || exit 1

# Generate Bindings using dpp
dub run --build=release --quiet --yes dpp -- \
 "$SCRIPT_DIR"/src/despacer/libc/*.dpp \
 --inlcude-path "x86intrin.h" --ignore-system-paths --include-path "../../../../../include" \
 --preprocess-only --clang-option "-march=native"

# Beautify
echo "Beautifying the code..."
dub run --build=release --quiet --yes dfmt -- --soft_max_line_length 110 --indent_size 2 --inplace "$SCRIPT_DIR/src/despacer/libc"

cd ./src/libc

# Generate Bindings using dpp
dub run --build=release --quiet --yes dpp --  "./despacer_generic.dpp" "./despacer_SSSE3.dpp" "./despacer_SSE4_1.dpp" "./despacer_SSE4_2.dpp" --ignore-system-paths --inlcude-path "x86intrin.h" --preprocess-only --define __SSSE3__ --define __SSE4_1__ --define __SSE4_2__ # --define __AVX2__

{
  dub run --build=release --quiet --yes dpp -- "./despacer_AVX2.dpp" --ignore-system-paths --inlcude-path "x86intrin.h" --preprocess-only --define __AVX2__
} || {
  echo "AVX2 is not supported on this system"
}

cd ../..

# Beautify
echo "Beautifying the code..."
dub run --build=release --quiet --yes dfmt -- --soft_max_line_length 110 --indent_size 2 --inplace "./src/libc"

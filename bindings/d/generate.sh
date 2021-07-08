compiler="ldc2"

cd src

# Generate Bindings using dpp
dub run --build=release --compiler=$compiler --quiet --yes dpp -- "./libc.dpp" --preprocess-only --ignore-system-paths --no-sys-headers -I "../../../include"

# Beautify
dub run --build=release --compiler=$compiler --quiet --yes dfmt -- --soft_max_line_length 110 --indent_size 2 --inplace "./libc.d"

cd ..

all: build unit benchmark

build: ./src ./include
ifeq ($(OS), Windows_NT)
	# build fails with MSVC generator
	cmake -G Ninja -S ./ -B ./build && cmake --build ./build --config Release
else
	cmake -S ./ -B ./build && cmake --build ./build --config Release
endif

unit: ./tests
	cd build && ctest unit

benchmark: ./benchmarks
	./build/despacer_benchmark

clean: ./build
ifeq ($(OS), Windows_NT)
	cmd /c 'rmdir /s /q build'
else
	rm -rf ./build
endif

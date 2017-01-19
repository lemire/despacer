# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
ifeq ($(DEBUG),1)
CFLAGS = -fPIC  -std=c99 -ggdb -march=native -Wall -Wextra -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS = -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow
endif # debug
all: despacebenchmark
HEADERS=./include/despacer.h

despacebenchmark: ./benchmarks/despacebenchmark.c $(HEADERS)
	$(CC) $(CFLAGS) -o despacebenchmark ./benchmarks/despacebenchmark.c -Iinclude
clean:
	rm -f despacebenchmark 

# despacer
Fast C library to remove white space from strings (also called "strip white space")

We want to remove the space (' ') and the line feeds characters ('\n', '\r') from a string
as fast as possible. To avoid unnecessary allocations, we wish to do the processing in-place.

Let us consider any array of bytes representing a string in one of these encodings:
* UTF-8
* ASCII
* Any of the 8-bit ASCII supersets such as Latin1

How fast can we go?


Usage:
```
make
./despacebenchmark
```

Possible results...

```
$ ./despacebenchmark
memcpy(tmpbuffer,buffer,N):  0.078125 cycles / ops
countspaces(buffer, N):  3.689453 cycles / ops
despace(buffer, N):  3.544922 cycles / ops
faster_despace(buffer, N):  1.548828 cycles / ops
despace64(buffer, N):  1.595703 cycles / ops
despace_to(buffer, N, tmpbuffer):  3.390625 cycles / ops
avx2_countspaces(buffer, N):  0.091797 cycles / ops
avx2_despace(buffer, N):  2.533203 cycles / ops
sse4_despace(buffer, N):  0.617188 cycles / ops
sse4_despace_trail(buffer, N):  1.148438 cycles / ops
sse42_despace_branchless(buffer, N):  0.589844 cycles / ops
sse42_despace_branchless_lookup(buffer, N):  0.585938 cycles / ops
sse42_despace_to(buffer, N,tmpbuffer):  1.292969 cycles / ops
```

This indicates how many cycles are used to despace one byte.




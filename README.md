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
despace(buffer, N):  6.253906 cycles / ops
avx2_despace(buffer, N):  3.287109 cycles / ops
sse_despace(buffer, N):  2.951172 cycles / ops
sse4_despace(buffer, N):  0.791016 cycles / ops
```

This indicates how many cycles are used to despace one byte.




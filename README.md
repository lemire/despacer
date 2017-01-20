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
memcpy(tmpbuffer,buffer,N):  0.085938 cycles / ops
countspaces(buffer, N):  3.689453 cycles / ops
despace(buffer, N):  5.283203 cycles / ops
despace64(buffer, N):  2.718750 cycles / ops
despace_to(buffer, N, tmpbuffer):  5.519531 cycles / ops
avx2_countspaces(buffer, N):  0.345703 cycles / ops
avx2_despace(buffer, N):  3.984375 cycles / ops
sse4_despace(buffer, N):  0.861328 cycles / ops
sse4_despace_trail(buffer, N):  1.468750 cycles / ops
sse42_despace_branchless(buffer, N):  0.636719 cycles / ops
sse42_despace_branchless_lookup(buffer, N):  0.621094 cycles / ops
sse42_despace_to(buffer, N,tmpbuffer):  1.693359 cycles / ops
```

This indicates how many cycles are used to despace one byte.




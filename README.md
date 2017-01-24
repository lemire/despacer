# despacer
Fast C library to remove white space from strings (also called "strip white space")

We want to remove the space (' ') and the line feeds characters ('\n', '\r') from a string
as fast as possible. To avoid unnecessary allocations, we wish to do the processing in-place.

Let us consider any array of bytes representing a string in one of these encodings:
* UTF-8
* ASCII
* Any of the 8-bit ASCII supersets such as Latin1

How fast can we go?

Blog post: 
http://lemire.me/blog/2017/01/20/how-quickly-can-you-remove-spaces-from-a-string/


Usage:
```
make
./despacebenchmark
```

Note that clang seems to give better results than gcc.

Possible results...

```
$ ./despacebenchmark
pointer alignment = 16 bytes
memcpy(tmpbuffer,buffer,N):  0.109375 cycles / ops
countspaces(buffer, N):  3.675781 cycles / ops
despace(buffer, N):  5.574219 cycles / ops
faster_despace(buffer, N):  1.720703 cycles / ops
despace64(buffer, N):  2.523438 cycles / ops
despace_to(buffer, N, tmpbuffer):  5.587891 cycles / ops
avx2_countspaces(buffer, N):  0.365234 cycles / ops
avx2_despace(buffer, N):  3.640625 cycles / ops
sse4_despace(buffer, N):  0.808594 cycles / ops
sse4_despace_branchless(buffer, N):  0.386719 cycles / ops
sse4_despace_branchless_u2(buffer, N):  0.380859 cycles / ops
sse4_despace_branchless_u4(buffer, N):  0.357422 cycles / ops
sse4_despace_branchless_mask8(buffer, N):  0.464844 cycles / ops
sse4_despace_trail(buffer, N):  1.537109 cycles / ops
sse42_despace_branchless(buffer, N):  0.666016 cycles / ops
sse42_despace_branchless_lookup(buffer, N):  0.679688 cycles / ops
sse42_despace_to(buffer, N,tmpbuffer):  1.685547 cycles / ops
```

This indicates how many cycles are used to despace one byte.




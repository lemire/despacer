# despacer
[![Build Status](https://travis-ci.org/lemire/despacer.png)](https://travis-ci.org/lemire/despacer)

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
memcpy(tmpbuffer,buffer,N):  0.111328 cycles / ops
countspaces(buffer, N):  3.687500 cycles / ops
despace(buffer, N):  5.337891 cycles / ops
faster_despace(buffer, N):  1.689453 cycles / ops
despace64(buffer, N):  2.429688 cycles / ops
despace_to(buffer, N, tmpbuffer):  5.585938 cycles / ops
avx2_countspaces(buffer, N):  0.367188 cycles / ops
avx2_despace(buffer, N):  3.990234 cycles / ops
avx2_despace_branchless(buffer, N):  0.593750 cycles / ops
avx2_despace_branchless_u2(buffer, N):  0.535156 cycles / ops
sse4_despace(buffer, N):  0.734375 cycles / ops
sse4_despace_branchless(buffer, N):  0.384766 cycles / ops
sse4_despace_branchless_u2(buffer, N):  0.380859 cycles / ops
sse4_despace_branchless_u4(buffer, N):  0.351562 cycles / ops
sse4_despace_trail(buffer, N):  1.142578 cycles / ops
sse42_despace_branchless(buffer, N):  0.763672 cycles / ops
sse42_despace_branchless_lookup(buffer, N):  0.673828 cycles / ops
sse42_despace_to(buffer, N,tmpbuffer):  1.703125 cycles / ops
```

This indicates how many cycles are used to despace one byte.



# Related work

- a related problem on ARM processors with iOS app https://github.com/DerekScottLedbetter/space-pruner

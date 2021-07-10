#ifdef __SSE4_1__

#include <x86intrin.h>

/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
size_t sse4_despace(char *bytes, size_t howmany);

size_t sse4_despace_branchless(char *bytes, size_t howmany);

size_t sse4_despace_branchless32(char *bytes, size_t howmany);

__m128i cleanm128(__m128i x, __m128i spaces, __m128i newline, __m128i carriage, int *mask16);

size_t sse4_despace_branchless_u4(char *bytes, size_t howmany);

size_t sse4_despace_branchless_u2(char *bytes, size_t howmany);

/// unlike cleanm128, we just eliminate everything that is < 0x20
// credit: Anime Tosho
__m128i skinnycleanm128(__m128i x, int *mask16);

size_t sse4_despace_skinny_u4(char *bytes, size_t howmany);

size_t sse4_despace_skinny_u2(char *bytes, size_t howmany);

/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
size_t sse4_despace_trail(char *bytes, size_t howmany);

#endif

#ifdef __AVX2__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <x86intrin.h>

#ifndef __clang__

__m256i _mm256_loadu2_m128i(__m128i const *__addr_hi, __m128i const *__addr_lo);

void _mm256_storeu2_m128i(__m128i *__addr_hi, __m128i *__addr_lo,  __m256i __a);

#endif

// credit: https://github.com/aqrit
size_t despace_avx2_vpermd( void* dst_void, void* src_void, size_t length );


/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
size_t avx2_despace(char *bytes, size_t howmany);

size_t avx2_countspaces(const char *bytes, size_t howmany);

/** Check if the given bytes with length howmany have space among them */
bool avx2_hasspace(const char *bytes, size_t howmany);

__m256i cleanm256(__m256i x, __m256i spaces, __m256i newline, __m256i carriage, unsigned int *mask1, unsigned int *mask2);

size_t avx2_despace_branchless(char *bytes, size_t howmany);

size_t avx2_despace_branchless_u2(char *bytes, size_t howmany);


#endif

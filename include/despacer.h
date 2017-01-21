#ifndef DESPACER_H
#define DESPACER_H

#include <stdint.h>
#include <stddef.h>

#include "despacer_tables.h"

int jump_table[256] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
static inline size_t despace(char *bytes, size_t howmany) {
  size_t pos = 0;
  for (size_t i = 0; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}
static inline size_t faster_despace( char* bytes, size_t howmany )
{
  size_t i = 0, pos = 0;
  while( i < howmany )
  {
    bytes[pos] = bytes[i++];
    pos += jump_table[ (unsigned char)bytes[pos] ];
  }
  return pos;
}
static inline size_t countspaces(const char *bytes, size_t howmany) {
  size_t count = 0;
  for (size_t i = 0; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      count ++;
    }
  }
  return count;
}
// standard bit twiddling
#define haszero(v)                                                             \
  (((v)-UINT64_C(0x0101010101010101)) & ~(v)&UINT64_C(0x8080808080808080))

static inline size_t despace64(char *bytes, size_t howmany) {
  size_t pos = 0;
  size_t i = 0;
  uint64_t word = 0;
  uint64_t mask1 = ~UINT64_C(0) / 255 * (uint64_t)('\r');
  uint64_t mask2 = ~UINT64_C(0) / 255 * (uint64_t)('\n');
  uint64_t mask3 = ~UINT64_C(0) / 255 * (uint64_t)(' ');

  for (; i + 7 < howmany; i += 8) {
    memcpy(&word, bytes + i, sizeof(word));
    uint64_t xor1 = word ^ mask1;
    uint64_t xor2 = word ^ mask2;
    uint64_t xor3 = word ^ mask3;

    if (haszero(xor1) ^ haszero(xor2) ^ haszero(xor3)) {
      for (int k = 0; k < 8; k++) {
        char c = bytes[i + k];
        if (c == '\r' || c == '\n' || c == ' ') {
          continue;
        }
        bytes[pos++] = c;
      }

    } else {
      memmove(bytes + pos, bytes + i, sizeof(word));
      pos += 8;
    }
  }

  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

/**
* remove spaces from string bytes (UTF-8 or ASCII) containing "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
* Result is stored in "out", where the caller is responsible for memory
* allocation.
*/
static inline size_t despace_to(const char *__restrict__ bytes, size_t howmany,
                                char *__restrict__ out) {
  size_t pos = 0;
  for (size_t i = 0; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    out[pos++] = c;
  }
  return pos;
}

#ifdef __AVX2__

#include <x86intrin.h>

/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
static inline size_t avx2_despace(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m256i spaces = _mm256_set1_epi8(' ');
  __m256i newline = _mm256_set1_epi8('\n');
  __m256i carriage = _mm256_set1_epi8('\r');
  size_t i = 0;
  for (; i + 31 < howmany; i += 32) {
    __m256i x = _mm256_loadu_si256((const __m256i *)(bytes + i));
    // we do it the naive way, could be smarter?
    __m256i xspaces = _mm256_cmpeq_epi8(x, spaces);
    __m256i xnewline = _mm256_cmpeq_epi8(x, newline);
    __m256i xcarriage = _mm256_cmpeq_epi8(x, carriage);
    __m256i anywhite =
        _mm256_or_si256(_mm256_or_si256(xspaces, xnewline), xcarriage);
    if (_mm256_testz_si256(anywhite, anywhite) == 1) { // no white space
      _mm256_storeu_si256((__m256i *)(bytes + pos), x);
      pos += 32;
    } else {
      // do it the hard way
      for (size_t j = 0; j < 32; j++) {
        char c = bytes[i + j];
        if (c == '\r' || c == '\n' || c == ' ') {
          continue;
        }
        bytes[pos++] = c;
      }
    }
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

static inline size_t avx2_countspaces(const char *bytes, size_t howmany) {
  size_t count = 0;
  __m256i spaces = _mm256_set1_epi8(' ');
  __m256i newline = _mm256_set1_epi8('\n');
  __m256i carriage = _mm256_set1_epi8('\r');
  size_t i = 0;
  for (; i + 31 < howmany; i += 32) {
    __m256i x = _mm256_loadu_si256((const __m256i *)(bytes + i));
    // we do it the naive way, could be smarter?
    __m256i xspaces = _mm256_cmpeq_epi8(x, spaces);
    __m256i xnewline = _mm256_cmpeq_epi8(x, newline);
    __m256i xcarriage = _mm256_cmpeq_epi8(x, carriage);
    __m256i anywhite =
        _mm256_or_si256(_mm256_or_si256(xspaces, xnewline), xcarriage);
    count += _mm_popcnt_u32(_mm256_movemask_epi8(anywhite));
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      count ++ ;
    }
  }
  return count;
}


#endif

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
static inline size_t sse4_despace(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    // we do it the naive way, could be smarter?
    __m128i xspaces = _mm_cmpeq_epi8(x, spaces);
    __m128i xnewline = _mm_cmpeq_epi8(x, newline);
    __m128i xcarriage = _mm_cmpeq_epi8(x, carriage);
    __m128i anywhite = _mm_or_si128(_mm_or_si128(xspaces, xnewline), xcarriage);
    int mask16 = _mm_movemask_epi8(anywhite);
    if (mask16 == 0) { // no match!
      _mm_storeu_si128((__m128i *)(bytes + pos), x);
      pos += 16;
    } else {
      x = _mm_shuffle_epi8(
          x, _mm_loadu_si128((const __m128i *)despace_mask16 + mask16));
      _mm_storeu_si128((__m128i *)(bytes + pos), x);
      pos += 16 - _mm_popcnt_u32(mask16);
    }
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

static inline size_t sse4_despace_branchless(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    __m128i xspaces = _mm_cmpeq_epi8(x, spaces);
    __m128i xnewline = _mm_cmpeq_epi8(x, newline);
    __m128i xcarriage = _mm_cmpeq_epi8(x, carriage);
    __m128i anywhite = _mm_or_si128(_mm_or_si128(xspaces, xnewline), xcarriage);
    uint64_t mask16 = _mm_movemask_epi8(anywhite);
    x = _mm_shuffle_epi8(x, *((__m128i *)despace_mask16 + mask16));
    _mm_storeu_si128((__m128i *)(bytes + pos), x);
    pos += 16 - _mm_popcnt_u64(mask16);
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}


/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
static inline size_t sse4_despace_trail(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    // we do it the naive way, could be smarter?
    __m128i xspaces = _mm_cmpeq_epi8(x, spaces);
    __m128i xnewline = _mm_cmpeq_epi8(x, newline);
    __m128i xcarriage = _mm_cmpeq_epi8(x, carriage);
    __m128i anywhite = _mm_or_si128(_mm_or_si128(xspaces, xnewline), xcarriage);
    int mask16 = _mm_movemask_epi8(anywhite);
    if (mask16 == 0) { // no match!
      _mm_storeu_si128((__m128i *)(bytes + pos), x);
      pos += 16;
    } else {
      for (int k = 0; k < 16; ++k) {
        if (mask16 & (1 << k))
          continue;
        bytes[pos++] = bytes[i + k];
      }
    }
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

#endif

#ifdef __SSE4_2__

#include <x86intrin.h>

static inline size_t sse42_despace_to(const char *__restrict__ bytes,
                                      size_t howmany, char *__restrict__ out) {
  size_t pos = 0;
  __m128i targetchars =
      _mm_set_epi8(' ', '\n', '\r', ' ', ' ', '\n', '\r', ' ', ' ', '\n', '\r',
                   ' ', ' ', '\n', '\r', ' ');
  size_t i = 0;
  for (; i + 15 < howmany;) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int result =
        _mm_cmpestri(targetchars, 3, x, 16,
                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK);
    i += result + (result < 16);
    _mm_storeu_si128((__m128i *)(out + pos), x);
    pos += result;
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    out[pos++] = c;
  }
  return pos;
}

static inline size_t sse42_despace(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i targetchars =
      _mm_set_epi8(' ', '\n', '\r', ' ', ' ', '\n', '\r', ' ', ' ', '\n', '\r',
                   ' ', ' ', '\n', '\r', ' ');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16 = _mm_cvtsi128_si32(
        _mm_cmpestrm(targetchars, 3, x, 16,
                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK));
    if (mask16 == 0) { // no match!
      _mm_storeu_si128((__m128i *)(bytes + pos), x);
      pos += 16;
    } else {
      x = _mm_shuffle_epi8(
          x, _mm_loadu_si128((const __m128i *)despace_mask16 + mask16));
      _mm_storeu_si128((__m128i *)(bytes + pos), x);
      pos += 16 - _mm_popcnt_u32(mask16);
    }
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

static inline size_t sse42_despace_branchless(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i targetchars =
      _mm_set_epi8(' ', '\n', '\r', ' ', ' ', '\n', '\r', ' ', ' ', '\n', '\r',
                   ' ', ' ', '\n', '\r', ' ');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16 = _mm_cvtsi128_si32(
        _mm_cmpestrm(targetchars, 3, x, 16,
                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK));
    x = _mm_shuffle_epi8(
          x, _mm_loadu_si128((const __m128i *)despace_mask16 + mask16));
    _mm_storeu_si128((__m128i *)(bytes + pos), x);
    pos += 16 - _mm_popcnt_u32(mask16);
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}

static inline size_t sse42_despace_branchless_lookup(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i targetchars =
      _mm_set_epi8(' ', '\n', '\r', ' ', ' ', '\n', '\r', ' ', ' ', '\n', '\r',
                   ' ', ' ', '\n', '\r', ' ');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16 = _mm_cvtsi128_si32(
        _mm_cmpestrm(targetchars, 3, x, 16,
                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK));
    x = _mm_shuffle_epi8(
          x, _mm_loadu_si128((const __m128i *)despace_mask16 + mask16));
    _mm_storeu_si128((__m128i *)(bytes + pos), x);
    pos += 16 - despace_popcnt16[mask16];
  }
  for (; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}
#endif
#endif

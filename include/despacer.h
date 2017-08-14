#ifndef DESPACER_H
#define DESPACER_H

#include <stdint.h>
#include <stddef.h>

#include "despacer_tables.h"

int jump_table[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

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


// make the store unconditional
// credit: https://github.com/aqrit
size_t despace_branchless( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	for( ; length != 0; length-- ){
		uint8_t c = *src++;
		*dst = c;
		dst += !!((c != 0x20) && (c != 0x0A) && (c != 0x0D) && (c != 0x09));
	}
	return (size_t)(dst - ((uint8_t*)dst_void));
}


// explicit branchless
// credit: https://github.com/aqrit
size_t despace_cmov( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	for( ; length != 0; length-- ){
		uint8_t c = *src++;
		uint64_t m = ( c > 0x20 ) ? 0xFFFFFFFFFFFFFFFF : 0xFFFFFFFEFFFFD9FF;
		*dst = c;
		dst += ((m >> (c & 63)) & 1);
	}
	return (size_t)(dst - ((uint8_t*)dst_void));
}

// credit: https://github.com/aqrit
size_t despace_table( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;
	static const uint8_t table[256] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1,
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
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	for( ; length != 0; length-- ){
		size_t c = *src++;
		*dst = (uint8_t)c;
		dst += table[c];
	}
	return (size_t)(dst - ((uint8_t*)dst_void));
}

static inline size_t despace32(char *bytes, size_t howmany) {
  size_t pos = 0;
  for (size_t i = 0; i < howmany; i++) {
    char c = bytes[i];
    if ((unsigned char)c <= 32) {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}
static inline size_t faster_despace(char *bytes, size_t howmany) {
  size_t i = 0, pos = 0;
  while (i < howmany) {
    const char c = bytes[i++];
    bytes[pos] = c;
    pos += jump_table[(unsigned char)c];
  }
  return pos;
}
static inline size_t faster_despace32(char *bytes, size_t howmany) {
  size_t i = 0, pos = 0;
  while (i < howmany) {
    const char c = bytes[i++];
    bytes[pos] = c;
    pos += ((unsigned char)c > 32 ? 1 : 0);
  }
  return pos;
}

static inline size_t countspaces(const char *bytes, size_t howmany) {
  size_t count = 0;
  for (size_t i = 0; i < howmany; i++) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      count++;
    }
  }
  return count;
}

static inline size_t countspaces32(const char *bytes, size_t howmany) {
  size_t count = 0;
  for (size_t i = 0; i < howmany; i++) {
    const char c = bytes[i];
    count += (c <= 32 ? 1 : 0);
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

    if (haszero(xor1) || haszero(xor2) || haszero(xor3)) {
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

static inline size_t sse4_despace_branchless32(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  size_t i = 0;
  for (; i + 15 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    __m128i anywhite = _mm_cmpeq_epi8(spaces, _mm_max_epu8(spaces, x));
    uint64_t mask16 = _mm_movemask_epi8(anywhite);
    x = _mm_shuffle_epi8(x, *((__m128i *)despace_mask16 + mask16));
    _mm_storeu_si128((__m128i *)(bytes + pos), x);
    pos += 16 - _mm_popcnt_u64(mask16);
  }

  for (; i < howmany; i++) {
    const char c = bytes[i];
    bytes[pos] = c;
    pos += ((unsigned char)c > 32 ? 1 : 0);
  }
  return pos;
}

static inline __m128i cleanm128(__m128i x, __m128i spaces, __m128i newline,
                                __m128i carriage, int *mask16) {
  __m128i xspaces = _mm_cmpeq_epi8(x, spaces);
  __m128i xnewline = _mm_cmpeq_epi8(x, newline);
  __m128i xcarriage = _mm_cmpeq_epi8(x, carriage);
  __m128i anywhite = _mm_or_si128(_mm_or_si128(xspaces, xnewline), xcarriage);
  *mask16 = _mm_movemask_epi8(anywhite);
  return _mm_shuffle_epi8(
      x, _mm_loadu_si128((const __m128i *)despace_mask16 + *mask16));
}

static inline size_t sse4_despace_branchless_u4(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');
  size_t i = 0;
  for (; i + 64 - 1 < howmany; i += 64) {
    __m128i x1 = _mm_loadu_si128((const __m128i *)(bytes + i));
    __m128i x2 = _mm_loadu_si128((const __m128i *)(bytes + i + 16));
    __m128i x3 = _mm_loadu_si128((const __m128i *)(bytes + i + 32));
    __m128i x4 = _mm_loadu_si128((const __m128i *)(bytes + i + 48));

    int mask16;
    x1 = cleanm128(x1, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x1);
    pos += 16 - _mm_popcnt_u32(mask16);

    x2 = cleanm128(x2, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x2);
    pos += 16 - _mm_popcnt_u32(mask16);

    x3 = cleanm128(x3, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x3);
    pos += 16 - _mm_popcnt_u32(mask16);

    x4 = cleanm128(x4, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x4);
    pos += 16 - _mm_popcnt_u32(mask16);
  }
  for (; i + 16 - 1 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16;
    x = cleanm128(x, spaces, newline, carriage, &mask16);
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

static inline size_t sse4_despace_branchless_u2(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');
  size_t i = 0;
  for (; i + 32 - 1 < howmany; i += 32) {
    __m128i x1 = _mm_loadu_si128((const __m128i *)(bytes + i));
    __m128i x2 = _mm_loadu_si128((const __m128i *)(bytes + i + 16));
    int mask16;
    x1 = cleanm128(x1, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x1);
    pos += 16 - _mm_popcnt_u32(mask16);

    x2 = cleanm128(x2, spaces, newline, carriage, &mask16);
    _mm_storeu_si128((__m128i *)(bytes + pos), x2);
    pos += 16 - _mm_popcnt_u32(mask16);
  }
  for (; i + 16 - 1 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16;
    x = cleanm128(x, spaces, newline, carriage, &mask16);
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

#ifdef __SSSE3__

// credit: https://github.com/aqrit
size_t despace_ssse3_cumsum( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const __m128i is_3or7 = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	const __m128i lut_cntrl = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00);
	const __m128i id = _mm_setr_epi8(
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
	const __m128i mask_02 = _mm_set1_epi8(0x02);
	const __m128i mask_04 = _mm_add_epi8(mask_02, mask_02);
	const __m128i mask_20 = _mm_slli_epi64(mask_02, 4);
	const __m128i mask_70 = _mm_set1_epi8(0x70);

	for( ; length >= 16; length-=16 ){
		__m128i a,b,c,d,s,t,v;
		size_t cnt0, cnt1;

		// load
		v = _mm_loadu_si128((__m128i*)src);
		src += 16;

		// detect spaces ( 0x01 == space, 0x00 == non-space )
		s = _mm_or_si128(_mm_abs_epi8(_mm_cmpeq_epi8(mask_20, v)),
			_mm_shuffle_epi8(lut_cntrl, _mm_adds_epu8(mask_70, v)));

		// create non-space mask ( 0x00 == space, 0xFF == non-space )
		b = _mm_cmpeq_epi8(_mm_setzero_si128(), s);

		// (qword) prefix sum of spaces
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 8));
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 16));
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 32));

		// get non-space byte totals
		t = _mm_srli_epi64(s, 56); // hi-byte is total_spaces
		cnt0 = 8 - _mm_cvtsi128_si64(t);
		t = _mm_unpackhi_epi64(t, t);
		cnt1 = 8 - _mm_cvtsi128_si64(t);

		// compress
		b = _mm_andnot_si128(b, s); // zero non-spaces
		//
		c = _mm_srli_epi64(_mm_and_si128(mask_02, b), 9);
		d = _mm_srli_epi64(_mm_shuffle_epi8(is_3or7, b), 16);
		a = _mm_or_si128(_mm_cmpgt_epi8(mask_04, s), _mm_cmpeq_epi8(b, mask_04)); // match first 4 and below
		//
		s = _mm_add_epi8(s, c);
		s = _mm_add_epi8(s, d);
		//
		s = _mm_max_epu8(s, _mm_srli_epi64(_mm_andnot_si128(a, s), 32));
		v = _mm_shuffle_epi8(v, _mm_add_epi8(s, id));

		// store
		_mm_storel_epi64((__m128i*)dst, v);
		dst += cnt0;
		_mm_storel_epi64((__m128i*)dst, _mm_unpackhi_epi64(v, v));
		dst += cnt1;
	}
	dst += despace_branchless(dst, src, length);
	return (size_t)(dst - ((uint8_t*)dst_void));
}

// credit: https://github.com/aqrit
size_t despace_ssse3_lut_1kb( void* dst_void, void* src_void, size_t length )
{
	__m128i * src = (__m128i *)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	static const uint64_t table[128] __attribute__((aligned(64))) = {
		0x0706050403020100, 0x0007060504030201, 0x0107060504030200, 0x0100070605040302,
		0x0207060504030100, 0x0200070605040301, 0x0201070605040300, 0x0201000706050403,
		0x0307060504020100, 0x0300070605040201, 0x0301070605040200, 0x0301000706050402,
		0x0302070605040100, 0x0302000706050401, 0x0302010706050400, 0x0302010007060504,
		0x0407060503020100, 0x0400070605030201, 0x0401070605030200, 0x0401000706050302,
		0x0402070605030100, 0x0402000706050301, 0x0402010706050300, 0x0402010007060503,
		0x0403070605020100, 0x0403000706050201, 0x0403010706050200, 0x0403010007060502,
		0x0403020706050100, 0x0403020007060501, 0x0403020107060500, 0x0403020100070605,
		0x0507060403020100, 0x0500070604030201, 0x0501070604030200, 0x0501000706040302,
		0x0502070604030100, 0x0502000706040301, 0x0502010706040300, 0x0502010007060403,
		0x0503070604020100, 0x0503000706040201, 0x0503010706040200, 0x0503010007060402,
		0x0503020706040100, 0x0503020007060401, 0x0503020107060400, 0x0503020100070604,
		0x0504070603020100, 0x0504000706030201, 0x0504010706030200, 0x0504010007060302,
		0x0504020706030100, 0x0504020007060301, 0x0504020107060300, 0x0504020100070603,
		0x0504030706020100, 0x0504030007060201, 0x0504030107060200, 0x0504030100070602,
		0x0504030207060100, 0x0504030200070601, 0x0504030201070600, 0x0504030201000706,
		0x0607050403020100, 0x0600070504030201, 0x0601070504030200, 0x0601000705040302,
		0x0602070504030100, 0x0602000705040301, 0x0602010705040300, 0x0602010007050403,
		0x0603070504020100, 0x0603000705040201, 0x0603010705040200, 0x0603010007050402,
		0x0603020705040100, 0x0603020007050401, 0x0603020107050400, 0x0603020100070504,
		0x0604070503020100, 0x0604000705030201, 0x0604010705030200, 0x0604010007050302,
		0x0604020705030100, 0x0604020007050301, 0x0604020107050300, 0x0604020100070503,
		0x0604030705020100, 0x0604030007050201, 0x0604030107050200, 0x0604030100070502,
		0x0604030207050100, 0x0604030200070501, 0x0604030201070500, 0x0604030201000705,
		0x0605070403020100, 0x0605000704030201, 0x0605010704030200, 0x0605010007040302,
		0x0605020704030100, 0x0605020007040301, 0x0605020107040300, 0x0605020100070403,
		0x0605030704020100, 0x0605030007040201, 0x0605030107040200, 0x0605030100070402,
		0x0605030207040100, 0x0605030200070401, 0x0605030201070400, 0x0605030201000704,
		0x0605040703020100, 0x0605040007030201, 0x0605040107030200, 0x0605040100070302,
		0x0605040207030100, 0x0605040200070301, 0x0605040201070300, 0x0605040201000703,
		0x0605040307020100, 0x0605040300070201, 0x0605040301070200, 0x0605040301000702,
		0x0605040302070100, 0x0605040302000701, 0x0605040302010700, 0x0605040302010007};

	const __m128i mask_01 = _mm_abs_epi8(_mm_cmpeq_epi8(_mm_setzero_si128(),_mm_setzero_si128()));
	const __m128i mask_20 = _mm_slli_epi64(mask_01, 5);
	const __m128i mask_70 = _mm_set1_epi8( 0x70 );
	const __m128i lut_cntrl = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00);

	for( ; length >= 16; length-=16 ){
		// load
		__m128i vector0 = _mm_loadu_si128(src++);
		__m128i vector1 = _mm_unpackhi_epi64(vector0, vector0);

		// detect spaces
		__m128i bytemask0 = _mm_or_si128(_mm_cmpeq_epi8(mask_20, vector0),
			_mm_shuffle_epi8(lut_cntrl, _mm_adds_epu8(mask_70, vector0)));

		// sort (compress)
		uint32_t bitmask0 = _mm_movemask_epi8(bytemask0);
		vector0 = _mm_shuffle_epi8(vector0, _mm_loadl_epi64((__m128i*) &table[bitmask0 & 0x7F]));
		vector1 = _mm_shuffle_epi8(vector1, _mm_loadl_epi64((__m128i*) &table[(bitmask0 >> 8) & 0x7F]));

		// count non-spaces
		__m128i hsum = _mm_sad_epu8(_mm_add_epi8(bytemask0, mask_01), _mm_setzero_si128());

		// store
		_mm_storel_epi64((__m128i*)dst, vector0);
		dst += _mm_cvtsi128_si64(hsum);
		_mm_storel_epi64((__m128i*)dst, vector1);
		dst += _mm_cvtsi128_si64(_mm_unpackhi_epi64(hsum, hsum));
	}

	// do remaining bytes
	dst += despace_branchless(dst, src, length);

	return (size_t)(dst - ((uint8_t*)dst_void));
}

__m128i lut_1mb [0x10000];
void gen_table_1mb( void ){
	for( size_t i = 0; i < 0x10000; i++ ){
		uint8_t* p = (uint8_t*) &lut_1mb[i];
		for( size_t j = 0; j < 16; j++ ){
			if( !(i & (1 << j)) ){
				*p++ = j;
			}
		}
	}
}
// credit: https://github.com/aqrit
size_t despace_ssse3_lut_1mb( void* dst_void, void* src_void, size_t length )
{
	__m128i * src = (__m128i *)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const __m128i mask_01 = _mm_abs_epi8(_mm_cmpeq_epi8(_mm_setzero_si128(),_mm_setzero_si128()));
	const __m128i mask_20 = _mm_slli_epi64(mask_01, 5);
	const __m128i mask_70 = _mm_set1_epi8( 0x70 );
	const __m128i lut_cntrl = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00);

	for( ; length >= 16; length-=16 ){
		__m128i v = _mm_loadu_si128(src++);
		__m128i bytemask = _mm_or_si128(_mm_cmpeq_epi8(mask_20, v),
			_mm_shuffle_epi8(lut_cntrl, _mm_adds_epu8(mask_70, v)));
		int bitmask = _mm_movemask_epi8(bytemask);
		v = _mm_shuffle_epi8(v, _mm_load_si128(&lut_1mb[bitmask]));
		_mm_storeu_si128((__m128i*)dst, v);
		__m128i hsum = _mm_sad_epu8(_mm_add_epi8(bytemask, mask_01), _mm_setzero_si128());
		hsum = _mm_add_epi64(hsum, _mm_unpackhi_epi64(hsum, hsum));
		dst += _mm_cvtsi128_si64(hsum);
	}
	dst += despace_branchless(dst, src, length);
	return (size_t)(dst - ((uint8_t*)dst_void));
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

static inline size_t sse42_despace_branchless_lookup(char *bytes,
                                                     size_t howmany) {
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

#ifdef __AVX2__

#include <x86intrin.h>

#ifndef __clang__
static inline __m256i _mm256_loadu2_m128i(__m128i const *__addr_hi,
                                          __m128i const *__addr_lo) {
  __m256i __v256 = _mm256_castsi128_si256(_mm_loadu_si128(__addr_lo));
  return _mm256_insertf128_si256(__v256, _mm_loadu_si128(__addr_hi), 1);
}

static inline void _mm256_storeu2_m128i(__m128i *__addr_hi, __m128i *__addr_lo,
                                        __m256i __a) {
  __m128i __v128;

  __v128 = _mm256_castsi256_si128(__a);
  _mm_storeu_si128(__addr_lo, __v128);
  __v128 = _mm256_extractf128_si256(__a, 1);
  _mm_storeu_si128(__addr_hi, __v128);
}
#endif

// credit: https://github.com/aqrit
size_t despace_avx2_vpermd( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const __m256i mask_20  = _mm256_set1_epi8( 0x20 );
	const __m256i mask_70  = _mm256_set1_epi8( 0x70 );
	const __m256i lut_cntrl = _mm256_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
		//
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
	);

	const __m256i mask_index = _mm256_set1_epi64x( 0x80A0908884828180 );
	const __m256i mask_shift = _mm256_set1_epi64x( 0x0000000008080808 );
	const __m256i mask_invert = _mm256_set1_epi64x( 0x0020100800000000 );
	const __m256i mask_fixup = _mm256_set_epi32(
		0x08080808, 0x0F0F0F0F, 0x00000000, 0x07070707,
		0x08080808, 0x0F0F0F0F, 0x00000000, 0x07070707
	);
	const __m256i lut = _mm256_set_epi32(
		0x04050607, // 0x03020100', 0x000000'07
		0x04050704, // 0x030200'00, 0x0000'0704
		0x04060705, // 0x030100'00, 0x0000'0705
		0x04070504, // 0x0300'0000, 0x00'070504
		0x05060706, // 0x020100'00, 0x0000'0706
		0x05070604, // 0x0200'0000, 0x00'070604
		0x06070605, // 0x0100'0000, 0x00'070605
		0x07060504  // 0x00'000000, 0x'07060504
	);

	for( ; length >= 32; length-=32 ){
		__m256i r0,r1,r2,r3,r4;

		r0 = _mm256_loadu_si256((__m256i *)src); // asrc
		src += 32;

		r1 = _mm256_adds_epu8(mask_70, r0);
		r2 = _mm256_cmpeq_epi8(mask_20, r0);
		r1 = _mm256_shuffle_epi8(lut_cntrl, r1);
		r1 = _mm256_or_si256(r1, r2); // bytemask of spaces

		r2 = _mm256_andnot_si256(r1, mask_index);
		r1 = _mm256_and_si256(r1, mask_shift);
		r2 = _mm256_sad_epu8(r2, mask_invert); // bitmap[0:5], popcnt[7:15]
		r1 = _mm256_sad_epu8(r1, _mm256_setzero_si256()); // shift amount
		r3 = _mm256_slli_epi64(r2, 29); // move hi index to 2nd dword
		r4 = _mm256_srli_epi64(r2, 7); // popcnt
		r2 = _mm256_or_si256(r2, r3);
		r2 = _mm256_permutevar8x32_epi32(lut, r2);
		r2 = _mm256_xor_si256(r2, mask_fixup);
		r2 = _mm256_srlv_epi64(r2, r1);
		r0 = _mm256_shuffle_epi8(r0, r2);

		*((uint64_t*)dst) = _mm256_extract_epi64(r0, 0);
		dst += _mm256_extract_epi64(r4, 0);
		*((uint64_t*)dst) = _mm256_extract_epi64(r0, 1);
		dst += _mm256_extract_epi64(r4, 1);
		*((uint64_t*)dst) = _mm256_extract_epi64(r0, 2);
		dst += _mm256_extract_epi64(r4, 2);
		*((uint64_t*)dst) = _mm256_extract_epi64(r0, 3);
		dst += _mm256_extract_epi64(r4, 3);
	}
	dst += despace_branchless(dst, src, length);
	return (size_t)(dst - ((uint8_t*)dst_void));
}

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
      count++;
    }
  }
  return count;
}

static inline __m256i cleanm256(__m256i x, __m256i spaces, __m256i newline,
                                __m256i carriage, unsigned int *mask1,
                                unsigned int *mask2) {
  __m256i xspaces = _mm256_cmpeq_epi8(x, spaces);
  __m256i xnewline = _mm256_cmpeq_epi8(x, newline);
  __m256i xcarriage = _mm256_cmpeq_epi8(x, carriage);
  __m256i anywhite =
      _mm256_or_si256(_mm256_or_si256(xspaces, xnewline), xcarriage);
  unsigned int mask32 = _mm256_movemask_epi8(anywhite);
  unsigned int maskhigh = (mask32) >> 16;
  unsigned int masklow = (mask32)&0xFFFF;
  assert(maskhigh < (1 << 16));
  assert(masklow < (1 << 16));
  *mask1 = masklow;
  *mask2 = maskhigh;
  __m256i mask = _mm256_loadu2_m128i((const __m128i *)despace_mask16 + maskhigh,
                                     (const __m128i *)despace_mask16 + masklow);
  return _mm256_shuffle_epi8(x, mask);
}

static inline size_t avx2_despace_branchless(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');

  __m256i spaces256 = _mm256_set1_epi8(' ');
  __m256i newline256 = _mm256_set1_epi8('\n');
  __m256i carriage256 = _mm256_set1_epi8('\r');

  size_t i = 0;
  for (; i + 32 - 1 < howmany; i += 32) {
    __m256i x = _mm256_loadu_si256((const __m256i *)(bytes + i));
    unsigned int masklow, maskhigh;
    x = cleanm256(x, spaces256, newline256, carriage256, &masklow, &maskhigh);
    int offset1 = 16 - _mm_popcnt_u32(masklow);
    int offset2 = 16 - _mm_popcnt_u32(maskhigh);
    _mm256_storeu2_m128i((__m128i *)(bytes + pos + offset1),
                         (__m128i *)(bytes + pos), x);
    pos += offset1 + offset2;
  }
  for (; i + 16 - 1 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16;
    x = cleanm128(x, spaces, newline, carriage, &mask16);
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
static inline size_t avx2_despace_branchless_u2(char *bytes, size_t howmany) {
  size_t pos = 0;
  __m128i spaces = _mm_set1_epi8(' ');
  __m128i newline = _mm_set1_epi8('\n');
  __m128i carriage = _mm_set1_epi8('\r');

  __m256i spaces256 = _mm256_set1_epi8(' ');
  __m256i newline256 = _mm256_set1_epi8('\n');
  __m256i carriage256 = _mm256_set1_epi8('\r');

  size_t i = 0;
  for (; i + 64 - 1 < howmany; i += 64) {
    __m256i x1, x2;
    int offset11, offset12, offset21, offset22;
    unsigned int masklow1, maskhigh1, masklow2, maskhigh2;

    x1 = _mm256_loadu_si256((const __m256i *)(bytes + i));
    x2 = _mm256_loadu_si256((const __m256i *)(bytes + i + 32));

    x1 = cleanm256(x1, spaces256, newline256, carriage256, &masklow1,
                   &maskhigh1);
    offset11 = 16 - _mm_popcnt_u32(masklow1);
    offset12 = 16 - _mm_popcnt_u32(maskhigh1);
    x2 = cleanm256(x2, spaces256, newline256, carriage256, &masklow2,
                   &maskhigh2);
    offset21 = 16 - _mm_popcnt_u32(masklow2);
    offset22 = 16 - _mm_popcnt_u32(maskhigh2);

    _mm256_storeu2_m128i((__m128i *)(bytes + pos + offset11),
                         (__m128i *)(bytes + pos), x1);
    pos += offset11 + offset12;

    _mm256_storeu2_m128i((__m128i *)(bytes + pos + offset21),
                         (__m128i *)(bytes + pos), x2);
    pos += offset21 + offset22;
  }

  for (; i + 32 - 1 < howmany; i += 32) {
    unsigned int masklow, maskhigh;

    int offset1, offset2;
    __m256i x = _mm256_loadu_si256((const __m256i *)(bytes + i));
    x = cleanm256(x, spaces256, newline256, carriage256, &masklow, &maskhigh);
    offset1 = 16 - _mm_popcnt_u32(masklow);
    offset2 = 16 - _mm_popcnt_u32(maskhigh);
    _mm256_storeu2_m128i((__m128i *)(bytes + pos + offset1),
                         (__m128i *)(bytes + pos), x);
    pos += offset1 + offset2;
  }
  for (; i + 16 - 1 < howmany; i += 16) {
    __m128i x = _mm_loadu_si128((const __m128i *)(bytes + i));
    int mask16;
    x = cleanm128(x, spaces, newline, carriage, &mask16);
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

#endif

#endif

#ifdef __SSE4_2__

#include <x86intrin.h>

inline size_t sse42_despace_to(const char *__restrict__ bytes, size_t howmany, char *__restrict__ out);

inline size_t sse42_despace(char *bytes, size_t howmany);

inline size_t sse42_despace_branchless(char *bytes, size_t howmany);

inline size_t sse42_despace_branchless_lookup(char *bytes, size_t howmany);

#endif

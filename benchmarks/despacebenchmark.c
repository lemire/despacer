#define _POSIX_C_SOURCE 199309L
#include "./clock_gettime.h"
#include "despacer.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for clock_t, clock()
struct timespec start;
struct timespec end;
// time spent in nanoseconds
double timespent() {
  return (end.tv_sec - start.tv_sec) * 1000000000 +
         (end.tv_nsec - start.tv_nsec);
}
#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("cpuid\n\t"                                                 \
                   "rdtsc\n\t"                                                 \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("rdtscp\n\t"                                                \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   "cpuid\n\t"                                                 \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define BEST_TIME_NOCHECK(test, pre, repeat, number)                           \
  do {                                                                         \
    printf("%-50s: ", #test);                                                  \
    fflush(NULL);                                                              \
    uint64_t tm1, tm2;                                                         \
    uint64_t min_diff = (uint64_t)-1;                                          \
    double t = 1e200;                                                          \
    clock_gettime(CLOCK_REALTIME, &start);                                     \
    RDTSC_START(tm1);                                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      test;                                                                    \
    }                                                                          \
    RDTSC_FINAL(tm2);                                                          \
    clock_gettime(CLOCK_REALTIME, &end);                                       \
    double frequency = (tm2 - tm1) / timespent();                              \
    printf(" base frequency  %.2f GHz", frequency);                            \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      clock_gettime(CLOCK_REALTIME, &start);                                   \
      RDTSC_START(tm1);                                                        \
      test;                                                                    \
      RDTSC_FINAL(tm2);                                                        \
      clock_gettime(CLOCK_REALTIME, &end);                                     \
      uint64_t tmus = tm2 - tm1;                                               \
      double ts = timespent();                                                 \
      if (tmus < min_diff)                                                     \
        min_diff = tmus;                                                       \
      if (ts < t)                                                              \
        t = ts;                                                                \
    }                                                                          \
    printf(" speed: %.2f GB/s", frequency / min_diff * number);                \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

#define BEST_TIME_CHECK(test, check, pre, repeat, number)                      \
  do {                                                                         \
    for (int trial = 0; trial < 3; trial++) {                                  \
      printf("%-50s: ", #test);                                                \
      fflush(NULL);                                                            \
      uint64_t tm1, tm2;                                                       \
      uint64_t min_diff = (uint64_t)-1;                                        \
      double t = 1e200;                                                        \
      clock_gettime(CLOCK_REALTIME, &start);                                   \
      RDTSC_START(tm1);                                                        \
      for (int i = 0; i < repeat; i++) {                                       \
        pre;                                                                   \
        if (test != check)                                                     \
          printf("bug");                                                       \
      }                                                                        \
      RDTSC_FINAL(tm2);                                                        \
      clock_gettime(CLOCK_REALTIME, &end);                                     \
      double frequency = (tm2 - tm1) / timespent();                            \
      printf(" base frequency  %.2f GHz", frequency);                          \
      for (int i = 0; i < repeat; i++) {                                       \
        pre;                                                                   \
        __asm volatile("" ::: /* pretend to clobber */ "memory");              \
        clock_gettime(CLOCK_REALTIME, &start);                                 \
        RDTSC_START(tm1);                                                      \
        if (test != check)                                                     \
          printf("bug");                                                       \
        RDTSC_FINAL(tm2);                                                      \
        clock_gettime(CLOCK_REALTIME, &end);                                   \
        uint64_t tmus = tm2 - tm1;                                             \
        double ts = timespent();                                               \
        if (tmus < min_diff)                                                   \
          min_diff = tmus;                                                     \
        if (ts < t)                                                            \
          t = ts;                                                              \
      }                                                                        \
      printf(" speed: %.2f GB/s", frequency / min_diff * number);              \
      printf("\n");                                                            \
      fflush(NULL);                                                            \
    }                                                                          \
  } while (0)

// let us estimate that we have a 1% proba of hitting a white space
size_t fillwithtext(char *buffer, size_t size) {
  size_t howmany = 0;
  for (size_t i = 0; i < size; ++i) {
    double r = ((double)rand() / (RAND_MAX));
    if (r < 0.01) {
      buffer[i] = ' ';
      howmany++;
    } else if (r < 0.02) {
      buffer[i] = '\n';
      howmany++;
    } else if (r < 0.03) {
      buffer[i] = '\r';
      howmany++;
    } else {
      do {
        buffer[i] = (char)rand();
      } while (buffer[i] <= 32);
    }
  }
  return howmany;
}

int main(int argc, char **argv) {
#ifdef __SSSE3__
  gen_table_512kb();
#endif
  const int N = 1024 * 8;
  int alignoffset = 0;
  if (argc > 1) {
    alignoffset = atoi(argv[1]);
    printf("alignment offset = %d \n", alignoffset);
  }
  char *origbuffer = malloc(N + alignoffset);
  char *origtmpbuffer = malloc(N + alignoffset);
  char *buffer = origbuffer + alignoffset;
  char *tmpbuffer = origtmpbuffer + alignoffset;
  printf("pointer alignment = %d bytes \n",
         1 << __builtin_ctzll((uintptr_t)(const void *)(buffer)));
  int repeat = 100;
  size_t howmanywhite;
  BEST_TIME_NOCHECK(memcpy(tmpbuffer, buffer, N),
                    howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(countspaces(buffer, N), howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(countspaces32(buffer, N), howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace32(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(faster_despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(faster_despace32(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace64(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_to(buffer, N, tmpbuffer), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_branchless(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_cmov(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_table(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
#ifdef __SSSE3__
  BEST_TIME_CHECK(despace_ssse3_cumsum(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_ssse3_lut_1kb(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_ssse3_lut_512kb(tmpbuffer, buffer, N),
                  N - howmanywhite, howmanywhite = fillwithtext(buffer, N),
                  repeat, N);
#endif
#ifdef __AVX2__
  BEST_TIME_CHECK(avx2_countspaces(buffer, N), howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(despace_avx2_vpermd(tmpbuffer, buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(avx2_despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(avx2_despace_branchless(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(avx2_despace_branchless_u2(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
#endif
#ifdef __SSE4_1__
  BEST_TIME_CHECK(sse4_despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_branchless(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_branchless32(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_branchless_u2(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_branchless_u4(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_skinny_u2(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_skinny_u4(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse4_despace_trail(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
#endif
#ifdef __SSE4_2__
  BEST_TIME_CHECK(sse42_despace_branchless(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse42_despace_branchless_lookup(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME_CHECK(sse42_despace_to(buffer, N, tmpbuffer), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
#endif
#ifdef __AVX512VBMI2__
  BEST_TIME_CHECK(vbmi2_despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
#endif
  free(origbuffer);
  free(origtmpbuffer);
  printf("Warning: the estimated speed is for illustrative purposes and may "
         "not be accurate.\n");
}

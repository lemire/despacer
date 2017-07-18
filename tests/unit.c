#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "despacer.h"



// let us estimate that we have a high proba of hitting a white space
size_t fillwithtext(char *buffer, size_t size) {
  size_t howmany = 0;
  for (size_t i = 0; i < size; ++i) {
    double r = ((double)rand() / (RAND_MAX));
    if (r < 0.01) {
      buffer[i] = ' ';
      howmany++;
    } else if (r < 0.20) {
      buffer[i] = '\n';
      howmany++;
    } else if (r < 0.30) {
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

#define DESPACE_CHECK(despacecommand,N, buffer, solution, expectednonwhite)                      \
  do {                                                                                      \
    assert(countspaces(buffer, N)==N-expectednonwhite);\
    size_t observedwhite = (size_t) despacecommand(buffer,N);                                          \
        assert(countspaces(buffer, observedwhite)==0);\
    if(observedwhite != expectednonwhite) {                                                    \
       printf("%s: bad white space count %d %d\n", #despacecommand, (int) observedwhite,  (int)expectednonwhite); \
        return -1;                                                                          \
    }                                                                                       \
    int aretheydifferent = memcmp ( buffer, solution,expectednonwhite) ; \
    if(aretheydifferent != 0) {                                         \
       printf("%s: content differs %d \n", #despacecommand, aretheydifferent);                                      \
       for(size_t z = 0; z < (size_t) expectednonwhite; z++) {                                             \
         printf("%d: correct = '%c'  computed = '%c' ", (int) z,buffer[z], solution[z]);                                    \
         if(buffer[z] != solution[z]) printf(" THEY DIFFER ");                              \
         printf("\n");                                                                   \
       }                                                                                   \
       return -2;                                                                          \
    }                                                                                       \
  } while (0)

#define DESPACE_CHECK_SD(despacecommand,N, buffer, solution, expectednonwhite)                      \
  do {                                                                                      \
    assert(countspaces(buffer, N)==N-expectednonwhite);\
    size_t observedwhite = (size_t) despacecommand(buffer,buffer,N);                                          \
        assert(countspaces(buffer, observedwhite)==0);\
    if(observedwhite != expectednonwhite) {                                                    \
       printf("%s: bad white space count %d %d\n", #despacecommand, (int) observedwhite,  (int)expectednonwhite); \
        return -1;                                                                          \
    }                                                                                       \
    int aretheydifferent = memcmp ( buffer, solution,expectednonwhite) ; \
    if(aretheydifferent != 0) {                                         \
       printf("%s: content differs %d \n", #despacecommand, aretheydifferent);                                      \
       for(size_t z = 0; z < (size_t) expectednonwhite; z++) {                                             \
         printf("%d: correct = '%c'  computed = '%c' ", (int) z,buffer[z], solution[z]);                                    \
         if(buffer[z] != solution[z]) printf(" THEY DIFFER ");                              \
         printf("\n");                                                                   \
       }                                                                                   \
       return -2;                                                                          \
    }                                                                                       \
  } while (0)

int main() {
  printf("testing ");

#ifdef __SSSE3__
  gen_table_1mb();
#endif
  size_t repeat = 1000;
  for(size_t i = 0; i < repeat; i++) {
    printf(".");
    fflush(NULL);
    const int N = rand() % 1000000;
    char *buffer = malloc(N);
    char *solution = malloc(N);
    char *tmp = malloc(N);
    size_t howmanynonwhite = N - fillwithtext(buffer, N);
    memcpy(solution,buffer,N);
    despace(solution,N);
    memcpy(tmp,buffer,N);
    assert(countspaces(buffer, N)==N-howmanynonwhite);
    size_t hm1 = despace(tmp,N);
    assert(hm1 == howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(despace,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(faster_despace,N,tmp,solution,howmanynonwhite);
    assert(countspaces(buffer, N)==N-howmanynonwhite);
    memcpy(tmp,buffer,N);
    assert(countspaces(tmp, N)==N-howmanynonwhite);
    DESPACE_CHECK(faster_despace32,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_branchless,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_cmov,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_table,N,tmp,solution,howmanynonwhite);
#ifdef __AVX2__
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_avx2_vpermd,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(avx2_despace,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(avx2_despace_branchless,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(avx2_despace_branchless_u2,N,tmp,solution,howmanynonwhite);
#endif
#ifdef __SSSE3__
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_ssse3_cumsum,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_ssse3_lut_1kb,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK_SD(despace_ssse3_lut_1mb,N,tmp,solution,howmanynonwhite);
#endif
#ifdef __SSE4_1__
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse4_despace,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse4_despace_branchless,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse4_despace_branchless32,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse4_despace_branchless_u2,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse4_despace_branchless_u4,N,tmp,solution,howmanynonwhite);
#endif
#ifdef __SSE4_2__
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse42_despace_branchless,N,tmp,solution,howmanynonwhite);
    memcpy(tmp,buffer,N);
    DESPACE_CHECK(sse42_despace_branchless_lookup,N,tmp,solution,howmanynonwhite);
#endif
  free(buffer);
  free(tmp);
  free(solution);

  }
  printf(" the code looks ok.\n");
  return 0;
}

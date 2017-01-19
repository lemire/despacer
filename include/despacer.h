#ifndef DESPACER_H
#define DESPACER_H

#include <stdint.h>
#include <stddef.h>

size_t despace(char * bytes, size_t howmany) {
  size_t pos = 0;
  for(size_t i = 0; i < howmany; i++) {
      char c = bytes[i];
      if (c == '\r' || c == '\n' || c == ' ') {
        continue;
      }
      bytes[pos++] = c;
  }
  return pos;
}

#endif

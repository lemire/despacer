module despacer.despacer;

// re-export
public import despacer.libc.despacer_generic;

version (D_SIMD)
{
  public import despacer.libc.despacer_SSSE3;
  public import despacer.libc.despacer_SSE4_1;
  public import despacer.libc.despacer_SSE4_2;
}

version (D_AVX2)
{
  public import despacer.libc.despacer_AVX2;
}

// idiomatic wrappers:

/**
* remove spaces from string bytes (UTF-8 or ASCII)
*/
string ddespace(string str) nothrow
{
  import std : toStringz, fromStringz;

  auto length = str.length;
  auto cstr = cast(char*) toStringz(str.dup);
  despace(cstr, length);
  return cast(string) fromStringz(cstr);
}

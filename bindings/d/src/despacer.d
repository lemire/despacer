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

/** Writes a wrapper around the C functions. All the C functions will be prefixed with `d_`.
  For example:
    string d_despacer(string str) nothrow;
*/
private string d_despacer_mixin(const string functionName, const string castTo = "char*")
{
  import std : format;

  return "
    /**
      Params:
        str = the passed string which contains space, \n, or \r.
      Return:
        a slice to a the string without any spaces
    */
    auto d_"~functionName~"(string str) nothrow
    {
      import std : toStringz, fromStringz;

      auto length = str.length;
      auto cstr = cast(" ~ castTo ~ ") toStringz(str.dup);
      auto pos = "~functionName~`(cstr, length);
      return (cast(string) fromStringz(cstr))[0..pos];
    }

    unittest {
      import std: writeln;
      writeln("Testing " ~ "`~functionName~`");
      auto tmp = d_`~functionName~`("some \n  string      here \n\r");
      assert(tmp == "somestringhere", "`~functionName~` returned " ~ tmp ~ " instead of somestringhere");
    }
  `;
}

// idiomatic wrappers:

// generic
mixin(d_despacer_mixin("despace"));
mixin(d_despacer_mixin("despace32"));
mixin(d_despacer_mixin("despace64"));
mixin(d_despacer_mixin("faster_despace"));
mixin(d_despacer_mixin("faster_despace32"));

version (D_SIMD)
{
  // sse4_1
  mixin(d_despacer_mixin("sse4_despace"));
  mixin(d_despacer_mixin("sse4_despace_branchless"));
  mixin(d_despacer_mixin("sse4_despace_branchless32"));
  mixin(d_despacer_mixin("sse4_despace_branchless_u4"));
  mixin(d_despacer_mixin("sse4_despace_branchless_u2"));
  mixin(d_despacer_mixin("sse4_despace_skinny_u4"));
  mixin(d_despacer_mixin("sse4_despace_skinny_u2"));
  mixin(d_despacer_mixin("sse4_despace_trail"));

  // sse4_2
  mixin(d_despacer_mixin("sse42_despace"));
  mixin(d_despacer_mixin("sse42_despace_branchless"));
  mixin(d_despacer_mixin("sse42_despace_branchless_lookup"));
}


version (D_AVX) {
  // avx2
  mixin(d_despacer_mixin("avx2_despace"));
  mixin(d_despacer_mixin("avx2_despace_branchless"));
  mixin(d_despacer_mixin("avx2_despace_branchless_u2"));
}

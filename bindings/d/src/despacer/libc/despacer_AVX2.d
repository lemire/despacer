module despacer.libc.despacer_AVX2;

import core.stdc.config;
import core.stdc.stdarg : va_list;
static import core.simd;
static import std.conv;

struct Int128
{
  long lower;
  long upper;
}

struct UInt128
{
  ulong lower;
  ulong upper;
}

struct __locale_data
{
  int dummy;
} // FIXME

alias _Bool = bool;
struct dpp
{
  static struct Opaque(int N)
  {
    void[N] bytes;
  }
  // Replacement for the gcc/clang intrinsic
  static bool isEmpty(T)()
  {
    return T.tupleof.length == 0;
  }

  static struct Move(T)
  {
    T* ptr;
  }
  // dmd bug causes a crash if T is passed by value.
  // Works fine with ldc.
  static auto move(T)(ref T value)
  {
    return Move!T(&value);
  }

  mixin template EnumD(string name, T, string prefix) if (is(T == enum))
  {
    private static string _memberMixinStr(string member)
    {
      import std.conv : text;
      import std.array : replace;

      return text(` `, member.replace(prefix, ""), ` = `, T.stringof, `.`, member, `,`);
    }

    private static string _enumMixinStr()
    {
      import std.array : join;

      string[] ret;
      ret ~= "enum " ~ name ~ "{";
      static foreach (member; __traits(allMembers, T))
      {
        ret ~= _memberMixinStr(member);
      }
      ret ~= "}";
      return ret.join("\n");
    }

    mixin(_enumMixinStr());
  }
}

extern (C)
{

  size_t despace_avx2_vpermd(void*, void*, size_t) @nogc nothrow;
  /**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
  size_t avx2_despace(char*, size_t) @nogc nothrow;

  size_t avx2_countspaces(const(char)*, size_t) @nogc nothrow;
  /** Check if the given bytes with length howmany have space among them */
  bool avx2_hasspace(const(char)*, size_t) @nogc nothrow;

  __m256i cleanm256(__m256i, __m256i, __m256i, __m256i, uint*, uint*) @nogc nothrow;

  size_t avx2_despace_branchless(char*, size_t) @nogc nothrow;

  size_t avx2_despace_branchless_u2(char*, size_t) @nogc nothrow;
}

import core.simd;

alias __m128i = long2;
alias __m256i = __vector(long[4]);

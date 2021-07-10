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
}

alias _Bool = bool;
struct dpp
{
  static struct Opaque(int N)
  {
    void[N] bytes;
  }

  static bool isEmpty(T)()
  {
    return T.tupleof.length == 0;
  }

  static struct Move(T)
  {
    T* ptr;
  }

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
  ulong despace_avx2_vpermd(void*, void*, ulong) @nogc nothrow;
  ulong avx2_despace(char*, ulong) @nogc nothrow;
  ulong avx2_countspaces(const(char)*, ulong) @nogc nothrow;
  core.simd.long4 cleanm256(core.simd.long4, core.simd.long4, core.simd.long4, core.simd.long4, uint*, uint*) @nogc nothrow;
  ulong avx2_despace_branchless(char*, ulong) @nogc nothrow;
  ulong avx2_despace_branchless_u2(char*, ulong) @nogc nothrow;
}

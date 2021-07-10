module despacer.libc.despacer_SSE4_1;

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
  ulong sse4_despace(char*, ulong) @nogc nothrow;
  ulong sse4_despace_branchless(char*, ulong) @nogc nothrow;
  ulong sse4_despace_branchless32(char*, ulong) @nogc nothrow;
  core.simd.long2 cleanm128(core.simd.long2, core.simd.long2, core.simd.long2, core.simd.long2, int*) @nogc nothrow;
  ulong sse4_despace_branchless_u4(char*, ulong) @nogc nothrow;
  ulong sse4_despace_branchless_u2(char*, ulong) @nogc nothrow;
  core.simd.long2 skinnycleanm128(core.simd.long2, int*) @nogc nothrow;
  ulong sse4_despace_skinny_u4(char*, ulong) @nogc nothrow;
  ulong sse4_despace_skinny_u2(char*, ulong) @nogc nothrow;
  ulong sse4_despace_trail(char*, ulong) @nogc nothrow;
}

module despacer.libc.despacer_SSSE3;

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

  void gen_table_512kb() @nogc nothrow;

  size_t despace_ssse3_lut_512kb(void*, void*, size_t) @nogc nothrow;

  size_t despace_ssse3_lut_1kb(void*, void*, size_t) @nogc nothrow;

  size_t despace_ssse3_cumsum(void*, void*, size_t) @nogc nothrow;
}

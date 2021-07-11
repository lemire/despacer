module despacer.libc.despacer_generic;

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
  ulong despace_to(const(char)*, ulong, char*) @nogc nothrow;
  ulong despace64(char*, ulong) @nogc nothrow;
  bool hasspace(const(char)*, ulong) @nogc nothrow;
  ulong countspaces32(const(char)*, ulong) @nogc nothrow;
  ulong countspaces(const(char)*, ulong) @nogc nothrow;
  ulong faster_despace32(char*, ulong) @nogc nothrow;
  ulong faster_despace(char*, ulong) @nogc nothrow;
  ulong despace32(char*, ulong) @nogc nothrow;
  ulong despace_table(void*, void*, ulong) @nogc nothrow;
  ulong despace_cmov(void*, void*, ulong) @nogc nothrow;
  ulong despace_branchless(void*, void*, ulong) @nogc nothrow;
  ulong despace(char*, ulong) @nogc nothrow;
}

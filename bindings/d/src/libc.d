module despacer.libc;

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
  /**

* remove spaces from string bytes (UTF-8 or ASCII) containing "howmany"

* characters (not counting the null character if a C string is used), returns

* the new number of characters

* this function does add the null character, you have to do it yourself if you

* need it.

* Result is stored in "out", where the caller is responsible for memory

* allocation.

*/
  static ulong despace_to(const(char)*, ulong, char*) @nogc nothrow;

  static ulong despace64(char*, ulong) @nogc nothrow;

  static ulong countspaces32(const(char)*, ulong) @nogc nothrow;

  static ulong countspaces(const(char)*, ulong) @nogc nothrow;

  static ulong faster_despace32(char*, ulong) @nogc nothrow;

  static ulong faster_despace(char*, ulong) @nogc nothrow;

  static ulong despace32(char*, ulong) @nogc nothrow;

  ulong despace_table(void*, void*, ulong) @nogc nothrow;

  ulong despace_cmov(void*, void*, ulong) @nogc nothrow;

  ulong despace_branchless(void*, void*, ulong) @nogc nothrow;
  /**

* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing

* "howmany"

* characters (not counting the null character if a C string is used), returns

* the new number of characters

* this function does add the null character, you have to do it yourself if you

* need it.

*/
  static ulong despace(char*, ulong) @nogc nothrow;

  extern __gshared int[256] jump_table;

}

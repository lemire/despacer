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
  size_t despace_to(const(char)*, size_t, char*) @nogc nothrow;

  size_t despace64(char*, size_t) @nogc nothrow;
  /** Check if the given bytes with length howmany have space among them */
  bool hasspace(const(char)*, size_t) @nogc nothrow;

  size_t countspaces32(const(char)*, size_t) @nogc nothrow;

  size_t countspaces(const(char)*, size_t) @nogc nothrow;

  size_t faster_despace32(char*, size_t) @nogc nothrow;

  size_t faster_despace(char*, size_t) @nogc nothrow;

  size_t despace32(char*, size_t) @nogc nothrow;

  size_t despace_table(void*, void*, size_t) @nogc nothrow;
  /// explicit branchless
  size_t despace_cmov(void*, void*, size_t) @nogc nothrow;
  /// make the store unconditional
  size_t despace_branchless(void*, void*, size_t) @nogc nothrow;
  /**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
  size_t despace(char*, size_t) @nogc nothrow;
}

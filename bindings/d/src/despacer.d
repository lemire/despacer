module despacer.despacer;

// idiomatic wrappers:

// do not import libc as the extern C functions will conflict
public import libc = despacer.libc;

/**
* remove spaces from string bytes (UTF-8 or ASCII)
*/
string despace(string str) nothrow
{
  import std : toStringz, fromStringz;

  auto length = str.length;
  auto cstr = cast(char*) toStringz(str.dup);
  libc.despace(cstr, length);
  return cast(string) fromStringz(cstr);
}

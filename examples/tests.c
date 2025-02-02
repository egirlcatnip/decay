#include "../decay.h"

void print_limits();
void print_edge_cases();
void print_hex();
void print_bin();

i32 main() {
  println("Binary Representation:");
  println("");
  print_bin();
  println("Hex Representation:");
  println("");
  print_hex();
  println("Edge Cases:");
  println("");
  print_edge_cases();
  println("Type Limits:");
  println("");
  print_limits();
  return 0;
}

void print_hex() {
  println("i8 positive hex:  (i8)123");
  println("{i8:x}", (i8)123);
  println("i8 negative hex:  (i8)-123");
  println("{i8:x}", (i8)-123);

  println("u32 hex: (u32)12345");
  println("{u32:x}", (u32)12345);

  println("i8 uppercase hex:  (i8)123");
  println("{i8:X}", (i8)123);
  println("i8 lowercase hex:  (i8)123");
  println("{i8:x}", (i8)123);

  println("");
}

void print_bin() {
  println("i8 positive bin:  (i8)123");
  println("{i8:b}", (i8)123);
  println("i8 negative bin:  (i8)-123");
  println("{i8:b}", (i8)-123);

  println("u16 bin with _: (u16)12345");
  println("{u16:B}", (u16)12345);

  println("u32 bin: (u32)1234567");
  println("{u32:b}", (u32)1234567);

  println("f32 bin: (f32)3.1415");
  println("{f32:b}", (f32)3.1415);

  println("");
}

void print_edge_cases() {
  println("Empty string:     \t\"{s}\"", "");
  println("Null string:      \t\"{s}\"", NULL);
  println("Escaped:        \t{{i32}}", 1);
  println("Braces:           \t{{");
  println("Braces:           \t{");
  println("Braces:           \t}}");
  println("Braces:           \t}");
  println("");

  println("Malformed:        \t{i32", 42);
  println("Malformed:        \ti32}", 42);
  println("Excess (2 args):  \t{i32}", 42, 64);

  println("");

  println("Invalid:          \t{s.0}", 0);
  println("Invalid:          \t{c.0}", '&');
  println("Invalid:          \t{p.0}", 3);
  println("Invalid:          \t{i32.0}", 1);
  println("Invalid:          \t{i32:b.0}", 2);
  println("Invalid:          \t{i32:x.0}", 4);
  println("Invalid:          \t{f32.-1}", 5);
  println("Invalid:          \t{invalid}", 6);
  println("");
}

void print_limits() {
  println("i8: ");
  println("Min: {i8}\nMax:  {i8}", I8_MIN, I8_MAX);
  println("");

  println("i16: ");
  println("Min: {i16}\nMax:  {i16}", I16_MIN, I16_MAX);
  println("");

  println("i32: ");
  println("Min: {i32}\nMax:  {i32}", I32_MIN, I32_MAX);
  println("");

  println("i64: ");
  println("Min: {i64}\nMax:  {i64}", I64_MIN, I64_MAX);
  println("");

  println("u8: ");
  println("Min: {u8}\nMax: {u8}", U8_MIN, U8_MAX);
  println("");

  println("u16 ");
  println("Min: {u16}\nMax: {u16}", U16_MIN, U16_MAX);
  println("");

  println("u32 ");
  println("Min: {u32}\nMax: {u32}", U32_MIN, U32_MAX);
  println("");

  println("u64 ");
  println("Min: {u64}\nMax: {u64}", U64_MIN, U64_MAX);
  println("");
}

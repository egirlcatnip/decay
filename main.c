#include "decay.h"

int main() {
  i32 num = 256;
  f32 pi = 3.1415f;
  f64 e = 2.718281828459045;
  f128 phi = 1.618033988749895;
  char *world = "World!";

  // Basic string printing
  print("{s}", "Hello ");
  println("{s}", world);
  println("");

  // Mixed types
  println("Mixed:      \n\
          i32: {i32}   \n\
          u8: {u8}     \n\
          f64: {f64.5} \n\
          String: {s}  \n\
",
          -num, (u8)num - 128, pi, "Decay");
  println("");

  // Floating-point printing
  println("f32 default:        \t{f32}", 1.0);
  println("f32 default:        \t{f32}", pi);
  println("f32 precision(3):   \t{f32.3}", pi);
  println("f64 default:        \t{f64}", e);
  println("f64 precision(6):   \t{f64.6}", e);
  println("f128 default:       \t{f128}", phi);
  println("f128 precision(9):  \t{f128.9}", phi);
  println("");

  // Pointer printing
  void *ptr = &num;
  println("*ptr lower:          \t{p}", ptr);
  println("*ptr upper:          \t{P}", ptr);
  println("");

  // Hexadecimal printing
  println("x8:  (u8)123");
  println("{x8}", (u8)123);
  println("X16: (u16)12345");
  println("{X16}", (u16)12345);
  println("x32: (u32)1234567");
  println("{x32}", (u32)1234567);
  println("X64: (u64)1234567890");
  println("{X64}", (u64)1234567890);
  println("");

  // Binary printing
  println("b8:  (u8)123");
  println("{b8}", (u8)123);
  println("b16: (u16)12345");
  println("{b16}", (u16)12345);
  println("b32: (u32)1234567");
  println("{b32}", (u32)1234567);
  println("b64: (u64)1234567890");
  println("{b64}", (u64)1234567890);
  println("");

  // Edge cases
  println("Empty string:     \t\"{s}\"", "");
  println("Null string:      \t\"{s}\"", NULL);
  println("Braces:           \t{{i32}} {i32}", 42);
  println("Braces:           \t{{");
  println("Braces:           \t}}");

  println("Malformed:        \t{i32", 42);
  println("Malformed:        \ti32}", 42);
  println("");

  // Type limits
  println("Type Limits:");
  print("i8: ");
  println("<{i8}, {i8}>", I8_MIN, I8_MAX);
  print("i16: ");
  println("<{i16}, {i16}>", I16_MIN, I16_MAX);
  print("i32: ");
  println("<{i32}, {i32}>", I32_MIN, I32_MAX);
  print("i64: ");
  println("<{i64}, {i64}>", I64_MIN, I64_MAX);
  println("");

  print("u8: ");
  println("<{u8}, {u8}>", U8_MIN, U8_MAX);
  print("u16 ");
  println("<{u16}, {u16}>", U16_MIN, U16_MAX);
  print("u32 ");
  println("<{u32}, {u32}>", U32_MIN, U32_MAX);
  print("u64 ");
  println("<{u64}, {u64}>", U64_MIN, U64_MAX);

  return 0;
}
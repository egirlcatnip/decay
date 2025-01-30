#include "decay.h"
#define limit_print false

void print_limits();

int main() {
  i32 num = 256;
  f32 pi = 3.1415f;
  f64 e = 2.718281828459045;
  f128 phi = 1.618033988749895;

  // Basic string printing
  print("{s}", "Hello ");
  print("{s}", "world");
  println("{c}", "!?."[0]);
  println("");

  // Mixed types
  println("Mixed:      \n\
          i32: {i32}   \n\
          u8: {u8}     \n\
          f64: {f64.5} \n\
          String: {s}  \n\
          Char: {c}  \n\
          Char: {c}  \n\
",
          -num, (u8)num - 128, pi, "Decay", 'Z', (u16)64);
  println("");

  // Floating-point printing
  println("f32 default:        \t{f32}", 1.0);
  println("");
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
  println("Invalid:          \t{i32.0}", 1);
  println("Invalid:          \t{b32.0}", 2);
  println("Invalid:          \t{p32.0}", 3);
  println("Invalid:          \t{x32.0}", 4);
  println("Invalid:          \t{f.-1}", 5);
  println("Invalid:          \t{invalid}", 6);
  println("");

  if (limit_print) {
    print_limits();
  }

  println("End.");
  return 0;
}

void print_limits() {

  // Type limits
  println("Type Limits:");
  println("i8: ");
  println("Min: {i8}\nMax: {i8}", I8_MIN, I8_MAX);
  println("");

  println("i16: ");
  println("Min: {i16}\nMax: {i16}", I16_MIN, I16_MAX);
  println("");

  println("i32: ");
  println("Min: {i32}\nMax: {i32}", I32_MIN, I32_MAX);
  println("");

  println("i64: ");
  println("Min: {i64}\nMax: {i64}", I64_MIN, I64_MAX);
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

  print("f32: ");
  println("Min: {f32}\nMax: {f32}>", F32_MIN, F32_MAX);
  println("");

  println("f64: ");
  println("Min: {f64}\nMax: {f64}>", F64_MIN, F64_MAX);
  println("");

  println("f128: ");
  println("Min: {f128}\nMax:{f128}>", F128_MIN, F128_MAX);

  println("");
}

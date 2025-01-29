#include "decay.h"
#include <stdio.h>

int main() {
  i32 sixty_nine = 69;
  i32 large = 987654321;
  i32 max_i32 = I32_MAX;
  i32 min_i32 = I32_MIN;
  u32 max_u32 = U32_MAX;

  f32 pi = 3.1415f;
  f64 e = 2.718281828459045;
  char *world = "World!";

  // Basic string printing
  print("{s}", "Hello ");
  println("{s}", world);
  println("");

  // Mixed types
  println("Mixed types:  \n\
            i32: {i32}   \n\
            f64: {f64.5} \n\
            String: {s}  \n\
            u32: {u32}   \n\
",
          -42, 3.1415926535, "Decay", 4294967295U);
  println("");

  // Integer printing
  println("+69:              \t{i32}", sixty_nine);
  println("-69:              \t{i32}", -sixty_nine);
  println("");

  // Floating-point printing
  println("f32 default:      \t{f32}", pi);
  println("f32 precision(3): \t{f32.3}", pi);
  println("f64 default:      \t{f64}", e);
  println("f64 precision(9): \t{f64.9}", e);
  println("");

  // Pointer printing
  void *ptr = &sixty_nine;
  println("Pointer lower:    \t{p}", ptr);
  println("Pointer upper:    \t{P}", ptr);
  printf("System printf:    \t%p\n", ptr);
  println("");

  // Hexadecimal printing
  println("x32:              \t{x32}", large);
  println("X64:              \t{X64}", (u64)large);
  printf("System hex:       \t%x\n", large);
  println("");

  // Binary printing
  println("b32:              \t{b32}", large);
  println("b64:              \t{b64}", (u64)large);
  println("");

  // Edge cases
  println("Edge cases:");
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
  println("");

  println("Testing succesfull.");
  return 0;
}
#include "../decay.h"

int main() {
  i32 num = (i32)666;
  u8 byte = (u8)111;
  f32 pi = (f32)3.1415;
  f64 e = (f64)2.718281828459045;
  f128 phi = (f128)1.618033988749895;

  // Basic string printing
  print("{s}", "Hello ");
  print("{s}", "World");
  println("{c}", "!?."[0]);
  println("");

  println("V temnote šepká šťastná čarodejnica. 🪄");
  println("W cieniu lasu skrywa się duch z przeszłości. 👻");
  println("Мрак скрывает древнюю магию в заброшенном замке. 🏰");
  println("");

  // Mixed types
  println("Mixed:      \n\
          i32: {i32}   \n\
          u8: {u8}     \n\
          f64: {f64.5} \n\
          String: {s}  \n\
          Char: {c}    \n\
          Char: {c}    \n\
",
          num, num, pi, "Decay", 'Z', (u16)64);
  println("");

  println("f32 precision(_):   \t{f32}", pi);
  println("f64 precision(6):   \t{f64.6}", e);
  println("f128 precision(9):  \t{f128.9}", phi);
  println("");

  println("i32 binary:          \t{i32:b}", num);
  println("i8 binary with _:   \t{i8:B}", byte);
  println("i16 lowercase hex:   \t{i16:x}", byte);
  println("i16 uppercase hex:   \t{i16:x}", byte);
  println("f32 binary:          \t{f32:b}", pi);
  println("");

  void *ptr = &num;
  println("*ptr lower:          \t{p}", ptr);
  println("*ptr upper:          \t{P}", ptr);
  printf("*ptr system:          \t%p \n", ptr);
  println("");

  return 0;
}

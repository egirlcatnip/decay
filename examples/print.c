#include "../decay.h"

void print_limits();
void print_edge_cases();
void print_hex();
void print_bin();
void print_unicode();

int main() {
  i32 num = (i32)256;
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
  println("i32 binary with _:   \t{i32:B}", num);
  println("i32 lowercase hex:   \t{i32:x}", num);
  println("i32 uppercase hex:   \t{i32:x}", num);
  println("f32 binary:          \t{f32:b}", pi);

  void *ptr = &num;
  println("*ptr lower:          \t{p}", ptr);
  println("*ptr upper:          \t{P}", ptr);
  printf("*ptr system:          \t%p \n", ptr);
  println("");

  print_edge_cases();

  return 0;
}

#include "decay.h"

typedef struct {
  int x;
  int y;
} Point2D;

int main() {
  println("Hello {s}, you are {f} years old.", "User", 30.5627);
  println("Decimal: {d}", 255);
  println("Hexadecimal: {x}", 255);
  println("Binary: {b}", 255);
  println("String: {s}", "Hello, World!");
  println("Character: {c}", 'A');
  println("NULL string: {s}", NULL);
  int x = 10;
  println("Pointer to number {d}: {p}", x, &x);
  println("Point: {{x: {d}, y: {d}}}", 10, 20);
  println("{}");
  println("{");
  println("{{");
  println("}");
  println("}}");

  println("");

  println("Countdown: {5} {4} {3} {2} {1} {0} Boom!", "Zero", "One", "Two",
          "Three", "Four", "Five");
  return 0;
}

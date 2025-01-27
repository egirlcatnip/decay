#ifndef DECAY_H
#define DECAY_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void println(const char *format, ...);

void print_string(const char *str);
void print_int(int value);
void print_float(double value, int precision);
void print_hex(int value);
void print_binary(int value);
void print_pointer(void *ptr);

inline void println(const char *format, ...) {
  va_list args;
  va_start(args, format);

  const char *p = format;
  while (*p) {
    if (*p == '{') {
      const char *start = p;
      p++;
      if (*p == '{') {
        putchar('{');
        p++;
      } else {
        const char *end = strchr(p, '}');
        if (end) {
          if (*(end - 1) >= '0' && *(end - 1) <= '9') {
            int index = *(end - 1) - '0';
            va_list args_copy;
            va_copy(args_copy, args);

            for (int i = 0; i < index; i++) {
              va_arg(args_copy, void *);
            }
            char *str = va_arg(args_copy, char *);

            print_string(str);
            va_end(args_copy);
            p = end + 1;
          } else {
            char specifier = *(end - 1);
            if (specifier == 's' || specifier == 'd' || specifier == 'x' ||
                specifier == 'b' || specifier == 'c' || specifier == 'p' ||
                specifier == 'f') {
              // Handle valid format specifier
              switch (specifier) {
              case 's': {
                char *str = va_arg(args, char *);
                print_string(str);
                break;
              }
              case 'd': {
                int num = va_arg(args, int);
                print_int(num);
                break;
              }
              case 'x': {
                int num = va_arg(args, int);
                print_hex(num);
                break;
              }
              case 'b': {
                int num = va_arg(args, int);
                print_binary(num);
                break;
              }
              case 'c': {
                int ch = va_arg(args, int);
                putchar(ch);
                break;
              }
              case 'p': {
                void *ptr = va_arg(args, void *);
                print_pointer(ptr);
                break;
              }
              case 'f': {
                break;
              }
              }
              p = end + 1;
            } else {
              // Print as normal literal
              while (start <= end) {
                putchar(*start++);
              }
              p = end + 1;
            }
          }
        } else {
          putchar('{');
        }
      }
    } else if (*p == '}') {
      if (*(p + 1) == '}') {
        putchar('}');
        p += 2;
      } else {
        putchar('}');
        p++;
      }
    } else {
      putchar(*p++);
    }
  }

  va_end(args);
  putchar('\n');
}

inline void print_string(const char *str) {
  if (str) {
    printf("%s", str);
  } else {
    printf("(null)");
  }
}

inline void print_int(int value) { printf("%d", value); }

inline void print_float(double value, int precision) {
  printf("%.*f", precision, value);
}

inline void print_hex(int value) { printf("0x%x", value); }

inline void print_binary(int value) {
  printf("0b");
  while (value) {
    putchar((value & 1) + '0');
    value >>= 1;
  }
}

inline void print_pointer(void *ptr) { printf("%p", ptr); }

#endif // DECAY_H

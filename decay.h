#ifndef DECAY_H
#define DECAY_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Types */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;
typedef long double f128;

typedef uintptr_t usize;
typedef intptr_t isize;

/* Integer limits */
#define I8_MAX INT8_MAX
#define I16_MAX INT16_MAX
#define I32_MAX INT32_MAX
#define I64_MAX INT64_MAX

#define I8_MIN INT8_MIN
#define I16_MIN INT16_MIN
#define I32_MIN INT32_MIN
#define I64_MIN INT64_MIN

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX

#define U8_MIN 0
#define U16_MIN 0
#define U32_MIN 0
#define U64_MIN 0

/*Format Specifiers*/

typedef enum {
  FMT_INVALID,
  FMT_I32,
  FMT_U32,
  FMT_I64,
  FMT_U64,
  FMT_I16,
  FMT_U16,
  FMT_I8,
  FMT_U8,
  FMT_F32,
  FMT_F64,
  FMT_F128,
  FMT_CHAR,
  FMT_STR,
  FMT_PTR,
  FMT_PTR_UPPER
} FormatType;

typedef struct {
  FormatType type;
  char alt_fmt;  // 'x', 'X', 'b' or 'B'
  i32 precision; // -1 means unspecified
  bool is_valid;
} FormatSpec;

/*Format Parsing Utilities*/

static FormatType parse_type(const char *str) {
  static const struct {
    const char *name;
    FormatType type;
  } types[] = {{"i32", FMT_I32}, {"u32", FMT_U32},   {"i64", FMT_I64},
               {"u64", FMT_U64}, {"i16", FMT_I16},   {"u16", FMT_U16},
               {"i8", FMT_I8},   {"u8", FMT_U8},     {"f32", FMT_F32},
               {"f64", FMT_F64}, {"f128", FMT_F128}, {"c", FMT_CHAR},
               {"s", FMT_STR},   {"p", FMT_PTR},     {"P", FMT_PTR_UPPER}};

  for (usize i = 0; i < sizeof(types) / sizeof(*types); i++) {
    if (strcmp(str, types[i].name) == 0) {
      return types[i].type;
    }
  }
  return FMT_INVALID;
}

static FormatSpec parse_specifier(const char **fmt) {
  FormatSpec spec = {FMT_INVALID, 0, -1, false};
  char type_buf[16] = {0};
  const char *p = *fmt;
  const char *spec_start = p;
  usize tlen = 0;

  while (p[0] && p[0] != '}')
    p++;
  isize spec_len = (*p == '}') ? (isize)(p - *fmt + 1) : (isize)(p - *fmt);

  p = *fmt + 1; // Skip initial '{'

  // Parse type name
  while (p[0] && p[0] != ':' && p[0] != '.' && p[0] != '}') {
    if (tlen < sizeof(type_buf) - 1)
      type_buf[tlen++] = *p;
    p++;
  }
  type_buf[tlen] = '\0';
  spec.type = parse_type(type_buf);
  spec.is_valid = (spec.type != FMT_INVALID);

  // Parse optional components
  while (spec.is_valid && p[0] && p[0] != '}') {
    if (*p == '.') {
      p++;
      char *endptr;
      spec.precision = strtol(p, &endptr, 10);
      p = endptr;

      if (spec.precision < 0) {
        spec.is_valid = false;
      }
    } else if (p[0] == ':') {
      p++;
      spec.alt_fmt = *p++;
    } else {
      spec.is_valid = false;
      break;
    }
  }

  // Validate closing brace
  if (p[0] != '}') {
    spec.is_valid = false;
  } else {
    p++;
  }

  // Type-specific validation
  if (spec.is_valid) {
    switch (spec.type) {
    case FMT_F32:
    case FMT_F64:
    case FMT_F128:
      if ((spec.alt_fmt && spec.alt_fmt != 'b' && spec.alt_fmt != 'B') ||
          (spec.precision < -1)) {
        spec.is_valid = false;
      }
      break;
    case FMT_CHAR:
    case FMT_STR:
      if (spec.alt_fmt || spec.precision != -1) {
        spec.is_valid = false;
      }
      break;
    case FMT_PTR:
    case FMT_PTR_UPPER:
      if (spec.precision != -1) {
        spec.is_valid = false;
      }
      break;
    default: // Integer types
      if (spec.precision != -1 ||
          (spec.alt_fmt && spec.alt_fmt != 'x' && spec.alt_fmt != 'X' &&
           spec.alt_fmt != 'b' && spec.alt_fmt != 'B')) {
        spec.is_valid = false;
      }
      break;
    }
  }

  // Always advance to end of specifier
  *fmt = spec_start + spec_len;
  return spec;
}

/*Binary Representation Helpers*/

static void print_bits(FILE *f, uint64_t value, size_t nbits, bool group) {
  char buf[128] = {0};
  size_t idx = 0;

  for (i32 i = (i32)nbits - 1; i >= 0; i--) {
    buf[idx++] = (value & (1ULL << i)) ? '1' : '0';
    if (group && i > 0 && (i % 4) == 0)
      buf[idx++] = '_';
  }
  fputs(buf, f);
}

static void print_float_bits(FILE *f, f32 value, bool group) {
  union {
    f32 f;
    u32 i;
  } u = {value};
  print_bits(f, u.i, 32, group);
}

static void print_double_bits(FILE *f, f64 value, bool group) {
  union {
    f64 f;
    u64 i;
  } u = {value};
  print_bits(f, u.i, 64, group);
}

static void print_quad_bits(FILE *f, f128 value, bool group) {
  u8 bytes[sizeof(f128)];
  memcpy(bytes, &value, sizeof(f128));
  char buf[256] = {0};
  size_t idx = 0;

  for (i32 i = sizeof(f128) - 1; i >= 0; i--) {
    for (i32 b = 7; b >= 0; b--) {
      buf[idx++] = (bytes[i] & (1 << b)) ? '1' : '0';
      if (group && (i * 8 + b) % 4 == 0 && (i * 8 + b) != 0) {
        buf[idx++] = '_';
      }
    }
  }
  fputs(buf, f);
}

/*Format Processing Core*/

static void process_specifier(FILE *f, FormatSpec spec, va_list *args) {
  if (!spec.is_valid)
    return;

  switch (spec.type) {
  case FMT_I32: {
    i32 val = va_arg(*args, i32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%08x", (u32)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%08X", (u32)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u32)val, 32, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u32)val, 32, true);
    else
      fprintf(f, "%d", val);
  } break;

  case FMT_U32: {
    u32 val = va_arg(*args, u32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%08x", val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%08X", val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, val, 32, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, val, 32, true);
    else
      fprintf(f, "%u", val);
  } break;

  case FMT_I64:
  case FMT_PTR: {
    i64 val = va_arg(*args, i64);
    if (spec.alt_fmt == 'x' || spec.type == FMT_PTR)
      fprintf(f, "%016llx", (u64)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%016llX", (u64)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u64)val, 64, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u64)val, 64, true);
    else
      fprintf(f, "%lld", val);
  } break;

  case FMT_U64:
  case FMT_PTR_UPPER: {
    u64 val = va_arg(*args, u64);
    if (spec.alt_fmt == 'x' || spec.type == FMT_PTR_UPPER)
      fprintf(f, "%016llX", val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%016llX", val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, val, 64, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, val, 64, true);
    else
      fprintf(f, "%llu", val);
  } break;

  case FMT_I16: {
    i32 val = va_arg(*args, i32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%04x", (u16)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%04X", (u16)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u16)val, 16, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u16)val, 16, true);
    else
      fprintf(f, "%hd", (i16)val);
  } break;

  case FMT_U16: {
    u32 val = va_arg(*args, u32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%04x", (u16)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%04X", (u16)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u16)val, 16, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u16)val, 16, true);
    else
      fprintf(f, "%hu", (u16)val);
  } break;

  case FMT_I8: {
    i32 val = va_arg(*args, i32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%02x", (u8)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%02X", (u8)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u8)val, 8, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u8)val, 8, true);
    else
      fprintf(f, "%hhd", (i8)val);
  } break;

  case FMT_U8: {
    u32 val = va_arg(*args, u32);
    if (spec.alt_fmt == 'x')
      fprintf(f, "%02x", (u8)val);
    else if (spec.alt_fmt == 'X')
      fprintf(f, "%02X", (u8)val);
    else if (spec.alt_fmt == 'b')
      print_bits(f, (u8)val, 8, false);
    else if (spec.alt_fmt == 'B')
      print_bits(f, (u8)val, 8, true);
    else
      fprintf(f, "%hhu", (u8)val);
  } break;

  case FMT_F32: {
    f32 val = (f32)va_arg(*args, f64);
    if (spec.alt_fmt == 'b')
      print_float_bits(f, val, false);
    else if (spec.alt_fmt == 'B')
      print_float_bits(f, val, true);
    else if (spec.precision >= 0)
      fprintf(f, "%.*f", spec.precision, val);
    else
      fprintf(f, "%f", val);
  } break;

  case FMT_F64: {
    f64 val = va_arg(*args, f64);
    if (spec.alt_fmt == 'b')
      print_double_bits(f, val, false);
    else if (spec.alt_fmt == 'B')
      print_double_bits(f, val, true);
    else if (spec.precision >= 0)
      fprintf(f, "%.*f", spec.precision, val);
    else
      fprintf(f, "%f", val);
  } break;

  case FMT_F128: {
    f128 val = va_arg(*args, f128);
    if (spec.alt_fmt == 'b')
      print_quad_bits(f, val, false);
    else if (spec.alt_fmt == 'B')
      print_quad_bits(f, val, true);
    else if (spec.precision >= 0)
      fprintf(f, "%.*Lf", spec.precision, val);
    else
      fprintf(f, "%Lf", val);
  } break;

  case FMT_CHAR: {
    i32 c = va_arg(*args, i32);
    fputc(c, f);
  } break;

  case FMT_STR: {
    const char *s = va_arg(*args, const char *);
    fputs(s ? s : "(null)", f);
  } break;

  default:
    break;
  }
}

static void format_text(FILE *f, const char *fmt, va_list args) {
  const char *p = fmt;
  while (*p) {
    if (*p == '{') {
      const char *spec_start = p;

      // Handle escaped {{
      if (p[1] == '{') {
        fputc('{', f);
        p += 2;
        continue;
      }

      FormatSpec spec = parse_specifier(&p);

      if (!spec.is_valid) {
        fwrite(spec_start, 1, (usize)(p - spec_start), f);
      } else {
        process_specifier(f, spec, &args);
      }
    } else if (*p == '}') {
      if (p[1] == '}') {
        fputc('}', f);
        p += 2;
      } else {
        fputc(*p++, f);
      }
    } else {
      fputc(*p++, f);
    }
  }
}

/*Public Interface*/

static inline void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  format_text(stdout, fmt, args);
  va_end(args);
}

static inline void println(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  format_text(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

static inline void fprint(FILE *f, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  format_text(f, fmt, args);
  va_end(args);
}

static inline void fprintln(FILE *f, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  format_text(f, fmt, args);
  fputc('\n', f);
  va_end(args);
}

#endif // DECAY_H

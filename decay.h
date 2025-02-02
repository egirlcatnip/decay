#ifndef DECAY_H
#define DECAY_H

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*========================================================================*/
/*                          Type Definitions                              */
/*========================================================================*/

/* Fixed width integer and floating point types */
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

/* Integer limits macros */
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
#define U8_MIN 0U
#define U16_MIN 0U
#define U32_MIN 0U
#define U64_MIN 0U

/*========================================================================*/
/*                       Format Specification                             */
/*========================================================================*/

/* Supported format specifiers */
typedef enum {
  SPEC_INVALID = 0,
  SPEC_I32,
  SPEC_U32,
  SPEC_I64,
  SPEC_U64,
  SPEC_I16,
  SPEC_U16,
  SPEC_I8,
  SPEC_U8,
  SPEC_F32,
  SPEC_F64,
  SPEC_F128,
  SPEC_C,
  SPEC_S,
  SPEC_P,    /* {p} is equivalent to {i64:x} */
  SPEC_CAP_P /* {P} is equivalent to {i64:X} */
} FormatType;

/* Structure for a parsed format specifier */
typedef struct {
  FormatType type;
  char alt;      /* Alternative format (e.g. 'x', 'X', 'b', 'B') */
  int precision; /* Precision for floats, -1 if not specified */
  int valid;     /* 1 if valid, 0 if invalid */
} FormatSpec;

/*========================================================================*/
/*                         Helper Prototypes                              */
/*========================================================================*/
static FormatSpec _parse_format_spec(const char **fmt_ptr);
static FormatType _get_format_type(const char *type_str);
static void _print_binary(FILE *f, unsigned long long value, usize group,
                          usize total_bits);

/* Helpers for printing float binary representations */
static void _print_float_binary_f32(FILE *f, f32 value, usize group);
static void _print_float_binary_f64(FILE *f, f64 value, usize group);
static void _print_float_binary_f128(FILE *f, f128 value, usize group);

/*========================================================================*/
/*                      Helper Implementations                            */
/*========================================================================*/

static FormatType _get_format_type(const char *type_str) {
  if (strcmp(type_str, "i32") == 0)
    return SPEC_I32;
  if (strcmp(type_str, "u32") == 0)
    return SPEC_U32;
  if (strcmp(type_str, "i64") == 0)
    return SPEC_I64;
  if (strcmp(type_str, "u64") == 0)
    return SPEC_U64;
  if (strcmp(type_str, "i16") == 0)
    return SPEC_I16;
  if (strcmp(type_str, "u16") == 0)
    return SPEC_U16;
  if (strcmp(type_str, "i8") == 0)
    return SPEC_I8;
  if (strcmp(type_str, "u8") == 0)
    return SPEC_U8;
  if (strcmp(type_str, "f32") == 0)
    return SPEC_F32;
  if (strcmp(type_str, "f64") == 0)
    return SPEC_F64;
  if (strcmp(type_str, "f128") == 0)
    return SPEC_F128;
  if (strcmp(type_str, "c") == 0)
    return SPEC_C;
  if (strcmp(type_str, "s") == 0)
    return SPEC_S;
  if (strcmp(type_str, "p") == 0)
    return SPEC_P;
  if (strcmp(type_str, "P") == 0)
    return SPEC_CAP_P;
  return SPEC_INVALID;
}

/* Parse a placeholder starting after the '{'.
 * Advances *fmt_ptr past the closing '}'.
 * Does minimal parsing; further validation is done later.
 */
static FormatSpec _parse_format_spec(const char **fmt_ptr) {
  FormatSpec spec = {SPEC_INVALID, 0, -1, 1};
  const char *p = *fmt_ptr;
  char type_buf[32] = {0};
  int t = 0;

  /* Parse type name until ':' or '.' or '}' */
  while (*p && *p != ':' && *p != '.' && *p != '}') {
    if (t < (int)sizeof(type_buf) - 1)
      type_buf[t++] = *p;
    p++;
  }
  type_buf[t] = '\0';
  spec.type = _get_format_type(type_buf);

  /* Parse optional precision or alternative format */
  if (*p == '.') {
    p++; /* skip '.' */
    char prec_buf[8] = {0};
    int pt = 0;
    /* If negative sign is encountered, mark invalid */
    if (*p == '-') {
      spec.valid = 0;
    }
    while (*p && *p != '}') {
      if (pt < (int)sizeof(prec_buf) - 1)
        prec_buf[pt++] = *p;
      p++;
    }
    prec_buf[pt] = '\0';
    spec.precision = atoi(prec_buf);
  } else if (*p == ':') {
    p++; /* skip ':' */
    spec.alt = *p;
    p++;
  }
  if (*p == '}') {
    p++; /* skip '}' */
  } else {
    /* Malformed: missing closing brace */
    spec.valid = 0;
  }
  *fmt_ptr = p;
  return spec;
}

static void _print_binary(FILE *f, unsigned long long value, usize group,
                          usize total_bits) {
  char buf[256];
  int idx = 0;
  for (isize i = (isize)(total_bits - 1); i >= 0; i--) {
    buf[idx++] = (value & (1ULL << i)) ? '1' : '0';
    if (group && i && (i % 4 == 0))
      buf[idx++] = '_';
  }
  buf[idx] = '\0';
  fputs(buf, f);
}

static void _print_float_binary_f32(FILE *f, f32 value, usize group) {
  union {
    f32 f;
    u32 i;
  } u = {.f = value};
  _print_binary(f, u.i, group, 32);
}

static void _print_float_binary_f64(FILE *f, f64 value, usize group) {
  union {
    f64 f;
    u64 i;
  } u = {.f = value};
  _print_binary(f, u.i, group, 64);
}

static void _print_float_binary_f128(FILE *f, f128 value, usize group) {
  /* For f128, use memcpy to extract bytes.
     We'll print the bits in big-endian order. */
  unsigned char bytes[sizeof(f128)];
  memcpy(bytes, &value, sizeof(f128));
  char buf[256];
  int idx = 0;
  for (int byte = sizeof(f128) - 1; byte >= 0; byte--) {
    for (int bit = 7; bit >= 0; bit--) {
      buf[idx++] = (bytes[byte] & (1 << bit)) ? '1' : '0';
      int current_bit = byte * 8 + bit;
      if (group && current_bit && (current_bit % 4 == 0))
        buf[idx++] = '_';
    }
  }
  buf[idx] = '\0';
  fputs(buf, f);
}

/*========================================================================*/
/*                    Main Format Processor Function                      */
/*========================================================================*/

static void _vprint(FILE *f, const char *fmt, va_list args) {
  const char *p = fmt;
  while (*p) {
    if (*p == '{') {
      /* Handle escaped '{{' */
      if (*(p + 1) == '{') {
        fputc('{', f);
        p += 2;
        continue;
      }
      const char *placeholder_start = p;
      p++; /* Skip '{' */
      FormatSpec spec = _parse_format_spec(&p);
      /* Validate specifier combinations */
      int invalid = 0;
      switch (spec.type) {
      case SPEC_F32:
      case SPEC_F64:
      case SPEC_F128:
        /* For floats, alt, if specified, must be 'b' or 'B' or none.
         * Precision, if specified, must be >= 0 (already ensured not negative
         * in parsing).
         */
        if (spec.alt && spec.alt != 'b' && spec.alt != 'B') {
          invalid = 1;
        }
        break;
      case SPEC_C:
      case SPEC_S:
        /* char and string must not have alt or precision */
        if (spec.alt || spec.precision != -1)
          invalid = 1;
        break;
      case SPEC_I32:
      case SPEC_U32:
      case SPEC_I64:
      case SPEC_U64:
      case SPEC_I16:
      case SPEC_U16:
      case SPEC_I8:
      case SPEC_U8:
      case SPEC_P:
      case SPEC_CAP_P:
        /* Integer types must not have precision */
        if (spec.precision != -1)
          invalid = 1;
        break;
      default:
        invalid = 1;
        break;
      }
      if (!spec.valid)
        invalid = 1;
      if (invalid) {
        /* Invalid specifier: print the placeholder literally */
        usize len = (usize)(p - placeholder_start);
        fwrite(placeholder_start, 1, len, f);
        continue;
      }

      /* Process valid specifiers */
      switch (spec.type) {
      case SPEC_I32: {
        i32 value = va_arg(args, i32);
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[32];
          int width = 8;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, (u32)value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, (u32)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u32)value, (spec.alt == 'B'), 32);
        } else {
          char buf[32];
          snprintf(buf, sizeof(buf), "%" PRId32, value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_U32: {
        u32 value = va_arg(args, u32);
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[32];
          int width = 8;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, value, (spec.alt == 'B'), 32);
        } else {
          char buf[32];
          snprintf(buf, sizeof(buf), "%" PRIu32, value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_I64:
      case SPEC_P: { /* {p} acts as {i64:x} */
        i64 value = va_arg(args, i64);
        if (spec.alt == 'x' || spec.alt == 'X' || spec.type == SPEC_P) {
          char buf[40];
          int width = 16;
          if (spec.alt == 'X' || spec.type == SPEC_CAP_P)
            snprintf(buf, sizeof(buf), "%0*llX", width,
                     (unsigned long long)(u64)value);
          else
            snprintf(buf, sizeof(buf), "%0*llx", width,
                     (unsigned long long)(u64)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u64)value, (spec.alt == 'B'), 64);
        } else {
          char buf[40];
          snprintf(buf, sizeof(buf), "%" PRId64, value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_U64:
      case SPEC_CAP_P: { /* {P} acts as {i64:X} */
        u64 value = va_arg(args, u64);
        if (spec.alt == 'x' || spec.alt == 'X' || spec.type == SPEC_CAP_P) {
          char buf[40];
          int width = 16;
          if (spec.alt == 'X' || spec.type == SPEC_CAP_P)
            snprintf(buf, sizeof(buf), "%0*llX", width,
                     (unsigned long long)value);
          else
            snprintf(buf, sizeof(buf), "%0*llx", width,
                     (unsigned long long)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, value, (spec.alt == 'B'), 64);
        } else {
          char buf[40];
          snprintf(buf, sizeof(buf), "%" PRIu64, value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_I16: {
        i32 value = va_arg(args, int); /* promoted */
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[24];
          int width = 4;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, (u16)value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, (u16)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u16)value, (spec.alt == 'B'), 16);
        } else {
          char buf[24];
          snprintf(buf, sizeof(buf), "%" PRId16, (i16)value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_U16: {
        i32 value = va_arg(args, int);
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[24];
          int width = 4;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, (u16)value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, (u16)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u16)value, (spec.alt == 'B'), 16);
        } else {
          char buf[24];
          snprintf(buf, sizeof(buf), "%" PRIu16, (u16)value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_I8: {
        int value = va_arg(args, int);
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[16];
          int width = 2;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, (u8)value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, (u8)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u8)value, (spec.alt == 'B'), 8);
        } else {
          char buf[16];
          snprintf(buf, sizeof(buf), "%" PRId8, (i8)value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_U8: {
        int value = va_arg(args, int);
        if (spec.alt == 'x' || spec.alt == 'X') {
          char buf[16];
          int width = 2;
          if (spec.alt == 'X')
            snprintf(buf, sizeof(buf), "%0*X", width, (u8)value);
          else
            snprintf(buf, sizeof(buf), "%0*x", width, (u8)value);
          fputs(buf, f);
        } else if (spec.alt == 'b' || spec.alt == 'B') {
          _print_binary(f, (u8)value, (spec.alt == 'B'), 8);
        } else {
          char buf[16];
          snprintf(buf, sizeof(buf), "%" PRIu8, (u8)value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_F32: {
        f32 value = (f32)va_arg(args, double); /* promoted */
        if (spec.alt == 'b' || spec.alt == 'B') {
          _print_float_binary_f32(f, value, (spec.alt == 'B'));
        } else {
          char buf[64];
          if (spec.precision >= 0)
            snprintf(buf, sizeof(buf), "%.*f", spec.precision, value);
          else
            snprintf(buf, sizeof(buf), "%f", value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_F64: {
        f64 value = va_arg(args, f64);
        if (spec.alt == 'b' || spec.alt == 'B') {
          _print_float_binary_f64(f, value, (spec.alt == 'B'));
        } else {
          char buf[64];
          if (spec.precision >= 0)
            snprintf(buf, sizeof(buf), "%.*f", spec.precision, value);
          else
            snprintf(buf, sizeof(buf), "%f", value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_F128: {
        f128 value = va_arg(args, f128);
        if (spec.alt == 'b' || spec.alt == 'B') {
          _print_float_binary_f128(f, value, (spec.alt == 'B'));
        } else {
          char buf[80];
          if (spec.precision >= 0)
            snprintf(buf, sizeof(buf), "%.*Lf", spec.precision, value);
          else
            snprintf(buf, sizeof(buf), "%Lf", value);
          fputs(buf, f);
        }
        break;
      }
      case SPEC_C: {
        int ch = va_arg(args, int);
        fputc(ch, f);
        break;
      }
      case SPEC_S: {
        const char *str = va_arg(args, const char *);
        fputs(str ? str : "(null)", f);
        break;
      }
      default: {
        /* Should not happen; fallback to printing literal */
        usize len = (usize)(p - placeholder_start);
        fwrite(placeholder_start, 1, len, f);
        break;
      }
      }
    } else if (*p == '}' && *(p + 1) == '}') {
      /* Handle escaped '}}' */
      fputc('}', f);
      p += 2;
    } else {
      fputc(*p, f);
      p++;
    }
  }
}

/*========================================================================*/
/*                            Public API                                  */
/*========================================================================*/

/* Print to stdout */
static inline void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _vprint(stdout, fmt, args);
  va_end(args);
}

/* Print to stdout with a newline appended */
static inline void println(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _vprint(stdout, fmt, args);
  va_end(args);
  fputc('\n', stdout);
}

/* Print to a specified FILE stream */
static inline void fprint(FILE *f, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _vprint(f, fmt, args);
  va_end(args);
}

/* Print to a specified FILE stream with a newline appended */
static inline void fprintln(FILE *f, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _vprint(f, fmt, args);
  va_end(args);
  fputc('\n', f);
}

#endif // DECAY_H

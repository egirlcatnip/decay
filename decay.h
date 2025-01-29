#ifndef DECAY_H
#define DECAY_H

/*==============================================================================
  Type System
==============================================================================*/
#include <float.h>
#include <inttypes.h>
#include <stdint.h>

// Fixed-width integer types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating-point types
typedef float f32;
typedef double f64;
typedef long double f128;

// Architecture-dependent size types
typedef unsigned long usize;
typedef signed long isize;

// Integer limits
#define I8_MAX 127
#define I16_MAX 32767
#define I32_MAX 2147483647
#define I64_MAX 9223372036854775807LL
#define I8_MIN (-I8_MAX - 1)
#define I16_MIN (-I16_MAX - 1)
#define I32_MIN (-I32_MAX - 1)
#define I64_MIN (-I64_MAX - 1)

#define U8_MAX 255U
#define U16_MAX 65535U
#define U32_MAX 4294967295UL
#define U64_MAX 18446744073709551615ULL

#define U8_MIN 0U
#define U16_MIN 0U
#define U32_MIN 0UL
#define U64_MIN 0ULL

// Floating-point limits
#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX
#define F128_MAX LDBL_MAX
#define F32_MIN FLT_MIN
#define F64_MIN DBL_MIN
#define F128_MIN LDBL_MIN

/*==============================================================================
  Print System
==============================================================================*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define DECAY_BUFFER_SIZE 4096
#define DECAY_MAX_SPEC_LEN 32

typedef enum {
  FMT_INVALID,
  FMT_I8,
  FMT_I16,
  FMT_I32,
  FMT_I64,
  FMT_U8,
  FMT_U16,
  FMT_U32,
  FMT_U64,
  FMT_F32,
  FMT_F64,
  FMT_F128,
  FMT_STRING,
  FMT_PTR_LOWER,
  FMT_PTR_UPPER,
  FMT_HEX32_LOWER,
  FMT_HEX32_UPPER,
  FMT_HEX64_LOWER,
  FMT_HEX64_UPPER,
  FMT_BIN32,
  FMT_BIN64
} FormatType;

typedef struct {
  FormatType type;
  int precision;
} FormatSpec;

static FormatSpec decay_parse_spec(const char *spec) {
  FormatSpec result = {FMT_INVALID, -1};
  const char *p = spec;

  switch (*p++) {
  case 'i':
    if (strncmp(p, "8", 1) == 0) {
      result.type = FMT_I8;
      p += 1;
    } else if (strncmp(p, "16", 2) == 0) {
      result.type = FMT_I16;
      p += 2;
    } else if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_I32;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_I64;
      p += 2;
    }
    break;

  case 'u':
    if (strncmp(p, "8", 1) == 0) {
      result.type = FMT_U8;
      p += 1;
    } else if (strncmp(p, "16", 2) == 0) {
      result.type = FMT_U16;
      p += 2;
    } else if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_U32;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_U64;
      p += 2;
    }
    break;

  case 'f':
    if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_F32;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_F64;
      p += 2;
    } else if (strncmp(p, "128", 3) == 0) {
      result.type = FMT_F128;
      p += 3;
    }
    break;

  case 's':
    result.type = FMT_STRING;
    break;
  case 'p':
    result.type = FMT_PTR_LOWER;
    break;
  case 'P':
    result.type = FMT_PTR_UPPER;
    break;
  case 'x':
    if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_HEX32_LOWER;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_HEX64_LOWER;
      p += 2;
    } else {
      result.type = FMT_HEX32_LOWER;
    }
    break;
  case 'X':
    if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_HEX32_UPPER;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_HEX64_UPPER;
      p += 2;
    } else {
      result.type = FMT_HEX32_UPPER;
    }
    break;
  case 'b':
    if (strncmp(p, "32", 2) == 0) {
      result.type = FMT_BIN32;
      p += 2;
    } else if (strncmp(p, "64", 2) == 0) {
      result.type = FMT_BIN64;
      p += 2;
    } else {
      result.type = FMT_BIN32;
    }
    break;
  default:
    return result;
  }

  if (*p == '.') {
    result.precision = 0;
    while (*++p >= '0' && *p <= '9') {
      result.precision = result.precision * 10 + (*p - '0');
    }
  }

  // Check if we consumed entire specifier
  return *p ? (FormatSpec){FMT_INVALID, -1} : result;
}

static void decay_format_value(char **buf, size_t *rem, FormatSpec spec,
                               va_list *args) {
  char num[256];
  size_t len = 0;
  uintptr_t ptr_val;
  unsigned long long ull_val;

  switch (spec.type) {
  // Signed integers
  case FMT_I8:
    len = snprintf(num, sizeof(num), "%hhd", va_arg(*args, int));
    break;
  case FMT_I16:
    len = snprintf(num, sizeof(num), "%hd", va_arg(*args, int));
    break;
  case FMT_I32:
    len = snprintf(num, sizeof(num), "%d", va_arg(*args, int));
    break;
  case FMT_I64:
    len = snprintf(num, sizeof(num), "%lld", va_arg(*args, long long));
    break;

  // Unsigned integers
  case FMT_U8:
    len = snprintf(num, sizeof(num), "%hhu", va_arg(*args, unsigned));
    break;
  case FMT_U16:
    len = snprintf(num, sizeof(num), "%hu", va_arg(*args, unsigned));
    break;
  case FMT_U32:
    len = snprintf(num, sizeof(num), "%u", va_arg(*args, unsigned));
    break;
  case FMT_U64:
    len = snprintf(num, sizeof(num), "%llu", va_arg(*args, unsigned long long));
    break;

  // Floating-point
  case FMT_F32:
    len = snprintf(num, sizeof(num), "%.*f",
                   spec.precision >= 0 ? spec.precision : 6,
                   va_arg(*args, double));
    break;
  case FMT_F64:
    len = snprintf(num, sizeof(num), "%.*f",
                   spec.precision >= 0 ? spec.precision : 15,
                   va_arg(*args, double));
    break;
  case FMT_F128:
    len = snprintf(num, sizeof(num), "%.*Lf",
                   spec.precision >= 0 ? spec.precision : 18,
                   va_arg(*args, long double));
    break;

  // Strings and pointers
  case FMT_STRING: {
    const char *s = va_arg(*args, const char *);
    len = snprintf(num, sizeof(num), "%s", s ? s : "(null)");
    break;
  }
  case FMT_PTR_LOWER:
    ptr_val = (uintptr_t)va_arg(*args, void *);
    len = snprintf(num, sizeof(num), "%" PRIxPTR, ptr_val);
    break;
  case FMT_PTR_UPPER:
    ptr_val = (uintptr_t)va_arg(*args, void *);
    len = snprintf(num, sizeof(num), "%" PRIXPTR, ptr_val);
    break;

  // Hexadecimal
  case FMT_HEX32_LOWER:
    len = snprintf(num, sizeof(num), "%08x", va_arg(*args, unsigned));
    break;
  case FMT_HEX32_UPPER:
    len = snprintf(num, sizeof(num), "%08X", va_arg(*args, unsigned));
    break;
  case FMT_HEX64_LOWER:
    ull_val = va_arg(*args, unsigned long long);
    len = snprintf(num, sizeof(num), "%llx", ull_val);
    break;
  case FMT_HEX64_UPPER:
    ull_val = va_arg(*args, unsigned long long);
    len = snprintf(num, sizeof(num), "%llX", ull_val);
    break;

  // Binary
  case FMT_BIN32: {
    unsigned val = va_arg(*args, unsigned);
    char *p = num;
    for (int i = 31; i >= 0; i--) {
      *p++ = (val >> i) & 1 ? '1' : '0';
      if (i > 0 && i % 4 == 0)
        *p++ = '_';
    }
    *p = '\0';
    len = p - num;
    break;
  }
  case FMT_BIN64: {
    unsigned long long val = va_arg(*args, unsigned long long);
    char *p = num;
    for (int i = 63; i >= 0; i--) {
      *p++ = (val >> i) & 1 ? '1' : '0';
      if (i > 0 && i % 4 == 0)
        *p++ = '_';
    }
    *p = '\0';
    len = p - num;
    break;
  }
  default:
    len = snprintf(num, sizeof(num), "[INVALID]");
    break;
  }

  if (len < *rem) {
    memcpy(*buf, num, len);
    *buf += len;
    *rem -= len;
  }
}

static void decay_vprint(FILE *stream, const char *fmt, va_list args) {
  char buffer[DECAY_BUFFER_SIZE];
  char *buf_ptr = buffer;
  size_t remaining = sizeof(buffer);
  const char *start = fmt;
  va_list args_copy;
  va_copy(args_copy, args);

  while (*fmt) {
    if (*fmt == '{') {
      // Write preceding content
      size_t seg_len = fmt - start;
      if (seg_len > 0) {
        if (seg_len > remaining)
          seg_len = remaining;
        memcpy(buf_ptr, start, seg_len);
        buf_ptr += seg_len;
        remaining -= seg_len;
      }

      const char *spec_start = ++fmt;
      while (*fmt && *fmt != '}')
        fmt++;

      if (*fmt != '}') {
        memcpy(buf_ptr, "{", 1);
        buf_ptr += 1;
        remaining -= 1;
        start = spec_start;
        continue;
      }

      // Extract specifier
      char spec_buf[DECAY_MAX_SPEC_LEN + 1];
      size_t spec_len = fmt - spec_start;
      if (spec_len > DECAY_MAX_SPEC_LEN)
        spec_len = DECAY_MAX_SPEC_LEN;
      memcpy(spec_buf, spec_start, spec_len);
      spec_buf[spec_len] = '\0';

      // Parse specifier
      FormatSpec spec = decay_parse_spec(spec_buf);
      if (spec.type == FMT_INVALID) {
        // Invalid specifier - print original text
        memcpy(buf_ptr, "{", 1);
        buf_ptr += 1;
        remaining -= 1;
        if (spec_len > remaining)
          spec_len = remaining;
        memcpy(buf_ptr, spec_buf, spec_len);
        buf_ptr += spec_len;
        remaining -= spec_len;
        memcpy(buf_ptr, "}", 1);
        buf_ptr += 1;
        remaining -= 1;
      } else {
        // Valid specifier - format value
        decay_format_value(&buf_ptr, &remaining, spec, &args_copy);
      }

      start = ++fmt;
    } else {
      fmt++;
    }
  }

  // Write remaining content
  size_t final_len = fmt - start;
  if (final_len > 0) {
    if (final_len > remaining)
      final_len = remaining;
    memcpy(buf_ptr, start, final_len);
    buf_ptr += final_len;
  }

  fwrite(buffer, 1, buf_ptr - buffer, stream);
  va_end(args_copy);
}

/*==============================================================================
  Public Interface
==============================================================================*/

/**
 * Print formatted text to stdout
 * @param fmt Format string using {spec} placeholders
 */
void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  decay_vprint(stdout, fmt, args);
  va_end(args);
}

/**
 * Print formatted text to stdout with newline
 * @param fmt Format string using {spec} placeholders
 */
void println(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  decay_vprint(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

/**
 * Print formatted text to specified stream
 * @param stream Output stream (e.g. stdout, stderr)
 * @param fmt Format string using {spec} placeholders
 */
void fprint(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  decay_vprint(stream, fmt, args);
  va_end(args);
}

/**
 * Print formatted text to specified stream with newline
 * @param stream Output stream (e.g. stdout, stderr)
 * @param fmt Format string using {spec} placeholders
 */
void fprintln(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  decay_vprint(stream, fmt, args);
  fputc('\n', stream);
  va_end(args);
}

#endif // DECAY_H
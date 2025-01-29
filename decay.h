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
typedef unsigned long long usize;
typedef signed long long isize;

// Integer limits
#define I8_MAX (i8)127
#define I16_MAX (i16)32767
#define I32_MAX (i32)2147483647L
#define I64_MAX (i64)9223372036854775807LL
#define I8_MIN (i8)(-I8_MAX - 1)
#define I16_MIN (i16)(-I16_MAX - 1)
#define I32_MIN (i32)(-I32_MAX - 1)
#define I64_MIN (i64)(-I64_MAX - 1)

#define U8_MAX (u8)255U
#define U16_MAX (u16)65535U
#define U32_MAX (u32)4294967295UL
#define U64_MAX (u64)18446744073709551615ULL

#define U8_MIN (u8)0
#define U16_MIN (u16)0
#define U32_MIN (u32)0
#define U64_MIN (u64)0

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
  FMT_HEX8_LOWER,
  FMT_HEX8_UPPER,
  FMT_HEX16_LOWER,
  FMT_HEX16_UPPER,
  FMT_HEX32_LOWER,
  FMT_HEX32_UPPER,
  FMT_HEX64_LOWER,
  FMT_HEX64_UPPER,
  FMT_BIN8,
  FMT_BIN16,
  FMT_BIN32,
  FMT_BIN64
} FormatType;

typedef struct {
  FormatType type;
  int precision;
} FormatSpec;

static FormatSpec __decay_parse_spec(const char *spec) {
  FormatSpec result = {FMT_INVALID, -1};
  const char *p = spec;

  switch (*p++) {
  case 'i':
    if (!strncmp(p, "8", 1)) {
      result.type = FMT_I8;
      p++;
    } else if (!strncmp(p, "16", 2)) {
      result.type = FMT_I16;
      p += 2;
    } else if (!strncmp(p, "32", 2)) {
      result.type = FMT_I32;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_I64;
      p += 2;
    }
    break;
  case 'u':
    if (!strncmp(p, "8", 1)) {
      result.type = FMT_U8;
      p++;
    } else if (!strncmp(p, "16", 2)) {
      result.type = FMT_U16;
      p += 2;
    } else if (!strncmp(p, "32", 2)) {
      result.type = FMT_U32;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_U64;
      p += 2;
    }
    break;
  case 'f':
    if (!strncmp(p, "32", 2)) {
      result.type = FMT_F32;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_F64;
      p += 2;
    } else if (!strncmp(p, "128", 3)) {
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
    if (!strncmp(p, "8", 1)) {
      result.type = FMT_HEX8_LOWER;
      p++;
    } else if (!strncmp(p, "16", 2)) {
      result.type = FMT_HEX16_LOWER;
      p += 2;
    } else if (!strncmp(p, "32", 2)) {
      result.type = FMT_HEX32_LOWER;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_HEX64_LOWER;
      p += 2;
    }
    break;
  case 'X':
    if (!strncmp(p, "8", 1)) {
      result.type = FMT_HEX8_UPPER;
      p++;
    } else if (!strncmp(p, "16", 2)) {
      result.type = FMT_HEX16_UPPER;
      p += 2;
    } else if (!strncmp(p, "32", 2)) {
      result.type = FMT_HEX32_UPPER;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_HEX64_UPPER;
      p += 2;
    }
    break;
  case 'b':
    if (!strncmp(p, "8", 1)) {
      result.type = FMT_BIN8;
      p++;
    } else if (!strncmp(p, "16", 2)) {
      result.type = FMT_BIN16;
      p += 2;
    } else if (!strncmp(p, "32", 2)) {
      result.type = FMT_BIN32;
      p += 2;
    } else if (!strncmp(p, "64", 2)) {
      result.type = FMT_BIN64;
      p += 2;
    }
    break;
  default:
    return result;
  }

  if (*p == '.') {
    result.precision = 0;
    while (*++p >= '0' && *p <= '9')
      result.precision = result.precision * 10 + (*p - '0');
  }

  if (result.precision != -1 &&
      !(result.type >= FMT_F32 && result.type <= FMT_F128))
    return (FormatSpec){FMT_INVALID, -1};

  return *p ? (FormatSpec){FMT_INVALID, -1} : result;
}

static void __decay_format_value(char **buf, size_t *rem, FormatSpec spec,
                                 va_list *args) {
  char num[256];
  size_t len = 0;

  switch (spec.type) {
  case FMT_I8: {
    i8 val = (i8)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%hhd", val);
    break;
  }
  case FMT_I16: {
    i16 val = (i16)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%hd", val);
    break;
  }
  case FMT_I32:
    len = snprintf(num, sizeof(num), "%d", va_arg(*args, i32));
    break;
  case FMT_I64:
    len = snprintf(num, sizeof(num), "%lld", va_arg(*args, i64));
    break;

  case FMT_U8: {
    u8 val = (u8)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%hhu", val);
    break;
  }
  case FMT_U16: {
    u16 val = (u16)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%hu", val);
    break;
  }
  case FMT_U32:
    len = snprintf(num, sizeof(num), "%u", va_arg(*args, u32));
    break;
  case FMT_U64:
    len = snprintf(num, sizeof(num), "%llu", va_arg(*args, u64));
    break;

  case FMT_F32:
    len = snprintf(num, sizeof(num), "%.*f",
                   spec.precision >= 0 ? spec.precision : 6,
                   (f64)va_arg(*args, double));
    break;
  case FMT_F64:
    len =
        snprintf(num, sizeof(num), "%.*f",
                 spec.precision >= 0 ? spec.precision : 15, va_arg(*args, f64));
    break;
  case FMT_F128:
    len = snprintf(num, sizeof(num), "%.*Lf",
                   spec.precision >= 0 ? spec.precision : 18,
                   va_arg(*args, f128));
    break;

  case FMT_STRING: {
    const char *s = va_arg(*args, const char *);
    len = snprintf(num, sizeof(num), "%s", s ? s : "(null)");
    break;
  }
  case FMT_PTR_LOWER: {
    usize val = (usize)va_arg(*args, void *);
    len = snprintf(num, sizeof(num), "%llx", val);
    break;
  }
  case FMT_PTR_UPPER: {
    usize val = (usize)va_arg(*args, void *);
    len = snprintf(num, sizeof(num), "%llX", val);
    break;
  }

  case FMT_HEX8_LOWER: {
    u8 val = (u8)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%x", val);
    break;
  }
  case FMT_HEX8_UPPER: {
    u8 val = (u8)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%X", val);
    break;
  }
  case FMT_HEX16_LOWER: {
    u16 val = (u16)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%x", val);
    break;
  }
  case FMT_HEX16_UPPER: {
    u16 val = (u16)va_arg(*args, int);
    len = snprintf(num, sizeof(num), "%X", val);
    break;
  }
  case FMT_HEX32_LOWER: {
    u32 val = va_arg(*args, u32);
    len = snprintf(num, sizeof(num), "%x", val);
    break;
  }
  case FMT_HEX32_UPPER: {
    u32 val = va_arg(*args, u32);
    len = snprintf(num, sizeof(num), "%X", val);
    break;
  }
  case FMT_HEX64_LOWER: {
    u64 val = va_arg(*args, u64);
    len = snprintf(num, sizeof(num), "%llx", val);
    break;
  }
  case FMT_HEX64_UPPER: {
    u64 val = va_arg(*args, u64);
    len = snprintf(num, sizeof(num), "%llX", val);
    break;
  }

  case FMT_BIN8: {
    u8 val = (u8)va_arg(*args, int);
    char *p = num;
    for (int i = 7; i >= 0; i--) {
      *p++ = (val >> i) & 1 ? '1' : '0';
      if (i > 0 && i % 4 == 0)
        *p++ = '_';
    }
    *p = '\0';
    len = p - num;
    break;
  }
  case FMT_BIN16: {
    u16 val = (u16)va_arg(*args, int);
    char *p = num;
    for (int i = 15; i >= 0; i--) {
      *p++ = (val >> i) & 1 ? '1' : '0';
      if (i > 0 && i % 4 == 0)
        *p++ = '_';
    }
    *p = '\0';
    len = p - num;
    break;
  }
  case FMT_BIN32: {
    u32 val = (u32)va_arg(*args, int);
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
    u64 val = va_arg(*args, u64);
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

  if (len <= *rem) {
    memcpy(*buf, num, len);
    *buf += len;
    *rem -= len;
  }
}

static void __process_segment(const char *start, size_t len, char **buf_ptr,
                              size_t *remaining) {
  const char *p = start;
  const char *end = start + len;
  while (p < end && *remaining > 0) {
    if (p + 1 < end && *p == '{' && *(p + 1) == '{') {
      *(*buf_ptr)++ = '{';
      (*remaining)--;
      p += 2;
    } else if (p + 1 < end && *p == '}' && *(p + 1) == '}') {
      *(*buf_ptr)++ = '}';
      (*remaining)--;
      p += 2;
    } else {
      *(*buf_ptr)++ = *p++;
      (*remaining)--;
    }
  }
}

static void __decay_vprint(FILE *stream, const char *fmt, va_list args) {
  char buffer[DECAY_BUFFER_SIZE];
  char *buf_ptr = buffer;
  size_t remaining = sizeof(buffer);
  const char *start = fmt;
  va_list args_copy;
  va_copy(args_copy, args);

  while (*fmt) {
    if (*fmt == '{') {
      if (fmt[1] == '{') {
        // Process segment up to current position
        __process_segment(start, fmt - start, &buf_ptr, &remaining);
        // Write single '{'
        if (remaining > 0) {
          *buf_ptr++ = '{';
          remaining--;
        }
        fmt += 2;
        start = fmt;
      } else {
        // Process segment up to current position
        __process_segment(start, fmt - start, &buf_ptr, &remaining);
        // Parse format specifier
        const char *spec_start = fmt + 1;
        const char *spec_end = spec_start;
        while (*spec_end && *spec_end != '}')
          spec_end++;

        if (!*spec_end) { // No closing '}'
          if (remaining > 0) {
            *buf_ptr++ = '{';
            remaining--;
          }
          fmt++;
          start = fmt;
        } else {
          char spec_buf[DECAY_MAX_SPEC_LEN + 1];
          size_t spec_len = spec_end - spec_start;
          if (spec_len > DECAY_MAX_SPEC_LEN)
            spec_len = DECAY_MAX_SPEC_LEN;
          memcpy(spec_buf, spec_start, spec_len);
          spec_buf[spec_len] = '\0';

          FormatSpec spec = __decay_parse_spec(spec_buf);
          if (spec.type == FMT_INVALID) {
            // Invalid specifier, output literal
            if (remaining > 0) {
              *buf_ptr++ = '{';
              remaining--;
            }
            size_t invalid_len = spec_len;
            if (invalid_len > remaining)
              invalid_len = remaining;
            memcpy(buf_ptr, spec_start, invalid_len);
            buf_ptr += invalid_len;
            remaining -= invalid_len;
            if (remaining > 0) {
              *buf_ptr++ = '}';
              remaining--;
            }
          } else {
            __decay_format_value(&buf_ptr, &remaining, spec, &args_copy);
          }
          fmt = spec_end + 1; // Move past '}'
          start = fmt;
        }
      }
    } else if (*fmt == '}') {
      if (fmt[1] == '}') {
        // Process segment up to current position
        __process_segment(start, fmt - start, &buf_ptr, &remaining);
        // Write single '}'
        if (remaining > 0) {
          *buf_ptr++ = '}';
          remaining--;
        }
        fmt += 2;
        start = fmt;
      } else {
        // Single '}', treat as literal
        fmt++;
      }
    } else {
      fmt++;
    }
  }

  // Process remaining segment
  __process_segment(start, fmt - start, &buf_ptr, &remaining);

  fwrite(buffer, 1, buf_ptr - buffer, stream);
  va_end(args_copy);
}

static inline void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __decay_vprint(stdout, fmt, args);
  va_end(args);
}

static inline void println(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __decay_vprint(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

static inline void fprint(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __decay_vprint(stream, fmt, args);
  va_end(args);
}

static inline void fprintln(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __decay_vprint(stream, fmt, args);
  fputc('\n', stream);
  va_end(args);
}

#endif // DECAY_H
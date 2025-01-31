#ifndef DECAY_H
#define DECAY_H

#include <float.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX
#define F128_MAX LDBL_MAX
#define F32_MIN FLT_MIN
#define F64_MIN DBL_MIN
#define F128_MIN LDBL_MIN

typedef struct {
  char *data;
  usize len;
  usize cap;
} Buffer;

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
  FMT_CHAR,
  FMT_BIN8,
  FMT_BIN16,
  FMT_BIN32,
  FMT_BIN64,
  FMT_HEX8,
  FMT_HEX16,
  FMT_HEX32,
  FMT_HEX64,
  FMT_PTR8,
  FMT_PTR16,
  FMT_PTR32,
  FMT_PTR64
} FormatType;

typedef struct {
  FormatType type;
  i32 width;
  i32 precision;
  bool uppercase;
} FormatSpec;

static Buffer buffer_create(usize initial) {
  return (Buffer){(char *)malloc(initial), 0, initial};
}

static void buffer_free(Buffer *buf) {
  free(buf->data);
  buf->data = NULL;
  buf->len = buf->cap = 0;
}

static void buffer_append(Buffer *buf, const char *src, usize len) {
  if (buf->len + len >= buf->cap) {
    buf->cap = (buf->cap * 2) + len;
    buf->data = (char *)realloc(buf->data, buf->cap);
  }
  memcpy(buf->data + buf->len, src, len);
  buf->len += len;
}

static FormatSpec __parse_spec(const char **fmt) {
  FormatSpec spec = {FMT_INVALID, 0, -1, 0};
  const char *p = *fmt;

  switch (p[0]) {
  case 'i':
    p++;
    if (p[0] == '8') {
      spec.type = FMT_I8;
      p++;
    } else if (p[0] == '1' && p[1] == '6') {
      spec.type = FMT_I16;
      p += 2;
    } else if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_I32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_I64;
      p += 2;
    }
    break;
  case 'u':
    p++;
    if (p[0] == '8') {
      spec.type = FMT_U8;
      p++;
    } else if (p[0] == '1' && p[1] == '6') {
      spec.type = FMT_U16;
      p += 2;
    } else if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_U32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_U64;
      p += 2;
    }
    break;
  case 'f':
    p++;
    if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_F32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_F64;
      p += 2;
    } else if (p[0] == '1' && p[1] == '2' && p[2] == '8') {
      spec.type = FMT_F128;
      p += 3;
    }
    break;
  case 's':
    spec.type = FMT_STRING;
    p++;
    break;
  case 'c':
    spec.type = FMT_CHAR;
    p++;
    break;
  case 'x':
  case 'X':
    spec.uppercase = (p[0] == 'X');
    p++;
    if (p[0] == '8') {
      spec.type = FMT_HEX8;
      p++;
    } else if (p[0] == '1' && p[1] == '6') {
      spec.type = FMT_HEX16;
      p += 2;
    } else if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_HEX32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_HEX64;
      p += 2;
    }
    break;

  case 'p':
  case 'P':
    spec.uppercase = (p[0] == 'P');

    p++;
    if (p[0] == '8') {
      spec.type = FMT_PTR8;
      p++;
    } else if (p[0] == '1' && p[1] == '6') {
      spec.type = FMT_PTR16;
      p += 2;
    } else if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_PTR32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_PTR64;
      p += 2;
    }
    break;

  case 'b':
  case 'B':
    spec.uppercase = (p[0] == 'B');
    p++;
    if (p[0] == '8') {
      spec.type = FMT_BIN8;
      p++;
    } else if (p[0] == '1' && p[1] == '6') {
      spec.type = FMT_BIN16;
      p += 2;
    } else if (p[0] == '3' && p[1] == '2') {
      spec.type = FMT_BIN32;
      p += 2;
    } else if (p[0] == '6' && p[1] == '4') {
      spec.type = FMT_BIN64;
      p += 2;
    }
    break;

  default:
    return spec;
  }

  while (p[0]) {
    if (p[0] == '.') {
      if (!(spec.type >= FMT_F32 && spec.type <= FMT_F128))
        break;
      spec.precision = 0;
      p++;
      while (p[0] >= '0' && p[0] <= '9') {
        spec.precision = spec.precision * 10 + (p[0] - '0');
        p++;
      }
    } else if (p[0] == '}')
      break;
    else
      p++;
  }

  *fmt = p;
  return spec;
}

static char *__format_value(FormatSpec spec, va_list *args) {
  Buffer buf = buffer_create(512);
  char num_buf[512];
  char *output = NULL;
  usize len = 0;

  switch (spec.type) {

  // Integer
  case FMT_I8:
    len =
        snprintf(num_buf, sizeof(num_buf), "%" PRId8, (i8)va_arg(*args, usize));
    break;
  case FMT_I16:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRId16,
                   (i16)va_arg(*args, usize));
    break;
  case FMT_I32:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRId32,
                   (i32)va_arg(*args, usize));
    break;
  case FMT_I64:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRId64,
                   (i64)va_arg(*args, usize));
    break;
  case FMT_U8:
    len =
        snprintf(num_buf, sizeof(num_buf), "%" PRIu8, (u8)va_arg(*args, usize));
    break;
  case FMT_U16:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRIu16,
                   (u16)va_arg(*args, usize));
    break;
  case FMT_U32:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRIu32,
                   (u32)va_arg(*args, usize));
    break;
  case FMT_U64:
    len = snprintf(num_buf, sizeof(num_buf), "%" PRIu64,
                   (u64)va_arg(*args, usize));
    break;

  // Float
  case FMT_F32:
    len = snprintf(num_buf, sizeof(num_buf), "%.*f", spec.precision,
                   va_arg(*args, f64));
    break;
  case FMT_F64:
    len = snprintf(num_buf, sizeof(num_buf), "%.*f", spec.precision,
                   va_arg(*args, f64));
    break;
  case FMT_F128:
    len = snprintf(num_buf, sizeof(num_buf), "%.*Lf", spec.precision,
                   va_arg(*args, f128));
    break;

  // String
  case FMT_STRING: {
    const char *s = va_arg(*args, const char *);
    len = s ? strlen(s) : 6;
    output = (char *)malloc(len + 1);
    if (output)
      snprintf(output, len + 1, "%s", s ? s : "(null)");
    break;
  }

  // Char
  case FMT_CHAR: {
    char c = (char)va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), "%c", c);

    break;
  }

  // Hex
  case FMT_HEX8: {
    u8 val = (u8)va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%02X" : "%02x",
                   val);
    break;
  }
  case FMT_HEX16: {
    u16 val = (u16)va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%04X" : "%04x",
                   val);
    break;
  }
  case FMT_HEX32: {
    u32 val = va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%08X" : "%08x",
                   val);
    break;
  }
  case FMT_HEX64: {
    u64 val = va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf),
                   spec.uppercase ? "%016" PRIX64 : "%016" PRIx64, val);
    break;
  }

  // Pointer
  case FMT_PTR8: {
    u8 val = (u8)va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%02X" : "%02x",
                   val);
    break;
  }
  case FMT_PTR16: {
    u16 val = (u16)va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%04X" : "%04x",
                   val);
    break;
  }
  case FMT_PTR32: {
    u32 val = va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf), spec.uppercase ? "%08X" : "%08x",
                   val);
    break;
  }
  case FMT_PTR64: {
    u64 val = va_arg(*args, usize);
    len = snprintf(num_buf, sizeof(num_buf),
                   spec.uppercase ? "%016" PRIX64 : "%016" PRIx64, val);
    break;
  }

  // Binary
  case FMT_BIN8:
  case FMT_BIN16:
  case FMT_BIN32:
  case FMT_BIN64: {
    u64 value = 0;
    int bits = 0;
    switch (spec.type) {
    case FMT_BIN8:
      value = (u8)va_arg(*args, usize);
      bits = 8;
      break;
    case FMT_BIN16:
      value = (u16)va_arg(*args, usize);
      bits = 16;
      break;
    case FMT_BIN32:
      value = va_arg(*args, usize);
      bits = 32;
      break;
    case FMT_BIN64:
      value = va_arg(*args, usize);
      bits = 64;
      break;
    default:
      return NULL;
    }

    char tmp[128];
    int pos = 0;
    for (int i = bits - 1; i >= 0; i--) {
      tmp[pos++] = (value & ((u64)1 << i)) ? '1' : '0';
      if (spec.uppercase && i > 0 && i % 4 == 0)
        tmp[pos++] = '_';
    }
    tmp[pos] = '\0';
    len = snprintf(num_buf, sizeof(num_buf), "%s", tmp);

    if (!output) {
      output = (char *)malloc(len + 1);
      if (output) {
        memcpy(output, num_buf, len);
        output[len] = '\0';
      }
    }
    buffer_free(&buf);
    return output ? output : (char *)malloc(1);
  }

  default:
    return NULL;
  }

  if (!output) {
    output = (char *)malloc(len + 1);
    if (output) {
      memcpy(output, num_buf, len);
      output[len] = '\0';
    }
  }
  buffer_free(&buf);
  return output ? output : (char *)malloc(1);
}

static void __vprint(FILE *stream, const char *fmt, va_list args) {
  Buffer buf = buffer_create(256);
  const char *start = fmt;
  va_list args_copy;
  va_copy(args_copy, args);

  while (*fmt) {
    if (*fmt == '{') {
      if (fmt[1] == '{') {
        buffer_append(&buf, start, fmt - start);
        buffer_append(&buf, "{", 1);
        fmt += 2;
        start = fmt;
      } else {
        buffer_append(&buf, start, fmt - start);
        const char *spec_start = fmt;
        fmt++;
        const char *spec_end = fmt;
        while (*spec_end && *spec_end != '}')
          spec_end++;

        if (*spec_end == '}') {
          const char *spec_content = fmt;
          FormatSpec spec = __parse_spec(&spec_content);

          if (spec.type == FMT_INVALID || spec_content != spec_end) {
            buffer_append(&buf, spec_start, spec_end - spec_start + 1);
          } else {
            char *value = __format_value(spec, &args_copy);
            if (value) {
              buffer_append(&buf, value, strlen(value));
              free(value);
            }
          }
          fmt = spec_end + 1;
        } else {
          buffer_append(&buf, "{", 1);
        }
        start = fmt;
      }
    } else if (*fmt == '}') {
      if (fmt[1] == '}') {
        buffer_append(&buf, start, fmt - start);
        buffer_append(&buf, "}", 1);
        fmt += 2;
        start = fmt;
      } else {
        buffer_append(&buf, start, fmt - start + 1);
        fmt++;
        start = fmt;
      }
    } else {
      fmt++;
    }
  }

  buffer_append(&buf, start, fmt - start);
  fwrite(buf.data, 1, buf.len, stream);
  buffer_free(&buf);
  va_end(args_copy);
}

static inline void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __vprint(stdout, fmt, args);
  va_end(args);
}

static inline void println(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __vprint(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

static inline void fprint(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __vprint(stream, fmt, args);
  va_end(args);
}

static inline void fprintln(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __vprint(stream, fmt, args);
  fputc('\n', stream);
  va_end(args);
}

#endif // DECAY_H
# decay.h

**decay.h** is a lightweight, single-header C library providing sane types and a modern printf alternative. It supports integer and floating-point types, pointer formatting, and binary/hexadecimal representation... all in a single header file.

## Features

- **Formatted printing** – Type-safe, flexible print system supporting various formats.
- **Binary & Hex printing** – Easily print numbers in multiple bases.
- **Header-only** – Just include `decay.h`, no extra dependencies.

## Installation

Drop `decay.h` into your project and include it:

```c
#include "decay.h"

int main() {
    i32 num = 256;
    f64 e = 2.718281828459045;

    println("Integer: {i32}", num);
    println("Float: {f64.6}", e);
    println("Pointer: {p}", &num);
    println("Hex (u32): {X32}", (u32)num);
    println("Binary (u8): {b8}", (u8)num);

    return 0;
}
```
## Supported Format Specifiers

| Specifier | Description                  |
| --------- | ---------------------------- |
| `{s}`     | String                       |
| `{c}`     | Character                    |
| `{i8}`    | 8-bit signed integer         |
| `{i16}`   | 16-bit signed integer        |
| `{i32}`   | 32-bit signed integer        |
| `{i64}`   | 64-bit signed integer        |
| `{u8}`    | 8-bit unsigned integer       |
| `{u16}`   | 16-bit unsigned integer      |
| `{u32}`   | 32-bit unsigned integer      |
| `{u64}`   | 64-bit unsigned integer      |
| `{f32}`   | 32-bit floating-point        |
| `{f64}`   | 64-bit floating-point        |
| `{f128}`  | 128-bit floating-point       |
| `{x8}`    | 8-bit hex (lowercase)        |
| `{x16}`   | 16-bit hex (lowercase)       |
| `{x32}`   | 32-bit hex (lowercase)       |
| `{x64}`   | 64-bit hex (lowercase)       |
| `{X8}`    | 8-bit hex (uppercase)        |
| `{X16}`   | 16-bit hex (uppercase)       |
| `{X32}`   | 32-bit hex (uppercase)       |
| `{X64}`   | 64-bit hex (uppercase)       |
| `{p}`     | Pointer (lowercase hex)      |
| `{P}`     | Pointer (uppercase hex)      |
| `{b8}`    | 8-bit binary representation  |
| `{b16}`   | 16-bit binary representation |
| `{b32}`   | 32-bit binary representation |
| `{b64}`   | 64-bit binary representation |


## Types

| Type    | Description             |
| ------- | ----------------------- |
| `bool`  | Boolean type            |
| `i8`    | 8-bit signed integer    |
| `i16`   | 16-bit signed integer   |
| `i32`   | 32-bit signed integer   |
| `i64`   | 64-bit signed integer   |
| `u8`    | 8-bit unsigned integer  |
| `u16`   | 16-bit unsigned integer |
| `u32`   | 32-bit unsigned integer |
| `u64`   | 64-bit unsigned integer |
| `f32`   | 32-bit floating-point   |
| `f64`   | 64-bit floating-point   |
| `f128`  | 128-bit floating-point  |
| `usize` | Architecture-dependent  |
| `isize` | Architecture-dependent  |





# decay.h

**decay.h** is a lightweight, single-header C library providing sane types and a modern printf alternative. It supports integer and floating-point types, pointer formatting, and binary/hexadecimal representation... all in a single header file.

## Features

- **Formatted printing** – Type-safe, flexible print system supporting various formats.
- **Binary & Hex printing** – Easily print numbers in multiple bases.
- **Header-only** – Just include `decay.h`, no extra dependencies.

## Use

Drop `decay.h` into your project and include it:

```c
#include "decay.h"

i32 main() {
    i32 num = 255;
    f64 e = 2.718281828459045;

    println("Hello, {s}", "World!");          //Hello, World!
    println("Character: {c}", (u32)64);       //Character: @
    println("");                              //
    println("Integer: {i32}", num);           //256
    println("Float: {f64.6}", e);             //2.718282
    println("Pointer: {p}", &num);            //00000012F6F8F7FC
    println("Hex (u32): {u32:x}", (u32)num);  //000000FF
    println("Binary (u8): {u8:b}", (u8)num);  //11111111



    return 0;
}
```

## Types

| Type    | Description             |
| ------- | ----------------------- |
| `bool`  | Boolean type            |
| `char`  | Character               |
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

## Supported Format Specifiers

| Specifier | Description             |
| --------- | ----------------------- |
| `{s}`     | String (char *)         |
| `{c}`     | Character               |
| `{i8}`    | 8-bit signed integer    |
| `{i16}`   | 16-bit signed integer   |
| `{i32}`   | 32-bit signed integer   |
| `{i64}`   | 64-bit signed integer   |
| `{u8}`    | 8-bit unsigned integer  |
| `{u16}`   | 16-bit unsigned integer |
| `{u32}`   | 32-bit unsigned integer |
| `{u64}`   | 64-bit unsigned integer |
| `{f32}`   | 32-bit floating-point   |
| `{f64}`   | 64-bit floating-point   |
| `{f128}`  | 128-bit floating-point  |
| `{p}`     | Architecture-dependent  |

## Format Specifier Modifiers

| Modifier | Description                    |
| -------- | ------------------------------ |
| `.n`     | Set precision to n (floats)    |
| `:x`     | Print in hexadecimal lowercase |
| `:X`     | Print in hexadecimal uppercase |
| `:b`     | Print in binary                |
| `:B`     | Print in binary, _ separators  |







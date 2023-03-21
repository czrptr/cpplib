#pragma once

template <typename StorageType>
struct Integer;

template <typename StorageType>
struct Float;

using u8  = Integer<unsigned char>;  // 0u,                  255u
using u16 = Integer<unsigned short>; // 0u,                65535u
using u32 = Integer<unsigned int>;   // 0u,           4294967295u
using u64 = Integer<unsigned long>;  // 0u, 18446744073709551615u

using i8  = Integer<signed char>;  //                   -128,                 127
using i16 = Integer<signed short>; //                 -32768,               32767
using i32 = Integer<signed int>;   //            -2147483648,          2147483647
using i64 = Integer<signed long>;  // -9223372036854775807-1, 9223372036854775807

// TODO better floats
using f32 = Float<float>;
using f64 = Float<double>;

// TODO better chars
// struct c8;
// struct c16;
// struct c32;
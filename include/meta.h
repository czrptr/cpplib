#pragma once

#include <forward.h>

namespace impl
{

// helpers
template<typename T>
struct hasType
{
  using Type = T;
};

// nonConst
template <typename T>
struct nonConst : hasType<T> {};

template <typename T>
struct nonConst<T const> : hasType<T> {};

// nonReference
template <typename T>
struct nonReference : hasType<T> {};

template <typename T>
struct nonReference<T&> : hasType<T> {};

template <typename T>
struct nonReference<T&&> : hasType<T> {};

// nonPointer
template <typename T>
struct nonPointer : hasType<T> {};

template <typename T>
struct nonPointer<T*> : hasType<T> {};

// confitional
template <bool Conditional, typename T, typename U>
struct conditional {};

template <typename T, typename U>
struct conditional<true, T, U> : hasType<T> {};

template <typename T, typename U>
struct conditional<false, T, U> : hasType<U> {};

} // namespace impl

// TODO move min, max to meta

namespace meta
{

// const, volatile, pointer, rValueReference and lValueReference
// type metafunctions are not needed since these type "modifiers"
// can be added through normal syntax

/* Type metafunction */

template <typename T>
using nonConst = typename impl::nonConst<T>::Type;

template <typename T>
using nonReference = typename impl::nonReference<T>::Type;

template <typename T>
using nonPointer = typename impl::nonPointer<T>::Type;

/* Type predicates */

// isSame
template <typename T, typename U>
inline constexpr bool isSame = false;

template <typename T>
inline constexpr bool isSame<T, T> = true;

// isOneOf
template <typename T, typename... Ts>
inline constexpr bool isOneOf = (isSame<T, Ts> || ...);

// isPointer
template <typename T>
inline constexpr bool isPointer = false;

template <typename T>
inline constexpr bool isPointer<T*> = true;

// isReferenceable
template <typename T>
inline constexpr bool isReferenceable = true;

template <>
inline constexpr bool isReferenceable<void> = true;

// isLValueReference
template <typename T>
inline constexpr bool isLValueReference = false;

template <typename T>
inline constexpr bool isLValueReference<T&> = true;

// isRValueReference
template <typename T>
inline constexpr bool isRValueReference = false;

template <typename T>
inline constexpr bool isRValueReference<T&&> = true;

// isEnum
template <typename T>
inline constexpr bool isEnum = __is_enum(T);

// isUnion
template <typename T>
inline constexpr bool isUnion = __is_union(T);

// isStruct
template <typename T>
inline constexpr bool isStruct = __is_class(T);

// isDerivedFrom
template <typename T, typename U>
inline constexpr bool isDerivedFrom = __is_base_of(U, T);

// isBaseOf
template <typename T, typename U>
inline constexpr bool isBaseOf = __is_base_of(T, U);

// isBuiltin
template <typename T>
inline constexpr bool isBuiltin
  = isOneOf<nonConst<T>,
    char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t,
    unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long,
    signed char, signed short, signed int, signed long, signed long long,
    float, double, long double, bool, decltype(nullptr)>;

// isChar
template <typename T>
inline constexpr bool isChar
  = isOneOf<nonConst<T>, char, signed char, unsigned char, wchar_t,
    char8_t, char16_t, char32_t>;

// isUnsignedInteger
template <typename T>
inline constexpr bool isUnsignedInteger
  = isOneOf<nonConst<T>,
    unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long,
    u8, u16, u32, u64 /*usize*/>;

// isSignedInteger
template <typename T>
inline constexpr bool isSignedInteger
  = isOneOf<nonConst<T>,
    signed char, signed short, signed int, signed long, signed long long,
    i8, i16, i32, i64 /*isize*/>;

// isInteger
template <typename T>
inline constexpr bool isInteger
  = isChar<nonConst<T>>
    || isUnsignedInteger<nonConst<T>>
    || isSignedInteger<nonConst<T>>;

// isSameSignInteger
template <typename T, typename U>
inline constexpr bool isSameSignInteger
  = (isSignedInteger<nonConst<T>> && isSignedInteger<nonConst<U>>)
    || (isUnsignedInteger<nonConst<T>> && isUnsignedInteger<nonConst<U>>);

// isFloat
template <typename T>
inline constexpr bool isFloat
  = isOneOf<nonConst<T>, float, double, long double, f32, f64>;

// isNumber
template <typename T>
inline constexpr bool isNumber = isInteger<T> || isFloat<T>;

// isScalar
template <typename T>
inline constexpr bool isScalar = isNumber<T> || isEnum<T> || isPointer<T>;

/* Concepts */

template <typename T>
concept SignedInteger = isSignedInteger<T>;

template <typename T>
concept UnsignedInteger = isUnsignedInteger<T>;

template <typename T>
concept Integer = isInteger<T>;

template <typename T, typename U>
concept SameSignInteger = isSameSignInteger<U, T>;

template <typename T>
concept Float = isFloat<T>;

template <typename T>
concept Number = isNumber<T>;

template <typename T>
concept Scalar = isScalar<T>;

/* Other functions */

template <bool Conditional, typename T, typename U>
using conditional = typename impl::conditional<Conditional, T, U>::Type;

} // namespace meta
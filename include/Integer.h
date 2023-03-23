#pragma once

#include <meta.h>
#include <tuple>

namespace impl
{

/* meta functions */

template <typename T>
inline constexpr bool isInteger = false;

template <typename T>
inline constexpr bool isInteger<Integer<T>> = true;

template <typename T, typename U>
inline constexpr bool bothAreIntegersAndHaveSameSign
  = (impl::isInteger<meta::nonConst<T>> || impl::isInteger<meta::nonConst<U>>)
    && meta::isSameSignInteger<meta::nonConst<T>, meta::nonConst<U>>;

template <typename T>
struct StorageImpl : impl::hasType<T> {};

template <typename T>
struct StorageImpl<Integer<T>> : impl::hasType<T> {};

template <typename T>
using StorageOf = typename StorageImpl<T>::Type;

// NOTE:
//   template <typename T>
//   using StorageOf = meta::conditional<isInteger<T>, typename T::Storage, T>;
//
//   This ☝️ implementation is what I want to write but for [T = int] for example
//   T::Storage is malformed and the program doesn't compile. This could be solved
//   if compile time expressions we're evaluated lazily but that would be sane...

template <typename T, typename U>
  requires (impl::bothAreIntegersAndHaveSameSign<T, U>)
using CommonStorageOf
  = meta::conditional<(sizeof(StorageOf<T>) > sizeof(StorageOf<U>)), StorageOf<T>, StorageOf<U>>;

/* helpers */

template <typename T>
StorageOf<T> valueOf(T const t)
{
  if constexpr (impl::isInteger<T>)
  {
    return t.m_data;
  }
  else
  {
    return t;
  }
}

template <typename T, typename U, typename Storage = CommonStorageOf<T, U>>
std::tuple<Storage, Storage> valuesOf(T const t, U const u)
{
  Storage const
    valueT { static_cast<Storage>(valueOf(t)) },
    valueU { static_cast<Storage>(valueOf(u)) };

  return { valueT, valueU };
}

} // namespace impl

template <typename StorageType>
struct Integer
{
private:
  using Storage = StorageType;
  static constexpr unsigned long Size = sizeof(Storage);

  template <typename>
  friend struct Integer;

  template <typename T>
  friend auto impl::valueOf(T const t);

public:
  constexpr Integer() = default;

  /* Implicit conversions */

  template <typename T>
    requires (meta::isSameSignInteger<T, Storage> && sizeof(T) <= Size)
  constexpr Integer(T const value)
    : m_data { static_cast<Storage>(impl::valueOf(value)) }
  {}

  template <typename T>
    requires (meta::isSameSignInteger<T, Storage> && sizeof(T) >= Size)
  constexpr operator T() const
  {
    return T { m_data };
  }

  /* Explicit conversions */

  template <typename T>
    requires ((meta::isSameSignInteger<T, Storage> && sizeof(T) > Size)
      || (!meta::isSameSignInteger<T, Storage> && meta::isScalar<T>))
  explicit constexpr Integer(T const value)
    : m_data { static_cast<Storage>(impl::valueOf(value)) }
  {}

  template <typename T>
    requires (meta::isScalar<T>)
  explicit constexpr operator T() const
  {
    return T { m_data };
  }

private:
  Storage m_data;
};

/* Relational operators */

// TODO: add support for a < b < c > d expressions
//   each operator return an intermediary tuple like value
//   which has a bool conversion operator implemented

template <typename T, typename U>
  requires (impl::bothAreIntegersAndHaveSameSign<T, U>)
constexpr auto operator <=> (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT <=> valueU;
}

template <typename T, typename U>
  requires (impl::bothAreIntegersAndHaveSameSign<T, U>)
constexpr bool operator == (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT == valueU;
}

/* Arithmetic operators */

template <typename T, typename U, typename Storage = impl::CommonStorageOf<T, U>>
constexpr Integer<Storage> operator + (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT + valueU;
}

template <typename T, typename U, typename Storage = impl::CommonStorageOf<T, U>>
constexpr Integer<Storage> operator - (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT - valueU;
}

template <typename T, typename U, typename Storage = impl::CommonStorageOf<T, U>>
constexpr Integer<Storage> operator * (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT * valueU;
}

template <typename T, typename U, typename Storage = impl::CommonStorageOf<T, U>>
constexpr Integer<Storage> operator / (T const t, U const u)
{
  auto const [valueT, valueU] = impl::valuesOf(t, u);
  return valueT / valueU;
}
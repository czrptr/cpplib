#pragma once

#include <forward.h>
#include <meta.h>

namespace impl
{

template <typename T>
inline constexpr bool isInteger = false;

template <typename T>
inline constexpr bool isInteger<Integer<T>> = true;

template <typename T, typename U>
inline constexpr bool typesAreIntegersAndHaveSameSign
  = (impl::isInteger<meta::nonConst<T>> || impl::isInteger<meta::nonConst<U>>)
    && meta::isSameSignInteger<meta::nonConst<T>, meta::nonConst<U>>;

template <typename T>
auto extractValue(T const t)
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

} // namespace impl

#define FRIEND_OPERATOR(type, op)                                                                           \
  template <typename T, typename U>                                                                         \
    requires (impl::typesAreIntegersAndHaveSameSign<T, U>)                                                  \
  friend type operator op (T const t, U const u)

#define INTEGER_OPERATOR(type, op)                                                                          \
template <typename T, typename U>                                                                           \
  requires (impl::typesAreIntegersAndHaveSameSign<T, U>)                                                    \
type operator op (T const t, U const u)                                                                     \
{                                                                                                           \
  using StorageTypeT = meta::conditional<impl::isInteger<T>, typename T::Storage, T>;                       \
  using StorageTypeU = meta::conditional<impl::isInteger<U>, typename U::Storage, U>;                       \
                                                                                                            \
  using CommonStorage                                                                                       \
    = meta::conditional<(sizeof(StorageTypeT) > sizeof(StorageTypeU)), StorageTypeT, StorageTypeU>;         \
                                                                                                            \
  CommonStorage const                                                                                       \
    valueT { static_cast<CommonStorage>(impl::extractValue(t)) },                                           \
    valueU { static_cast<CommonStorage>(impl::extractValue(u)) };                                           \
                                                                                                            \
  return valueT op valueU;                                                                                  \
}

#define INTEGER_ARITHMETIC_OPERATOR(type, op)                                                               \
template <typename T, typename U>                                                                           \
  requires (impl::typesAreIntegersAndHaveSameSign<T, U>)                                                    \
type operator op (T const t, U const u)                                                                     \
{                                                                                                           \
  using StorageTypeT = meta::conditional<impl::isInteger<T>, typename T::Storage, T>;                       \
  using StorageTypeU = meta::conditional<impl::isInteger<U>, typename U::Storage, U>;                       \
                                                                                                            \
  using CommonStorage                                                                                       \
    = meta::conditional<(sizeof(StorageTypeT) > sizeof(StorageTypeU)), StorageTypeT, StorageTypeU>;         \
                                                                                                            \
  CommonStorage const                                                                                       \
    valueT { static_cast<CommonStorage>(impl::extractValue(t)) },                                           \
    valueU { static_cast<CommonStorage>(impl::extractValue(u)) };                                           \
                                                                                                            \
  return Integer<CommonStorage>{ valueT op valueU };                                                        \
}

template <typename StorageType>
struct Integer
{
private:
  using Storage = StorageType;
  static constexpr unsigned long Size = sizeof(Storage);

  template <typename>
  friend struct Integer;

  template <typename T>
  friend auto impl::extractValue(T const t);

  FRIEND_OPERATOR(auto, <=>);
  FRIEND_OPERATOR(bool, ==);
  FRIEND_OPERATOR(auto, +);
  FRIEND_OPERATOR(auto, -);
  FRIEND_OPERATOR(auto, *);
  FRIEND_OPERATOR(auto, /);

public:
  Integer() = default;

  template <typename T>
    requires (meta::isSameSignInteger<T, Storage> && sizeof(T) <= Size)
  Integer(T const value)
    : m_data { static_cast<Storage>(impl::extractValue(value)) }
  {}

  template <typename T>
    requires ((meta::isSameSignInteger<T, Storage> && sizeof(T) > Size)
      || (!meta::isSameSignInteger<T, Storage> && meta::isScalar<T>))
  explicit Integer(T const value)
    : m_data { static_cast<Storage>(impl::extractValue(value)) }
  {}

  template <typename T>
    requires (meta::isSameSignInteger<T, Storage> && sizeof(T) >= Size)
  operator T() const
  {
    return T { m_data };
  }

  template <typename T>
    requires (meta::isScalar<T>)
  explicit operator T() const
  {
    return T { m_data };
  }

private:
  Storage m_data;
};

INTEGER_OPERATOR(auto, <=>);
INTEGER_OPERATOR(bool, ==);
INTEGER_ARITHMETIC_OPERATOR(auto, +);
INTEGER_ARITHMETIC_OPERATOR(auto, -);
INTEGER_ARITHMETIC_OPERATOR(auto, *);
INTEGER_ARITHMETIC_OPERATOR(auto, /);

#undef FRIEND_OPERATOR
#undef INTEGER_OPERATOR
#undef INTEGER_ARITHMETIC_OPERATOR
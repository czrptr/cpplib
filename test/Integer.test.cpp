#include <gtest/gtest.h>

#include <meta.h>
#include <Integer.h>

#define EXPECT_SAME(t1, t2) EXPECT_TRUE((meta::isSame<t1, t2>))

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;

TEST(IntegerTest, arithmetic_operations)
{
  EXPECT_SAME(u32, decltype(u16{} + u32{}));
  EXPECT_SAME(u32, decltype(u32{} + u16{}));

  EXPECT_SAME(u64, decltype(u8{} + u64{}));

  EXPECT_SAME(u16, decltype(u8{} + ushort{}));
  EXPECT_SAME(i64, decltype(long{} + i32{}));
}
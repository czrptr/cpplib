load("@compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")

refresh_compile_commands(
  name = "refresh_compile_commands",
  targets = {
    ":libcpp": "--config=clang_15",
    ":libcpptest": "--config=clang_15",
  },
)

cc_library(
  name = "libcpp",
  srcs = glob(["source/**/*.cpp"]),
  hdrs = glob(["include/**/*.h"]),
  deps = ["@fmt"],
  strip_include_prefix = "include",
)

cc_test(
  name = "libcpptest",
  srcs = glob(["test/**/*.cpp"]),
  deps = [
    "@gtest//:gtest_main",
    ":libcpp",
  ],
)
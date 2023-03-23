load(":cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

cc_toolchain_config(name = "clang_15_native_toolchain_config")

cc_toolchain(
  name = "clang_15_native_toolchain",
  toolchain_identifier = "clang_15_native_toolchain",
  toolchain_config = ":clang_15_native_toolchain_config",
  all_files = ":empty",
  compiler_files = ":empty",
  dwp_files = ":empty",
  linker_files = ":empty",
  objcopy_files = ":empty",
  strip_files = ":empty",
  supports_param_files = 0,
)

cc_toolchain_suite(
  name = "clang_15_suite",
  toolchains = {
    "native": ":clang_15_native_toolchain",
  },
)
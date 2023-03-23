load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
  "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
  "feature", "flag_group", "flag_set", "tool_path",
)

all_link_actions = [
  ACTION_NAMES.cpp_link_executable,
  ACTION_NAMES.cpp_link_dynamic_library,
  ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

all_compile_actions = [
  ACTION_NAMES.cpp_compile
]

def _impl(ctx):
  tool_paths = [
    tool_path(
      name = "gcc",
      path = "/usr/bin/clang++-15"
    ),
    tool_path(
      name = "ld",
      path = "/usr/bin/lld-15",
    ),
    tool_path(
      name = "ar",
      path = "/usr/bin/ar",
    ),
    tool_path(
      name = "cpp",
      path = "/bin/false",
    ),
    tool_path(
      name = "gcov",
      path = "/bin/false",
    ),
    tool_path(
      name = "nm",
      path = "/bin/false",
    ),
    tool_path(
      name = "objdump",
      path = "/bin/false",
    ),
    tool_path(
      name = "strip",
      path = "/bin/false",
    ),
  ]

  features = [
    feature(
      name = "linker_flags",
      enabled = True,
      flag_sets = [
        flag_set(
          actions = all_link_actions,
          flag_groups = [
            flag_group(
              flags = [
                "-lstdc++", # stdlib
                "-lrt", # runtime
                "-lm", # math
              ],
            ),
          ],
        ),
      ],
    ),
    feature(
      name = "compiler_flags",
      enabled = True,
      flag_sets = [
          flag_set(
          actions = all_compile_actions,
          flag_groups = [
            flag_group(
              flags = [
                "-std=c++20",
                "-Wall",
                "-Wextra",
                "-pedantic",
                "-Wno-user-defined-literals",
                "-fno-exceptions",
                "-fno-rtti",
              ],
            ),
          ],
        ),
      ],
    ),
  ]

  return cc_common.create_cc_toolchain_config_info(
    ctx = ctx,
    features = features,
    cxx_builtin_include_directories = [
      "/usr/lib/llvm-15/lib/clang/15.0.6/include",
      "/usr/include",
    ],
    toolchain_identifier = "clang_15_native_toolchain",
    host_system_name = "local",
    target_system_name = "local",
    target_cpu = "native",
    target_libc = "unknown",
    compiler = "clang",
    abi_version = "unknown",
    abi_libc_version = "unknown",
    tool_paths = tool_paths
  )

cc_toolchain_config = rule(
  implementation = _impl,
  attrs = {},
  provides = [CcToolchainConfigInfo],
)
workspace(name = "cpplib")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
  name = "gtest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
  sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)

http_archive(
  name = "compile_commands",
  url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/752014925d055387ff4590a9862fb382350b0e5d.tar.gz",
  strip_prefix = "bazel-compile-commands-extractor-752014925d055387ff4590a9862fb382350b0e5d",
  sha256 = "9a68018c9120a636e60988b265c85e56f169a1408ed0c6bd164dd3a6996a9b36",
)

git_repository(
  name = "fmt",
  commit = "a33701196adfad74917046096bf5a2aa0ab0bb50", # 9.1.0
  patch_cmds = [
    "mv support/bazel/.bazelrc .bazelrc",
    "mv support/bazel/.bazelversion .bazelversion",
    "mv support/bazel/BUILD.bazel BUILD.bazel",
    "mv support/bazel/WORKSPACE.bazel WORKSPACE.bazel",
  ],
  remote = "https://github.com/fmtlib/fmt",
)
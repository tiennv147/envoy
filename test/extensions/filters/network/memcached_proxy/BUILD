licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_package",
)
load(
    "//test/extensions:extensions_build_system.bzl",
    "envoy_extension_cc_test",
)

envoy_package()

envoy_extension_cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
    extension_name = "envoy.filters.network.memcached_proxy",
    deps = [
        "//source/common/buffer:buffer_lib",
        "//source/extensions/filters/network/memcached_proxy:config",
        "//test/mocks/network:network_mocks",
    ],
)

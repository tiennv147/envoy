licenses(["notice"])  # Apache 2

# Memcached proxy L4 network filter.

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "codec_interface",
    hdrs = ["codec.h"],
    deps = [
        "//include/envoy/buffer:buffer_interface",
    ]
)

envoy_cc_library(
    name = "ascii_codec",
    srcs = ["ascii_codec_impl.cc"],
    hdrs = ["ascii_codec_impl.h"],
    deps = [
        ":codec_interface",
        "//include/envoy/buffer:buffer_interface",
        "//source/common/common:logger_lib",
    ],
)

envoy_cc_library(
    name = "binary_codec",
    srcs = ["binary_codec_impl.cc"],
    hdrs = ["binary_codec_impl.h"],
    deps = [
        ":codec_interface",
        ":utility_lib",
        "//source/common/common:logger_lib",
    ],
)

envoy_cc_library(
    name = "filter_lib",
    srcs = ["filter.cc"],
    hdrs = ["filter.h"],
    deps = [
        ":ascii_codec",
        ":binary_codec",
        ":codec_interface",
        "//include/envoy/network:connection_interface",
        "//include/envoy/network:filter_interface",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    deps = [
        ":filter_lib",
        "//source/extensions/filters/network:well_known_names",
        "//source/extensions/filters/network/common:factory_base_lib",
        "@envoy_api//envoy/config/filter/network/memcached_proxy/v1alpha1:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "utility_lib",
    srcs = ["utility.cc"],
    hdrs = ["utility.h"],
    deps = [
        "//include/envoy/buffer:buffer_interface",
    ]
)

licenses(["notice"])  # Apache 2

# OAuth L7 HTTP filter (https://tools.ietf.org/html/rfc6749)

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "filter_lib",
    srcs = ["filter.cc"],
    hdrs = ["filter.h"],
    deps = [
        "//source/extensions/filters/http/common:pass_through_filter_lib",
        "@envoy_api//envoy/extensions/filters/http/oauth/v3:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "http_client_lib",
    srcs = ["http_client.cc"],
    hdrs = ["http_client.h"],
    deps = [],
)

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    security_posture = "robust_to_untrusted_downstream",
    status = "wip",
    deps = [
        ":filter_lib",
        "//include/envoy/registry",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/common:factory_base_lib",
        "@envoy_api//envoy/extensions/filters/http/oauth/v3:pkg_cc_proto",
    ],
)

package(default_visibility = ["//visibility:public"])


filegroup(
    name = "trt_inference_srcs",
    srcs = glob([
        "**/*.cc",
        "**/*.h",
    ]),
    visibility = ["//visibility:public"],
)

cc_library(

    name = "trt_inference_libs",

    srcs = [":trt_inference_srcs"],
   
    deps = [
	"@trt//:trt"
	],
    alwayslink = 1,

)

cc_binary(
   name = "libtrt_inference.so",
   srcs = [],
   deps = [
	":trt_inference_libs",
	],
   linkshared = 1,
   linkstatic = 1,

)

cc_binary(
   name = "mnist_app",
   srcs = ["mnist_test.cpp"],
   deps = [
	":trt_inference_libs",
	"@opencv//:opencv",
	],

)





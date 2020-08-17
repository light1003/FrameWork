# Description:
#    c+++ Project

package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])




filegroup(
    name = "tensorflow_inference_srcs",
    srcs = glob([
        "**/*.cc",
        "**/*.h",
    ]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tensorflow_inference",
    srcs = [":tensorflow_inference_srcs"],
    copts = ["-fexceptions",
	     #"-DDEBUG"   
	     ],
    visibility = ["//visibility:public"],
    deps = [
	"@darknet//:darknet",
	"@opencv//:opencv",
	"@tensorflow_cc//:tensorflow_cc"
    ],
    alwayslink = 1,

    linkopts = [
    "-fexceptions",
    ]

)
LINKER_SCRIPT = ":Srcs/version_script.lds"

cc_binary(
    name = "libcigarettecase_recognition.so",
    srcs = [],
    copts = [
        "-ffunction-sections",
        "-fdata-sections",
    	"-fexceptions",

    ],

    linkopts = [
        "-fexceptions",
        "-z defs",
        "-s",
        "-Wl,--gc-sections",
        "-Wl,--version-script",  # This line must be directly followed by LINKER_SCRIPT.
        "$(location {})".format(LINKER_SCRIPT),
    ],
    linkshared = 1,
    linkstatic = 1,
    tags = [
        "manual",
        "notap",
    ],
    
    deps = [
        ":tensorflow_inference",
	 LINKER_SCRIPT,
    ],
)



cc_binary(
    name = "test",
    srcs = ["Test/test.cpp"],
    deps = [
        ":tensorflow_inference",

    ],
    copts = ["-g"]
)
cc_binary(
    name = "save_embs",
    srcs = ["Test/save_embs.cpp"],
    deps = [
        ":tensorflow_inference",

    ],
)



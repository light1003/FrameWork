package(default_visibility = ["//visibility:public"])

cc_library(

    name = "time-lib",

    srcs = ["time.cc"],

    hdrs = ["time.h"],

    linkstatic = 1,

)


cc_library(

    name = "splitstr-lib",

    srcs = ["splitstr.cc"],

    hdrs = ["splitstr.h"],

    linkstatic = 1,

)
cc_library(

    name = "byte2binarystring-lib",

    srcs = ["byte2binarystring.cc"],

    hdrs = ["byte2binarystring.h"],

    linkstatic = 1,

)

# also can use cc_binary
cc_test(

    name = "time_test",

    srcs = ["time_test.cc"],

    deps = [":time-lib"],

)



cc_test(

    name = "splitstr_test",

    srcs = ["splitstr_test.cc"],

    deps = [":splitstr-lib"],

)

cc_test(

    name = "byte2binarystring_test",

    srcs = ["byte2binarystring_test.cc"],

    deps = [":byte2binarystring-lib"],

)



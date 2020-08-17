package(default_visibility = ["//visibility:public"])

cc_library(

    name = "mysql_op-lib",

    srcs = ["mysql_op.cc"],

    hdrs = ["mysql_op.h"],

    deps = [
	"@mysql//:mysql",
	"//mysql/utils:splitstr-lib",
	"//mysql/utils:byte2binarystring-lib",	
	],
    linkstatic = 1,

)



cc_binary(

    name = "main",

    srcs = ["test.cpp","sql.hpp"],
   
    deps = [
	":mysql_op-lib",
	#"//mysql/utils:time-lib",	
	],

)


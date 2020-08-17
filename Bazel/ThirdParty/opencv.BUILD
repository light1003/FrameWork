cc_library(
    name = "opencv",
    srcs = glob(["lib/*.so*"]),
    hdrs = glob(["include/**/*.h","include/**/*.hpp"]),
    includes = ["include"],
    #linkopts = ["-I/home/zz/JinHui/ThirdParty/opencv_x86_64/include -L/home/zz/JinHui/ThirdParty/opencv_x86_64/lib -lopencv_videostab -lopencv_highgui -lopencv_dnn -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_imgcodecs -lopencv_shape -lopencv_video -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_calib3d -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_core"],
    visibility = ["//visibility:public"], 
    linkstatic = 1,
)

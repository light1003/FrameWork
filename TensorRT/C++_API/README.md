g++ trtexec.cpp -Wall -std=c++11 -I"/usr/local/cuda-9.0/include" -I"/usr/local/include" -I"../../include" -I"../common"  -D_REENTRANT -L"/usr/local/cuda-9.0/lib64" -lcudnn -lcublas -lcudart_static -lnvToolsExt -lcudart -lpthread -ldl -lrt -L"../../lib" -lnvinfer -lnvparsers -lnvinfer_plugin `pkg-config --cflags --libs opencv`


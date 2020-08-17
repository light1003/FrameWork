#g++ mnist.cpp mnist_test.cpp -std=c++11 -I"/usr/local/cuda-9.0/include"  -I"/home/zz/JinHui/ThirdParty/TensorRT-4.0.1.6/include"  -L"/usr/local/cuda-9.0/lib64" -lcudart -lpthread -ldl -lrt -L"/home/zz/JinHui/ThirdParty/TensorRT-4.0.1.6/lib" -lnvinfer `pkg-config --cflags --libs opencv` -D_REENTRANT #多线程
g++ mnist_test.cpp -I. -L/usr/local/lib -ltrt_inference `pkg-config --cflags --libs opencv` -std=c++11 -D_REENTRANT


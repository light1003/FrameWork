1. 针对tensorflow pb模型优化(目前在tensorflow1.12以上成功运行，python端)
- [Tensorflow-TensorRT](https://github.com/ardianumam/Tensorflow-TensorRT)
2. 针对tensort框架（c++）
- cuda10.0
- cudnn7.6.3
- cmake3.13.x(mini_version->3.13.3)
- protoc3.8.x(mini_version)
- onnx-tensorrt7.0源码
- [tensorrt7.0.0.11源码](https://developer.nvidia.com/zh-cn/tensorrt)
> 1. 配置tensorrt环境变量
```
export LD_LIBRARY_PATH=<TensorRT-dir>/lib:$LD_LIBRARY_PATH
export CUDA_INSTALL_DIR=/usr/local/cuda-10.0
export CUDNN_INSTALL_DIR=/usr/local/cuda-10.0
```
> 2. pb->uff->trt
```
convert-to-uff -o squeezenet.uff --input-file squeezenet.pb -O embeddings  #-l 查看网络节点
./trtexec --uff=squeezenet.uff --uffInput=input,3,300,200 --output=probs/BiasAdd --batch=1 --saveEngine=squeezenet.trt
```
> 3. onnx->trt
```
onnx2trt model.onnx -o mode.trt
```
#### Ref:
- 参考github-> onnx
##### Notes： 
-  -DTENSORRT_DIR=<tensorrt_dir>/targets/x86_64-linux-gnu

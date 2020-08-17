1. 官方下载 （没意思）
[Libtorch](https://download.pytorch.org/libtorch/cu90/libtorch-shared-with-deps-latest.zip)
2. 源码编译（刺激）
下载对应版本[pytorch](https://github.com/pytorch/pytorch)源码
并把thirdparty里面的文件全都下载到对应位置（默认没有）
```
mkdir build  && cd build && cmake-gui .. 
```
配置(建议修改默认存放位置)：
- BUILD_PYTHON OFF
- BUILD_TORCH ON
```
make -j8 && make install
```

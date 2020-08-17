## c/c++
#### linux平台
> 1. 下载tensorflow源码
> 2. [安装对应版本bazel](https://tensorflow.google.cn/install/lang_c)
> 3. [bazel安装](https://docs.bazel.build/versions/master/install-ubuntu.html)（选择使用二进制安装程序）
[用法](https://github.com/bazelbuild/bazel/tree/master/examples)
> 4. 开始编译tensorflow库
```
./configure // tensorflow源码目录下执行 配置cpu/gpu/android
bazel build -c opt --config=monolithic --config=cuda //tensorflow:libtensorflow.so --verbose_failures // c api gpu版本 
bazel build -c opt --config=monolithic --config=cuda //tensorflow:libtensorflow_cc.so --verbose_failures // c++ api gpu版本
```
> 5. c/c++动态调用tensorflow_cc库 头文件c_api.h
```
 g++ x.cpp -L<库路径> -ltensorflow_cc -std==c++11
```
> 6. tensorflow源码中静态编译 c/c++工程

> - 将整个c/c++工程放在tensorflow/contrib下
> - 根据需要修改BUILD文件，参考android下的BUILD文件,修改tensorflow/java/src/main/native下BUILD的包可见性
> - tensorflow源码根目录下运行
```
bazel build -c opt --config=monolithic --config=cuda //tensorflow/contrib/<工程目录>:<库名称> --verbose_failures
```

#### Windows平台
- [windows编译tensorflow源码](https://www.cnblogs.com/dakewei/p/10272018.html)

 > 1. 获取Msys2 添加路径C:\msys64\usr\bin到PATH环境变量 pacman -S git patch unzip 禁用MSYS2路径:MSYS_NO_PATHCONV=1 	 MSYS2_ARG_CONV_EXCL="*"（use msys2 bash）
[Msys2下载](http://repo.msys2.org/distrib/x86_64/msys2-x86_64-20180531.exe)
> 2. [Visual C++ Build Tools 2015下载](https://www.microsoft.com/en-us/download/details.aspx?id=48159)
>3. [Microsoft Visual C++ 2015 Redistributable Update 3下载](https://www.microsoft.com/en-us/download/details.aspx?id=53587)
> 4. Visual Studio 2015 set BAZEL_VS=C:\Program Files (x86)\Microsoft Visual Studio 14.0 (这个变量是用来告知　bazel，Visual Studio所安装的目录)
[Visual Studio 2015下载](https://download.microsoft.com/download/b/e/d/bedddfc4-55f4-4748-90a8-ffe38a40e89f/vs2015.3.com_enu.iso)
> 5. [bazel下载](https://github.com/bazelbuild/bazel/releases)
> 6. 设置JAVA_HOME环境变量
> 7. [编译项目参考](https://github.com/tensorflow/tensorflow/issues/14456)
**--features=static_link_msvcrt （/MD -> /MT）**
> 8. 编译过程中如果报错，直接注释报错代码

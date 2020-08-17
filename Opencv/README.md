### Linux or android
1. 安装依赖
```
sudo apt-get install build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg.dev libtiff4.dev libswscale-dev libjasper-dev
```
2. 动态库
- [动态库编译](https://www.jianshu.com/p/f646448da265)

```
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make -4j && sudo make install
sudo gedit /etc/ld.so.conf.d/opencv.conf  -> 末尾添加 /usr/local/lib 
sudo ldconfig 
sudo gedit /etc/bash.bashrc -> 末尾添加 PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig export PKG_CONFIG_PATH
source /etc/bash.bashrc
pkg-config --cflags --libs opencv -> 测试
```
3. 静态库
```
cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_L IBS=0 -DBUILD_PNG=1 -DBUILD_JASPER=1 -DBUILD_JPEG=1 -DBUILD_TIFF=1 -DBUILD_ ZLIB=1 -DWITH_JPEG=1 -DWITH_PNG=1 -DWITH_JASPER=1 -DWITH_TIFF=1 .. 
```
4. android版本静态库(sdk版本为25.x)
	```
	1. cmake -DANDROID_NDK=/home/zz/Android/Ndk -DANDROID_ABI="x86_64" -DANDROID_NATIVE_API_LEVEL=24 -DANDROID_SDK=/home/zz/Android/Sdk -DANDROID_NO_UNDEFINE D=ON -DANDROID_SO_UNDEFINED=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=0 -DBUILD_PNG=1 -DBUILD_JASPER=1 -DBUILD_JPEG=1 -DBUILD_TIFF=1 -DBUILD_ZLIB=1 -DWITH_JPEG=1 -DWITH_PNG=1 -DWITH_JASPER=1 -DWITH_TIFF=1 -DBUILD_ANDROID_PROJECTS=OFF -DCMAKE_TOOLCHAIN_FILE=../platforms/android/android.toolchain.cmake ..
	```

### Windows
- [CMake + OpenCV + VS2015/vs2017静态编译](https://blog.csdn.net/yixing98/article/details/86540949)
> 1. cmake-gui选择与编译 
> 2. OpenCV的源码中的CMAKE文件默认编译动态库，默认勾选BUILD_SHARED_LIBS选项，编译静态库时只需要取消该选项即可，这也是编译静态库和动态库的在设置上唯一差别。
强烈建议去掉BUILD_EXAMPLES，BUILD_TESTS，BUILD_PERF_TESTS这个三个选项（这三个选项默认勾选），否则静态编译的示例程序非常大。
> 3. 在文件输出目录找到OpenCV.sln，打开，选择编译版本是调试还是发布，建议两个版本都编译一下
> 4. 点击x64旁边的下拉菜单选择生成 - >重新生成解决方案，打开配置管理器，默认情况下，IANSTALL项目是没打勾的，一定记得打上
> 5. 选择生成 - >重新生成解决方案
> 6. 找到CMakeTargets中的INSTALL，然后右键选择“仅限于项目” - >“仅生成INSTALL”
> 7. 项目 - > XX（项目名）属性
> 8. 要想静态链接，必须要设置“MFC的使用：在静态库中使用MFC”和“C / C ++  - >代码生成 - >运行库：/ MT（调试版本为：/ MTD）”

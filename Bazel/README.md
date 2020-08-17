### 源码编译bazel
1. why？：官网只提供x86_64,其他平台（tegra2 jetson）需自己编,基于armv8/arrch64
2. how to do?:
[下载dist压缩包](https://github.com/bazelbuild/bazel/releases)

```
sudo apt-get install g++ unzip zip
# Ubuntu 16.04 (LTS) uses OpenJDK 8 by default:
sudo apt-get install openjdk-8-jdk
# Ubuntu 18.04 (LTS) uses OpenJDK 11 by default:
sudo apt-get install openjdk-11-jdk
# 命令行执行
export EXTRA_BAZEL_ARGS="--host_javabase=@local_jdk//:jdk"
./compile.sh
```

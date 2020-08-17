1. 安装docker
```
sudo apt-get remove docker docker-engine docker-ce docker.io
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
sudo apt-get update
sudo apt-get install -y docker-ce
systemctl status docker
docker --version -> 测试
```
2. 安装nvidia-docker
```
curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add -
distribution=$(. /etc/os-release;echo $ID$VERSION_ID)
curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list
sudo apt-get update
sudo apt-get install -y nvidia-docker2
sudo pkill -SIGHUP dockerd
nvidia-docker --version -> 测试
```

#番外篇
### 1. 常用指令:
```
1. docker images
2. docker ps [-a]
3. docker rename old_name new_name
4. docker restart/start/stop 容器ID // docker attach 容器ID(退出时容器结束进程) / docker exec -it 容器ID /bin/bash(退出时容器仍然运行)
5. docker save 容器ID name.tar.gz  -> docker load -i name.tar.gz
6. docker history 容器ID // 制作步骤
7. docker logs 容器ID  // 显示容器运行日志
8. docker run [-v -itd --name -p/P --link -e --network] 容器ID  // 后台运行
9. docker ps -a -q // 查看所有容器ID
10. docker stop $(sudo docker ps -a -q) //  stop停止所有容器
11. docker  rm $(sudo docker ps -a -q) //   remove删除所有容器
12. docker rmi `docker images | grep  "<none>" | awk '{print $3}'` //删除所有None镜像
13. docker tag <原镜像> <新镜像>
13. docker login/logout/push  <新镜像> // 推送镜像到个人docker hub仓库 
```
### 2. Dockerfile(制作)
```
docker build -t <仓库名>:<tag> .
docker/nvidia-docker run -v <宿主机目录>:<容器目录 >-it --rm/-d <镜像ID> /bin/bash
```
###### Sample:
1. 基础镜像:nvidia/cuda:9.0-cudnn7-ubuntu:16.04 需自己添加cudnn头文件与库文件
2. 配置python3 pip3环境
3. 配置tensorflow环境 头文件c_api.h 库文件libtensorflow_cc.so
4. 配置编译工具cmake bazel 
5. 配置opencv环境,不用配置环境变量即可运行
6. 删除临时文件
```
FROM nvidia/cuda:9.0-cudnn7-devel-ubuntu16.04 
MAINTAINER piscesdream  piscesdream_zz@163.com

# 环境变量
ENV WORKDIR /root 
ENV LANG C.UTF-8

COPY ./cuda $WORKDIR/tmp/cuda 
COPY ./bazel-0.16.0-installer-linux-x86_64.sh $WORKDIR/tmp/
COPY ./libtensorflow_cc.so $WORKDIR/thirdParty/tensorflow-1.9/lib/
COPY ./opencv-3.4 $WORKDIR/thirdParty/opencv-3.4

##更新源
RUN sed -i 's/archive.ubuntu/mirrors.aliyun/g' /etc/apt/sources.list \
	&& apt-get update --fix-missing \
	&& apt-get install -y vim cmake git \
	&& apt-get install -y unzip g++ zip \
	&& apt-get install -y python3-pip && pip3 install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple --trusted-host pypi.tuna.tsinghua.edu.cn \
	&& apt-get install -y openjdk-8-jdk \
	&& apt-get install -y build-essential pkg-config libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg.dev libtiff4.dev libswscale-dev libjasper-dev libdc1394-22-dev libcanberra-gtk-module 
	
RUN mv $WORKDIR/tmp/cuda/include/cudnn.h /usr/local/cuda/include/ \
	&& mv $WORKDIR/tmp/cuda/lib64/libcudnn* /usr/local/cuda/lib64/ \
        # && chmod a+r /usr/local/cuda/include/cudnn.h \
        # && chmod a+r /usr/local/cuda/lib64/libcudnn* \
	&& ln -sf /usr/local/cuda-9.0/lib64/libcudnn.so.7.0.5 /usr/local/cuda-9.0/lib64/libcudnn.so.7 \
	#&& chmod +x $WORKDIR/tmp/bazel-0.16.0-installer-linux-x86_64.sh 
	&& ./$WORKDIR/tmp/bazel-0.16.0-installer-linux-x86_64.sh --bin=/usr/local/bin \
	&& echo "export LD_LIBRARY_PATH=/root/thirdParty/tensorflow-1.9/lib:$LD_LIBRARY_PATH" >> /etc/bash.bashrc \
	&& echo $WORKDIR/thirdParty/opencv-3.4/lib >> /etc/ld.so.conf.d/opencv.conf && ldconfig \
	&& echo "PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/root/thirdParty/opencv-3.4/lib/pkgconfig" >>  /etc/bash.bashrc \
	&& echo "export PKG_CONFIG_PATH" >>  /etc/bash.bashrc \
	&& rm -rf $WORKDIR/tmp \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR $WORKDIR
```

## 镜像缩小技巧
1. 拷贝文件:先解压再拷贝->镜像缩小 内部用mv代替cp
2. RUN操作最好用&&链接
3. 外部文件可用docker run -v挂载
4. .dockerignore 忽略读取文件/目录
## Note:
1. [容器显示图片](https://blog.csdn.net/Frank_Abagnale/article/details/80243939)
2. [宿主机链接docker中的mysql](https://www.cnblogs.com/lgj8/p/12425185.html)
> a. localhost->本地数据库 
> b. 172.17.0.1->宿主机数据库 
> c. mysql镜像->mysql别名[--link]
3. [docker删除镜像依赖问题](https://blog.csdn.net/lyh03601/article/details/86736417)


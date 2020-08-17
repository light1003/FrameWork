准备(下载对应版本)：
- [nvidia驱动](https://www.nvidia.cn/geforce/drivers/)
- [cuda](https://developer.nvidia.com/cuda-toolkit-archive)
- [cudnn](https://developer.nvidia.com/rdp/cudnn-archive)

安装：
> 1. nvidia驱动
```
sudo apt-get --purge remove nvidia-*
sudo gedit /etc/modprobe.d/blacklist.conf
sudo update-initramfs -u
sudo service lightdm stop
sudo chmod +x NVIDIA-Linux-x86_64-<version>.run
sudo ./NVIDIA-Linux-x86_64-<version>.run -no-opengl-files
sudo service lightdm start
nvidia-smi // 测试
```
> 2. cuda
```
sudo sh cuda_9.0.176_384.81_linux.run // 不要安装驱动
设置环境变量：~/.bashrc
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda-9.0/lib64
export PATH=$PATH:/usr/local/cuda-9.0/bin
export CUDA_HOME=$CUDA_HOME:/usr/local/cuda-9.0
source ~/.bashrc
nvcc --version // 测试
```
> 3. cudnn
```
sudo cp cuda/include/cudnn.h /usr/local/cuda/include/
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/
sudo chmod a+r /usr/local/cuda/include/cudnn.h
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
cat /usr/local/cuda/include/cudnn.h | grep CUDNN_MAJOR -A 2 // 测试
```

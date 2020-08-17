1. c接口怎样控制显存占用or指定gpu ID?
-  [Tensorflow gpu配置1(c cpi)](http://www.gpu-computing.cn/?p=151)
- [Tensorflow gpu配置2(c cpi)](https://github.com/tensorflow/tensorflow/issues/13853)

2. 关于windows函数导出dll方法 .def文件
```
LIBRARY "dll名字" 
EXPORT
函数名 @1 #不能直接导出类名
。。。
```
调用dll，显式调用只用.dll库，隐式调用需要.dll .lib 头文件

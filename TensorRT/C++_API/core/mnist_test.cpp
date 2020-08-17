#include <assert.h>
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "mnist.h"
int main(){
	cv::Mat resized;
	std::string img_path = "/home/zz/JinHui/ThirdParty/TensorRT-4.0.1.6/data/mnist/9.pgm";
	cv::Mat tmp_frame = cv::imread(img_path.data()); //读取图片路径
	cv::cvtColor(tmp_frame, tmp_frame, cv::COLOR_BGR2GRAY); // BGR->RGB
	cv::resize(tmp_frame,resized,cv::Size(28,28));
	float *frame = new float[28*28];		
			
	cv::Mat img(28,28,CV_32FC1,frame);
	float *prewhite_frame = new float[28*28];	
	resized.convertTo(img,CV_32FC1);
	for(int i=0;i<28*28;i++){
		prewhite_frame[i] = 1.0 - frame[i] / 255.0;
	}
	
	MnistReg *m = new MnistReg;
	
	std::string model_path = "/home/zz/JinHui/SdkProjects/TensorRT/core/lenet5_mnist_frozen.trt";
	bool flag = m->Init(model_path);

	assert(flag==true);
	/*
	//input = ;
	//output = ;
	//batchSize = 1;
	
	
	*/
	
	for(int j =0;j<200;j++){
		
	
		float *output = new float[10];
		m->doInference( prewhite_frame,output , 1);
		for(int i =0;i<10;i++){
			std::cout <<output[i] <<" ";
		}
		std::cout << "\n";
		delete [] output;
	}
	delete [] prewhite_frame;
	delete [] frame;
	
	delete m;
	return 0;
}


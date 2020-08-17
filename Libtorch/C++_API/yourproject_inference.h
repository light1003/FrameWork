#ifndef TENSORFLOW_EXAMPLES_YOURPROJECT_TENSORFLOW_C_PLUS_PLUS_H_
#define TENSORFLOW_EXAMPLES_YOURPROJECT_TENSORFLOW_C_PLUS_PLUS_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <torch/script.h> // One-stop header. 


const int channel = 3;
const int height = 80;
const int width = 80;
const int embedding_dim = 4;
const int Crop_N = 10;



class YourProject{
   public:
	
        void initialize(const std::string &cfg_path);
	void inference(cv::Mat &image);
	
   private:
      // 定义全局sess grap变量
 	std::shared_ptr<torch::jit::script::Module> module;
	
	
};


#endif




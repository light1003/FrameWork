#ifndef TENSORFLOW_EXAMPLES_YOURPROJECT_TENSORFLOW_C_H_
#define TENSORFLOW_EXAMPLES_YourProject_TENSORFLOW_C_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include "../tf_src/c_api.h" 


const int channel = 3;
const int height = 270;
const int width = 180;
const int embedding_dim = 15;



class YourProject{
   public:
	YourProject();
        void initialize(const std::string &cfg_path,int gpu_id);
   
	void inference(float (*aligned_frame)[height*width*channel], int batch,std::vector<int> &outputs,std::vector<float> &scores);
        void getEmbs(float (*aligned_frame)[height*width*channel], int batch,float (*embedding)[embedding_dim]);
	
	void cleanup();
   private:
      // 定义全局sess grap变量
 	TF_Session * sess;
	TF_Graph * graph;
	
};


#endif




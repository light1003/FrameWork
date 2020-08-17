#ifndef TENSORFLOW_EXAMPLES_YOURPROJECT_TENSORFLOW_C_PLUS_PLUS_H_
#define TENSORFLOW_EXAMPLES_YOURPROJECT_TENSORFLOW_C_PLUS_PLUS_H_
#include <vector>
#include <opencv2/opencv.hpp>


class YourProject
{
   public:  
      YourProject();
      int YourProject_Init();
      void YourProject_Recognition(cv::Mat& image,std::vector<DETRES>& detResult,int *outputs,float *scores); 
      void YourProject_getEmbs(float *aligned_frame,int batch,float *embedding);
      ~YourProject_Extractor();
  
};




#endif




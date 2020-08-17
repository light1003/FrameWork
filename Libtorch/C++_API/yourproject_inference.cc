#include <assert.h>
#include <string>
#include <iostream>
#include "yourproject_inference.h"
 

static bool Debug = 0;
static void crop_bbox(cv::Mat &image,std::vector<cv::Mat> &patch_images){
	
	
	int rawImage_width = image.cols;
	int rawImage_height = image.rows;

	int count = 0;
	for(int i=rawImage_width/3;i<rawImage_width/3*2-width;i+=width){
		if(count==Crop_N){
			break;
		}
		for(int j=rawImage_height/3;j<rawImage_height/3*2-height;j+=height){
			std::cout << count << "\n";
 	 		
			if(count==Crop_N){
				break;
			}
			int x1 = i;
			int y1 = j;
			int x2 = x1 + width;
			int y2 = y1 + height;

			std::cout << x1 << " " << y1 <<" " << x2 << " " << y2 << std::endl;
			//cv::Mat roi_=image(cv::Rect(cv::Point(x1,y1),cv::Point(x2,y2)));
			cv::Mat roi_=image(cv::Rect(x1,y1,height,width));
			patch_images.push_back(roi_);
			
			//cv::rectangle(image,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar(0,255,0),3);
			//cv::Mat resized;
			//cv::resize(image,resized,cv::Size(500,500));
			cv::imshow("patch",roi_);
			cv::waitKey(0);
			
		
			//cv::Mat roi(height,width,CV_32FC3,patch_images[count]);
			//roi_.convertTo(roi, CV_32FC3);
			
			count++;
			
		}
		
	}
}

//  加载model 
void YourProject::initialize(const std::string &cfg_path){
	if(Debug){
		std::cout << "*******************开始初始化识别模型***************\n";

	}

  	
	
	std::string frozen_fname = cfg_path + "torch_script_eval.pt";
	module = torch::jit::load(frozen_fname);
	module->to(at::kCUDA);
	assert(module != nullptr);
  	
	if(Debug){
		std::cout << "*******************初始化识别模型完毕***************\n";

	}

}




// CigaretteCase_inference
void YourProject:inference(cv::Mat &image){
	if(Debug){
		std::cout << "*******************开始获取识别Feature***************\n";

	}

	// CROP n IMAGES
	//float (*patch_images)[height*width*channel] = new float[Crop_N][height*width*channel];
	std::vector<cv::Mat> patch_images;
	crop_bbox(image,patch_images);
	for(int i = 0;i<Crop_N;i++){
		 // 下方的代码即将图像转化为Tensor，随后导入模型进行预测
		at::Tensor tensor_image = torch::from_blob(patch_images[i].data, {1,height, width,3}, torch::kByte);
		tensor_image = tensor_image.permute({0,3,1,2});
		tensor_image = tensor_image.toType(torch::kFloat);
		//auto tensor_image_tmp = torch::autograd::make_variable(tensor_image,false);
		//tensor_image = tensor_image.div(255);
		tensor_image = tensor_image.to(torch::kCUDA);
		torch::Tensor result = module->forward({tensor_image}).toTensor();
	
	
		std::cout << result.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';

		//float *output = result.data<float>();  .to(torch::kCPU)
	
		//std::tuple<torch::Tensor,torch::Tensor> max_classes = torch::max(result,1);
		//auto max_1 = std::get<0>(max_classes);
		//auto max_index = std::get<1>(max_classes);
		//std::cout << max_1 << std::endl;
		//std::cout << max_index << std::endl;
		break;
	}
	
	
      
 	//delete [] patch_images;
	 if(Debug){
		std::cout << "*******************获取识别Feature完毕***************\n";

	}
	
	
}	






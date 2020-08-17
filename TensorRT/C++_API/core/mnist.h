#ifndef TRT_EXAMPLES_MNIST_TRT_H_
#define TRT_EXAMPLES_MNIST_TRT_H_

#include <string>
#include <vector>
#include "trt_src/NvInfer.h"
const int INPUT_C = 1;
const int INPUT_H  = 28;
const int INPUT_W = 28;
const int OUTPUT_SIZE = 10;
using namespace nvinfer1;
class MnistReg{

	public:
		MnistReg();
		~MnistReg();
		bool Init(std::string model_path);
		void doInference(float* input, float* output, int batchSize);
		
	private:
		IExecutionContext* context;
		ICudaEngine* engine;
};



#endif

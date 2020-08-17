#include <assert.h>
#include <fstream>
#include <iostream>
#include <cuda_runtime_api.h>
#include "mnist.h"

#define CHECK(status)                                   \
{                                                       \
    if (status != 0)                                    \
    {                                                   \
        std::cout << "Cuda failure: " << status;        \
        abort();                                        \
    }                                                   \
}
static class Logger : public ILogger
{
    void log(Severity severity, const char* msg) override
    {
        // suppress info-level messages
        if (severity != Severity::kINFO)
            std::cout << msg << std::endl;
    }
} gLogger;
static void load_model(std::string model_path,std::vector<char> &trtModelStream)
{
	size_t size{ 0 };
	
	std::ifstream file(model_path, std::ios::binary);
	if (file.good())
	{
		file.seekg(0, file.end);
		size = file.tellg();
		file.seekg(0, file.beg);
		trtModelStream.resize(size);
		file.read(trtModelStream.data(), size);
		file.close();
	}
	
}


MnistReg::MnistReg(){

}
MnistReg::~MnistReg(){
	context->destroy();
	engine->destroy();
}


bool MnistReg::Init(std::string model_path)
{
	std::vector<char> trtModelStream;
	
	//class init
	load_model(model_path,trtModelStream);
	
	IRuntime* infer = createInferRuntime(gLogger);
	engine = infer->deserializeCudaEngine(trtModelStream.data(), trtModelStream.size(), nullptr);
	

	if (!engine)
	{
		std::cout << "Engine created erro ...." << std::endl;
		return 0;
	}
	context = engine->createExecutionContext();
	if (context == nullptr)
	{
		std::cout << "context created erro ...." << std::endl;
		return 0;
	}
	infer->destroy();
	
	return true;
}
void MnistReg::doInference(float* input, float* output, int batchSize)
{
	
	void* buffers[2];

	CHECK(cudaMalloc(&buffers[0], batchSize *INPUT_C * INPUT_H * INPUT_W * sizeof(float)));
	CHECK(cudaMalloc(&buffers[1], batchSize * OUTPUT_SIZE * sizeof(float)));

	cudaStream_t stream;
	CHECK(cudaStreamCreate(&stream));

	CHECK(cudaMemcpyAsync(buffers[0], input, batchSize * INPUT_C * INPUT_H * INPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
	
	context->enqueue(batchSize, buffers, stream, nullptr);
	
	CHECK(cudaMemcpyAsync(output, buffers[1], batchSize * OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost, stream));
	cudaStreamSynchronize(stream);

	cudaStreamDestroy(stream);
	CHECK(cudaFree(buffers[0]));
	CHECK(cudaFree(buffers[1]));
}







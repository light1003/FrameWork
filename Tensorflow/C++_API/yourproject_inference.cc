#include <assert.h>
#include <stdio.h>
#include <utility>
#include <unistd.h>
#include <math.h>
#include <string>
#include <fstream>

#include "tensorflow/core/public/session.h"      
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/graph/default_device.h"

#include "yourproject_inference.h"   

#include "yourproject_model.h" // 对齐模型

using namespace tensorflow;

// 定义全局sess 变量

Session* align_session;          // 定义 对齐sess
Session* sess;                   // 定义 识别sess
ConfigProto configProto;
GPUOptions gpuOptions;
SessionOptions sessionOptions;




// Mat2Tensor
static tensorflow::Tensor Mat2Tensor(float (*ScaleChanged_frame)[align_height*align_width*align_channel],int batch){
      
	
   	tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({batch, align_height, align_width, align_channel}));

	float *p = input_tensor.flat<float>().data();
	

	for(int i=0;i<batch;i++){
		for(int j=0;j<align_height*align_width*align_channel;j++){
			p[i*align_height*align_width*align_channel+j] = ScaleChanged_frame[i][j];
		
			}		
		}

	//std::cout << input_tensor.DebugString() << std::endl;
	
	
	return input_tensor;
}
static void YourProject_getEmbs(float *aligned_frame, int batch,float *embedding){
	
	float (*tmp_aligned_frame)[height*width*channel] = new float[batch][height*width*channel];
	
	for(int i=0; i<batch; i++){ 
	  for(int j=0; j<height*width*channel; j++){	
		tmp_aligned_frame[i][j] = aligned_frame[i*height*width*channel+j];
			
	  	}	
	}

	
	float (*prewhited_frame)[height*width*channel] = new float[batch][channel*height*width];
	
	prewhite(tmp_aligned_frame,batch,prewhited_frame);
	
	
	tensorflow::Tensor input_tensor = Mat2Tensor(prewhited_frame,batch);   
	
	std::string input_node = "input:0";
	std::string phase_train_node = "phase_train:0";
	std::string output_node = "embeddings:0";
	
	tensorflow::Tensor phase_tensor(tensorflow::DT_BOOL,tensorflow::TensorShape());
	phase_tensor.scalar<bool>()() = false;
 	
	std::vector<tensorflow::Tensor> outputs;
	

	std::vector<std::pair<string, tensorflow::Tensor>> feed_dict = {
		  {input_node, input_tensor},  
		  {phase_train_node, phase_tensor},
	};   
   
 	Status status_run = sess->Run(feed_dict, {output_node}, {}, &outputs);

	if (!status_run.ok()) {
		std::cout << "ERROR: RUN failed..."  << std::endl;
		std::cout << status_run.ToString() << "\n";
	}
	//std::cout << "Output: " << outputs[0].DebugString() << std::endl;
	
	
	float* out_emb = outputs[0].flat<float>().data();
	
	for(int i=0; i<batch*embedding_dim; i++){ 
			embedding[i] = out_emb[i];	
	}
	std::vector<std::pair<string, tensorflow::Tensor>>().swap(feed_dict);
	std::vector<tensorflow::Tensor>().swap(outputs);
	//outputs.clear();
    	//outputs.shrink_to_fit();
	delete [] prewhited_frame;
	delete [] tmp_aligned_frame;
}
	

// 初始化sess graph变量 加载model 
static void YourProject_initialize(){
	// Initialize a tensorflow session
	//Status status = NewSession(SessionOptions(), &sess);
	Status status = NewSession(sessionOptions, &sess);
	if (!status.ok()) {
		std::cerr << status.ToString() << std::endl; 
	} else {
		std::cout << "recognition Session created successfully" << std::endl;
	}

  	// Load the protobuf graph
	GraphDef graph_def;
     	
	std::string facenet_string(cigratettcase_pb, cigratettcase_pb_size);
     	bool ctrl = graph_def.ParseFromString(facenet_string);
     	

    	if (!ctrl){
		std::cout << "Load recognition graph protobuf successfully" << std::endl;
		}
    	// std::cout << tensorflow::SummarizeGraphDef(graph_def) << std::endl;
	//graph::SetDefaultDevice("/gpu:0", &graph_def);
	
	// Add the graph to the session	
	status = sess->Create(graph_def);	
	if (!status.ok()) {
		std::cerr << status.ToString() << std::endl;
	} else {
		std::cout << "Add recognition graph to recognition session successfully" << std::endl;
	}

  	
}

// 销毁sess graph 释放内存
static void YourProject_cleanup(){
   sess->Close();
   delete sess;

}



//########################################################################################## 封装类
YourProject::YourProject(void){   //构造函数	
	
	//CigaretteCaseAlign_initialize();
	//CigaretteCaseRecognition_initialize();
	read_sql();
}

YourProject::~YourProject(void){  //析构函数   
	  
	CigaretteCaseAlign_cleanup();   //释放 对齐sess
	CigaretteCaseRecognition_cleanup(); //释放 识别sess
	
}

int YourProject::YourProject_Init(void){ // 初始化 对齐 识别sess
	gpuOptions.set_per_process_gpu_memory_fraction(0.2);  // 限制每个进程使用0.5
	configProto.set_allocated_gpu_options(&gpuOptions);
	sessionOptions.config=configProto;
	
	CigaretteCaseAlign_initialize();
	CigaretteCaseRecognition_initialize();
	return 1;
	
}


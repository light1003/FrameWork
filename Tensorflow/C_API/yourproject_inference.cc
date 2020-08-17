#include <assert.h>
#include <stdio.h>
#include <utility>
#include <unistd.h>
#include <math.h>
#include <string>
#include <fstream>

#include "yourproject_inference.h"


static bool Debug = 0;

static int load_file(const std::string & fname, std::vector<char>& buf)
{
	std::ifstream fs(fname, std::ios::binary | std::ios::in);

	if(!fs.good())
	{
		std::cerr<<fname<<" does not exist"<<std::endl;
		return -1;
	}


	fs.seekg(0, std::ios::end);
	int fsize=fs.tellg();

	fs.seekg(0, std::ios::beg);
	buf.resize(fsize);
	fs.read(buf.data(),fsize);

	fs.close();

	return 0;

}
//#####################################################################################
static void prewhite(float (*tmp_frame)[height*width*channel],int batch,float (*prewhited_frame)[height*width*channel]){
	if(Debug){
		std::cout << "*******************开始识别归一化***************\n";

	}
	for(int num=0;num<batch;num++){
			float tmp_m = 0.0; // 计算平均值临时变量
			for(int k=0; k<height*width*channel; k++){
				tmp_m = tmp_m + tmp_frame[num][k]; 
			}
			float m = tmp_m / (height*width*channel); //图片RGB三通道标准差
			 
			float tmp_s = 0.0;  // 计算标准差临时变量
			for(int i=0; i<height*width*channel; i++){
				tmp_s = tmp_s+(tmp_frame[num][i]-m)*(tmp_frame[num][i]-m); 
			}
			float s = sqrt(tmp_s/(height*width*channel)); //图片RGB三通道标准差
		
			for(int j=0; j<height*width*channel; j++){
				if(s > (1.0 / sqrt(height*width*channel))){
					prewhited_frame[num][j] = (tmp_frame[num][j] - m) / s; //减均值除标准差
				}else{
					prewhited_frame[num][j] = (tmp_frame[num][j] - m) / (1.0 / sqrt(height*width*channel));
				}
			}		
	}
	if(Debug){
		std::cout << "*******************识别归一化完毕***************\n";

	}
}


YourProject::YourProject(){

	#ifdef DEBUG
		Debug = 1;
	#endif 
	if(Debug){
		std::cout << "*******************开始构造识别对象***************\n";

	}


}
// 初始化sess graph变量 加载model 
void YourProject::initialize(const std::string &cfg_path,int gpu_id){
	if(Debug){
		std::cout << "*******************开始初始化识别模型***************\n";

	}

  	TF_Status* s = TF_NewStatus();
	
	graph = TF_NewGraph();
	
	// read pb from file
	std::vector<char> model_buf;
	
	std::string frozen_fname = cfg_path + "CigaretteCaseRecognition_model.pb";
		
	load_file(frozen_fname,model_buf);
	TF_Buffer graph_def = {model_buf.data(), model_buf.size(), nullptr};

	// read pb model
	//TF_Buffer graph_def = {cigratettcase_pb, cigratettcase_pb_size, nullptr};

	TF_ImportGraphDefOptions* import_opts = TF_NewImportGraphDefOptions();
	TF_ImportGraphDefOptionsSetPrefix(import_opts, "");
	TF_GraphImportGraphDef(graph, &graph_def, import_opts, s);


	if(TF_GetCode(s) != TF_OK)
	{
		printf("load graph failed!\n Error: %s\n",TF_Message(s));

	}

	TF_SessionOptions* sess_opts = TF_NewSessionOptions();
	
	if(gpu_id == 0){
		const uint8_t config[14] = {0x32,0xc,0x9,0x9a,0x99,0x99,0x99,0x99,0x99,0xc9,0x3f,0x2a,0x1,0x30}; //0.2 gpuid 0
		TF_SetConfig(sess_opts, (void*)config, 14, s);
	}else if(gpu_id == 1){
		const uint8_t config[14] = {0x32,0xc,0x9,0x9a,0x99,0x99,0x99,0x99,0x99,0xc9,0x3f,0x2a,0x1,0x31}; //0.2 gpuid 1
		TF_SetConfig(sess_opts, (void*)config, 14, s);	
	}else if(gpu_id == 2){
		const uint8_t config[14] = {0x32,0xc,0x9,0x9a,0x99,0x99,0x99,0x99,0x99,0xc9,0x3f,0x2a,0x1,0x32}; //0.2 gpuid 2
		TF_SetConfig(sess_opts, (void*)config, 14, s);	
	}else if(gpu_id == 3){
		const uint8_t config[14] = {0x32,0xc,0x9,0x9a,0x99,0x99,0x99,0x99,0x99,0xc9,0x3f,0x2a,0x1,0x33}; //0.2 gpuid 3	
		TF_SetConfig(sess_opts, (void*)config, 14, s);
	}
		
        
	
        assert(TF_GetCode(s) == TF_OK);

	sess = TF_NewSession(graph, sess_opts, s);
	assert(TF_GetCode(s) == TF_OK);
	
	TF_DeleteImportGraphDefOptions(import_opts);
	TF_DeleteSessionOptions(sess_opts);
    	TF_DeleteStatus(s);
	std::vector<char>().swap(model_buf);
	if(Debug){
		std::cout << "*******************初始化识别模型完毕***************\n";

	}

}

// 销毁sess graph 释放内存
void YourProject::cleanup(){
    	if(Debug){
		std::cout << "*******************开始释放识别模型***************\n";

	}
	  TF_Status* s = TF_NewStatus();
	  TF_CloseSession(sess,s);
	  TF_DeleteSession(sess,s);
	  TF_DeleteGraph(graph);
	  TF_DeleteStatus(s);
	 if(Debug){
		std::cout << "*******************释放识别模型完毕***************\n";

	}

}

/* To make tensor release happy...*/
static void dummy_deallocator(void* data, size_t len, void* arg)
{
}

// CigaretteCase_getEmbs
void YourProject::getEmbs(float (*aligned_frame)[height*width*channel], int batch,float (*embedding)[embedding_dim]){
	if(Debug){
		std::cout << "*******************开始获取识别Feature***************\n";

	}
	
	int input_size=batch*height*width*channel;
	float (*frame_buffer)[height*width*channel] = new float[batch][channel*height*width];
	
	prewhite(aligned_frame,batch,frame_buffer);
	

	// set input 
	std::vector<TF_Output> input_names;
	std::vector<TF_Tensor*> input_values;
	
	TF_Operation* input_name=TF_GraphOperationByName(graph, "input");

	input_names.push_back({input_name, 0});

	const int64_t dim[4] = {batch,height,width,channel};

	
	//TF_Tensor* input_tensor = TF_NewTensor(TF_FLOAT, dim, 4, (float **)frame_buffer, sizeof(float)*input_size, dummy_deallocator, nullptr);
	TF_Tensor* input_tensor = TF_NewTensor(TF_FLOAT, dim, 4, frame_buffer, sizeof(float)*input_size, dummy_deallocator, nullptr);
	input_values.push_back(input_tensor);
	
	
	// set bool  yes/no train
	TF_Operation* train_phase_name =TF_GraphOperationByName(graph, "phase_train");

	input_names.push_back({train_phase_name, 0});

	const int64_t train_phase_dim[1] = {};
	bool train_phase_data[1] = {false};

	TF_Tensor* train_phase_tensor = TF_NewTensor(TF_BOOL, train_phase_dim, 0, train_phase_data, sizeof(bool), dummy_deallocator, nullptr);

	input_values.push_back(train_phase_tensor);
	
	// set output
	std::vector<TF_Output> output_names;

	TF_Operation* output_name = TF_GraphOperationByName(graph,"Logits/BiasAdd");
	output_names.push_back({output_name,0});

	std::vector<TF_Tensor*> output_values(output_names.size(), nullptr);
  	TF_Status* s = TF_NewStatus();
	
	// session_run
	TF_SessionRun(sess,nullptr,input_names.data(),input_values.data(),input_names.size(),
			output_names.data(),output_values.data(),output_names.size(),
			nullptr,0,nullptr,s);

	assert(TF_GetCode(s) == TF_OK);
	
	// 输出为 batch*embedding_dims / one dim array
	const float * output_emb = (const float *)TF_TensorData(output_values[0]);

	// #########################################################################################################
	// save sql： if True 注释下面代码　，１＊128
	for(int i=0; i<batch; i++){ 
	   for(int j=0; j<embedding_dim; j++){	
			embedding[i][j] = output_emb[i*embedding_dim+j];
	  	}	
	}
	
	
	// ######################################################################################################## 
	// clear Tensors //需要最后销毁
	TF_DeleteTensor(output_values[0]);
	TF_DeleteTensor(input_tensor);
	TF_DeleteTensor(train_phase_tensor);
    	TF_DeleteStatus(s);
	std::vector<TF_Tensor*>().swap(output_values);
	std::vector<TF_Output>().swap(output_names);
	std::vector<TF_Tensor*>().swap(input_values);
	std::vector<TF_Output>().swap(input_names);
	delete [] frame_buffer;
	 if(Debug){
		std::cout << "*******************获取识别Feature完毕***************\n";

	}
	
	
}	

static void softmax(float *input,float *output){
	
	
	// 二分类
	float b = 0.0;

	for(int i=0;i<embedding_dim;i++){
		//std::cout << exp(a[i]) << " ";
		b=b+exp(input[i]);
		
	}
	//std::cout << "\n";
	//std::cout << b << std::endl;
	
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);
	for(int j=0;j<embedding_dim;j++){
		float c = exp(input[j])/b;
		//std::cout <<  c << std::endl;
		output[j] = c;
		
	}
}
static void sort(float *a, int length, int *b){ // a为要排序的数组 length为数组长度 b为排序后的原未知索引
        int i, j, t1;
        float t;
        for(j = 0;j<length;j++){
                for(i=0;i<length-1-j;i++){
                        if(a[i] < a[i+1]){       // < 倒序   > 升序
                                t = a[i];
                                a[i] = a[i+1];
                                a[i+1] = t;
                                t1 = b[i];
                                b[i] = b[i+1];
                                b[i+1] = t1;
                        }
                }
        }

}



// CigaretteCase_recognition
void YourProject::inference(float (*aligned_frame)[height*width*channel], int batch, std::vector<int> &outputs,std::vector<float> &scores)
{	
	if(Debug){
		std::cout << "*******************开始识别inference***************\n";

	}
        float (*embedding)[embedding_dim] = new float[batch][embedding_dim];  //存放特征
        getEmbs(aligned_frame, batch,embedding); //inference 得到特征
	
	
    	for(int i=0;i<batch;i++){
                for(int n =0;n<embedding_dim;n++){
                //        std::cout << embedding[i][n] << " ";
                }
		//std::cout << "\n";
		float *softmax_op = new float[embedding_dim];   // 存放softmax后概率
		softmax(embedding[i],softmax_op);
		
		for(int n =0;n<embedding_dim;n++){
                       // std::cout << softmax_op[n] << " ";
                }
		//std::cout << "\n";  
		float *softmax_op_tmp = new float[embedding_dim];   // 存放softmax后概率
		int *b = new int[embedding_dim];  // 存放排序后的index 降序
		for(int k =0;k<embedding_dim;k++){
                	b[k] = k;
			softmax_op_tmp[k] = softmax_op[k];
        	}

		
		sort(softmax_op,embedding_dim,b);  // softmax_op 发生改变,需要softmax_op_tmp
		
					
		outputs.push_back(b[0]);
		scores.push_back(softmax_op_tmp[b[0]]);
		
		delete [] b;
		delete [] softmax_op;
		delete [] softmax_op_tmp;
	}



    
        delete [] embedding;
	if(Debug){
		std::cout << "*******************识别inference完毕***************\n";

	}
}

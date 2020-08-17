// code at 2019/7/16
// code by zz

#include <fstream>
#include <vector>
#include <iostream>
#include <string>
//#include "mtcnn_model.h"

static int load_file(const std::string & fname, std::vector<char>& buf)
{
	std::ifstream fs(fname.data(), std::ios::binary | std::ios::in);

	if (!fs.good())
	{
		std::cout << fname << " does not exist" << std::endl;
		return 1;
	}


	fs.seekg(0, std::ios::end);
	int fsize = fs.tellg();

	fs.seekg(0, std::ios::beg);
	buf.resize(fsize);
	fs.read(buf.data(), fsize);

	fs.close();

	return 0;

}

void pb_tram(std::string &fname, std::string &save_path) {
	
	std::vector<char> model_buf;
	load_file(fname, model_buf);


	std::ofstream outfile;
	outfile.open(save_path.data());
	outfile << "const char mtcnn_pb[]={\n";

	char *mtcnn_pb = new char [model_buf.size()];

	for (int i = 0; i < model_buf.size(); i++) {
		
		mtcnn_pb[i] = int(model_buf[i]);
		
		outfile << int(model_buf[i]) << ',';
		if ((i + 1) % 18 == 0) {
			outfile << "\n";
		}
		std::cout << int(mtcnn_pb[i]) << std::endl;

	}


	outfile << "};\n";
	outfile << "const int mtcnn_pb_size = " << model_buf.size() << ';';
	outfile.close();
	delete[] mtcnn_pb;


}
void test_tram(const char *p,const int p_size) {
	std::vector<char> v(p, p + p_size);
	std::cout << v.size() << std::endl;
	std::cout << v.data() << std::endl;
}
int main() {
    //将ｐｂ文件转换为十进制（可读性）的ｃｈａｒ类型数组并保存在头文件中
    std::string fname = "mtcnn_frozen_model.pb";
    std::string save_path = "mtcnn_model.h";
    //测试ｍｏｄｅｌ
    pb_tram(fname,save_path);
    //test_tram(mtcnn_pb,mtcnn_pb_size);
	return 0;
}

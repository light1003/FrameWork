#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "sql.hpp"
#include "../Utils/byte2binarystring.h"
int main(){


	std::string labels = "";
	for(int i=0;i<sql_batch;i++){
			
			labels += std::to_string(sql_labels[i]);
			labels += " ";
	}
	std::cout << labels << std::endl;
	std::string enstr = "";
	Encrypt(labels.c_str(),enstr);
	printf("%s",enstr.c_str());

	return 0;
}

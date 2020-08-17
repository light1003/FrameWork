#include "byte2binarystring.h"
#include <string.h>
#include "sql.hpp"
#include <assert.h>

int main(){
	
	//const char *str = "中国加油";
	std::string labels = "";
	for(int i=0;i<sql_batch;i++){
		//for(int j= 0;j<128;j++){
			
			labels += std::to_string(sql_labels[i]);				
			labels += " ";
		//}
			
	}
	
	
	
	//printf("%s",labels.c_str());
	std::string enstr = "";
	Encrypt(labels.c_str(),enstr);
	//printf(" %s ->",enstr.c_str());
	
	/************* 解密 **************/ //Note: const char * vs char *
	int charLen = strlen(enstr.c_str());
	int byteLen=charLen%8==0?charLen/8:charLen/8+1;
	char *charBuf = new char[charLen];
	for(int i= 0;i<charLen;i++){
		charBuf[i] = enstr.c_str()[i];


	}	
	
	//assert(charBuf[charLen-2]=='5');
	char *destr = new char[byteLen];
	Decrypt(charBuf,destr);
	
	printf(" %s\n",destr);	
	delete [] destr;
	delete [] charBuf;
	
}

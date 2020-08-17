
#include "byte2binarystring.h"


/***********字节流<->二进制**********/
int byteStream2BinaryString(uint8_t byteBuf[],int byteLen,char charBuf[],int charLen,int type){
    if(type!=0&&type!=1){
        return -1;
    }
    //字节流转二进制字符流
    if(type==0){
        if(charLen<8*byteLen)
            return -1;
        int charBufIndex=0;
        for(int i=0;i<byteLen;++i){
            for(int j=7;j>=0;--j){
                if(byteBuf[i]>>j&0x1){
                    charBuf[charBufIndex++]='1';
                }else{
                    charBuf[charBufIndex++]='0';
                }
            }
        }
        return 0;
    }

    //二进制字符流转字节流
    if(type==1){
        if(8*byteLen<charLen)
            return -1;
        char tmpStr[9]="";
        int byteBufIndex=0;
        for(int i=0;i<charLen;i=i+8){
            strncpy(tmpStr,charBuf+i,8);
            for(int j=0;j<8;++j){
                byteBuf[byteBufIndex]+=tmpStr[j]-'0'<<7-j;
            }
            ++byteBufIndex;
        }
        return 0;
    }
}
/************* 加密 **************/
void Encrypt(const char *byteBuf,std::string &enStr){
	int byteLen = strlen(byteBuf);
	
	char* charBuf=new char[byteLen*8];
	int ret=byteStream2BinaryString((uint8_t*)byteBuf,byteLen,charBuf,byteLen*8,0);
	if(ret!=0){
            printf("encrypt failed!\n");
        }
	enStr = charBuf;
	delete [] charBuf;
}
/************* 解密 **************/
void Decrypt(char *charBuf,char *destr){
	int charLen = strlen(charBuf);	
	int byteLen=charLen%8==0?charLen/8:charLen/8+1;
	uint8_t* byteBuf=new uint8_t[byteLen];
	int ret=byteStream2BinaryString(byteBuf,byteLen,charBuf,charLen,1);
	if(ret!=0){
		printf("decrypt failed!\n");
	}
	//printf(" %s\n",(char *)byteBuf);
	for(int i = 0;i<byteLen;i++){
		destr[i] = ((char *)byteBuf)[i];
	}
	delete [] byteBuf;
}

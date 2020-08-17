#ifndef BINARY_EXAMPLES_CIGARETTECASE_BINARY_H_
#define BINARY_EXAMPLES_CIGARETTECASE_BINARY_H_
#include <stdio.h>
#include <string.h>
#include <string>

int byteStream2BinaryString(uint8_t byteBuf[],int byteLen,char charBuf[],int charLen,int type);
void Encrypt(const char *byteBuf,std::string &enStr);
void Decrypt(char *charBuf,char *deStr);
#endif

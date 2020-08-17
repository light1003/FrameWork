#include <stdlib.h>

#include <string.h>
#include "splitstr.h"
/************* 分割float字符串 **************/
void split_str_float(char *str,float *output){
	char c[] = " ";
	char *p = strtok(str,c);
	
	output[0] = atof(p);	
	
	p = strtok(NULL,c);
	
	int count = 1;
	while(p){
		output[count] = atof(p);
		count++;				
		p = strtok(NULL,c); 
	}	
	
}
/************* 分割int字符串 **************/
void split_str_int(char *str,int *output){
	char c[] = " ";
	char *p = strtok(str,c);
		
	output[0] = atoi(p);	
		
	p = strtok(NULL,c);
	
	int count = 1;
	while(p){
		output[count] = atoi(p);
		count++;				
		p = strtok(NULL,c); 
	}	
	
}

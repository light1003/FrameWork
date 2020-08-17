#include <stdio.h>
#include "splitstr.h"
int main(){

	char str_f[50] = "0.123 0.234 0.345"; //TODO:why char* can't?
	float *output_f = new float[3];
	char str_i[50] = "1 2 3";
	int *output_i = new int[3]; 
	
	split_str_float(str_f,output_f);
	
	split_str_int(str_i,output_i);
	
	for(int i = 0;i<3;i++){
		printf("%f ",output_f[i]);
	}
	printf("\n");
	for(int i = 0;i<3;i++){
		printf("%d ",output_i[i]);
	}
	printf("\n");
	delete [] output_f;
	delete [] output_i;

}

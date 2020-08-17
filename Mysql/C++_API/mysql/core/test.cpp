#include <stdio.h>
#include <string>
#include <cstring>
#include "mysql_op.h"

#include "sql.hpp"

bool use_create = 0;
bool use_insert = 0;
bool use_select = 1;

int main(){
	
	Mysql_BaseOp *ms = new Mysql_BaseOp;
	
	if(use_create){
		std::string CreateInformationTable_query = "CREATE TABLE Information(DateTime varchar(50) not null PRIMARY KEY,EmbDims int(8) not null,SqlBatch int(11) not null,ClassNums int(11) not null,Threshold float(3,2) not null,Version float(2,1) not null,Author varchar(8) not null default 'zz')";
		std::string CreateFeatureTable_query = "CREATE TABLE Feature(DateTime varchar(50) not null PRIMARY KEY,SqlEmbs longblob not null,SqlLabels longblob not null)";
		ms->CreateTable(CreateInformationTable_query);
		ms->CreateTable(CreateFeatureTable_query);
	}
	
	if(use_insert){
		// 获取系统当前日期 
		
		struct InformationTable *IfTable = new struct InformationTable;
		struct FeatureTable *FtTable = new struct FeatureTable;
		IfTable->DateTime = datetime;
		FtTable->DateTime = datetime;
		IfTable->EmbDims = embedding_dims;
		IfTable->SqlBatch = sql_batch;
		IfTable->ClassNums = class_nums;
		IfTable->Threshold = threshold;
		IfTable->Version = version;
		IfTable->Author = author;
		FtTable->SqlEmbs = new float *[IfTable->SqlBatch];
		FtTable->SqlLabels = sql_labels;
		
		for (int i = 0; i < IfTable->SqlBatch; i++){
	   		 FtTable->SqlEmbs[i] = new float[IfTable->EmbDims];
		}
		
		for(int i=0; i<IfTable->SqlBatch; i++){
		   for(int j=0; j<IfTable->EmbDims; j++){
		       FtTable->SqlEmbs[i][j] = sql_embs[i][j];
		        }
		}
		
	
		ms->InsertInto(IfTable,FtTable);
		
		
		for (int i = 0; i < IfTable->SqlBatch; i++){
	   		 delete [] FtTable->SqlEmbs[i];
		}
		delete [] FtTable->SqlEmbs;
		delete IfTable;
		delete FtTable;
	}
	if(use_select){
		std::string SelectFromInformation_query = "select * from Information where DateTime = '2020-03-26'";
		std::string SelectFromFeature_query = "select * from Feature where DateTime = '2020-03-26'";
		struct InformationTable *IfTable = new struct InformationTable;
		ms->SelectFrom(SelectFromInformation_query,IfTable);
	
		struct FeatureTable *FtTable = new struct FeatureTable;
		
		FtTable->SqlEmbs = new float *[IfTable->SqlBatch];
		for (int i = 0; i < IfTable->SqlBatch; i++){
	   		 FtTable->SqlEmbs[i] = new float[IfTable->EmbDims];
		}
		FtTable->SqlLabels=new int[IfTable->SqlBatch];
		ms->SelectFrom(SelectFromFeature_query,IfTable->SqlBatch,IfTable->EmbDims,FtTable);
			//DEBUG
		for(int i=0; i<IfTable->SqlBatch; i++){
		
		    	for(int j=0; j< IfTable->EmbDims; j++){
			
		       		printf("%f ",FtTable->SqlEmbs[i][j]);
			}
		}
		printf("\n");
	
		for(int i=0; i<IfTable->SqlBatch; i++){
			printf("%d ",FtTable->SqlLabels[i]);	
		}
		printf("\n");
		printf("%s\n",IfTable->DateTime.c_str());	
		printf("%d\n",IfTable->EmbDims);
		printf("%d\n",IfTable->SqlBatch);
		printf("%d\n",IfTable->ClassNums);
		printf("%f\n",IfTable->Threshold);
		printf("%f\n",IfTable->Version);
		printf("%s\n",IfTable->Author.c_str());
		for (int i = 0; i < IfTable->SqlBatch; i++){
	   		 delete [] FtTable->SqlEmbs[i];
		}
		delete [] FtTable->SqlLabels;
		delete [] FtTable->SqlEmbs;
		delete IfTable;
		delete FtTable;
	}
	delete ms;
	printf("end!\n");
	return 0;
}

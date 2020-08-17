#include <stdio.h>
#include <string.h> //strlen()
#include <assert.h>
#include "mysql_op.h"
//#include "../utils/byte2binarystring.h"
#include "../utils/splitstr.h"

/*************构造函数->连接数据库 **************/
Mysql_BaseOp::Mysql_BaseOp(){
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,UI.HostAddress,UI.User,UI.Passwd,UI.Db,0, NULL, 0)){ 
		printf("Failed to connect to Mysql!\n");
	}else{
		printf("Connected to Mysql successfully!\n");
	}
}
/************析构函数->关闭数据库 **************/
Mysql_BaseOp::~Mysql_BaseOp(){
	mysql_close(&mysql);
}
/*************创建表结构 **************/
void Mysql_BaseOp::CreateTable(std::string &query){
	
	int flag = 0;
	//printf("[%s] made...\n", query.c_str());
	flag=mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
	if(flag) {
		printf("Query failed!\n");
	}else {
		printf("[%s] made...\n", query.c_str());
	}	
}

/*************插入数据**************/
void Mysql_BaseOp::InsertInto(struct InformationTable *IfTable,struct FeatureTable *FtTable){	

	
	std::string end = ")";
	
	// Date
	std::string day = "'" + IfTable->DateTime + "'";

	// embedding_dims
	std::string embedding_dims = std::to_string(IfTable->EmbDims);

	// sql_batch
	std::string batch =  std::to_string(IfTable->SqlBatch);

	// class_nums
	std::string cls = std::to_string(IfTable->ClassNums);

	// threshold 
	std::string thre = std::to_string(IfTable->Threshold);

	// version
	std::string ver = std::to_string(IfTable->Version);

	// author
	std::string auth = "'"+ IfTable->Author + "'";
	
	/* ************InformationQuery***************** */
	std::string informationTable_begin = "INSERT INTO " + UI.InformationTable_name + "("+InformationItems[0] + "," + InformationItems[1] + "," + InformationItems[2] + "," + InformationItems[3] + "," + InformationItems[4] + "," + InformationItems[5] + "," + InformationItems[6]+") VALUES(";
	std::string informationTable_query = informationTable_begin + day  + "," + embedding_dims + "," + batch + "," + cls + "," + thre + "," + ver +  "," + auth + end;
	
	//printf("[%s] made...\n", informationTable_query.c_str());
	CreateTable(informationTable_query);
	// embs

	std::string embs = "";
	for(int i=0;i<IfTable->SqlBatch;i++){
		for(int j=0;j<IfTable->EmbDims;j++){			
			embs += std::to_string(FtTable->SqlEmbs[i][j]);			
			embs += " ";
		}
	}
	
	//printf("%s",embs.c_str());
	/***************加密*********************/
	//std::string byteEmbs = "";
	//Encrypt(embs.c_str(),byteEmbs);
	//printf("%s",byteEmbs.c_str());
	/************************************/
	// labels
	std::string labels = "";
	for(int i=0;i<IfTable->SqlBatch;i++){
			
			labels += std::to_string(FtTable->SqlLabels[i]);
			labels += " ";
	}
	
	
	//std::string byteLabels = "";   // TODO:why can't use?
	//Encrypt(labels.c_str(),byteLabels);
	
	/* ************FeatureQuery***************** */
	std::string featureTable_begin = "INSERT INTO " + UI.FeatureTable_name + "("+FeatureItems[0] + "," + FeatureItems[1]+ "," +FeatureItems[2]+") VALUES(";
	std::string featureTable_query = featureTable_begin + day  + ",'"  + embs +  "','" + labels + "'"+  end; 
	//printf("[%s] made...\n", featureTable_query.c_str());
	CreateTable(featureTable_query);
} 
/************* 查询**************/
void Mysql_BaseOp::SelectFrom(std::string query,struct InformationTable *IfTable){
	
	CreateTable(query);
	/*mysql_store_result讲全部的查询结果读取到客户端*/
	res = mysql_store_result(&mysql);
	/*mysql_fetch_row检索结果集的下一行*/
	while(row = mysql_fetch_row(res)) {
		/*mysql_num_fields返回结果集中的字段数目*/
			//assert(mysql_num_fields(res) ==  9);
		
		//printf("%s\t", row[t]);
		// get date
		IfTable->DateTime = row[0];
		// get embedding_dims
		IfTable->EmbDims = atoi(row[1]);
		const int embedding_dims = IfTable->EmbDims;
		// get sql_batch
		IfTable->SqlBatch = atoi(row[2]);		
		
		// get class_nums
		IfTable->ClassNums = atoi(row[3]);
		
		// get threshold
		IfTable->Threshold = atof(row[4]);
		
		
		// get version
		IfTable->Version = atof(row[5]);
		
		// get author
		IfTable->Author = row[6];		
	}
}
/************* 查询**************/
void Mysql_BaseOp::SelectFrom(std::string query,int SqlBatch,int EmbDims,struct FeatureTable *FtTable){
	
	
	const int embedding_dims = EmbDims;
	CreateTable(query);
	/*mysql_store_result讲全部的查询结果读取到客户端*/
	res = mysql_store_result(&mysql);
	/*mysql_fetch_row检索结果集的下一行*/
	while(row = mysql_fetch_row(res)) {
		
		//printf("%s\t", row[t]);
		// get date		
		FtTable->DateTime = row[0];
			
		
		// get sql_embs
		/************* 解密 **************/
		/*
		int charLen = strlen(row[1]);
		int byteLen=charLen%8==0?charLen/8:charLen/8+1;
		char *charBuf = new char[charLen];
		for(int i= 0;i<charLen;i++){
			charBuf[i] = row[1][i];

		}	
		char *destr = new char[byteLen];
		Decrypt(charBuf,destr);
		
		
		*/
		/********************************/
		
		
		float *tmp_sql_embs = new float[SqlBatch*embedding_dims];
	
		split_str_float(row[1],tmp_sql_embs);

		//delete [] charBuf;
		//delete [] destr;
		for(int i=0; i<SqlBatch; i++){
			
		    	for(int j=0; j<embedding_dims; j++){
				
		       		FtTable->SqlEmbs[i][j] = tmp_sql_embs[i*embedding_dims+j];
				
		        }
	       	}
		
		delete [] tmp_sql_embs;

		// get sql_lables
		split_str_int(row[2],FtTable->SqlLabels);		
	}
}




#include <stdio.h>
#include <mysql.h>
#include <string.h> //strlen()
#include <assert.h>
#include <string>


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

int main()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *query;
	int flag, t;
	mysql_init(&mysql);
	
	if(!mysql_real_connect(&mysql, "localhost", "root", "123456", "zz", 0, NULL, 0)) {
		printf("Failed to connect to Mysql!\n");
		return 0;
	}else {
		printf("Connected to Mysql successfully!\n");
	}
	query = "select * from cigarettecase where author = 'zz'";
	/*查询，成功则返回0*/
	flag = mysql_real_query(&mysql, query, (unsigned int)strlen(query));  // 可以存储二进制文件
	if(flag) {
		printf("Query failed!\n");
		return 0;
	}else {
		printf("[%s] made...\n", query);
	}
	// global 声明
 	char *date;	
	int sql_batch;
	int class_nums;
	float threshold;
	int emb_dims;
	int *sql_labels = nullptr;
	float **sql_embs = nullptr;
	float version;
	char *author;
	/*mysql_store_result讲全部的查询结果读取到客户端*/
	res = mysql_store_result(&mysql);
	/*mysql_fetch_row检索结果集的下一行*/
	while(row = mysql_fetch_row(res)) {
		/*mysql_num_fields返回结果集中的字段数目*/
		assert(mysql_num_fields(res) == 9);
		
		//printf("%s\t", row[t]);
		// get date
		date = row[0];
		
		// get embedding_dims
		const int embedding_dims = atoi(row[1]);
		emb_dims = embedding_dims;
		// get sql_batch
		sql_batch = atoi(row[2]);		
		
		// get class_nums
		class_nums = atoi(row[3]);
		
		// get threshold
		threshold = atof(row[4]);
		
		// get sql_embs
		float *tmp_sql_embs = new float[sql_batch*embedding_dims];
		split_str_float(row[5],tmp_sql_embs);
		sql_embs = new float *[sql_batch];
		for (int i = 0; i < sql_batch; i++){
   		 	sql_embs[i] = new float[embedding_dims];
		}
		for(int i=0; i<sql_batch; i++){
            		for(int j=0; j<embedding_dims; j++){
               			 sql_embs[i][j] = tmp_sql_embs[i*embedding_dims+j];
                	}
       		 }
		delete [] tmp_sql_embs;
		
		// get sql_lables
		sql_labels=new int[sql_batch];
		split_str_int(row[6],sql_labels);
		

		// get version
		version = atof(row[7]);
		
		// get author
		author = row[8];
		
		
		
	}
	
	//DEBUG
	for(int i=0; i<sql_batch; i++){
            	for(int j=0; j< emb_dims; j++){
               		printf("%f ",sql_embs[i][j]);
        	}
	}
	printf("\n");
	
	for(int i=0; i<sql_batch; i++){
		printf("%d ",sql_labels[i]);	
	}
	printf("\n");
	printf("%s\n",date);	
	printf("%d\n",emb_dims);
	printf("%d\n",sql_batch);
	printf("%d\n",class_nums);
	printf("%f\n",threshold);
	printf("%f\n",version);
	printf("%s",author);
	for (int i = 0; i < sql_batch; i++){
   		 delete [] sql_embs[i];
	}
	delete [] sql_labels;
	delete [] sql_embs;
	mysql_close(&mysql);
	return 0;
}



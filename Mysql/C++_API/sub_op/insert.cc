#include <stdio.h>
#include <mysql.h>
#include <string.h> //strlen()
#include <string>
#include "sql.hpp"

int main(int argc,char *argv[]){
	MYSQL mysql;

	std::string query;
	int flag;
	std::string begin;
	
	std::string end = ")";
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,"localhost","root","123456","zz",0, NULL, 0)){ //"root":数据库管理员 "":root密码 "test":数据库的名字
		printf("Failed to connect to Mysql!\n");
		return 0;
	}else{
		printf("Connected to Mysql successfully!\n");
	}
	
	// Date
	std::string date = "20200317";

	// embs
	std::string embs = "'";
	for(int i=0;i<sql_batch;i++){
		
		for(int j=0;j<128;j++){
			
			embs += std::to_string(sql_embs[i][j]);
			embs += " ";
			
			//printf("%s ",std::to_string(sql_embs[i][j]).c_str());
		}
		
		//printf("%s",data.c_str());
		
	}
	embs += "'";
	// labels
	std::string labels = "'";
	for(int i=0;i<sql_batch;i++){
			
			labels += std::to_string(sql_labels[i]);
			labels += " ";
	}
	labels += "'";
	// sql_batch
	std::string batch =  std::to_string(sql_batch);
	// class_nums
	std::string cls = std::to_string(class_nums);
	// threshold 
	std::string thre = std::to_string(threshold);
	// version
	std::string ver = "1.0";
	// author
	std::string auth = "'zz'";
	
	begin = "INSERT INTO cigarettecase(date,sql_embs,sql_labels,sql_batch,class_nums,threshold,version,author) VALUES(";
		
	query = begin + date  + "," + embs + "," + labels + "," + batch + "," + cls + "," + thre + "," + ver + "," + auth + end; 
	printf("[%s] made...\n", query.c_str());
	flag=mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
	if(flag) {
		printf("Query failed!\n");
		return 0;
	}else {
		printf("[%s] made...\n", query.c_str());
	}
		
	mysql_close(&mysql);
	return 0;
 }

#include <stdio.h>
#include <mysql.h>
#include <string.h> //strlen()
#include <string>

int main(int argc,char *argv[]){
	MYSQL mysql;

	std::string query;
	int flag;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,"localhost","root","123456","zz",0, NULL, 0)){ //"root":数据库管理员 "":root密码 "test":数据库的名字
		printf("Failed to connect to Mysql!\n");
		return 0;
	}else{
		printf("Connected to Mysql successfully!\n");
	}

	


	query = "CREATE TABLE cigarettecase(date int(8) not null PRIMARY KEY,sql_embs longblob not null,sql_labels longblob not null,sql_batch int(11) not null,class_nums int(11) not null,threshold float(3,2) not null,version float(2,1) not null,author varchar(8) not null)";
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

#ifndef MYSQL_EXAMPLES_CIGARETTECASE_MYSQL_H_
#define MYSQL_EXAMPLES_CIGARETTECASE_MYSQL_H_

#include <mysql.h>
#include <string>

struct InformationTable{
	std::string DateTime;
	int EmbDims;	
	int SqlBatch;
	int ClassNums;
	float Threshold;
	float Version;
	std::string Author;
};

struct FeatureTable{
	std::string DateTime;
	float **SqlEmbs = nullptr;
	int *SqlLabels = nullptr;
};



struct UserInformation{
	const char *HostAddress;
	const char *User;
	const char *Passwd;
	const char *Db;
	std::string InformationTable_name;
	std::string FeatureTable_name;
	
};

class Mysql_BaseOp{

	public:
		Mysql_BaseOp();
		~Mysql_BaseOp();
		void CreateTable(std::string &query); 	
		void InsertInto(struct InformationTable *IfTable,struct FeatureTable *FtTable);
		void SelectFrom(std::string query,struct InformationTable *IfTable);
		void SelectFrom(std::string query,int SqlBatch,int EmbDims,struct FeatureTable *FtTable);
	private:
		MYSQL mysql;
		MYSQL_RES *res;
		MYSQL_ROW row;
		
		struct UserInformation UI = {"localhost","root","123456","cigarettecase","Information","Feature"};
		std::string InformationItems[7] = {"DateTime","EmbDims","SqlBatch","ClassNums","Threshold","Version","Author"};
		std::string FeatureItems[3] = {"DateTime","SqlEmbs","SqlLabels"};
};
#endif

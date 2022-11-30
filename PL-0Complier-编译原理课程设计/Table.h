#pragma once
#define tableMax 20
enum types{VAR,PROC};
extern int tblptr;
struct Table
{
	char name[tableMax];
	Table* header;
	int offset;
	tableItem* firstItem;
	tableItem* available;
};

struct tableItem
{
	char name[tableMax];
	types type;
	int offset;
	varInformation* varInfo;
	tableInformation* tableInfo;
	tableItem* next;
};
struct varInformation {

};
struct tableInformation {

};


Table* makeTable(Table* ,char[tableMax]); //创建一张新符号表，并且返回指向该表的指针
void enter(Table*,char[tableMax], int,varInformation*);//在指定符号表中创建一个新项，并填入信息
void enterProc(Table*, char[tableMax],tableInformation*);//在符号表中为嵌套符号表建立新项


void errorHandle();





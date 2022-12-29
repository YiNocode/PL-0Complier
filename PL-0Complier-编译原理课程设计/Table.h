#pragma once
#define lengthMax 20
enum class types{VAR,PROC};
enum class varTypes{CONST,VAR};
extern int tblptr;
extern int tmpId;
struct varInfomation;
struct Table;
struct TableItem;
extern Table* table[lengthMax];

struct varInformation {
	varTypes type;
};


struct tableItem
{
	char name[lengthMax];
	types type;
	int offset;
	varInformation* varInfo;
	Table* tablePtr;
	tableItem* next;
};

struct Table
{
	char name[lengthMax];
	Table* header;
	int offset;
	tableItem* firstItem;
	tableItem* available;
};

Table* makeTable(Table* ,const char[lengthMax]); //创建一张新符号表，并且返回指向该表的指针
void enter(Table*,const char[lengthMax], int,varTypes);//在指定符号表中创建一个新项，并填入信息
void enterProc(Table*, const char[lengthMax],Table*);//在符号表中为嵌套符号表建立新项


void errorHandle();
const char* newtemp();




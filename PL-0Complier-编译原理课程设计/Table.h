#pragma once
#define lengthMax 20
enum class types{VAR,PROC};
enum class varTypes{CONST,VAR};
extern int tblptr;
extern int tmpId;
struct varInfomation;
struct Table;
struct tableItem;
extern Table* table[lengthMax];
extern int offset[lengthMax];
extern int Level;
extern int pid;
struct varInformation {
	varTypes type;
	int offset;
	int addr;
};

struct tableItem
{
	char name[lengthMax];
	types type;
	varInformation* varInfo;
	Table* tablePtr;
	tableItem* next;
	int* base;
};

struct Table
{
	char name[lengthMax];
	Table* header;
	int width;
	int* base;
	tableItem* firstItem;
	tableItem* available;
	int level;
	int id;
	int* display;
};

Table* makeTable(Table* ,const char[lengthMax]); //创建一张新符号表，并且返回指向该表的指针
void enter(Table*,const char[lengthMax], int,varTypes);//在指定符号表中创建一个新项，并填入信息
void enterProc(Table*, const char[lengthMax],Table*,int);//在符号表中为嵌套符号表建立新项
tableItem* lookup(const char[lengthMax]);//查找符号表中是否有某一变量
void errorHandle(const char*);
void errorHandle();
int newtemp();
void gen(const char*, int*, int, int*);
void genPcode();
void printPcode();




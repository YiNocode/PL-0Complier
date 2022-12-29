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

Table* makeTable(Table* ,const char[lengthMax]); //����һ���·��ű����ҷ���ָ��ñ��ָ��
void enter(Table*,const char[lengthMax], int,varTypes);//��ָ�����ű��д���һ�������������Ϣ
void enterProc(Table*, const char[lengthMax],Table*);//�ڷ��ű���ΪǶ�׷��ű�������


void errorHandle();
const char* newtemp();




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


Table* makeTable(Table* ,char[tableMax]); //����һ���·��ű����ҷ���ָ��ñ��ָ��
void enter(Table*,char[tableMax], int,varInformation*);//��ָ�����ű��д���һ�������������Ϣ
void enterProc(Table*, char[tableMax],tableInformation*);//�ڷ��ű���ΪǶ�׷��ű�������


void errorHandle();





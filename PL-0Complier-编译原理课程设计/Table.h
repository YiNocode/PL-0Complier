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

Table* makeTable(Table* ,const char[lengthMax]); //����һ���·��ű����ҷ���ָ��ñ��ָ��
void enter(Table*,const char[lengthMax], int,varTypes);//��ָ�����ű��д���һ�������������Ϣ
void enterProc(Table*, const char[lengthMax],Table*,int);//�ڷ��ű���ΪǶ�׷��ű�������
tableItem* lookup(const char[lengthMax]);//���ҷ��ű����Ƿ���ĳһ����
void errorHandle(const char*);
void errorHandle();
int newtemp();
void gen(const char*, int*, int, int*);
void genPcode();
void printPcode();




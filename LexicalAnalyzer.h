#pragma once
#include<string>
#include <utility>
#include<set>
#include<fstream>
#define $ID 30
#define $INT 31
#define $ADD 14	//"+",
#define $SUB 15 //"-",
#define	$MUL 16	 //"*",
#define $DIV 17 //"/",
#define $EQ 18 //"=", 
#define $NEQ 19 //"<>",
#define $LESS 20//"<", 
#define $LESSEQ 21//"<=", 
#define $GREAT 22//">",
#define $GREATEQ 23//">=",
#define $ASSIGN 24//":=", 
#define $LPAR 25//"(",
#define $RPAR 26//")", 
#define $COMMA	27//",",
#define $SEM 28//";",
typedef std::pair<int, int> Token;
extern std::string inputBuffer;	//���뻺����
extern std::string strToken;	//��Ź��ɵ��ʷ��ŵ��ַ���
extern int beginIndex;			//���ָʾ��
extern int seekIndex;			//����ָʾ��
extern char ch;					//�ַ�������������¶�����Դ�����ַ�
void GetChar();
void GetBC();
void Concat();
bool IsLetter();
bool IsDigit();
int Reserve();
void Retract();
int InsertId(std::string);
int InsertConst(std::string);
Token LexicalAnalzer();

#pragma once
#include"PL0Common.h"
#include"Table.h"
extern std::string inputBuffer;	//输入缓冲区
extern std::string strToken;	//存放构成单词符号的字符串
extern int beginIndex;			//起点指示器
extern int seekIndex;			//搜索指示器
extern char ch;					//字符变量，存放最新读进的源程序字符
extern bool sucFlag;			//标记词法分析是否成功
bool GetChar();
void GetBC();
void Concat();
bool IsLetter();
bool IsDigit();
int Reserve();
void Retract();
int InsertId(std::string);
int InsertConst(std::string);
Token LexicalAnalzer();
std::string getWrongToken();


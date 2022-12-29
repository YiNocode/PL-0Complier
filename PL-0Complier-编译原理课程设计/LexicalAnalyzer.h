#pragma once
#include"PL0Common.h"
#include"Table.h"
extern std::string inputBuffer;	//���뻺����
extern std::string strToken;	//��Ź��ɵ��ʷ��ŵ��ַ���
extern int beginIndex;			//���ָʾ��
extern int seekIndex;			//����ָʾ��
extern char ch;					//�ַ�������������¶�����Դ�����ַ�
extern bool sucFlag;			//��Ǵʷ������Ƿ�ɹ�
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


#pragma once
#include"PL0Common.h"
#include"Table.h"
extern std::vector<Token> tokenList;
extern int curIndex;
extern int tokenListLenth; 
extern std::vector<std::string> errBox;
const char* getStr();
extern int Top;
int getInt();
void Prog();		//<prog> �� program <id>��<block>	
void Block();		//<block> �� [<condecl>][<vardecl>][<proc>]<body>
void Condecl();		//<condecl> �� const <const>{,<const>};
void Const();		//<const> �� <id>:=<integer>
void Vardecl();		//<vardecl> �� var <id>{,<id>};
void Proc();		//<proc> �� procedure <id>([<id>{,<id>}]);<block>{;<proc>}
void Body();		//<body> �� begin <statement>{;<statement>}end
void Statement();	/*<statement> �� <id> := <exp>
						| if <lexp> then <statement>[else <statement>]
						| while <lexp> do <statement>
						| call <id>([<exp>{, <exp>}])
						| <body>
						| read(<id>{��<id>})
						| write(<exp>{, <exp>})*/
void Lexp(list**,list**);		//<lexp> �� <exp> <lop> <exp>|odd <exp>
int Exp();			//<exp> �� [+|-]<term>{<aop><term>}
int Term();		//<term> �� <factor>{<mop><factor>}
int Factor();		//<factor>��<id>|<integer>|(<exp>)
char* Lop();			//<lop> �� =|<>|<|<=|>|>=
void Aop();			//<aop> �� + | -
void Mop();			//<mop> �� *|/
void Id();			//<id> �� l{l|d}   (ע��l��ʾ��ĸ)
int Integer();	//<integer> �� d{d}
void ProgramParser();
void Advance();
void ErrorHandle(int);
void AdvanceWithError();
/*
<prog>������ ��<block>���顢������ ��<condecl>������˵�� ��<const>��������
<vardecl>������˵�� ��<proc>���ֳ��� �� <body>��������� ��<statement>����䣻
<exp>�����ʽ ��<lexp>������ ��<term>���� �� <factor>������ ��<aop>���ӷ��������
<mop>���˷�������� <lop>����ϵ�����
odd���жϱ��ʽ����ż�ԡ�
*/
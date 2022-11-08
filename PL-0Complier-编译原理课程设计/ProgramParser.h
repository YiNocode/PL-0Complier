#pragma once
#include"PL0Common.h"
extern std::vector<Token> tokenList;
extern int curIndex;
extern int tokenListLenth;
void Prog();		//<prog> �� program <id>��<block>	
void Block();		//<block> �� [<condecl>][<vardecl>][<proc>]<body>
void Condecl();		//<condecl> �� const <const>{,<const>};
void Const();		//<const> �� <id>:=<integer>
void Vardecl();		//<vardecl> �� var <id>{,<id>};
void Proc();		//<proc> �� procedure <id>��[<id>{,<id>}]��;<block>{;<proc>}
void Body();		//<body> �� begin <statement>{;<statement>}end
void Statement();	/*<statement> �� <id> := <exp>
						| if <lexp> then <statement>[else <statement>]
						| while <lexp> do <statement>
						| call <id>��[<exp>{, <exp>}]��
						| <body>
						| read(<id>{��<id>})
						| write(<exp>{, <exp>})*/
void Lexp();		//<lexp> �� <exp> <lop> <exp>|odd <exp>
bool Exp();			//<exp> �� [+|-]<term>{<aop><term>}
bool Term();		//<term> �� <factor>{<mop><factor>}
bool Factor();		//<factor>��<id>|<integer>|(<exp>)
bool Lop();			//<lop> �� =|<>|<|<=|>|>=
bool Aop();			//<aop> �� + | -
bool Mop();			//<mop> �� *|/
bool Id();			//<id> �� l{l|d}   ��ע��l��ʾ��ĸ��
bool Integer();		//<integer> �� d{d}
void ProgramParser();
void Advance();
/*
<prog>������ ��<block>���顢������ ��<condecl>������˵�� ��<const>��������
<vardecl>������˵�� ��<proc>���ֳ��� �� <body>��������� ��<statement>����䣻
<exp>�����ʽ ��<lexp>������ ��<term>���� �� <factor>������ ��<aop>���ӷ��������
<mop>���˷�������� <lop>����ϵ�����
odd���жϱ��ʽ����ż�ԡ�
*/
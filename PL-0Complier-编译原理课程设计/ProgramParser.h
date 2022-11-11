#pragma once
#include"PL0Common.h"
extern std::vector<Token> tokenList;
extern int curIndex;
extern int tokenListLenth; 
extern std::vector<std::string> errBox;
void Prog();		//<prog> → program <id>；<block>	
void Block();		//<block> → [<condecl>][<vardecl>][<proc>]<body>
void Condecl();		//<condecl> → const <const>{,<const>};
void Const();		//<const> → <id>:=<integer>
void Vardecl();		//<vardecl> → var <id>{,<id>};
void Proc();		//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}
void Body();		//<body> → begin <statement>{;<statement>}end
void Statement();	/*<statement> → <id> := <exp>
						| if <lexp> then <statement>[else <statement>]
						| while <lexp> do <statement>
						| call <id>（[<exp>{, <exp>}]）
						| <body>
						| read(<id>{，<id>})
						| write(<exp>{, <exp>})*/
void Lexp();		//<lexp> → <exp> <lop> <exp>|odd <exp>
void Exp();			//<exp> → [+|-]<term>{<aop><term>}
void Term();		//<term> → <factor>{<mop><factor>}
void Factor();		//<factor>→<id>|<integer>|(<exp>)
void Lop();			//<lop> → =|<>|<|<=|>|>=
void Aop();			//<aop> → + | -
void Mop();			//<mop> → *|/
void Id();			//<id> → l{l|d}   （注：l表示字母）
void Integer();		//<integer> → d{d}
void ProgramParser();
void Advance();
void ErrorHandle(int);
void AdvanceWithError();
/*
<prog>：程序 ；<block>：块、程序体 ；<condecl>：常量说明 ；<const>：常量；
<vardecl>：变量说明 ；<proc>：分程序 ； <body>：复合语句 ；<statement>：语句；
<exp>：表达式 ；<lexp>：条件 ；<term>：项 ； <factor>：因子 ；<aop>：加法运算符；
<mop>：乘法运算符； <lop>：关系运算符
odd：判断表达式的奇偶性。
*/
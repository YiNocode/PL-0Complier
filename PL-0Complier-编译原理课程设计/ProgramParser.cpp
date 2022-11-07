#include"ProgramParser.h"
std::vector<Token> tokenList;
int curIndex;
Token LexicalAnalzer();
void Prog()			//<prog> → program <id>；<block>	
{
	if (tokenList[curIndex].first == $PROGRAM) {
		Advance();
	}
	else {
		RaiseError($KeywordExpectd);
	}
	if (Id()) {
			Advance();
	}
	else {
		RaiseError($IdentifierExpected);
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
		Block();
	}
	else {
		RaiseError($SEMexpected);
	}
}

void Block()		//<block> → [<condecl>][<vardecl>][<proc>]<body>
{
	if (tokenList[curIndex].first == $CONST) {
		Condecl();
	}
	else if (tokenList[curIndex].first == $VAR) {
		Vardecl();
	}
	else if (tokenList[curIndex].first == $PROCEDUCE) {
		Proc();
	}
	if (tokenList[curIndex].first == $BEGIN) {
		Body();
	}
	else {
		RaiseError($BeginExpected);
	}
}

void Condecl()		//<condecl> → const <const>{,<const>};
{
	if (tokenList[curIndex].first == $CONST) {
		Advance();
		Const();
	}
	while (tokenList[curIndex].first == $COMMA) {
		Advance();
		Const();
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		RaiseError($SEMexpected);
	}
	
}

void Const()		//<const> → <id>:=<integer>
{
	if (Id()) {
		Advance();
	}
	else {
		RaiseError($IdentifierExpected);
	}
	if (tokenList[curIndex].first == $ASSIGN) {
		Advance();
	}
	else {
		RaiseError($AssignExpected);
	}
	if (Integer()) {
		Advance();
	}
	else {
		RaiseError($IntegerExpected);
	}
}

void Vardecl()		//<vardecl> → var <id>{,<id>};
{
	if (tokenList[curIndex].first == $VAR) {
		Advance();
	}
	else {
		RaiseError($VarExpected);
	}
	if (Id()) {
		Advance();
	}
	else {
		RaiseError($IdentifierExpected);
	}
	while (tokenList[curIndex].first == $COMMA) {
			Advance();
			if (Id()) {
				Advance();
			}
			else {
				RaiseError($IdentifierExpected);
			}
		}
	if (tokenList[curIndex].first == $SEM) {
			Advance();
	}
	else {
		RaiseError($SEMexpected);		
	}
}

void Proc()		//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}
{
	if (tokenList[curIndex].first == $PROCEDUCE) {
		Advance();
	}
	if (Id()) {
		Advance();
	}
	if (tokenList[curIndex].first == $LPAR) {
		Advance();
	}
	else {
		RaiseError($LparExpected);
	}
	if (Id()) {
		Advance();
		while (tokenList[curIndex].first == $COMMA) {
			Advance();
			if (Id()) {
				Advance();
			}
			else {
				RaiseError($IdentifierExpected);
			}
		}
	}
	if (tokenList[curIndex].first == $RPAR) {
			Advance();
	}
	else {
		RaiseError($RparExpected);
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		RaiseError($SEMexpected);
	}
	Block();
	while (tokenList[curIndex].first == $SEM) {
		Advance();
		Proc();
	}
}

void Body()		//<body> → begin <statement>{;<statement>}end
{
	if (tokenList[curIndex].first == $BEGIN) {
		Advance();
	}
	else {
		RaiseError($BeginExpected);
	}
	Statement();
	while (tokenList[curIndex].first == $SEM) {
		Advance();
		Statement();
	}
	if (tokenList[curIndex].first == $END) {
		Advance();
	}
	else {
		RaiseError($EndExpected);
	}
}
	
void Statement()		/*
						<statement> → <id> := <exp>
						| if <lexp> then <statement>[else <statement>]
						| while <lexp> do <statement>
						| call <id>（[<exp>{, <exp>}]）
						| <body>
						| read(<id>{，<id>})
						| write(<exp>{, <exp>})
						*/
{
	if (Id()) {
		if (tokenList[curIndex].first == $ASSIGN) {
			Advance();
		}
		else {
			RaiseError($AssignExpected);
		}
			Exp();
	}
	else if (tokenList[curIndex].first == $IF) {
		Advance();
		Lexp();
		if (tokenList[curIndex].first == $THEN) {
			Advance();
		}
		else { 
			RaiseError($ThenExpected);
		}
		Statement();
		if (tokenList[curIndex].first == $ELSE) {
			Advance();
			Statement();
		}
	}
	else if (tokenList[curIndex].first == $WHILE) {
		Advance();
		Lexp();
		if (tokenList[curIndex].first == $DO) {
			Advance();
		}
		else {
			RaiseError($DoExpected);
		}
		Statement();
	}
	else if (tokenList[curIndex].first == $CALL) {
		Advance();
		if (Id()) {
			Advance();
		}
		else {
			RaiseError($IdentifierExpected);
		}
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		else {
			RaiseError($LparExpected);
		}
		if (Exp()) {
			Advance();
			while (tokenList[curIndex].first == $COMMA) {
				if (Exp()) {
					Advance();
				}
				else {
					RaiseError($ExpressionExpected);
				}
			}
		}
		if (tokenList[curIndex].first == $RPAR) {
				Advance();
		}
		else {
			RaiseError($RparExpected);
		}
	}
	else if (tokenList[curIndex].first == $BEGIN) {
		Body();
	}
	else if (tokenList[curIndex].first == $READ) {
		Advance();
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		else {
			RaiseError($LparExpected);
		}
		if (Id()) {
			Advance();
			while (tokenList[curIndex].first == $COMMA) {
				Advance();
				if (Id()) {
					Advance();
				}
				else {
					RaiseError($IdentifierExpected);
				}
			}
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else {
			RaiseError($RparExpected);
		}
	}
	else if (tokenList[curIndex].first == $WRITE) {
		Advance();
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		else {
			RaiseError($LparExpected);
		}
		if (Exp()) {
			Advance();
			while (tokenList[curIndex].first == $COMMA) {
				Advance();
				if (Exp()) {
					Advance();
				}
				else {
					RaiseError($ExpressionExpected);
				}
			}
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else {
			RaiseError($RparExpected);
		}
	}
	else {
		RaiseError($UndefinedError);
	}
}

void Lexp()		//<lexp> → <exp> <lop> <exp>|odd <exp>
{
	if (tokenList[curIndex].first == $ODD) {
		Advance();
		if (Exp()) {
			Advance();
		}
		else {
			RaiseError($InvalidExpression);
		}
	}
	else if (Exp()) {
		Advance();
		if (Lop()) {
			Advance();
		}
		else {
			RaiseError($InvalidExpression);
		}
		if (Exp()) {
			Advance();
		}
		else {
			RaiseError($InvalidExpression);
		}
	}
}

bool Exp()			//<exp> → [+|-]<term>{<aop><term>}
{	
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		Advance();
	}
	Term();
	while (Aop()) {
		Advance();
		Term();
	}
	return true;
}

void Term()			//<term> → <factor>{<mop><factor>}
{
	Factor();
	while (Mop()) {
		Advance();
		Factor();
	}
}

void Factor()			//<factor>→<id>|<integer>|(<exp>)
{
	if (Id()) {
		Advance();
	}
	else if (Integer()) {
		Advance();
	}
	else if (Exp()) {
		Advance();
	}
	else {
		RaiseError($InvalidExpression);
	}
}

bool Lop()
{
	if (tokenList[curIndex].first == $EQ || tokenList[curIndex].first == $NEQ || tokenList[curIndex].first == $LESS || tokenList[curIndex].first == $LESSEQ || tokenList[curIndex].first == $GREAT || tokenList[curIndex].first == $GREATEQ) {
		return true;
	}
	else
	return false;
}

bool Aop()
{
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		return true;
	}
	else {
		return false;
	}
}

bool Mop()
{
	if (tokenList[curIndex].first == $MUL || tokenList[curIndex].first == $DIV) {
		return true;
	}
	else {
		return false;
	}
}

bool Id()
{
	if(tokenList[curIndex].first==$ID)
	return true;
	else {
		return false;
	}
}

bool Integer()
{
	if(tokenList[curIndex].first==$INT)
	return true;
	else {
		return false;
	}
}
void Advance() {
	Token temp = LexicalAnalzer();
	if (temp.first == -2) {
		std::cout << "分析完成！" << '\n';
	}
	if (temp.first == -1) {
		std::cout << "分析失败！" << '\n';
	}
	std::cout << "<" << temp.first << "," << temp.second << ">" << '\n';
	tokenList.push_back(temp);
	curIndex++;
}
void ProgramParser() {
	curIndex = -1;
	Advance();
	Prog();
	
}

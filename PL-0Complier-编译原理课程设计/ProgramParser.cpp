#include"ProgramParser.h"
std::vector<Token> tokenList;
int curIndex;
int tokenListLenth;
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
	if (tokenList[curIndex].first == $VAR) {
		Vardecl();
	}
	if (tokenList[curIndex].first == $PROCEDUCE) {
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
		Advance();
		if (tokenList[curIndex].first == $ASSIGN) {
			Advance();
		}
		else {
			RaiseError($AssignExpected);
		}
		if (Exp()) {
			Advance();
		}
		else {
			RaiseError($ExpressionExpected);
		}
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
	if (Term())
	{
		Advance();
		if (Aop()) {
			while (Aop()) {
				Advance();
				if (Term())
				{
					Advance();
				}
				else {
					curIndex--;
					return false;
				}
			}
			curIndex--;
			return true;
		}
		else {
			curIndex--;
			return true;
		}
		
	}
	else {
		return false;
	}
}

bool Term()			//<term> → <factor>{<mop><factor>}
{
	if (Factor()) {
		Advance();
		if (Mop()) {
			while (Mop()) {
				Advance();
				if (Factor()) {
					Advance();
				}
				else {
					curIndex--;
					return false;
				}
			}
			curIndex--;
			return true;
		}
		else {
			curIndex--;
			return true;
		}
		
	}
	else
		return false;
}

bool Factor()			//<factor>→<id>|<integer>|(<exp>)
{
	if (Id()) {
		return true;
	}
	else if (Integer()) {
		return true;
	}
	else if (Exp()) {
		return true;
	}
	else {
		return false;
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
	if (isFinish) {
		if (curIndex < tokenListLenth) {
			curIndex++;
		}
	}
	else {
		if (curIndex == tokenListLenth - 1) {
				Token temp = LexicalAnalzer();
				std::cout << "<" << temp.first << "," << temp.second << ">" << '\n';
				tokenList.push_back(temp);
				tokenListLenth++;
				curIndex++;
				if (temp.first == -1) {
					std::cout << "词法分析出错！" << '\n';
				}
				if (temp.first == -2) {
					std::cout << "读取完成";
					isFinish = true;
				}
		}
		else {
			if (curIndex < tokenListLenth) {
				curIndex++;
			}
		}
	}

}
void ProgramParser() {
	curIndex = -1;
	Advance();
	Prog();
	
}

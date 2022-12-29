#include"ProgramParser.h"
std::vector<Token> tokenList;
int curIndex;
int tokenListLenth;
Token LexicalAnalzer();
std::string getWrongToken();
std::vector<std::string> errBox;
const char* getStr()
{
	return name_token[tokenList[curIndex - 1].second].c_str();
}
void Prog()			//<prog> → program <id>；<block>	
{
	if (tokenList[curIndex].first == $PROGRAM) {
		Advance();
	}
	else {
		ErrorHandle($KeywordExpectd);
	}
	Id();
	makeTable(NULL, getStr());
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		ErrorHandle($SEMexpected);
	}
	
	Block();

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
	Body();
}

void Condecl()		//<condecl> → const <const>{,<const>};
{
	if (tokenList[curIndex].first == $CONST) {
		Advance();
	}
	else {
		ErrorHandle($ConstExpected);
	}
	Const();
	while (tokenList[curIndex].first == $COMMA) {
			Advance();
			Const();
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		ErrorHandle($SEMexpected);
	}
}

void Const()		//<const> → <id>:=<integer>
{
	Id();
	enter(table[tblptr - 1], getStr(), 4, varTypes::CONST);
	if (tokenList[curIndex].first == $ASSIGN) {
		Advance();
	}
	else {
		ErrorHandle($AssignExpected);
	}
	Integer();
	
}

void Vardecl()		//<vardecl> → var <id>{,<id>};
{
	if (tokenList[curIndex].first == $VAR) {
		Advance();
	}
	else {
		ErrorHandle($VarExpected);
	}
	Id();
	enter(table[tblptr - 1], getStr(), 4, varTypes::VAR);
	while (tokenList[curIndex].first == $COMMA ) {
		Advance();
		Id();
		enter(table[tblptr - 1], getStr(), 4, varTypes::VAR);
		if (tokenList[curIndex].first == $SEM)break;
		if (tokenList[curIndex].first != $COMMA) {
				ErrorHandle($CommaExpected);
		}
			
			
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		ErrorHandle($SEMexpected);
	}
}

void Proc()		//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}
{
	if (tokenList[curIndex].first == $PROCEDUCE) {
		Advance();
	}
	else {
		ErrorHandle($ProcedureExpected);
	}
	Id();
	enterProc(table[tblptr-1],getStr(), makeTable(table[tblptr - 1], getStr()));
	if (tokenList[curIndex].first == $LPAR) {
		Advance();
	}
	else {
		ErrorHandle($LparExpected);
	}
	if (tokenList[curIndex].first == $RPAR) {
		Advance();
	}
	else if(tokenList[curIndex].first == $ID){
		Id();
		while (tokenList[curIndex].first == $COMMA||tokenList[curIndex].first==$ID) {
			Advance();
			Id();
			if (tokenList[curIndex].first == $RPAR)break;
			if (tokenList[curIndex].first != $COMMA) {
				ErrorHandle($CommaExpected);
			}


		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else {
			ErrorHandle($RparExpected);
		}
	}
	if (tokenList[curIndex].first == $SEM) {
		Advance();
	}
	else {
		ErrorHandle($SEMexpected);
	}
	Block();
	while (tokenList[curIndex].first == $SEM) {
		Advance();
		tblptr--;
		Proc();
	}
}

void Body()		//<body> → begin <statement>{;<statement>}end
{
	if (tokenList[curIndex].first == $BEGIN) {
		Advance();
	}
	else {
		ErrorHandle($BeginExpected);
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
		ErrorHandle($EndExpected);
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
	if (tokenList[curIndex].first == $ID) {
		Id();
		if (tokenList[curIndex].first == $ASSIGN) {
			Advance();
		}
		else {
			ErrorHandle($AssignExpected);
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
			ErrorHandle($ThenExpected);
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
			Statement();
		}
		else {
			ErrorHandle($DoExpected);
		}
	}
	else if (tokenList[curIndex].first == $CALL) {
		Advance();
		Id();
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			Exp();
			while (tokenList[curIndex].first == $COMMA || tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
				Advance();
				Exp();
			}
			if (tokenList[curIndex].first == $RPAR) {
				Advance();
			}
			else {
				ErrorHandle($RparExpected);
			}
		}
	}
	else if (tokenList[curIndex].first == $BEGIN) {
		Body();
	}
	else if (tokenList[curIndex].first == $READ) {
		Advance();
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
			Id();
			while (tokenList[curIndex].first == $COMMA) {
				Advance();
				Id();
				if (tokenList[curIndex].first == $RPAR)break;
				if (tokenList[curIndex].first != $COMMA) {
					ErrorHandle($CommaExpected);
				}
			}
			if (tokenList[curIndex].first == $RPAR) {
				Advance();
			}
			else {
				ErrorHandle($RPAR);
			}
		}
		else {
			ErrorHandle($LparExpected);
		}
	}
	else if (tokenList[curIndex].first == $WRITE) {
		Advance();
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		else {
			ErrorHandle($LparExpected);
		}
		Exp();
		while (tokenList[curIndex].first == $COMMA|| tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			Advance();
			Exp();
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else {
			ErrorHandle($RparExpected);
		}
	}
	else {
	ErrorHandle($StatementExpected);
}
}

void Lexp()		//<lexp> → <exp> <lop> <exp>|odd <exp>
{
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
		Exp();
		Lop();
		Exp();
	}
	else if (tokenList[curIndex].first == $ODD) {
		Advance();
		Exp();
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

void Exp()			//<exp> → [+|-]<term>{<aop><term>}
{	
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		Advance();
	}
	Term();
	while(tokenList[curIndex].first == $ADD||tokenList[curIndex].first==$SUB){
		Aop();
		Term();
	}
}

void Term()			//<term> → <factor>{<mop><factor>}
{
	Factor();
	while (tokenList[curIndex].first == $MUL || tokenList[curIndex].first == $DIV) {
		Mop();
		Factor();
	}
}

void Factor()			//<factor>→<id>|<integer>|(<exp>)
{
	if (tokenList[curIndex].first == $ID) {
		Id();
	}
	else if (tokenList[curIndex].first == $INT) {
		Integer();
	}
	else if (tokenList[curIndex].first == $LPAR) {
		Advance();
		Exp();
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
		}
		else {
			ErrorHandle($RparExpected);
		}
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

void Lop()
{
	if (tokenList[curIndex].first == $EQ || tokenList[curIndex].first == $NEQ || tokenList[curIndex].first == $LESS || tokenList[curIndex].first == $LESSEQ || tokenList[curIndex].first == $GREAT || tokenList[curIndex].first == $GREATEQ) {
		Advance();
	}
	else
		ErrorHandle($InvalidExpression);
}

void Aop()
{
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		Advance();
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

void Mop()
{
	if (tokenList[curIndex].first == $MUL || tokenList[curIndex].first == $DIV) {
		Advance();
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

void Id()
{
	if (tokenList[curIndex].first == $ID)
	{
		Advance();

	}
	else {
		ErrorHandle($IdentifierExpected);
	}
}

void Integer()
{
	if (tokenList[curIndex].first == $INT)
		Advance();
	else {
		ErrorHandle($IntegerExpected);
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
				tokenList.push_back(temp);
				tokenListLenth++;
				curIndex++;
				std::cout<<"<code,value>:\t" << "<" << tokenList[curIndex].first << "," << tokenList[curIndex].second << ">" << '\n';
				if (temp.first == -1) {
					std::cout << "词法分析出错！" << '\n';
					ErrorHandle($UndefinedSymbol);
					return;
				}
				if (temp.first == -2) {
					if (!isError) {
						std::cout << "读取完成";
						isFinish = true;
					}
					else {
						std::cout << "读取完成,有不合法语句";
						isFinish = true;
					}
					
				}
		}
		else {
			if (curIndex < tokenListLenth) {
				curIndex++;
			}
		}
	}

}
void ErrorHandle(int errCode)
{
	if (ErrorEnd) return;
	isError = true;
	ErrorEnd = false;
	switch (errCode)
	{
	case 0: {
		std::cout << "Error:Statement Expected " << "\trow:" << row << " col:" << col << '\n';
		do{
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
		} while (tokenList[curIndex].first != $END &&
			tokenList[curIndex].first != $BEGIN &&
			tokenList[curIndex].first != $SEM &&
			tokenList[curIndex].first != $ID &&
			tokenList[curIndex].first != $IF &&
			tokenList[curIndex].first != $WHILE &&
			tokenList[curIndex].first != $CALL &&
			tokenList[curIndex].first != $READ &&
			tokenList[curIndex].first != $WRITE &&
			tokenList[curIndex].first != $ELSE);
	}break;
	case 1: {
		std::cout << "Error:Undefined Symbol  " <<" '" <<getWrongToken()<<"'"<< "\trow:" << row << " col:" << col << '\n';
		Advance();
		
	}break;
	case 2: {
		std::cout << "Error: Illegal Identifier " << "\trow:" << row << " col:" << col << '\n';
		
	}break;
	case 3: {
		std::cout << "Error: Keyword 'program' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
		} while (tokenList[curIndex].first != $PROGRAM);
	}break;
	case 4: {
		std::cout << "Error: ';' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
		} while (tokenList[curIndex].first != $VAR && tokenList[curIndex].first != $PROCEDUCE && tokenList[curIndex].first != $BEGIN);
		
	}break;
	case 5: {
		std::cout << "Error: 'begin' Expected" << "\trow:" << row << " col:" << col << '\n';
		do{
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $ID && tokenList[curIndex].first != $IF && tokenList[curIndex].first != $WHILE && tokenList[curIndex].first != $CALL && tokenList[curIndex].first != $BEGIN && tokenList[curIndex].first != $READ && tokenList[curIndex].first != $WRITE);
	}break;
	case 6: {
		std::cout << "Error: ':=' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $ID && tokenList[curIndex].first != $COMMA && tokenList[curIndex].first != $SEM);
	}break;
	case 7: {
		std::cout << "Error: 'var' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
		} while (tokenList[curIndex].first != $VAR && tokenList[curIndex].first != $PROCEDUCE);
	}break;
	case 8: {
		std::cout << "Error: 'procedure' Expected" << "\trow:" << row << " col:" << col << '\n';
		do{
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
		} while (tokenList[curIndex].first != $PROCEDUCE && tokenList[curIndex].first != $BEGIN && tokenList[curIndex].first != $SEM);
	}break;
	case 9: {
		std::cout << "Error: '(' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $LPAR);
	}break;
	case 10: {
		std::cout << "Error: ')' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $RPAR);
	}break;
	case 11: {
		std::cout << "Error: 'end' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}

		} while (tokenList[curIndex].first != $END && tokenList[curIndex].first != $BEGIN && tokenList[curIndex].first != $READ);
	} break;
	case 12: {
		std::cout << "Error: 'then' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $THEN && tokenList[curIndex].first != $ELSE && tokenList[curIndex].first != $SEM && tokenList[curIndex].first != $END && tokenList[curIndex].first!= $ID && tokenList[curIndex].first != $IF && tokenList[curIndex].first!= $WHILE && tokenList[curIndex].first!=	$CALL && tokenList[curIndex].first!=$BEGIN && tokenList[curIndex].first != $READ && tokenList[curIndex].first!=$WRITE);
	}break;
	case 13: {
		std::cout << "Error: 'do' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $THEN && tokenList[curIndex].first != $ELSE && tokenList[curIndex].first != $SEM && tokenList[curIndex].first != $END && tokenList[curIndex].first != $ID && tokenList[curIndex].first != $IF && tokenList[curIndex].first != $WHILE && tokenList[curIndex].first != $CALL && tokenList[curIndex].first != $BEGIN && tokenList[curIndex].first != $READ && tokenList[curIndex].first != $WRITE);
	}break;
	case 14: {
		std::cout << "Error: Identifier Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $ID);
	}break;
	case 15: {
		std::cout << "Error: Integer Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $INT);
	}break;
	case 16: {
		std::cout << "Error: Expression Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $EQ &&
			tokenList[curIndex].first != $NEQ &&
			tokenList[curIndex].first != $LESS &&
			tokenList[curIndex].first != $LESSEQ &&
			tokenList[curIndex].first != $GREAT &&
			tokenList[curIndex].first != $GREATEQ &&
			tokenList[curIndex].first != $ID &&
			tokenList[curIndex].first != $INT &&
			tokenList[curIndex].first != $ODD &&
			tokenList[curIndex].first != $ADD &&
			tokenList[curIndex].first != $SUB &&
			tokenList[curIndex].first != $LPAR &&
			tokenList[curIndex].first != $COMMA);
	}break;
	case 17: {
		std::cout << "Error:Invalid Expression" << "\trow:" << row << " col:" << col << '\n';
	}break;
	case 18: {
		std::cout << "Error:',' Expected" << "\trow:" << row << " col:" << col << '\n';
		do {
			AdvanceWithError();
			if (ErrorEnd) {
				break;
			}
			
		} while (tokenList[curIndex].first != $COMMA && tokenList[curIndex].first != $ID && tokenList[curIndex].first != $ADD && tokenList[curIndex].first != $SUB && tokenList[curIndex].first != $INT && tokenList[curIndex].first != $LPAR && tokenList[curIndex].first != $SEM);
	}break;
	case 19: {
		std::cout << "Error:'Const' Expected" << "\trow:" << row << " col:" << col << '\n';
	}
	}
}
void AdvanceWithError()
{
	if (isFinish) {
		ErrorEnd = true;
		return;
		if (curIndex < tokenListLenth) {
			curIndex++;
		}
	}
	else {
		if (curIndex == tokenListLenth - 1) {
			Token temp = LexicalAnalzer();
			tokenList.push_back(temp);
			tokenListLenth++;
			curIndex++;
			if (temp.first == -1) {
				std::cout << "词法分析出错！" << '\n';
				return;
			}
			if (temp.first == -2) {
				std::cout << "读取完成,有无法跳过的错误";
				isFinish = true;
				return;
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
	std::ofstream out("C:\\Lyn\\Personal\\Study\\编译原理\\PL-0Complier-编译原理课程设计\\p.txt");
	curIndex = -1;
	Advance();
	Prog();
	if (!isFinish) {
		std::cout << "遇到无法跳过的错误！" << '\n';
	}
	out.close();
}

#include"ProgramParser.h"
#include <cassert>
std::vector<Token> tokenList;
int curIndex;
int tokenListLenth;
Token LexicalAnalzer();
std::string getWrongToken();
std::vector<std::string> errBox;
char Op[6];
int* Arg1;
int* Arg2;
int* Result;
int Top;
list* plist = makelist(0);
const char* getStr()
{
	return name_token[tokenList[curIndex - 1].second].c_str();
}
int getInt() {
	return tokenList[curIndex - 1].second + 300;
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
	tableItem* tmp = lookup(getStr());
	if (tmp == NULL) {
		errorHandle();
	}
	if (tokenList[curIndex].first == $ASSIGN) {
		Advance();
	}
	else {
		ErrorHandle($AssignExpected);
	}
	int t = Integer();
	//std::cout << "常数值：" << *(getIntPtr()) << std::endl;
	Emit(":=",t,0,tmp->varInfo->offset);
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

void Proc()		//<proc> → procedure <id>([<id>{,<id>}]);<block>{;<proc>}
{
	if (tokenList[curIndex].first == $PROCEDUCE) {
		Advance();
		Level++;
		plist = merge(plist, makelist(nextquad));
		Emit( "j", NULL, NULL, NULL);
	}
	else {
		ErrorHandle($ProcedureExpected);
	}
	Id();
	enterProc(table[tblptr-2],getStr(), makeTable(table[tblptr - 1], getStr()),nextquad);
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
		enter(table[tblptr - 1], getStr(), 4, varTypes::VAR);
		while (tokenList[curIndex].first == $COMMA||tokenList[curIndex].first==$ID) {
			Advance();
			Id();
			enter(table[tblptr - 1], getStr(), 4, varTypes::VAR);
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
	Emit("ret", 0, 0, 0);
	Level--;
	if (tokenList[curIndex].first == $SEM) {
		while (tokenList[curIndex].first == $SEM) {
			Advance();
			tblptr--;
			offset[tblptr] = 0;
			Proc();
		}	
	}
	else {
		tblptr--;
		offset[tblptr] = 0;
	}
	backpatch(plist, nextquad);

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
						| call <id>([<exp>{, <exp>}])
						| <body>
						| read(<id>{，<id>})
						| write(<exp>{, <exp>})
						*/
{
	int M, M1, M2, N = 0;
	list* nextlist = makelist(0);
	if (tokenList[curIndex].first == $ID) {
		Id();
		tableItem* tmp = lookup(getStr());
		if (!tmp)
			errorHandle();
		if (tokenList[curIndex].first == $ASSIGN) {
			Advance();
		}
		else {
			ErrorHandle($AssignExpected);
		}		
		Emit( ":=", Exp(), NULL, tmp->varInfo->offset);

	}
	else if (tokenList[curIndex].first == $IF) {
		Advance();
		list* falselist;
		list* truelist;
		Lexp(&truelist,&falselist);
		if (tokenList[curIndex].first == $THEN) {
			Advance();
			M1 = nextquad;
		}
		else {
			ErrorHandle($ThenExpected);
		}
		Statement();
		N = nextquad;
		if (tokenList[curIndex].first == $ELSE) {
			Advance();
			M2 = nextquad;
			Statement();
			backpatch(falselist, M2);
		}
		backpatch(truelist, M1);
		M = nextquad;
		nextlist = merge(falselist, nextlist);
		backpatch(nextlist, M);
	}
	else if (tokenList[curIndex].first == $WHILE) {
		Advance();
		list* falselist;
		list* truelist;
		M1 = nextquad;
		Lexp(&truelist,&falselist);
		if (tokenList[curIndex].first == $DO) {
			Advance();
			M2 = nextquad;
			Statement();
			backpatch(truelist, M2);
			Emit( "j", NULL, NULL, M1);
			backpatch(falselist, nextquad);
		}
		else {
			ErrorHandle($DoExpected);
		}
	}
	else if (tokenList[curIndex].first == $CALL) {
		Advance();
		Id();
		tableItem* t = lookup(getStr());
		int codeId = 0;
		int sp = 0;
		codeId = lookup(getStr())->tablePtr->id;
		
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
			Emit("call", t->tablePtr->width,t->tablePtr->display[t->tablePtr->level], codeId);
		}
		else if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			int queue[lengthMax] = {0};
			int head = 0;
			int tail = 0;
			queue[tail] = Exp();
			Emit( "par",NULL, NULL, queue[tail]);
			tail++;
			while (tokenList[curIndex].first == $COMMA || tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
				Advance();
				queue[tail] = Exp();
				Emit( "par", NULL, NULL, queue[tail]);
				tail++;
			}
			if (tokenList[curIndex].first == $RPAR) {
				Advance();
				Emit("call", t->tablePtr->width, t->tablePtr->display[t->tablePtr->level], codeId);
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
			Emit("read", lookup(getStr())->varInfo->offset, 0, 0);
			while (tokenList[curIndex].first == $COMMA) {
				Advance();
				Id();
				Emit("read", lookup(getStr())->varInfo->offset, 0, 0);
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
		Emit("write", Exp(), 0, 0);
		while (tokenList[curIndex].first == $COMMA|| tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			Advance();
			Emit("write", Exp(), 0, 0);
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

void Lexp(list** t,list** f)		//<lexp> → <exp> <lop> <exp>|odd <exp>
{
	int id1;
	int id2;
	char* relop = new char[10];
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
		id1 = Exp();
		strcpy_s(relop,2,"j");
		strcat_s(relop, 4, Lop());
		id2 = Exp();
		*t = makelist(nextquad);
		*f = makelist(nextquad + 1);
		Emit( relop, id1, id2, NULL);
		Emit( "j", NULL, NULL, NULL);
		
	}
	else if (tokenList[curIndex].first == $ODD) {
		Advance();
		id1 = Exp();
		*t = makelist(nextquad);
		*f = makelist(nextquad + 1);
		Emit( "odd", id1, NULL, NULL);
		Emit( "j", NULL, NULL, NULL);
		
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

int Exp()			//<exp> → [+|-]<term>{<aop><term>}
{	
	char op[6];
	int term1;
	int term2;
	int exp = NULL;
	int exp1;
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		if (tokenList[curIndex].first == $SUB) {
			strcpy_s(op,"-\0");
		}
		Advance();
	}
	term1 = Term();
	exp1 = term1;
	exp = exp1;
	while(tokenList[curIndex].first == $ADD||tokenList[curIndex].first==$SUB){
		if (tokenList[curIndex].first == $SUB) {
			strcpy_s(op, "-\0");
		}
		else
			strcpy_s(op, "+\0");
		exp = newtemp();
		Aop();
		term2 = Term();
		Emit( op, exp1, term2, exp);
		exp1 = exp;
	}
	if (strcmp(op, "-") == 0) {
		exp = newtemp();
		Emit( op, exp1, NULL, exp);
		return exp;
	}
	return exp;
}

int Term()			//<term> → <factor>{<mop><factor>}
{
	int factor1;
	int factor2;
	char op[6];
	int term;
	int term1;
	factor1 = Factor();
	term = factor1;
	term1 = factor1;
	while (tokenList[curIndex].first == $MUL || tokenList[curIndex].first == $DIV) {
		if(tokenList[curIndex].first == $MUL)
			strcpy_s(op, "*\0");
		else
			strcpy_s(op, "/\0");
		term = newtemp();
		Mop();
		factor2 = Factor();
		Emit( op, term1, factor2, term);
		term1 = term;
	}
	return term;
}

int Factor()			//<factor>→<id>|<integer>|(<exp>)
{
	if (tokenList[curIndex].first == $ID) {
		Id();
		tableItem* tmp = lookup(getStr());
		return (tmp->varInfo->offset);
	}
	else if (tokenList[curIndex].first == $INT) {
		return Integer();
	}
	else if (tokenList[curIndex].first == $LPAR) {
		Advance();
		int t = Exp();
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
			return t;
		}
		else {
			ErrorHandle($RparExpected);
		}
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

char* Lop() //< lop > → = | <> | <|<=|>|>=
{
	char* relopPtr = new char[6];
	if (tokenList[curIndex].first == $EQ || tokenList[curIndex].first == $NEQ || tokenList[curIndex].first == $LESS || tokenList[curIndex].first == $LESSEQ || tokenList[curIndex].first == $GREAT || tokenList[curIndex].first == $GREATEQ) {
		switch (tokenList[curIndex].first)
		{
		case $EQ:
			strcpy_s(relopPtr,6, "=\0"); break;
		case $NEQ:
			strcpy_s(relopPtr,6, "<>\0"); break;
		case $LESS:
			strcpy_s(relopPtr,6, "<\0"); break;
		case $LESSEQ:
			strcpy_s(relopPtr,6, "<=\0"); break;
		case $GREAT:
			strcpy_s(relopPtr,6, ">\0"); break;
		case $GREATEQ:
			strcpy_s(relopPtr,6, ">=\0"); break;
		default:
			break;
		}
		Advance();
	}
	else
		ErrorHandle($InvalidExpression);
	return relopPtr;
}

void Aop()	//<aop> → + | -
{
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		Advance();
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

void Mop()	//<mop> → *|/
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

int Integer()
{
	if (tokenList[curIndex].first == $INT) {
		Advance();
		int t = getInt();
		return t;
	}
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
				//std::cout<<"<code,value>:\t" << "<" << tokenList[curIndex].first << "," << tokenList[curIndex].second << ">" << '\n';
				if (temp.first == -1) {
					std::cout << "词法分析出错！" << '\n';
					ErrorHandle($UndefinedSymbol);
					return;
				}
				if (temp.first == -2) {
					if (!isError) {
						std::cout << "读取完成" << std::endl;
						isFinish = true;
					}
					else {
						std::cout << "读取完成,有不合法语句" << std::endl;
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
	case 6: {
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
	case 4: {
		std::cout << "Error: ':=' Ex6pected" << "\trow:" << row << " col:" << col << '\n';
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
	curIndex = -1;
	Advance();
	char d[] ="-";
	Prog();
	if (!isFinish) {
		std::cout << "遇到无法跳过的错误！" << '\n';
	}
	printThreeCode();
	genPcode();
	printPcode();
	runPcode();
	
}

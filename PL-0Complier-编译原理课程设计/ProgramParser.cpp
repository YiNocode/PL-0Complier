#include"ProgramParser.h"
#include <cassert>
std::vector<Token> tokenList;
int curIndex;
int tokenListLenth;
Token LexicalAnalzer();
std::string getWrongToken();
std::vector<std::string> errBox;
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
	//Emit(":=",t,0,tmp->varInfo->offset);
	gen("LOD", 0, t, &pid);
	gen("STO", Level - tmp->tablePtr->level, tmp->varInfo->offset, &pid);
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
		plist = merge(plist, makelist(nextquad,Level));
		//Emit( "j", NULL, NULL, NULL);
		gen("JMP", 0, 0, &pid);

	}
	else {
		ErrorHandle($ProcedureExpected);
	}
	Id();
	enterProc(table[tblptr - 2], getStr(), makeTable(table[tblptr - 1], getStr()),nextquad);
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
		enter(table[tblptr - 1], getStr(), 4, varTypes::PAR);
		while (tokenList[curIndex].first == $COMMA||tokenList[curIndex].first==$ID) {
			Advance();
			Id();
			enter(table[tblptr - 1], getStr(), 4, varTypes::PAR);
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
	//Emit("ret", 0, 0, 0);
	gen("OPR", 0, 0, &pid);//恢复调用过程：释放数据段（退栈），恢复调用该过程前正在运行的过程的数据段基址寄存器B和栈顶寄存器T的值

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
	backpatch(plist, nextquad,Level);

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
		//Emit( ":=", Exp(), NULL, tmp->varInfo->offset);
		std::pair<int, int> t = Exp();
		gen("LOD", Level - t.second, t.first, &pid);
		gen("STO", Level - tmp->tablePtr->level, tmp->varInfo->offset, &pid);
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
			//Emit( "j", NULL, NULL, M1);
			gen("JMP", 0, M1, &pid);

			backpatch(falselist, nextquad);
		}
		else {
			ErrorHandle($DoExpected);
		}
	}
	else if (tokenList[curIndex].first == $CALL) {
		Advance();
		Id();
		int codeId = 0;
		int sp = 0;
		codeId = lookup(getStr())->tablePtr->id;
		tableItem* t = lookup(getStr());
		if (tokenList[curIndex].first == $LPAR) {
			Advance();
		}
		if (tokenList[curIndex].first == $RPAR) {
			Advance();
			//Emit("call", t->tablePtr->width,t->tablePtr->display[t->tablePtr->level], codeId);
			gen("INT", 0, t->tablePtr->width + 3 * 4, &pid);//设置栈顶指针T，开辟空间（栈顶分配SL,DL,RA）
			gen("CSL", 0, t->tablePtr->width + 3 * 4, &pid);
			gen("CDL", 0, 0, &pid);
			gen("CRA", 0, pid+2, &pid);
			gen("CAL", 0, codeId, &pid);//设置基地址指针B,转到被调用过程
		}
		else if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			std::pair<int,int> queue[lengthMax];
			int head = 0;
			int tail = 0;
			int n = 0;
			queue[tail] = Exp();
			//Emit( "par", t->tablePtr->display[t->tablePtr->level], NULL, queue[tail]);
			gen("SED", 0, 0, &pid);
			gen("PAR", Level - queue[tail].second, queue[tail].first, &pid);
			tail++;
			n++;
			while (tokenList[curIndex].first == $COMMA || tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
				Advance();
				queue[tail] = Exp();
				//Emit( "par", t->tablePtr->display[t->tablePtr->level], NULL, queue[tail]);
				gen("PAR", Level - queue[tail].second, queue[tail].first, &pid);
				tail++;
			}
			if (tokenList[curIndex].first == $RPAR) {
				Advance();
				//Emit("call", t->tablePtr->width, t->tablePtr->display[t->tablePtr->level], codeId);
				gen("INT", 0, t->tablePtr->width + 3 * 4, &pid);//设置栈顶指针T，开辟空间（栈顶分配SL,DL,RA）
				gen("CSL", 0, t->tablePtr->width + 3 * 4, &pid);
				gen("CDL", 0, 0, &pid);
				gen("CRA", 0, pid + 2, &pid);
				gen("CAL", 0, codeId, &pid);//设置基地址指针B,转到被调用过程
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
			//Emit("read", lookup(getStr())->varInfo->offset, 0, 0);
			gen("OPR", 0, 16, &pid);
			gen("STO", Level, lookup(getStr())->varInfo->offset, &pid);
			while (tokenList[curIndex].first == $COMMA) {
				Advance();
				Id();
				//Emit("read", lookup(getStr())->varInfo->offset, 0, 0);
				gen("OPR", 0, 16, &pid);
				gen("STO", Level, lookup(getStr())->varInfo->offset, &pid);
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
		//Emit("write", Exp(), 0, 0);
		std::pair<int, int> t = Exp();
		gen("LOD", Level - t.second, t.first, &pid);
		gen("OPR", 0, 14, &pid);
		while (tokenList[curIndex].first == $COMMA|| tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
			Advance();
			//Emit("write", Exp(), 0, 0);
			t = Exp();
			gen("LOD", Level - t.second, t.first, &pid);
			gen("OPR", 0, 14, &pid);
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
	std::pair<int,int> id1;
	std::pair<int, int> id2;
	char* relop = new char[10];
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB || tokenList[curIndex].first == $ID || tokenList[curIndex].first == $INT || tokenList[curIndex].first == $LPAR) {
		id1 = Exp();
		strcpy_s(relop, 4, Lop());
		id2 = Exp();
		*t = makelist(nextquad + 3);
		*f = makelist(nextquad + 4);
		//Emit( relop, id1, id2, NULL);
		gen("LOD", Level - id1.second, id1.first, &pid);
		gen("LOD", Level - id2.second, id2.first, &pid);
		if (strcmp(relop, ">") == 0) { gen("OPR", 0, 12, &pid); }
		else if (strcmp(relop, "<") == 0) { gen("OPR", 0, 10, &pid); }
		else if (strcmp(relop, ">=") == 0) { gen("OPR", 0, 11, &pid); }
		else if (strcmp(relop, "<=") == 0) { gen("OPR", 0, 13, &pid); }
		else if (strcmp(relop, "<>") == 0) { gen("OPR", 0, 9, &pid); }
		else if (strcmp(relop, "=") == 0) { gen("OPR", 0, 8, &pid); }
		gen("JPC", 0, 0, &pid);
		//Emit( "j", NULL, NULL, NULL);
		gen("JMP", 0, 0, &pid);

		
	}
	else if (tokenList[curIndex].first == $ODD) {
		Advance();
		id1 = Exp();
		*t = makelist(nextquad + 2);
		*f = makelist(nextquad + 3);
		//Emit( "odd", id1, NULL, NULL);
		gen("LOD", Level - id1.second, id1.first, &pid);
		gen("OPR", 0, 6, &pid);
		gen("JPC", 0, 0, &pid);
		//Emit( "j", NULL, NULL, NULL);
		gen("JMP", 0,0, &pid);

		
	}
	else {
		ErrorHandle($InvalidExpression);
	}
}

std::pair<int,int> Exp()			//<exp> → [+|-]<term>{<aop><term>}
{	
	char op[6];
	std::pair<int, int> term1;
	std::pair<int, int> term2;
	std::pair<int, int> exp;
	std::pair<int, int> exp1;
	if (tokenList[curIndex].first == $ADD || tokenList[curIndex].first == $SUB) {
		if (tokenList[curIndex].first == $SUB) {
			strcpy_s(op,"-\0");
		}
		Advance();
		if (strcmp(op, "-") == 0) {
		exp = newtemp();
		//Emit( op, exp1, NULL, exp);
		gen("LOD", Level - exp1.second, exp1.first,&pid);
		gen("OPR", 0, 1,&pid);
		gen("STO", Level - exp.second, exp.first, &pid);	}
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
		//Emit( op, exp1, term2, exp);
		if(strcmp(op,"+")==0){
			gen("LOD", Level - exp1.second, exp1.first, &pid);
			gen("LOD", Level - term2.second, term2.first, &pid);
			gen("OPR", 0, 2, &pid);
			gen("STO", Level - exp.second, exp.first, &pid);
		}
		else {
			gen("LOD", Level - exp1.second, exp1.first, &pid);
			gen("LOD", Level - term2.second, term2.first, &pid);
			gen("OPR", 0, 3, &pid);
			gen("STO", Level - exp.second, exp.first, &pid);
		}
		exp1 = exp;
	}
	
	return exp;
}

std::pair<int,int> Term()			//<term> → <factor>{<mop><factor>}
{
	std::pair<int,int>  factor1;
	std::pair<int, int> factor2;
	char op[6];
	std::pair<int, int> term;
	std::pair<int, int> term1;
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
		//Emit( op, term1, factor2, term);
		if(strcmp(op,"*")==0){
			gen("LOD", Level - term1.second, term1.first, &pid);
			gen("LOD", Level - factor2.second, factor2.first, &pid);
			gen("OPR", 0, 4, &pid);
			gen("STO", Level - term.second, term.first, &pid);
		}
		else
		{
			gen("LOD", Level - term1.second, term1.first, &pid);
			gen("LOD", Level - factor2.second, factor2.first, &pid);
			gen("OPR", 0, 5, &pid);
			gen("STO", Level - term.second, term.first, &pid);
		}
		term1 = term;
	}
	return term;
}

std::pair<int,int> Factor()			//<factor>→<id>|<integer>|(<exp>)
{
	if (tokenList[curIndex].first == $ID) {
		Id();
		tableItem* tmp = lookup(getStr());
		return (std::pair<int,int>(tmp->varInfo->offset,tmp->tablePtr->level));
	}
	else if (tokenList[curIndex].first == $INT) {
		return std::pair<int,int>(Integer(),0);
	}
	else if (tokenList[curIndex].first == $LPAR) {
		Advance();
		std::pair<int,int> t = Exp();
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
	if(!isError) {
		gen("INT", 0,table[0]->width, 0);
		printPcode();
		runPcode();
	}
	
	
}

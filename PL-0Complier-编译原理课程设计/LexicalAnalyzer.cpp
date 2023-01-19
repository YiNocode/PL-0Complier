#include"LexicalAnalyzer.h"
std::string inputBuffer;	//输入缓冲区
std::string strToken;	//存放构成单词符号的字符串
int beginIndex;			//起点指示器
int seekIndex;			//搜索指示器
char ch;					//字符变量，存放最新读进的源程序字符
bool sucFlag = true;				//标记词法分析是否成功
int row, col;		//错误信息的行列
int name_token_id = 0;
std::string reserveTable[16] = { "begin","call",
					"const","do","end","if",
					"odd","procedure",
					"read","then",
					"var","while","write" ,"program","else"};
//保留字表(编码1-15)
bool GetChar() {	//将下一输入字符读到ch中，搜索指示器向前移一字符位置
	if (ch == '\n') {
		col = 1;
		row++;
	}
	if (seekIndex < inputBuffer.length()) {
		ch = inputBuffer[seekIndex++];
		col++;
	}
	if (ch < 0) {
		return false;
	}
	return true;
}
void GetBC() {				//检查ch中的字符是否为空白。若是，则调用GetChar直至ch中进入一个非空白字符
	while (ch == ' '||ch == '\n'||ch=='\t') {
		GetChar();
	}
	
}
void Concat() {				//将ch中的字符连接到strToken之后。
	strToken += ch;
}
bool IsLetter() {			//判断ch中的字符是否为字母
	if (ch > 64 && ch < 123) { return true; }
	return false;
}
bool IsDigit() {			//判断ch中的字符是否为数字
	if (ch > 47 && ch < 58) { return true; }
	return false;
}
int Reserve() {				//对strToken中的字符查找保留字表，若它是一个保留字则返回它的编码，否则返回0值
	int i = 0;
	for (i = 0; i < 15; i++) {
		if (strToken.compare(reserveTable[i])==0) {
			return (i + 1);
		}
	}
	return 0;
}
void Retract(){				//将搜索指示器回调一个字符位置，将ch置为空白字符
	seekIndex--;
	col--;
	ch = ' ';
}
int InsertId(std::string strT){				//将strToken中的标识符插入符号表，返回符号表指针
	name_token[name_token_id] = strT;
	name_token_id++;
	return name_token_id - 1;
}
int InsertConst(std::string strT){			//将strToken中的常数插入常数表，返回常数表指针
	constTable[constId] = atoi(strT.c_str());

	constId++;
	return constId - 1;
}

//todo:符号表和常数表



Token LexicalAnalzer()
{
	int code = 0, value = 0;
	strToken.clear();
	if (GetChar()) 
		GetBC();
	else return std::make_pair(-1, -1);
	if (IsLetter()) {
		while (IsLetter() || IsDigit()) {
			Concat();
			if (!GetChar()) {
				return std::make_pair(-1, -1);
			}
		}
		Retract();
		code = Reserve();
		if (code == 0) {	//不是保留字，插入符号表
			value = InsertId(strToken + '\0');
			
			return std::make_pair($ID, value);
		}
			
		return std::make_pair(code, value);
		
	}
	else if (IsDigit()) {
		while (IsDigit()) {
			Concat();
			if (!GetChar()) {
				return std::make_pair(-1, -1);
			}
			if (IsLetter()) {
				return std::make_pair(-1, -1);	//错误声明
			}
		}
		Retract();
		value = InsertConst(strToken); 
		return std::make_pair($INT, value);
	}
	else if (ch == '=') {
		
		return std::make_pair($EQ, -1);
	}
	else if (ch == ';') {
		
		return std::make_pair($SEM, -1);
	}
	else if (ch == '+') {
		
		return std::make_pair($ADD, -1);
	}
	else if (ch == '-') {
		
		return std::make_pair($SUB, -1);
	}
	else if (ch == '*') {
		
		return std::make_pair($MUL, -1);
	}
	else if (ch == '/') {
		
		return std::make_pair($DIV, -1);
	}
	else if (ch == ',') {
		
		return std::make_pair($COMMA, -1);
	}
	else if (ch == '(') {
		
		return std::make_pair($LPAR, -1);
	}
	else if (ch == ')') {
		
		return std::make_pair($RPAR, -1);
	}
	else if (ch == '>') {
		if (GetChar()) {
			if (ch == '=') {
				return std::make_pair($LESSEQ, -1);
			}
			else {
				Retract();
				return std::make_pair($GREAT, -1);
			}
		}
		else {
			return std::make_pair(-1, -1);
		}
	}
	else if (ch == '<') {
		if (GetChar()) {
			if (ch == '>') {

				return std::make_pair($NEQ, -1);
			}
			else if (ch == '=') {

				return std::make_pair($LESSEQ, -1);
			}
			else {
				Retract();
				return std::make_pair($LESS, -1);
			}
		}
		else return std::make_pair(-1, -1);
		
	}
	else if (ch == ':') {
		if (GetChar()) {
			if (ch == '=') {
				return std::make_pair($ASSIGN, -1);
			}
			Retract();
			return std::make_pair(-1, -1);	//错误声明
		}
		else {
			 return std::make_pair(-1, -1);
		}
		
	}
	else if(ch == '.'){
		return std::make_pair(-2, -1); //结束声明
	}	
	else {
			return std::make_pair(-1, -1);	//错误声明
	}

	
}

std::string getWrongToken()
{
	return std::string(strToken);
}

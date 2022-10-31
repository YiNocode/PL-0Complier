#include"LexicalAnalyzer.h"
std::string inputBuffer;	//输入缓冲区
std::string scanBuffer;		//扫描缓冲区
std::string strToken;	//存放构成单词符号的字符串
int beginIndex;			//起点指示器
int seekIndex;			//搜索指示器
char ch;					//字符变量，存放最新读进的源程序字符
void preprocesser(){		//将固定长度的输入字符装入扫描缓冲区中

	std::string temp = inputBuffer.substr(beginIndex, WordsLen);
	scanBuffer += temp;
}
void GetChar() {			//将下一输入字符读到ch中，搜索指示器向前移一字符位置
	ch = scanBuffer[seekIndex++];
}
void GetBC() {				//检查ch中的字符是否为空白。若是，则调用GetChar直至ch中进入一个非空白字符
	while (ch == ' ') {
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

	return 0;
}
void Retract(){				//将搜索指示器回调一个字符位置，将ch置为空白字符
	seekIndex--;
	ch = ' ';
}
int InsertId(std::string strT){				//将strToken中的标识符插入符号表，返回符号表指针

	return 0;
}
int InsertConst(std::string strT){			//将strToken中的常数插入常数表，返回常数表指针

	return 0;
}
//todo:符号表和常数表



Token LexicalAnalzer()
{
	int code = 0, value = 0;
	std::cin >> inputBuffer;
	preprocesser();
	GetChar();
	GetBC();
	if (IsLetter()) {
		while (IsLetter() || IsDigit()) {
			Concat();
			GetChar();
		}
		Retract();
		code = Reserve();
		if (code == 0) {
			value = InsertId(strToken);
			return std::make_pair(25, value);
		}
		else {
			return std::make_pair(code, value);
		}
	}
	else if (IsDigit()) {
		while (IsDigit()) {
			Concat();
			GetChar();
		}
		Retract();
		value = InsertConst(strToken);
		return std::make_pair(2, value);
	}
	else if (ch == '=') {
		return std::make_pair(17, -1);
	}
	else if (ch == ';') {
		return std::make_pair(26, -1);
	}

	else return std::make_pair(-1,-1);	//错误声明

	
}

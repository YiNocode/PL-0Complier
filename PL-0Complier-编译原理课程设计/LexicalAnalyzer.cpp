#include"LexicalAnalyzer.h"
std::string inputBuffer;	//���뻺����
std::string strToken;	//��Ź��ɵ��ʷ��ŵ��ַ���
int beginIndex;			//���ָʾ��
int seekIndex;			//����ָʾ��
char ch;					//�ַ�������������¶�����Դ�����ַ�
bool sucFlag = true;				//��Ǵʷ������Ƿ�ɹ�
int row, col;		//������Ϣ������
int name_token_id = 0;
std::string reserveTable[16] = { "begin","call",
					"const","do","end","if",
					"odd","procedure",
					"read","then",
					"var","while","write" ,"program","else"};
//�����ֱ�(����1-15)
bool GetChar() {	//����һ�����ַ�����ch�У�����ָʾ����ǰ��һ�ַ�λ��
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
void GetBC() {				//���ch�е��ַ��Ƿ�Ϊ�հס����ǣ������GetCharֱ��ch�н���һ���ǿհ��ַ�
	while (ch == ' '||ch == '\n'||ch=='\t') {
		GetChar();
	}
	
}
void Concat() {				//��ch�е��ַ����ӵ�strToken֮��
	strToken += ch;
}
bool IsLetter() {			//�ж�ch�е��ַ��Ƿ�Ϊ��ĸ
	if (ch > 64 && ch < 123) { return true; }
	return false;
}
bool IsDigit() {			//�ж�ch�е��ַ��Ƿ�Ϊ����
	if (ch > 47 && ch < 58) { return true; }
	return false;
}
int Reserve() {				//��strToken�е��ַ����ұ����ֱ�������һ���������򷵻����ı��룬���򷵻�0ֵ
	int i = 0;
	for (i = 0; i < 15; i++) {
		if (strToken.compare(reserveTable[i])==0) {
			return (i + 1);
		}
	}
	return 0;
}
void Retract(){				//������ָʾ���ص�һ���ַ�λ�ã���ch��Ϊ�հ��ַ�
	seekIndex--;
	col--;
	ch = ' ';
}
int InsertId(std::string strT){				//��strToken�еı�ʶ��������ű����ط��ű�ָ��
	name_token[name_token_id] = strT;
	name_token_id++;
	return name_token_id - 1;
}
int InsertConst(std::string strT){			//��strToken�еĳ������볣�������س�����ָ��
	constTable[constId] = atoi(strT.c_str());

	constId++;
	return constId - 1;
}

//todo:���ű�ͳ�����



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
		if (code == 0) {	//���Ǳ����֣�������ű�
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
				return std::make_pair(-1, -1);	//��������
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
			return std::make_pair(-1, -1);	//��������
		}
		else {
			 return std::make_pair(-1, -1);
		}
		
	}
	else if(ch == '.'){
		return std::make_pair(-2, -1); //��������
	}	
	else {
			return std::make_pair(-1, -1);	//��������
	}

	
}

std::string getWrongToken()
{
	return std::string(strToken);
}

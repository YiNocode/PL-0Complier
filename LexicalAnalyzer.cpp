#include"LexicalAnalyzer.h"
std::string inputBuffer;	//���뻺����
std::string scanBuffer;		//ɨ�軺����
std::string strToken;	//��Ź��ɵ��ʷ��ŵ��ַ���
int beginIndex;			//���ָʾ��
int seekIndex;			//����ָʾ��
char ch;					//�ַ�������������¶�����Դ�����ַ�
void preprocesser(){		//���̶����ȵ������ַ�װ��ɨ�軺������

	std::string temp = inputBuffer.substr(beginIndex, WordsLen);
	scanBuffer += temp;
}
void GetChar() {			//����һ�����ַ�����ch�У�����ָʾ����ǰ��һ�ַ�λ��
	ch = scanBuffer[seekIndex++];
}
void GetBC() {				//���ch�е��ַ��Ƿ�Ϊ�հס����ǣ������GetCharֱ��ch�н���һ���ǿհ��ַ�
	while (ch == ' ') {
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

	return 0;
}
void Retract(){				//������ָʾ���ص�һ���ַ�λ�ã���ch��Ϊ�հ��ַ�
	seekIndex--;
	ch = ' ';
}
int InsertId(std::string strT){				//��strToken�еı�ʶ��������ű����ط��ű�ָ��

	return 0;
}
int InsertConst(std::string strT){			//��strToken�еĳ������볣�������س�����ָ��

	return 0;
}
//todo:���ű�ͳ�����



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

	else return std::make_pair(-1,-1);	//��������

	
}

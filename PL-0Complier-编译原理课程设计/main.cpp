#include"LexicalAnalyzer.h"
#include"ProgramParser.h"
int main()
{
	row = 1;
	col = 1;
	char stop = '.';
	isFinish = false;
	isError = false;
	std::ifstream source("C:\\Lyn\\Personal\\Study\\����ԭ��\\PL-0Complier-����ԭ��γ����\\test.txt");
	if (!source.is_open()) {
		std::cout << "���ļ�ʧ�ܣ�";
	}
	std::getline(source,inputBuffer,stop);
	std::cout << inputBuffer << '\n';
	inputBuffer += stop;
	source.close();
	//intermediate.close();
	ProgramParser();
	for (int i = 0; i < 100; i++) {
		std::cout << i << ":" << name_token[i] << std::endl;
	}
	return 0;
}
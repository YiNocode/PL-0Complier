#include"LexicalAnalyzer.h"
#include"PL-0Complier-����ԭ��γ����/ProgramParser.h"
int main()
{
	row = 1;
	col = 1;
	char stop = '#';
	std::ifstream source("C:\\Lyn\\Personal\\Study\\����ԭ��\\PL-0Complier-����ԭ��γ����\\test.txt");
	if (!source.is_open()) {
		std::cout << "���ļ�ʧ�ܣ�";
	}
	//std::ofstream intermediate("C:\\Lyn\\Personal\\Study\\����ԭ��\\PL-0Complier-����ԭ��γ����\\intermediate.dat",std::ios::out|std::ios::binary);
	std::getline(source, inputBuffer,stop);
	inputBuffer += stop;
	source.close();
	//intermediate.close();
	ProgramParser();
	return 0;
}
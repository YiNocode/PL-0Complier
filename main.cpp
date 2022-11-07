#include"LexicalAnalyzer.h"
#include"PL-0Complier-编译原理课程设计/ProgramParser.h"
int main()
{
	row = 1;
	col = 1;
	char stop = '#';
	std::ifstream source("C:\\Lyn\\Personal\\Study\\编译原理\\PL-0Complier-编译原理课程设计\\test.txt");
	if (!source.is_open()) {
		std::cout << "打开文件失败！";
	}
	//std::ofstream intermediate("C:\\Lyn\\Personal\\Study\\编译原理\\PL-0Complier-编译原理课程设计\\intermediate.dat",std::ios::out|std::ios::binary);
	std::getline(source, inputBuffer,stop);
	inputBuffer += stop;
	source.close();
	//intermediate.close();
	ProgramParser();
	return 0;
}
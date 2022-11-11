#include"LexicalAnalyzer.h"
#include"PL-0Complier-编译原理课程设计/ProgramParser.h"
int main()
{
	row = 1;
	col = 1;
	char stop = '.';
	isFinish = false;
	isError = false;
	std::ifstream source("C:\\Lyn\\Personal\\Study\\编译原理\\PL-0Complier-编译原理课程设计\\test.txt");
	if (!source.is_open()) {
		std::cout << "打开文件失败！";
	}
	std::getline(source,inputBuffer,stop);
	std::cout << inputBuffer << '\n';
	inputBuffer += stop;
	source.close();
	//intermediate.close();
	ProgramParser();
	return 0;
}
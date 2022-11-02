#include<tuple>
#include<iostream>
#include"LexicalAnalyzer.h"

int main()
{
	int code, value;
	char stop = '#';	
	std::getline(std::cin, inputBuffer,stop);
	inputBuffer += stop;
	while (1) {
		std::tie(code, value) = LexicalAnalzer();
		if (ch == stop) { break; }
		std::cout << "code:" << code << "value:" << value << '\n';
		
	}
	
	
	

	return 0;
}
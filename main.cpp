#include<tuple>
#include"LexicalAnalyzer.h"

int main()
{
	int code, value;
	char stop = '#';
	while (1)
	{
		std::tie(code, value) = LexicalAnalzer();
		std::cout << "code:" << code << "value:" << value;
	}
	

	return 0;
}
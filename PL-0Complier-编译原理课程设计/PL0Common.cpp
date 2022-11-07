#include"PL0Common.h"
void RaiseError(int errCode) {
	switch (errCode)
	{
	case 0: {
		std::cout << "Undefined Error! " << "\trow:" << row << " col:" << col<< '\n';

	}break;
	case 1: {
		std::cout << "Error:'='Expected after ':'.  " << "\trow:" << row << " col:" << col << '\n';
	}break;
	case 2: {
		std::cout << "Error: Illegal Identifier!  " << "\trow:" << row << " col:" << col << '\n';
	}break;
	case 3: {
		std::cout << "Error: Keyword 'program' Expected" << "\trow:" << row << " col:" << col << '\n';
	}break;
	case 4: {
		std::cout << "Error: ';' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 5: {
		std::cout << "Error: 'begin' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 6: {
		std::cout << "Error: ':=' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 7: {
		std::cout << "Error: 'var' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 8: {
		std::cout << "Error: 'procedure' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 9: {
		std::cout << "Error: '(' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 10: {
		std::cout << "Error: ')' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 11: {
		std::cout << "Error: 'end' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 12: {
		std::cout << "Error: 'then' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 13: {
		std::cout << "Error: 'do' Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 14: {
		std::cout << "Error: Identifier Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 15: {
		std::cout << "Error: Integer Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 16: {
		std::cout << "Error: Expression Expected" << "\trow:" << row << " col:" << col << '\n';

	}break;
	case 17: {
		std::cout << "Error:Invalid Expression" << "\trow:" << row << " col:" << col << '\n';

	}break;
	default:
		break;
	}
}
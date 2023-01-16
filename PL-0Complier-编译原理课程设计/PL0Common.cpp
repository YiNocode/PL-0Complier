#include"PL0Common.h"
bool isFinish;
bool isError;
bool ErrorEnd;
std::string name_token[100];
int constId = 0;
threeCodeItem* threeCode[500];
int constTable[100];
int threeId = 1;
int nextquad = 1;
int* stack = new int[STACK_LENGTH];
void Emit( std::fstream& out,const char op[10], int* arg1, int* arg2, int* result)
{
	threeCode[threeId] = new threeCodeItem;
	strcpy_s(threeCode[threeId]->op, op);
	threeCode[threeId]->arg1 = arg1;
	threeCode[threeId]->arg2 = arg2;
	threeCode[threeId]->result = result;

	std::cout << "生成四元式"<<threeId<<" "<<op<<","<<add2name.at(arg1) <<","<< add2name.at(arg2) <<","<< add2name.at(result) << std::endl;
	
	out <<threeId<<" "<< op << ' ' << arg1 << ' ' << arg2<<' '<<result << '\n';
	threeId++;
	nextquad++;

}
void Emit(std::fstream& out, const char op[10], int* arg1, int* arg2, int result)
{
	threeCode[threeId] = new threeCodeItem;
	strcpy_s(threeCode[threeId]->op, op);
	threeCode[threeId]->arg1 = arg1;
	threeCode[threeId]->arg2 = arg2;
	(threeCode[threeId]->result) = new int;
	*(threeCode[threeId]->result) = result;

	std::cout << "生成四元式" << threeId << " " << op << "," << add2name.at(arg1) << "," << add2name.at(arg2) << "," << result << std::endl;

	out << threeId << " " << op << ' ' << arg1 << ' ' << arg2 << ' ' << result << '\n';
	threeId++;
	nextquad++;

}
int* addrAssign(int* base,int offset)
{
	return (base + offset/4);
}

list* makelist(int i)
{
	list* t = new list;
	t->id = i;
	t->next = NULL;
	return t;
}

list* merge(list* p1, list* p2)
{
	list* t = p1;
	while (t ->next!= NULL) {
		t = t->next;
	}
	t->next = p2;
	return p1;
}

void backpatch(list* p, int i)
{
	do {
		if (p->id) {
			if (threeCode[p->id]->result == NULL || *(threeCode[p->id]->result)==0) {
				threeCode[p->id]->result = new int;
				*(threeCode[p->id]->result) = i;
				std::cout << "回填四元式" << p->id << "为" << i << std::endl;
				p = p->next;
			}
			else
			{
				p = p->next;
			}
			
		}
		else
			p = p->next;
		
	} while (p != NULL);

}
void printThreeCode()
{
	for (int i = 1; i < nextquad; i++)
	{
		std::cout << i << "\t" << threeCode[i]->op << "," << threeCode[i]->arg1 << "," << threeCode[i]->arg2 << "," << threeCode[i]->result << std::endl;
	}
}




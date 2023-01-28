#include"PL0Common.h"
bool isFinish;
bool isError;
bool ErrorEnd;
std::string name_token[100];
int constId = 0;
int constTable[100];
int nextquad = 1;
Pcode* pcode[500];
int pid = 1;
std::fstream outFile("C:\\Lyn\\Personal\\Study\\����ԭ��\\PL-0Complier-����ԭ��γ����\\p.txt", std::ios::app);
void gen(const char* f, int l, int a, int* id) {
	Pcode* p = new Pcode;
	if (a >= 300) {
		if (strcmp(f, "PAR") == 0) {
			strcpy_s(p->f, 5, "IPA");
		}
		else {
			strcpy_s(p->f, 5, "LIT");
		}
		p->a = constTable[a - 300];
	}
	else {
		strcpy_s(p->f, 5, f);
		p->a = a;
	}
	p->l = l;
	if(id==0){ pcode[0] = p;}
	else{
		pcode[*id] = p;
		(*id)++;
		nextquad++;
	}
	
}
void printPcode() {
	for (int i = 1; i < pid; i++) {
		std::cout << i << "\t" << pcode[i]->f << "\t" << (pcode[i]->l) << "\t" << pcode[i]->a << std::endl;
	}
}

void runPcode() {

	int T = NULL;//ָ������STACKջ��
	int B = NULL;//��ǰ���й��̵���������STACK�е���ʼ��ַ
	int P = 0;//��һ��Ҫִ�е�ָ����
	int D = 0;
	int* STACK = NULL;//����ջSTACK
	while (1) {
		if (strcmp("INT", pcode[P]->f) == 0) {
			STACK = (int*)realloc(STACK, (T * 4 + pcode[P]->a));
			T = T + (pcode[P]->a) / 4;
			P++;
		}
		else if (strcmp("LIT", pcode[P]->f) == 0) {
			STACK = (int*)realloc(STACK, (T + 1) * 4);
			T++;
			STACK[T - 1] = pcode[P]->a;
			P++;
		}
		else if (strcmp("LOD", pcode[P]->f) == 0) {
			if(pcode[P]->l==0){
				STACK = (int*)realloc(STACK, (T + 1) * 4);
				T++;
				STACK[T - 1] = STACK[B + (pcode[P]->a) / 4];
			}
			else {
				int base = 0;
				base = STACK[T - 1];
				for (int i = 0; i < pcode[P]->l-1; i++) {
					base = STACK[base - 1];
				}
				base = STACK[T - 2];
				STACK = (int*)realloc(STACK, (T + 1) * 4);
				T++;
				STACK[T - 1] = STACK[base / 4 + (pcode[P]->a) / 4];
			}
			P++;
		}
		else if (strcmp("STO", pcode[P]->f) == 0) {
			if (pcode[P]->l == 0) {
				STACK[B + (pcode[P]->a) / 4] = STACK[T - 1];
			}
			else {
				int base = 0;
				base = STACK[T - 1];
				for (int i = 0; i < pcode[P]->l-1; i++) {
					base = STACK[base - 1];
				}
				base = STACK[T - 2];
				STACK[base/4 + (pcode[P]->a) / 4] = STACK[T - 1];
			}
			T--;
			P++;
		}
		else if (strcmp("CAL", pcode[P]->f) == 0) {
			P = pcode[P]->a;
			B = (STACK[T - 1]) / 4;
		}
		else if (strcmp("JMP", pcode[P]->f) == 0) {
				P = pcode[P]->a;
		}
		else if (strcmp("JPC", pcode[P]->f) == 0) {
			if (STACK[T - 1]) {
				P = pcode[P]->a;
			}
			else {
				P++;
			}
			T -= 1;
		}
		else if (strcmp("OPR", pcode[P]->f) == 0) {
			switch (pcode[P]->a)
			{
			case 0:
			{
				P = STACK[T - 3];
				B = STACK[T - 2];
				T = (STACK[T - 1]) / 4;
			}break;
			case 1:
			{
				STACK[T - 1] = ~STACK[T - 1];
				P++;
			}break;
			case 2:
			{
				STACK[T - 2] = STACK[T - 1] + STACK[T - 2];
				T -= 1;
				P++;
			}break;
			case 3: {
				STACK[T - 2] = STACK[T - 2] - STACK[T - 1];
				T -= 1;
				P++;
			}break;
			case 4: {
				STACK[T - 2] = STACK[T - 2] * STACK[T - 1];
				T -= 1;
				P++;
			}break;
			case 5: {
				STACK[T - 2] = STACK[T - 2] / STACK[T - 1];
				T -= 1;
				P++;
			}break;
			case 6: {
				STACK[T - 1] = STACK[T - 1] % 2;
				P++;
			}break;
			case 7: {
			}break;
			case 8: {
				STACK[T - 2] = (STACK[T - 1] == STACK[T - 2]);
				T -= 1;
				P++;
			}break;
			case 9: {
				STACK[T - 2] = (STACK[T - 1] != STACK[T - 2]);
				T -= 1;
				P++;
			}break;
			case 10: {
				STACK[T - 2] = (STACK[T - 2] < STACK[T - 1]);
				T -= 1;
				P++;
			}break;
			case 11: {
				STACK[T - 2] = (STACK[T - 2] >= STACK[T - 1]);
				T -= 1;
				P++;
			}break;
			case 12: {
				STACK[T - 2] = (STACK[T - 2] > STACK[T - 1]);
				T -= 1;
				P++;
			}break;
			case 13: {
				STACK[T - 2] = (STACK[T - 2] <= STACK[T - 1]);
				T -= 1;
				P++;
			}break;
			case  14: {
				std::cout << STACK[T - 1];
				T -= 1;
				P++;
			}break;
			case 15: {
				std::cout << std::endl;
			}break;
			case 16: {
				STACK = (int*)realloc(STACK, (T + 1) * 4);
				T++;
				std::cin >> STACK[T - 1];
				P++;
			}break;
			default:
				break;
			}
		}
		else if (strcmp("CSL", pcode[P]->f) == 0) {
			STACK[T - 1] = T * 4 - pcode[P]->a;
			P++;
		}
		else if (strcmp("CDL", pcode[P]->f) == 0) {
			STACK[T - 2] = B;
			P++;
		}
		else if (strcmp("CRA", pcode[P]->f) == 0) {
			STACK[T - 3] = pcode[P]->a;
			P++;
		}
		else if (strcmp("PAR", pcode[P]->f) == 0) {
				if (pcode[P]->l == 0) {
					STACK = (int*)realloc(STACK, (D + 1) * 4);
					D++;
					STACK[D - 1] = STACK[B + (pcode[P]->a) / 4];
				}
				else {
					int base = 0;
					base = STACK[T - 1];
					for (int i = 0; i < pcode[P]->l-1; i++) {
						base = STACK[base - 1];
					}
					base = STACK[T - 2];
					STACK = (int*)realloc(STACK, (D + 1) * 4);
					D++;
					STACK[D - 1] = STACK[base / 4 + (pcode[P]->a) / 4];
				}
				P++;
		}
		else if (strcmp("SED", pcode[P]->f) == 0) {
			D = T;
			P++;
		}
		else if (strcmp("IPA", pcode[P]->f) == 0) {
			STACK = (int*)realloc(STACK, (D + 1) * 4);
			D++;
			STACK[D - 1] = pcode[P]->a;
			P++;
		}
		for (int i = 0; i < T; i++) {
			std::cout << STACK[i] << " ";
		}
		std::cout << std::endl;
		std::cout << P << std::endl;
		if (P >= pid)break;
	}
	delete[]STACK;

}
list* makelist(int i,int l)
{
	list* t = new list;
	t->id = i;
	t->next = NULL;
	t->level = l;
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
				(pcode[p->id]->a) = i;
				std::cout << "����Pcode" << p->id << "Ϊ" << i << std::endl;
				p = p->next;
		}
		else
			p = p->next;
		
	} while (p != NULL);

}
void backpatch(list*p, int i, int l)
{
	do {
		if (p->id) {
			if (p->level == l + 1) {
				(pcode[p->id]->a) = i;
				std::cout << "����Pcode" << p->id << "Ϊ" << i << std::endl;
			}
			p = p->next;
		}
		else
			p = p->next;

	} while (p != NULL);
}






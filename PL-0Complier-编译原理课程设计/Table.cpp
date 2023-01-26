#include"Table.h"
#include"PL0Common.h"
Table* table[lengthMax];
int offset[lengthMax] = {0};
int tblptr = 0;
int tmpId = 0;
int Level = 0;
Pcode* pcode[500];
int pid = 0;
Table* makeTable(Table* previous,const char name[lengthMax])
{
	Table* tmpTable = new Table;
	if (tmpTable && tblptr < lengthMax - 1) {
		tmpTable->header = previous;
		tmpTable->width = 0;
		strcpy_s(tmpTable->name, name);
		tmpTable->firstItem = nullptr;
		table[tblptr++] = tmpTable;
		tmpTable->level = Level;
		tmpTable->display = new int[(tmpTable->level) + 1]();
		std::cout << "make table,name = " << name <<"level:"<<tmpTable->level<< std::endl;
		if (previous != NULL) {
			for (int i = 0; i < (previous->level)+1; i++) {
				tmpTable->display[i] = previous->display[i];
				tmpTable->display[tmpTable->level] += previous->width;
			}
			
		}
		std::cout << "display:";
		for (int j = 0; j < (tmpTable->level)+1; j++) {
			std::cout << tmpTable->display[j] << " " ;
		}
		std::cout << std::endl;
		return tmpTable;
	}
	errorHandle();
	return nullptr;
}

void enter(Table* tbl, const char name[lengthMax], int width, varTypes type)
{
	tableItem* tmpItem = new tableItem;
	varInformation* varInfoPtr = new varInformation;
	varInfoPtr->type = type;
	varInfoPtr->offset = offset[tblptr-1];
	varInfoPtr->addr = (varInfoPtr->offset);
	std::cout <<tbl->name<<"::" <<name << "����Ե�ַΪ"<< varInfoPtr->offset <<"���Ե�ַΪ" << ((tbl->display[tbl->level] + (varInfoPtr->offset))) << std::endl;
	offset[tblptr - 1] += width;
	tbl->width += width;
	if (tmpItem) {
		strcpy_s(tmpItem->name, name);
		tmpItem->type = types::VAR;
		tmpItem->tablePtr = nullptr;
		tmpItem->varInfo = varInfoPtr;
		tmpItem->next = nullptr;
		if (tbl->firstItem != nullptr) {
			tbl->available->next = tmpItem;
		}
		else {
			tbl->firstItem = tmpItem;
		}
		tbl->available = tmpItem;
	}
	else {
		errorHandle();
	}

}

void enterProc(Table* tbl,const char name[lengthMax],Table* tablePtr,int codeId)
{
	tableItem* tmpItem = new tableItem;
	if (tmpItem) {
		strcpy_s(tmpItem->name, name);
		tmpItem->type = types::PROC;
		tmpItem->tablePtr = tablePtr;
		tmpItem->tablePtr->id = codeId;
		tmpItem->varInfo = nullptr;
		tmpItem->next = nullptr;
		if (tbl->firstItem != nullptr) {
			tbl->available->next = tmpItem;
		}
		else {
			tbl->firstItem = tmpItem;
		}
		tbl->available = tmpItem;
	}
	else {
		errorHandle();
	}

}
tableItem* lookup(const char name[lengthMax]) {
	Table* curTable = table[tblptr - 1];
	Table* preTable = curTable->header;
	tableItem* seek = curTable->firstItem;
	do{
		seek = curTable->firstItem;
		while (seek!=NULL)
		{
			
				if (strcmp(name, seek->name) == 0) {
					return seek;
				}
			
			seek = seek->next;
		
		}
		if (preTable == NULL)break;
		else {
			curTable = preTable;
			preTable = curTable->header;
		}
	} while (curTable != NULL);
	errorHandle(name);
	return NULL;

}
int newtemp() {
	char* tname = new char[20];
	char* id = new char[5];
	_itoa_s(tmpId, id, 5,10);
	strcpy_s(tname,2 ,"t");
	strcat_s(tname, 10, id);
	tmpId++;
	int p = ((table[tblptr - 1]->display[table[tblptr - 1]->level] + (offset[tblptr - 1])));
	enter(table[tblptr-1], tname, 4, varTypes::VAR);
	return p;

}
void gen(const char* f, int *l, int a, int* id) {
	Pcode* p = new Pcode;
	if (a >= 300) {
		strcpy_s(p->f, 5, "LIT");
		p->a = constTable[a - 300];
	}
	else {
		strcpy_s(p->f, 5, f);
		p->a = a;
	}
	p->l = l;
	pcode[*id] = p;
	(*id)++;
}



void genPcode()
{
	
	int dif = 0;
	int* t2p[400] = {NULL};
	for (int i = 1; i <= threeId; i++) {
		t2p[i] = new int;
	}
	gen("INT", 0, table[0]->width, &pid);
	for (int i = 1; i <= threeId; i++)
	{
		*t2p[i] = dif;
		if (i == threeId)break;
		if (strcmp(threeCode[i]->op, ":=") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			
			gen("STO", 0, threeCode[i]->result, &pid);
			dif += 1;
		}
		else if (strcmp(threeCode[i]->op, "j") == 0) {
			gen("JMP", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j<") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 10, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j<=") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 13, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j<>") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 9, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j=") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 8, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j>") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 12, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "j>=") == 0) {
			
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 11, &pid);
			dif += 3;
			gen("JPC", t2p[threeCode[i]->result], threeCode[i]->result, &pid);
		}
		else if (strcmp(threeCode[i]->op, "+") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 2, &pid);
			gen("STO", 0, threeCode[i]->result, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "-") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 3, &pid);
			gen("STO", 0, threeCode[i]->result, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "*") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 4, &pid);
			gen("STO", 0, threeCode[i]->result, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "/") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 5, &pid);
			gen("STO", 0, threeCode[i]->result, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "write") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			dif += 1;
			gen("OPR", 0, 14, &pid);
		}
		else if (strcmp(threeCode[i]->op, "read") == 0) {
			gen("OPR", 0, 16,&pid);
			dif += 1;
			gen("STO", 0, threeCode[i]->arg1,&pid);
		}
		else if (strcmp(threeCode[i]->op, "call") == 0) {
			gen("INT", 0, threeCode[i]->arg1 + 3*4, &pid);//����ջ��ָ��T�����ٿռ䣨ջ������SL,DL,RA��
			gen("CSL", 0, threeCode[i]->arg2, &pid);
			gen("CDL", 0, 0 ,&pid);
			gen("CRA", 0, pid + 2, &pid);
			dif += 4;
			gen("CAL", t2p[threeCode[i]->result], threeCode[i]->result,&pid);//���û���ַָ��B,ת�������ù���
		}
		else if (strcmp(threeCode[i]->op, "ret") == 0) {
			gen("OPR", 0, 0, &pid);//�ָ����ù��̣��ͷ����ݶΣ���ջ�����ָ����øù���ǰ�������еĹ��̵����ݶλ�ַ�Ĵ���B��ջ���Ĵ���T��ֵ
		}
		else if (strcmp(threeCode[i]->op, "par") == 0) {
		while (strcmp(threeCode[i]->op, "par") == 0) {
			gen("PAR", 0, threeCode[i]->result, &pid);
			i++;
			}
		i -= 1;
		}
		else {
			std::cout << "err" << std::endl;
		}
	}


}
void printPcode() {
	for (int i = 0; i < pid; i++) {
		if (pcode[i]->l == 0) {
			std::cout << i << "\t" << pcode[i]->f << "\t" << 0 << "\t" << pcode[i]->a << std::endl;
		}else
		std::cout << i<<"\t"<<pcode[i]->f << "\t" << *(pcode[i]->l) << "\t" << pcode[i]->a << std::endl;
	}
}
void runPcode() {
	int T = NULL;//ָ������STACKջ��
	int B = NULL;//��ǰ���й��̵���������STACK�е���ʼ��ַ
	int P = 0;//��һ��Ҫִ�е�ָ����

	int* STACK = NULL;//����ջSTACK
	while (1) {
		if (strcmp("INT", pcode[P]->f) == 0) {
			STACK = (int*)realloc(STACK,(T * 4 + pcode[P]->a));
			//B = T;
			T = T + (pcode[P]->a)/4;
			P++;
		}
		else if (strcmp("LIT", pcode[P]->f) ==0 ){
			STACK = (int*)realloc(STACK, (T + 1) * 4);
			T++;
			STACK[T - 1] = pcode[P]->a;
			P++;
		}
		else if (strcmp("LOD", pcode[P]->f) == 0) {
			STACK = (int*)realloc(STACK, (T + 1) * 4);
			T++;
			STACK[T - 1] = STACK[B + (pcode[P]->a)/4];
			P++;
		}
		else if (strcmp("STO", pcode[P]->f) == 0) {
			STACK[B + (pcode[P]->a) / 4] = STACK[T - 1];
			T--;
			P++;
		}
		else if (strcmp("CAL", pcode[P]->f) == 0) {
			P = pcode[P]->a + *(pcode[P]->l);
		}
		else if (strcmp("JMP", pcode[P]->f) == 0) {
			if (pcode[P]->l == 0) {
				P = pcode[P]->a;
			}
			else
			P = pcode[P]->a + *(pcode[P]->l);
		}
		else if (strcmp("JPC", pcode[P]->f) == 0) {
			if (STACK[T - 1]) {
				P = pcode[P]->a + *(pcode[P]->l);
			}
			else {
				P++;
			}
		}
		else if (strcmp("OPR", pcode[P]->f) == 0) {
			switch (pcode[P]->a)
			{
			case 0:
			{
				P = STACK[T - 1];
				B = STACK[T - 2];
				T = (STACK[T - 3])/4;

			}break;
			case 1:
			{
				STACK[T-1] = ~STACK[T-1];
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

			STACK[T - 3] = pcode[P]->a;
			P++;

		}
		else if (strcmp("CDL", pcode[P]->f) == 0) {
			STACK[T - 2] = B;
			P++;
			
		}
		else if (strcmp("CRA", pcode[P]->f) == 0) {
			STACK[T - 1] = pcode[P]->a;
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

void errorHandle(const char * name)
{
	std::cout << "û���ڷ��ű����ҵ�"<<name << std::endl;
	exit(-1);
}
void errorHandle()
{
	
	exit(-1);
}

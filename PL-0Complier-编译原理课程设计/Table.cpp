#include"Table.h"
#include"PL0Common.h"
Table* table[lengthMax];
int offset[lengthMax] = {0};
int tblptr = 0;
int tmpId = 0;
int Level = 0;
Pcode* pcode[500];
int pid = 1;
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
	std::cout <<tbl->name<<"::" <<name << "的相对地址为"<< varInfoPtr->offset <<"绝对地址为" << ((tbl->display[tbl->level] + (varInfoPtr->offset))) << std::endl;
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
	strcpy_s(p->f, 5, f);
	p->a = a;
	p->l = l;
	pcode[*id] = p;
	(*id)++;
}



void genPcode()
{
	
	int dif = 0;
	int* t2p[400] = {NULL};
	for (int i = 1; i < threeId; i++) {
		t2p[i] = new int;
	}
	for (int i = 1; i < threeId; i++)
	{
		*t2p[i] = dif;
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
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "-") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 3, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "*") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 4, &pid);
			dif += 3;
		}
		else if (strcmp(threeCode[i]->op, "/") == 0) {
			gen("LOD", 0, threeCode[i]->arg1, &pid);
			gen("LOD", 0, threeCode[i]->arg2, &pid);
			gen("OPR", 0, 5, &pid);
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
			gen("LOD", 0, threeCode[i]->arg1,&pid);
		}
		else if (strcmp(threeCode[i]->op, "call") == 0) {
			gen("INT", 0, threeCode[i]->arg1 + 3*4, &pid);//设置栈顶指针T，开辟空间（栈顶分配SL,DL,RA）
			gen("CSL", 0, threeCode[i]->arg2, &pid);
			gen("CDL", 0, 0,&pid);
			gen("CRA", 0, pid + 2, &pid);
			dif += 4;
			gen("CAL", t2p[threeCode[i]->result], threeCode[i]->result,&pid);//设置基地址指针B,转到被调用过程
			gen("OPR", 0, 0, &pid);//恢复调用过程：释放数据段（退栈），恢复调用该过程前正在运行的过程的数据段基址寄存器B和栈顶寄存器T的值
			dif += 1;
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
	for (int i = 1; i < pid; i++) {
		if (pcode[i]->l == 0) {
			std::cout << i << "\t" << pcode[i]->f << "\t" << 0 << "\t" << pcode[i]->a << std::endl;
		}else
		std::cout << i<<"\t"<<pcode[i]->f << "\t" << *(pcode[i]->l) << "\t" << pcode[i]->a << std::endl;
	}
}
void errorHandle(const char * name)
{
	std::cout << "没有在符号表中找到"<<name << std::endl;
	exit(-1);
}
void errorHandle()
{
	
	exit(-1);
}

#include"Table.h"
#include"PL0Common.h"
Table* table[lengthMax];
int offset[lengthMax] = {0};
int tblptr = 0;
int tmpId = 0;
int Level = 0;
Table* makeTable(Table* previous,const char name[lengthMax])
{
	Table* tmpTable = new Table;
	if (tmpTable && tblptr < lengthMax - 1) {
		tmpTable->header = previous;
		tmpTable->width = 0;
		tmpTable->parWidth = 0;
		strcpy_s(tmpTable->name, name);
		tmpTable->firstItem = nullptr;
		table[tblptr++] = tmpTable;
		tmpTable->level = Level;
		tmpTable->display = new int[(tmpTable->level) + 1]();
		std::cout << "make table,name = " << name <<"level:"<<tmpTable->level<< std::endl;
		if (previous != NULL) {
			for (int i = 0; i < (previous->level)+1; i++) {
				tmpTable->display[i] = previous->display[i];
			}
			tmpTable->display[tmpTable->level] = tmpTable->display[(tmpTable->level) - 1] + previous->width;

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
	tbl->width += width;
	varInfoPtr->offset = offset[tblptr-1];
	varInfoPtr->addr = (varInfoPtr->offset);
	std::cout <<tbl->name<<"::" <<name << "的相对地址为"<< varInfoPtr->offset <<"绝对地址为" << ((tbl->display[tbl->level] + (varInfoPtr->offset))) << std::endl;
	offset[tblptr - 1] += width;
	
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
					if(seek->type==types::VAR){
						seek->tablePtr = curTable;
					}
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
std::pair<int,int> newtemp() {
	char* tname = new char[20];
	char* id = new char[5];
	_itoa_s(tmpId, id, 5,10);
	strcpy_s(tname,2 ,"t");
	strcat_s(tname, 10, id);
	tmpId++;
	int p =  (offset[tblptr - 1]);
	enter(table[tblptr-1], tname, 4, varTypes::VAR);
	return std::pair<int,int>(p, table[tblptr-1]->level);

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

#include"Table.h"
#include"PL0Common.h"
Table* table[lengthMax];
int tblptr = 0;
int tmpId = 0;

Table* makeTable(Table* previous,const char name[lengthMax])
{
	Table* tmpTable = new Table;
	if (tmpTable && tblptr < lengthMax - 1) {
		tmpTable->header = previous;
		tmpTable->offset = 0;
		strcpy_s(tmpTable->name, name);
		tmpTable->firstItem = nullptr;
		table[tblptr++] = tmpTable;
		std::cout << "make table,name = " << name << std::endl;
		if (previous != NULL) {
			std::cout << "Íâ²ãtable:" << previous->name << std::endl;
		}
		return tmpTable;
	}
	errorHandle();
	return nullptr;
}

void enter(Table* tbl, const char name[lengthMax], int offset, varTypes type)
{
	tableItem* tmpItem = new tableItem;
	varInformation* varInfoPtr = new varInformation;
	varInfoPtr->type = type;
	if (tmpItem) {
		strcpy_s(tmpItem->name, name);
		tmpItem->type = types::VAR;
		tmpItem->offset = offset;
		tmpItem->tablePtr = nullptr;
		tmpItem->varInfo = varInfoPtr;
		tmpItem->next = nullptr;
		tmpItem->offset += offset;
		if (tbl->firstItem != nullptr) {
			tbl->available->next = tmpItem;
		}
		else {
			tbl->firstItem = tmpItem;
		}
		tbl->available = tmpItem;
		std::cout << "enter item,name = " << name << std::endl;
	}
	else {
		errorHandle();
	}

}

void enterProc(Table* tbl,const char name[lengthMax],Table* tablePtr)
{
	tableItem* tmpItem = new tableItem;
	if (tmpItem) {
		strcpy_s(tmpItem->name, name);
		tmpItem->type = types::PROC;
		tmpItem->tablePtr = tablePtr;
		tmpItem->varInfo = nullptr;
		tmpItem->next = nullptr;
		if (tbl->firstItem != nullptr) {
			tbl->available->next = tmpItem;
		}
		else {
			tbl->firstItem = tmpItem;
		}
		tbl->available = tmpItem;
		std::cout << "enter proc,name = " << name << std::endl;
	}
	else {
		errorHandle();
	}

}
const char* newtemp() {
	/*std::string str = std::to_string(tmpId);
	const char* id =  strcpy()*/
	return NULL;

}
void errorHandle()
{
}

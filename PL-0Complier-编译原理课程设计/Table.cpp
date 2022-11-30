#include"Table.h"
#include"PL0Common.h"
Table* tablePtr[tableMax];
int tblptr = 0;
Table* makeTable(Table* previous,char name[tableMax])
{
	Table* tmpTable = new Table;
	if (tmpTable && tblptr < tableMax - 1) {
		tmpTable->header = previous;
		tmpTable->offset = 0;
		strcpy(tmpTable->name, name);
		tmpTable->firstItem = nullptr;
		tablePtr[tblptr++] = tmpTable;
		return tmpTable;
	}
	errorHandle();
	return nullptr;
}

void enter(Table* tbl, char name[tableMax],int offset,varInformation* varInfoPtr)
{
	tableItem* tmpItem = new tableItem;
	if (tmpItem) {
		strcpy(tmpItem->name, name);
		tmpItem->type = VAR;
		tmpItem->offset = offset;
		tmpItem->tableInfo = nullptr;
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
	}
	else {
		errorHandle();
	}

}

void enterProc(Table* tbl, char name[tableMax],tableInformation* tableInfoPtr)
{
	tableItem* tmpItem = new tableItem;
	if (tmpItem) {
		strcpy(tmpItem->name, name);
		tmpItem->type = PROC;
		tmpItem->tableInfo = tableInfoPtr;
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

void errorHandle()
{
}

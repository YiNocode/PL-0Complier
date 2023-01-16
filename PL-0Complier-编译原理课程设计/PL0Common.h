#pragma once
#include<utility>
#include<string>
#include<set>
#include<fstream>
#include<iostream>
#include<vector>
#include<tuple>
#include<stdlib.h>
#include<map>

extern int row, col;		//错误信息的行列
extern bool isFinish;
extern bool isError;
extern bool ErrorEnd;
extern int name_token_id;
extern int constTable[100];
extern int constId;
extern std::string name_token[100];
typedef std::pair<int, int> Token;
extern int threeId;
extern int nextquad;
extern std::map<int*, const char*>add2name;

extern int* stack;
struct threeCodeItem
{
	char op[10];
	int* arg1;
	int* arg2;
	int* result;
};
typedef struct list{
	int id;
	list* next;
}list;
extern threeCodeItem* threeCode[500];
//保留字及运算符定义
#define $BEGIN 1
#define	$CALL 2
#define $CONST 3
#define $DO 4
#define $END 5
#define $IF 6
#define $ODD 7
#define $PROCEDUCE 8
#define $READ 9
#define $THEN 10
#define $VAR 11
#define $WHILE 12
#define $WRITE 13
#define $PROGRAM 14
#define $ELSE 15
#define $ADD 16	//"+",
#define $SUB 17 //"-",
#define	$MUL 18	 //"*",
#define $DIV 19 //"/",
#define $EQ 20 //"=", 
#define $NEQ 21 //"<>",
#define $LESS 22//"<", 
#define $LESSEQ 23//"<=", 
#define $GREAT 24//">",
#define $GREATEQ 25//">=",
#define $ASSIGN 26//":=", 
#define $LPAR 27//"(",
#define $RPAR 28//")", 
#define $COMMA	29//",",
#define $SEM 30//";",
#define $ID 31
#define $INT 32
//错误码定义
#define $StatementExpected 0
#define $UndefinedSymbol 1
#define $IllegalIdentifier 2
#define $KeywordExpectd 3
#define $SEMexpected 4
#define $BeginExpected 5
#define $AssignExpected 6
#define $VarExpected 7
#define $ProcedureExpected 8
#define $LparExpected 9
#define $RparExpected 10
#define $EndExpected 11
#define $ThenExpected 12
#define $DoExpected 13
#define $IdentifierExpected 14
#define $IntegerExpected 15
#define $ExpressionExpected 16
#define $InvalidExpression  17
#define $CommaExpected 18
#define $ConstExpected 19
#define STACK_LENGTH 400
void Emit(std::fstream&, const char[10] = "" , int* = NULL, int* = NULL, int* = NULL);//生成三元式
void Emit(std::fstream& , const char[10] , int* , int* , int );
int* addrAssign(int*,int);
list* makelist(int);
list* merge(list*, list*);
void backpatch(list*, int);
void printThreeCode();

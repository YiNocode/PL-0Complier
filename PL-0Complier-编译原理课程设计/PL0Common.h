#pragma once
#include<utility>
#include<string>
#include<set>
#include<fstream>
#include<iostream>
#include<vector>
#include<tuple>
extern int row, col;		//错误信息的行列
typedef std::pair<int, int> Token;
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
#define $ODD 16
#define $ADD 17	//"+",
#define $SUB 18 //"-",
#define	$MUL 19	 //"*",
#define $DIV 20 //"/",
#define $EQ 21 //"=", 
#define $NEQ 22 //"<>",
#define $LESS 23//"<", 
#define $LESSEQ 24//"<=", 
#define $GREAT 25//">",
#define $GREATEQ 26//">=",
#define $ASSIGN 27//":=", 
#define $LPAR 28//"(",
#define $RPAR 29//")", 
#define $COMMA	30//",",
#define $SEM 31//";",
#define $ID 32
#define $INT 33
//错误码定义
#define $UndefinedError 0
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
void RaiseError(int);

#pragma once
#include<iostream>
#include<string>
#include <utility>
#define WordsLen 120
typedef std::pair<int, int> Token;
void preprocesser();
void GetChar();
void GetBC();
void Concat();
bool IsLetter();
bool IsDigit();
int Reserve();
void Retract();
int InsertId(std::string);
int InsertConst(std::string);
Token LexicalAnalzer();

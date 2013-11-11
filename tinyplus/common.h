#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;
typedef enum{
    START,NUM,ID,KEY,SYM,STR,NOTE
} STATE;
//符号集合

string sym=",;:<>=+-*/(){}";
#define SYMLENGTH 17
//(TK_GTR, >),  (TK_LEQ, <=),  (TK_GEQ, >=)
//(TK_COMMA, ,),  (TK_SEMICOLON, ;),  (TK_ASSIGN, :=)
//(TK_ADD, +),  (TK_SUB, -),  (TK_MUL, *)
//(TK_DIV, /),  (TK_LP, (),  (TK_RP, ))
//(TK_LSS, <),  (TK_EQU, =)

string symArray[17]={
    ">","<=",">=",",","\'","{","}",
    ";",":=","+","-","*","/","(",")","<","="
};
typedef enum{
    TK_GTR,TK_LEQ,TK_GEQ,TK_COMMA,TK_SEMICOLON,TK_ASSIGN,TK_ADD,TK_SUB,
    TK_MUL,TK_DIV,TK_LP,TK_RP,TK_LSS,TK_EQU
} SYMTYPE;

//保留字集合.使用key[TK_*]来引用
#define KEYLENGTH 18
string key[18]={"true","false","or","and","not","int","bool","string","while",
                "do","if","then","else","end","repeat","until","read","write"
};
typedef enum {
	// reserved words
    TK_TRUE,TK_FALSE,TK_OR,TK_AND,TK_NOT,TK_INT,TK_BOOL,TK_STRING,TK_WHILE,TK_DO,
    TK_IF,TK_THEN,TK_ELSE,TK_END,TK_REPEAT,TK_UNTIL,TK_READ,TK_WRITE
} TokenType;
//TOKEN对
typedef struct token{
	TokenType kind;	
	string value;
} Token;
#endif

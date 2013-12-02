#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <list>
using namespace std;


//


typedef enum{
    START,NUM,ID,KEY,SYM,STR,NOTE
} STATE;

#define KEYLENGTH 18
string key[KEYLENGTH]={
    "true","false","or","and","not","int","bool","string","while",
    "do","if","then","else","end","repeat","until","read","write"
};

string sym=",;:<>=+-*/(){}";

//(TK_GTR, >),  (TK_LEQ, <=),  (TK_GEQ, >=)
//(TK_COMMA, ,),  (TK_SEMICOLON, ;),  (TK_ASSIGN, :=)
//(TK_ADD, +),  (TK_SUB, -),  (TK_MUL, *)
//(TK_DIV, /),  (TK_LP, (),  (TK_RP, ))
//(TK_LSS, <),  (TK_EQU, =)
//保留字集合.使用key[TK_*]来引用

#define TokenLength 33
string tokenArray[TokenLength]={
    "true","false","or","and","not","int","bool","string","id","while",
    "do","if","then","else","end","repeat","until","read","write",
    ">","<=",">=",",",
    ";",":=","+","-","*","/","(",")","<","="
};
typedef enum _TokenType{
	// reserved words
    TK_TRUE,TK_FALSE,TK_OR,TK_AND,TK_NOT,TK_INT,TK_BOOL,TK_STR,TK_ID,TK_WHILE,TK_DO,
    TK_IF,TK_THEN,TK_ELSE,TK_END,TK_REPEAT,TK_UNTIL,TK_READ,TK_WRITE,
    TK_GTR,TK_LEQ,TK_GEQ,TK_COMMA,TK_SEMICOLON,TK_ASSIGN,TK_ADD,TK_SUB,
    TK_MUL,TK_DIV,TK_LP,TK_RP,TK_LSS,TK_EQU
} TokenType;
string tokenString[TokenLength]={
    "TK_TRUE","TK_FALSE","TK_OR","TK_AND","TK_NOT","TK_INT","TK_BOOL","TK_STR","TK_ID","TK_WHILE","TK_DO",
    "TK_IF","TK_THEN","TK_ELSE","TK_END","TK_REPEAT","TK_UNTIL","TK_READ","TK_WRITE",
    "TK_GTR","TK_LEQ","TK_GEQ","TK_COMMA","TK_SEMICOLON","TK_ASSIGN","TK_ADD","TK_SUB",
    "TK_MUL","TK_DIV","TK_LP","TK_RP","TK_LSS","TK_EQU"
};
//TOKEN对
typedef class _Token{
    public:
    _Token():value(""){
    }
	TokenType kind;	
	string value;
} Token;


//==================
//符号表
//==================

enum ObjType{
    OTYPE_FUN,
    OTYPE_VAR,
    OTYPE_CONST
};

enum ValType{
    VTYPE_INT=1,
    VTYPE_BOOL=2,
    VTYPE_STR=3
};

class Sym{
public:
    Sym(){};

    Token *token;
    ObjType objType;
    ValType valType;
    int addr;
    int size;
    int level;
};
class SymManager{
    public:
    //需要实现符号表操作
    //插入函数 insert(name)
    //查找find(name)
    //删除函数(name)//暂时不需要
        SymManager(){
            symNum=0;
        }
        void insert(Sym* _sym){
            symTable.push_back(_sym);
            symNum++;
        }
        Sym* find(string name){
            for(int i=0;i<symNum;i++){
                if(symTable[i]->token->value==name){
                    return symTable[i];
                }
            }
            return NULL;
        }


        vector<Sym*> symTable;
        int symNum;
        TokenType currentType;
};


enum NodeType{
    PROGRAM,
    STMT_SEQUENCE,
    IF_STMT,
    REPEAT_STMT,
    ASSIGN_STMT,
    READ_STMT,
    WRITE_STMT,
    WHILE_STMT,
    GTR_EXP,
    GEQ_EXP,
    LSS_EXP,
    LEQ_EXP,
    LOG_OR_EXP,
    LOG_AND_EXP,
    LOG_NOT_EXP,
    ADD_EXP,
    SUB_EXP,
    MUL_EXP,
    DIV_EXP,
    FACTOR  //原子节点


};

class TreeNode{
public:
    TreeNode(){
        child[0] = NULL; 
        child[1] = NULL; 
        child[2] = NULL; 
    }
    TreeNode(NodeType nt,TreeNode* l,TreeNode* r){
        child[0] = l;
        child[1] = r;
        child[2] = NULL;
        nodeType = nt;
    }
    NodeType nodeType;// 节点类型
    ValType valType;// 节点值类型
    TreeNode *child[3];

    Token *token;// 当节点是FACTOR类型时该成员才有效
};



//===================
//函数声明
//===================
bool isNumber(char);
bool isAlphabet(char);
bool isLegal(char);
bool isSym(char);
bool isKey(string);
bool isDelim(char);
TokenType matchToken(string);




//==================
//语法书生成函数声明
//==================
bool matchType(TokenType);
void nextToken();
TreeNode* program();
TreeNode* declarations();
TreeNode* stmt_sequence();
void decl();
void type_specifer();
void varlist();
TreeNode* statement();
TreeNode* if_stmt();
TreeNode* repeat_stmt();
TreeNode* assign_stmt();
TreeNode* read_stmt();
TreeNode* write_stmt();
TreeNode* while_stmt();
TreeNode* logical_or_exp();
TreeNode* logical_and_exp();
TreeNode* comparison_exp();
TreeNode* add_exp();
TreeNode* mul_exp();
TreeNode* addop();
TreeNode* factor();
TreeNode* mulop();

#endif

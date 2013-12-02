#include "common.h"
//#include "scan.h"
//全局变量
int lineNum = 1;
int colNum = 0;
fstream *origin;
fstream *step1;
Token *token=NULL;
static bool isError;
char currentLetter;
SymManager* symManager;
list<Token> tokenList;

char getNextChar(fstream *origin){
	if(origin==NULL){
		return -1;//indicate reading file fail;
	}
	char buffer;
    char lastLetter = currentLetter;
    origin->get(buffer);
    if(origin->eof()){
        return -2;//inform that the file complete
    }

	if(!isLegal(buffer)){
		cout<<"\n Illegal alphabet: "<<buffer<<" in the source code in Line:"<<lineNum<<" Column: "<<colNum<<endl;
	}
	if(buffer == '\n'||buffer == '\r'){
            colNum = 0;
            lineNum++;
	}else{
        colNum++;
    }
	//}
	return buffer;
}
void inform(int state){
    if(state == ID)
      cout<<"ID";
    if(state == SYM)
      cout<<"SYM";
    if(state == START)
      cout<<"START";
    if(state == NOTE)
      cout<<"NOTE";
    if(state == NUM)
      cout<<"NUM";
    if(state == STR)
      cout<<"STR";
}
void getToken(){
    
    Token *tmpToken;
	origin = new fstream("/home/slixurd/university/tiny/tinyplus/in",ios::in);
    int currentState = START;
    string currentWord = "";
    if((currentLetter = getNextChar(origin)) == -1){
        isError=true;
        cout << "INPUT FILE ERROR";
    }else{
        while(true){
                    
            switch(currentState){
                case START:
                    //NUM = d* , d>0&&d<9
                    if(isNumber(currentLetter))
                        currentState = NUM;
                    //ID=l(l|d)* , l>A&&a<z
                    else if(isAlphabet(currentLetter))
                        currentState = ID;
                    //sym属于sym集合
                    else if(currentLetter == '{')
                        currentState = NOTE;
                    else if(currentLetter == '}'){
                        cout<<"\n Miss Left Bracket in Line "<<lineNum<<endl; 
                        break;
                    }
                    //字符串由\'包含
                    else if(currentLetter == '\''){
                        currentState = STR;
                        break;//因为单引号本身不需要加入字符串中,所以break跳过
                    }
                    else if(sym.find(currentLetter)!=-1){
                        currentState = SYM;
                    }
                    //to do 由{}组成的comments
                    //起始状态不接收分隔符
                    else if(isDelim(currentLetter)){
                        break;
                    }
                    currentWord+=currentLetter;
                    break;
                case NUM:
                    if(isSym(currentLetter)||isDelim(currentLetter)){
                        tmpToken=new Token; 
                        tmpToken->kind = TK_INT; 
                        tmpToken->value+=currentWord;
                            tmpToken->value+="";
                        tokenList.push_back((*tmpToken));
                        cout<<" (NUM "<<currentWord<<") ";
                        currentWord="";
                        currentState = START;
                        if(isSym(currentLetter))
                          origin->unget();
                        break;
                    } 
                    
                    currentWord += currentLetter;
                    if(!isNumber(currentLetter)){
                        cout<< "\n Invalid variable in line "<<lineNum<<" colomn "<<colNum<<"\n";
                        //吃掉后面所有不合法的字母
                        while(true){
                            currentLetter = getNextChar(origin);
                            if(isDelim(currentLetter)||currentLetter==';'||isSym(currentLetter)){
                                if(isSym(currentLetter))
                                  origin->unget();
                                currentWord="";
                                currentState = START;
                                break;
                            }
                        }
                    }
                    break;
                case ID:
                    //DFA已经进入第二步,所有的字母,数字均可以让ID在本状态下保持
                    if(isAlphabet(currentLetter)||isNumber(currentLetter)){
                        currentWord += currentLetter; 
                    }else {
                        if(isKey(currentWord)){
                        tmpToken=new Token; 
                            tmpToken->kind = matchToken(currentWord);
                            tmpToken->value+=currentWord;
                            tmpToken->value+="";
                            tokenList.push_back((*tmpToken));
                            cout<<" (KEY "<<currentWord<<") ";
                        }else{ 
                        tmpToken=new Token; 
                            tmpToken->kind = TK_ID;
                            tmpToken->value+=currentWord;
                            tmpToken->value+="";
                            tokenList.push_back((*tmpToken));
                            cout<<" (ID "<<currentWord<<") ";
                        }
                        currentState = START;
                        currentWord="";
                        if(isSym(currentLetter)||currentLetter=='\''||currentLetter=='{')
                          origin->unget();
                    }
                    break;
                case STR:
                    //如果找到单引号后第一个字符就是换行符,表明本行缺少左单引号
                    if(currentLetter=='\n'||currentLetter=='\r'){
                        cout<<"\n Miss left quotation mark in Line "<<lineNum<<endl;
                        isError = true; 
                    }else
                    while(true){
                        if(currentLetter=='\''){
                        tmpToken=new Token; 
                            tmpToken->kind = TK_STR; 
                            tmpToken->value+=currentWord;
                            tmpToken->value+="";
                            tokenList.push_back((*tmpToken));
                            cout<<" (STR ";
                            cout<<currentWord<<") ";
                            break;
                        }
                        if(currentLetter=='\n'||currentLetter=='\r'){
                            isError = true; 
                            cout<<"\n Miss right quotation mark in Line "<<lineNum<<endl;
                            break;
                        }
                        currentWord+=currentLetter;
                        currentLetter=getNextChar(origin);
                    }
                    currentWord="";
                    currentState = START;

                    break;
                case SYM:
                    //因为第二个字母只有可能是=,例如>=,<=,:=
                    if(currentLetter=='='){
                        currentWord+=currentLetter;
                    }else if(currentLetter!='\r' && currentLetter!='\n'){
                            origin->unget();
                    }
                    //cout<<"START ";
                    //inform(currentState);
                    //cout<<endl;
                    //

                    bool isLegalSym;
                    isLegalSym=false;
                    for(int i=0;i<TokenLength;i++){
                        if(currentWord==tokenArray[i])
                          isLegalSym=true;
                    }
                    if(!isLegalSym){
                        isError = true; 
                        cout<<"\nERROR,NO SUCH SYMBOL;\n";
                    }
                    else{
                        tmpToken=new Token; 
                        tmpToken->kind = matchToken(currentWord);
                        tmpToken->value+=currentWord;
                        
                            tmpToken->value+="";
                        tokenList.push_back((*tmpToken));
                        cout<<" (SYM "<< currentWord<<") ";
                    }
                    currentState = START;
                    currentWord="";
                    break;
                    
                case NOTE:
                    if(currentLetter == '}'){
                        currentState = START;
                        currentWord = "";
                    }else if(currentLetter == '\n'||currentLetter == '\r'){
                        //无法匹配右括号
                        isError = true; 
                        cout<<"\n Miss right bracket in line "<<lineNum<<endl;
                    }

            }
            if((currentLetter = getNextChar(origin)) == -2){
                break;
            }
        }
    }

	origin->close();
}


int main(){
    isError = false;
    symManager=new SymManager();
    getToken();
    program();


    string _f="sss";
    cout<<"Type "<<symManager->find(_f)->valType<<" "<<tokenString[symManager->find(_f)->token->kind]<<" value "<<symManager->find(_f)->token->value<<endl;

    /*
     while(!tokenList.empty()){//获取所有token
        cout<<tokenString[tokenList.front().kind]<<" ";
        tokenList.pop_front();
    }
    */

    //origin = new fstream("/home/slixurd/university/tiny/in",ios::in);
    //getNextChar(origin);    
    //origin->unget();
    return 0;
}

TokenType matchToken(string toMatch){
    for(int i = 0;i < TokenLength;i++){
        if(toMatch == tokenArray[i]){
            return (TokenType) i;
        }
    }
    return (TokenType)-1;
}
bool isNumber(char check){
	if( check <= '9' && check >= '0')
		return true;
    return false;
}
bool isDelim(char check){
    if(check == ' '|| check == '\n' ||check == '\r'||check=='\t')
        return true;
    return false;
}
bool isAlphabet(char check){
	if( check>='A' && check <= 'z')
		return true;
	return false;
}
bool isSym(char check){
    if(sym.find(check)!=-1)
        return true;
    return false;
}
bool isKey(string id){
   for(int i=0;i<KEYLENGTH;i++){
        if(id==key[i])
          return true;
   } 
   return false;
}
bool isLegal(char check){
	if(isNumber(check)||isAlphabet(check)||sym.find(check)!=-1||check=='\''||
		check==' '||check=='\n'||check=='\r'||check=='\t'||check=='\"'
		)
		return true;
	return false;
}


bool matchType(TokenType kind_match){
    if(token->kind == kind_match){
        return true; 
    }else{
        //cout<<"类型应匹配"<<tokenString[kind_match]<<",遇到"<<tokenString[token->kind]<<"\n";
        return false;
    }
}

//==========================
//语法树的生成
//==========================
void nextToken(){
    if(token==NULL){
        token = new Token;
        token = &tokenList.front();
    }
    if(!tokenList.empty()){
        token = &tokenList.front();
        tokenList.pop_front();
    }
    cout<<tokenString[token->kind]<<endl;
}

void unNextToken(){
    if(token!=NULL){
        tokenList.push_front(*token);
    }
}

TreeNode* program(){//program -> declarations stmt_sequence
    cout<<"\n===========declarations分析============="<<endl;
    declarations();
    //return stmt_sequence();
}
//==========================
//变量声明分析
//==========================
TreeNode* declarations(){//declaration -> decl;declarations|nil
    decl();
    if(matchType(TK_SEMICOLON)){
        nextToken();
        if(token->kind==TK_BOOL||token->kind==TK_INT||token->kind==TK_STR){
            unNextToken();
            declarations();    
        }else{
            unNextToken();
            cout<<"\n=========变量声明结束===========\n";
        }
    }else{
        cout<<"错误匹配\n";
    }


}
void decl(){//decl = type_specifer varlist
    type_specifer();
    varlist();
}
void type_specifer(){//int|bool|string
    nextToken();
    if(matchType(TK_INT)){
       symManager->currentType=TK_INT; 
    }else if(matchType(TK_BOOL)){
       symManager->currentType=TK_BOOL; 
    }else if(matchType(TK_STR)){
       symManager->currentType=TK_STR; 
    }
}
void varlist(){//identifiers [, varlist]
    nextToken();
    while(matchType(TK_ID)){
        Sym* tmpSym = new Sym;
        tmpSym->token=token;
        if(symManager->currentType==TK_INT){
            tmpSym->valType=VTYPE_INT;
        }else if(symManager->currentType==TK_BOOL){
            tmpSym->valType=VTYPE_BOOL;
        }else if(symManager->currentType==TK_STR){
            tmpSym->valType=VTYPE_STR;
        }
        tmpSym->objType=OTYPE_VAR;
        symManager->insert(tmpSym);
        nextToken();
        if(!matchType(TK_COMMA)){
              return;
        }
        nextToken();
        
    }
    return;
}
//======================
//基本语句解析
//======================
TreeNode* stmt_sequence(){//stmt-sequence -> statment[;stmt-sequence]
    TreeNode* merge;
    TreeNode* left;
    left = statement();
    if(matchType(TK_SEMICOLON)){
        TreeNode* right;
        right = stmt_sequence(); 
        merge=new TreeNode(STMT_SEQUENCE,left,right);
    }else{
        merge=left;
    }
    return merge;
}
TreeNode* statement(){//if-stmt|repeat-stmt|read-stmt|write-stmt|while-stmt

}
TreeNode* if_stmt(){

}
TreeNode* repeat_stmt(){

}
TreeNode* assign_stmt(){

}
TreeNode* read_stmt(){

}
TreeNode* write_stmt(){

}
TreeNode* while_stmt(){

}
TreeNode* logical_or_exp(){

}
TreeNode* logical_and_exp(){

}
TreeNode* comparison_exp(){

}
TreeNode* add_exp(){

}
TreeNode* mul_exp(){

}
TreeNode* addop(){

}
TreeNode* factor(){

}
TreeNode* mulop(){

}



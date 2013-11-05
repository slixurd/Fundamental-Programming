#include "common.h"
//#include "scan.h"
//全局变量
int lineNum = 1;
int colNum = 0;
fstream *origin;
fstream *step1;

bool isNumber(char);
bool isAlphabet(char);
bool isLegal(char);
bool isSym(char);
bool isKey(string);
bool isDelim(char);
char getNextChar(fstream *origin){
	if(origin==NULL){
		return -1;//indicate reading file fail;
	}
	char buffer;
    origin->get(buffer);
    if(origin->eof()){
        return -2;//inform that the file complete
    }

	if(!isLegal(buffer)){
		cout<<"\n Illegal alphabet: "<<buffer<<" in the source code in Line:"<<lineNum<<" Column: "<<colNum<<endl;
	}
	if(buffer == '\n'||buffer == '\r'){
        lineNum++;
        colNum = 0;
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
	origin = new fstream("/home/slixurd/university/tiny/in",ios::in);
    int currentState = START;
    string currentWord = "";
    char currentLetter;
    if((currentLetter = getNextChar(origin)) == -1){
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
                            if(isDelim(currentLetter)||currentLetter==';'){
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
                            cout<<" (KEY "<<currentWord<<") ";
                        }else 
                            cout<<" (ID "<<currentWord<<") ";
                        currentState = START;
                        currentWord="";
                        if(isSym(currentLetter)||currentLetter=='\''||currentLetter=='{')
                          origin->unget();
                    }
                    break;
                case STR:
                    //如果找到单引号后第一个字符就是换行符,表明本行缺少左单引号
                    if(currentLetter=='\n'||currentLetter=='\r')
                        cout<<"\n Miss left quotation mark in Line "<<lineNum<<endl;
                    else
                    while(true){
                        if(currentLetter=='\''){
                            cout<<" (STR ";
                            cout<<currentWord<<") ";
                            break;
                        }
                        if(currentLetter=='\n'||currentLetter=='\r'){
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
                    if(isSym(currentLetter)){
                        currentWord+=currentLetter;
                    }else{
                        if(!currentLetter == '\n')
                            origin->unget();
                    }
                    //cout<<"START ";
                    //inform(currentState);
                    //cout<<endl;
                    cout<<" (SYM "<< currentWord<<") ";
                    currentState = START;
                    currentWord="";
                    break;
                    
                case NOTE:
                    if(currentLetter == '}'){
                        currentState = START;
                        currentWord = "";
                    }else if(currentLetter == '\n'||currentLetter == '\r'){
                        //无法匹配右括号
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
    getToken();
	//origin = new fstream("/home/slixurd/university/tiny/in",ios::in);
    //getNextChar(origin);    
    //origin->unget();
    return 0;
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

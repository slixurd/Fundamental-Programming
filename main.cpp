#include "common.h"
//#include "scan.h"
//全局变量
int lineNum = 0;
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
	//cout<<lineNum<<": ";
	//memset(buffer,0,256);
	//while(origin->get(buffer))
	//{
    if(origin->eof()){
        return -2;//inform that the file complete
    }

    origin->get(buffer);
	if(!isLegal(buffer)){
		cout<<"\n Illegal alphabet: "<<buffer<<" in the source code in Line:"<<lineNum<<" Column: "<<colNum<<endl;
	}
	if(buffer == '\n'||buffer== '\r'){
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
                    else if(sym.find(currentLetter)!=-1){
                        currentState = SYM;
                        currentWord+=currentLetter;
                        char nextToCheck = getNextChar(origin);
                        if(isSym(nextToCheck)){
                            currentWord+=nextToCheck;
                        }else{
                            origin->unget();
                        }
                        //cout<<"START ";
                        //inform(currentState);
                        //cout<<endl;
                        cout<<" (SYM "<< currentWord<<") ";
                        currentState = START;
                        currentWord="";
                        break;
                    }
                    //字符串由\'包含
                    else if(currentLetter == '\''){
                        currentState = STR;
                        //cout<<"START ";
                        //inform(currentState);
                        //cout<<endl;
                        break;
                    }
                    //to do 由{}组成的comments
                    //起始状态不接收分隔符
                    else if(isDelim(currentLetter)){
                        break;
                    }
                    //cout<<"START ";
                    //inform(currentState);
                    //cout<<endl;
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
                    } else if(!isNumber(currentLetter)){
                        cout<< "\nillegal variable name\n";
                        //吃掉后面所有不合法的字母
                    }
                    currentWord += currentLetter;
                    break;
                case ID:
                    //DFA已经进入第二步,所有的字母,数字均可以让ID在本状态下保持
                    if(isAlphabet(currentLetter)||isNumber(currentLetter)){
                        currentWord += currentLetter; 
                    }else if(isSym(currentLetter)||isDelim(currentLetter)){
                        if(isKey(currentWord)){
                            cout<<" (KEY "<<currentWord<<") ";
                        }else 
                            cout<<" (ID "<<currentWord<<") ";
                        currentState = START;
                        currentWord="";
                        if(isSym(currentLetter))
                          origin->unget();
                    }
                    break;
                case STR:
                    cout<<" (STR ";
                    while(true){
                        if(currentLetter=='\'')
                          break;
                        if(currentLetter=='\n'||currentLetter=='\r'){
                            cout<<" ERROR HERE ";
                            break;
                        }
                        currentWord+=currentLetter;
                        currentLetter=getNextChar(origin);
                    }
                    cout<<currentWord<<") ";
                    currentWord="";
                    currentState = START;

                    break;

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

#include<iostream>
#include<cmath>
#include<string>
#define MAX 65535
using namespace std;
string lsystem("BAB");//initial of l-system

string patternA("AA");
string patternB("A[B]AA(B)");
string replaceA("A");
string replaceB("B");

struct Point{
    float x1;
    float y1;
    float x2;
    float y2;
};
Point point[MAX];
int count=0;
int loc=0;
float degree(int _degree){
    return _degree*3.1415926/180;
}

void generate(int depth){
    if(depth<=0)return; 
    int found=lsystem.length()+1;
    int found_A=found;
    int found_B=found;
    do{
        found_A=lsystem.rfind(replaceA,found-1);
        found_B=lsystem.rfind(replaceB,found-1);
        //cout<<found_A<<" "<<found_B<<endl;
        found_A>found_B?found=found_A:found=found_B;
        lsystem.replace(found,1,found_A>found_B?patternA:patternB);
    }while(found!=string::npos&&found>0);//当查找到头的时候结束
    //这里查找应该由后往前,否则被替换的string会参与查找,进入死循环
    generate(depth-1);//生成深度

}

void pointGenerate(float x,float y,float turns){
        float xx=x;
        float yy=y;
    while(loc<lsystem.length()){
    
        if(lsystem.at(loc)=='A'){
            point[count].x1=xx;
            point[count].y1=yy;
            xx=xx+sin(degree(turns));
            yy=yy+cos(degree(turns));
            point[count].x2=xx;
            point[count].y2=yy;
            count++;
        loc++;
        }else if(lsystem.at(loc)=='B'){
            point[count].x1=xx;
            point[count].y1=yy;
            xx=xx+sin(degree(turns));
            yy=yy+cos(degree(turns));
            point[count].x2=xx;
            point[count].y2=yy;
            count++;
        loc++;
        }else if(lsystem.at(loc)=='['){
                loc++;
            pointGenerate(point[count-1].x2,point[count-1].y2,turns-45);
        }else if(lsystem.at(loc)==']'){
        loc++;
            return; 
        }else if(lsystem.at(loc)=='('){
                loc++;
            pointGenerate(point[count-1].x2,point[count-1].y2,turns+45);
        }else if(lsystem.at(loc)==')'){
        loc++;
            return;
        } 
    }
}

int main(){
    generate(2);
    pointGenerate(200,0,0);
    for(int i=0;i<count;i++){
        cout<<i<<" "<<point[i].x1<<" "<<point[i].y1<<" "<<point[i].x2<<" "<<point[i].y2<<endl; 
    }
    cout<<lsystem;
}

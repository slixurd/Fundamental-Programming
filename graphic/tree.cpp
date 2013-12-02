#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 #include "stdio.h"   
#include <GL/glut.h>
#include <GL/gl.h>   
#include <GL/glu.h>  
#define MAX 65535
#include<string>
using namespace std;
typedef struct { double x, y; } vec_t, *vec;
 string lsystem("BAB");//initial of l-system
#define DEPTH 4
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
            xx=xx+sin(degree(turns))*5;
            yy=yy+cos(degree(turns))*5;
            point[count].x2=xx;
            point[count].y2=yy;
            count++;
        loc++;
        }else if(lsystem.at(loc)=='B'){
            point[count].x1=xx;
            point[count].y1=yy;
            xx=xx+sin(degree(turns))*5;
            yy=yy+cos(degree(turns))*5;
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


 
void draw(float x0,float y0,float x2,float y2)  
{   

    glEnable(GL_LINE_STIPPLE);   
    glBegin(GL_LINES);    
    glVertex2f(x0,y0);
    glVertex2f(x2,y2);   
    glEnd();   
    glDisable(GL_LINE_STIPPLE);   
}   

void renderScene(void)    
{   
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2);

	int i;
	glColor3f(0.0f,255.0f, 0.0f);
    for(int i=0;i<count;i++){
        glBegin(GL_LINE_STRIP);
            glVertex3f(point[i].x1,point[i].y1,0.0f);   
            glVertex3f(point[i].x2,point[i].y2,0.0f);   
        glEnd();
    }

    glFlush();
       
}   

void ChangeSize(GLsizei w,GLsizei h)
{
    if (h==0) {
        h=1;
    }
   //设置视区尺寸
    glViewport(0, 0, w, h);
    //重置坐标系统，使用投影变换复位
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
    
    if (w<=h) {
       glOrtho (0.0f,600.0f, 0.0f,600.0f*h/w, 1.0f, -1.0f);
    }else
    {
       glOrtho (0.0f,600.0f*w/h, 0.0f,600.0f, 1.0f, -1.0f);
    }
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void SetupRC(void)
{
   glClearColor(1.0f,1.0f, 1.0f,1.0f);
}



int main(int argc, char* argv[])   
{

    generate(DEPTH);
    pointGenerate(300,50,0);
    	glutInit(&argc, argv);    
	glutInitDisplayMode (GLUT_SINGLE |GLUT_RGB |GLUT_DEPTH);
    	glutInitWindowSize(800, 800);     
    	glutInitWindowPosition(100, 100);     
    	glutCreateWindow("L system");   
    	glutDisplayFunc(renderScene);   
    	glutReshapeFunc(ChangeSize);   
	SetupRC();
    	glutMainLoop();   

	return 0;
}

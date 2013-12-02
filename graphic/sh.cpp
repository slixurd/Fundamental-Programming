#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 #include "stdio.h"   
#include <GL/glut.h>
#include <GL/gl.h>   
#include <GL/glu.h>  

typedef struct { double x, y; } vec_t, *vec;
 
inline double dot(vec a, vec b)
{
	return a->x * b->x + a->y * b->y;
}
 
inline double cross(vec a, vec b)
{
	return a->x * b->y - a->y * b->x;
}
 
inline vec vsub(vec a, vec b, vec res)
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	return res;
}
 
/* tells if vec c lies on the left side of directed edge a->b
 * 1 if left, -1 if right, 0 if colinear
 */
int left_of(vec a, vec b, vec c)
{
	vec_t tmp1, tmp2;
	double x;
	vsub(b, a, &tmp1);
	vsub(c, b, &tmp2);
	x = cross(&tmp1, &tmp2);
	return x < 0 ? -1 : x > 0;
}
 
int line_sect(vec x0, vec x1, vec y0, vec y1, vec res)
{
	vec_t dx, dy, d;
	vsub(x1, x0, &dx);
	vsub(y1, y0, &dy);
	vsub(x0, y0, &d);
	/* x0 + a dx = y0 + b dy ->
	   x0 X dx = y0 X dx + b dy X dx ->
	   b = (x0 - y0) X dx / (dy X dx) */
	double dyx = cross(&dy, &dx);
	if (!dyx) return 0;
	dyx = cross(&d, &dx) / dyx;
	if (dyx <= 0 || dyx >= 1) return 0;
 
	res->x = y0->x + dyx * dy.x;
	res->y = y0->y + dyx * dy.y;
	return 1;
}
 
/* === polygon stuff === */
typedef struct { int len, alloc; vec v; } poly_t, *poly;
 
poly poly_new()
{
	poly p = (poly)malloc(sizeof(poly_t));
	p->len = p->alloc = 0;
	p->v = 0;
	return p;
}
 
void poly_free(poly p)
{
	if (p->alloc) {
		free(p->v);
		free(p);
	}
}
 
void poly_append(poly p, vec v)
{
	if (p->len >= p->alloc) {
		p->alloc *= 2;
		if (!p->alloc) p->alloc = 4;
		p->v = (vec)realloc(p->v, sizeof(vec_t) * p->alloc);
	}
	p->v[p->len++] = *v;
}
 
/* this works only if all of the following are true:
 *   1. poly has no colinear edges;
 *   2. poly has no duplicate vertices;
 *   3. poly has at least three vertices;
 *   4. poly is convex (implying 3).
*/
int poly_winding(poly p)
{
	return left_of(p->v, p->v + 1, p->v + 2);
}
 
void poly_edge_clip(poly sub, vec x0, vec x1, int left, poly res)
{
	int i, side0, side1;
	vec_t tmp;
	vec v0 = sub->v + sub->len - 1, v1;
	res->len = 0;
 
	side0 = left_of(x0, x1, v0);
	if (side0 != -left) poly_append(res, v0);
 
	for (i = 0; i < sub->len; i++) {
		v1 = sub->v + i;
		side1 = left_of(x0, x1, v1);
		if (side0 + side1 == 0 && side0)
			/* last point and current straddle the edge */
			if (line_sect(x0, x1, v0, v1, &tmp))
				poly_append(res, &tmp);
		if (i == sub->len - 1) break;
		if (side1 != -left) poly_append(res, v1);
		v0 = v1;
		side0 = side1;
	}
}
 
poly poly_clip(poly sub, poly clip)
{
	int i;
	poly p1 = poly_new(), p2 = poly_new(), tmp;
 
	int dir = poly_winding(clip);
	poly_edge_clip(sub, clip->v + clip->len - 1, clip->v, dir, p2);
	for (i = 0; i < clip->len - 1; i++) {
		tmp = p2; p2 = p1; p1 = tmp;
		poly_edge_clip(p1, clip->v + i, clip->v + i + 1, dir, p2);
	}
 
	poly_free(p1);
	return p2;
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


	int i;
	vec_t c[] = {{100,100}, {300,100}, {300,300}, {100,300}};
	vec_t s[] = {	{50,150}, {200,50}, {350,150},
			{350,300},{250,300},{200,250},
			{150,350},{100,250},{100,200}};
	glColor3f(0.0f,255.0f, 0.0f);
  	glBegin(GL_LINE_STRIP);
		for (i = 0; i < 4; i++){
			glVertex3f(c[i].x,c[i].y,0.0f);   
		}
	  	glVertex3f(c[0].x,c[0].y,0.0f);   
   	 glEnd();

	glColor3f(0.0f,0.0f, 255.0f);
  	glBegin(GL_LINE_STRIP);
		for (i = 0; i < 9; i++){
			glVertex3f(s[i].x,s[i].y,0.0f);   
		}
	  	glVertex3f(s[0].x,s[0].y,0.0f);   
   	 glEnd();


	#define clen (sizeof(c)/sizeof(vec_t))
	#define slen (sizeof(s)/sizeof(vec_t))
	poly_t clipper = {clen, 0, c};
	poly_t subject = {slen, 0, s};
 
glColor3f(0.0f,0.0f, 0.0f);
	poly res = poly_clip(&subject, &clipper);
  glBegin(GL_LINE_STRIP);
	for (i = 0; i < res->len; i++){
		glVertex3f(res->v[i].x,res->v[i].y,0.0f);   
	}
  	glVertex3f(res->v[0].x,res->v[0].y,0.0f);   

    glEnd();
       
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
       glOrtho (0.0f,400.0f, 0.0f,400.0f*h/w, 1.0f, -1.0f);
    }else
    {
       glOrtho (0.0f,400.0f*w/h, 0.0f,400.0f, 1.0f, -1.0f);
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

    	glutInit(&argc, argv);    
	glutInitDisplayMode (GLUT_SINGLE |GLUT_RGB |GLUT_DEPTH);
    	glutInitWindowSize(800, 800);     
    	glutInitWindowPosition(100, 100);     
    	glutCreateWindow("sh algorithm ");   
    	glutDisplayFunc(renderScene);   
    	glutReshapeFunc(ChangeSize);   
	SetupRC();
    	glutMainLoop();   

	return 0;
}

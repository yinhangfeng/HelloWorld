#include <windows.h> 
#include <gl/gl.h> 
#include <gl/glu.h> 
#include <gl/glut.h>
#include <stdio.h>
#define INC 3	//����ת����

struct DATA
{
	int color;	//��ɫ
	int type;	//���������������
}data[55]={	
{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},		//ǰ
{1,},{1,},{1,},{1,},{1,},{1,},{1,},{1,},{1,},		//��
{2,},{2,},{2,},{2,},{2,},{2,},{2,},{2,},{2,},		//��
{3,},{3,},{3,},{3,},{3,},{3,},{3,},{3,},{3,},		//��
{4,},{4,},{4,},{4,},{4,},{4,},{4,},{4,},{4,},		//��
{5,},{5,},{5,},{5,},{5,},{5,},{5,},{5,},{5,}};		//��	
struct DATA temp;
struct DATA *rotation[6][26]={		//��ת����	
{&temp,data+0 ,data+2 ,data+8 ,data+6 ,
 &temp,data+1 ,data+5 ,data+7 ,data+3 ,
 &temp,data+42,data+27,data+47,data+26,
 &temp,data+44,data+33,data+45,data+20,
 &temp,data+43,data+30,data+46,data+23,&temp},		//ǰ

{&temp,data+9 ,data+11,data+17,data+15,
 &temp,data+10,data+14,data+16,data+12,
 &temp,data+38,data+18,data+51,data+35,
 &temp,data+36,data+24,data+53,data+29,
 &temp,data+37,data+21,data+52,data+32,&temp},		//��

{&temp,data+18,data+20,data+26,data+24,
 &temp,data+19,data+23,data+25,data+21,
 &temp,data+36,data+0 ,data+45,data+17,
 &temp,data+42,data+6 ,data+51,data+11,
 &temp,data+39,data+3 ,data+48,data+14,&temp},		//��

{&temp,data+27,data+29,data+35,data+33,
 &temp,data+28,data+32,data+34,data+30,
 &temp,data+44,data+9 ,data+53,data+8 ,
 &temp,data+38,data+15,data+47,data+2 ,
 &temp,data+41,data+12,data+50,data+5,&temp},		//��

{&temp,data+36,data+38,data+44,data+42,
 &temp,data+37,data+41,data+43,data+39,
 &temp,data+11,data+29,data+2 ,data+20,
 &temp,data+9 ,data+27,data+0 ,data+18,
 &temp,data+10,data+28,data+1 ,data+19,&temp},		//��

{&temp,data+45,data+47,data+53,data+51,
 &temp,data+46,data+50,data+52,data+48,
 &temp,data+6 ,data+33,data+15,data+24,
 &temp,data+8 ,data+35,data+17,data+26,
 &temp,data+7 ,data+34,data+16,data+25,&temp}};		//��

struct DATA *cube[26][6]={
//8���ǿ�
{data+0,NULL,data+20,NULL,data+42,NULL},
{data+2,NULL,NULL,data+27,data+44,NULL},
{NULL,data+11,data+18,NULL,data+36,NULL},
{NULL,data+9,NULL,data+29,data+38,NULL},
{data+6,NULL,data+26,NULL,NULL,data+45},
{data+8,NULL,NULL,data+33,NULL,data+47},
{NULL,data+17,data+24,NULL,NULL,data+51},
{NULL,data+15,NULL,data+35,NULL,data+53},
//12�����
{data+1,NULL,NULL,NULL,data+43,NULL},
{NULL,NULL,data+19,NULL,data+39,NULL},
{NULL,NULL,NULL,data+28,data+41,NULL},
{NULL,data+10,NULL,NULL,data+37,NULL},
{data+3,NULL,data+23,NULL,NULL,NULL},
{data+5,NULL,NULL,data+30,NULL,NULL},
{NULL,data+14,data+21,NULL,NULL,NULL},
{NULL,data+12,NULL,data+32,NULL,NULL},
{data+7,NULL,NULL,NULL,NULL,data+46},
{NULL,NULL,data+25,NULL,NULL,data+48},
{NULL,NULL,NULL,data+34,NULL,data+50},
{NULL,data+16,NULL,NULL,NULL,data+52},
//6�����Ŀ�
{NULL,NULL,NULL,NULL,data+40,NULL},
{data+4,NULL,NULL,NULL,NULL,NULL},
{NULL,NULL,data+22,NULL,NULL,NULL},
{NULL,NULL,NULL,data+31,NULL,NULL},
{NULL,data+13,NULL,NULL,NULL,NULL},
{NULL,NULL,NULL,NULL,NULL,data+49}};

float color[7][3]={	//����Ŷ�Ӧ��ɫ����
	1.00,0.00,0.00,		//��
	0.93,0.43,0.00,		//��
	0.00,0.00,1.00,		//��
	0.00,1.00,0.00,		//��
	1.00,1.00,0.00,		//��
	0.00,0.00,0.00,		//��
	0.50,0.50,0.50};	//��

float axis[6][3]={		//�����Ӧת�����
	0.0,0.0,1.0,
	0.0,0.0,-1.0,
	-1.0,0.0,0.0,
	1.0,0.0,0.0,
	0.0,1.0,0.0,
	0.0,-1.0,0.0,	
	};

float offset[26][3]={	//�������������ƫ����
	//8���ǿ�
	-12.0,12.0,12.0,
	12.0,12.0,12.0,
	-12.0,12.0,-12.0,
	12.0,12.0,-12.0,
	-12.0,-12.0,12.0,
	12.0,-12.0,12.0,
	-12.0,-12.0,-12.0,
	12.0,-12.0,-12.0,
	//12�����
	0.0,12.0,12.0,
	-12.0,12.0,0.0,
	12.0,12.0,0.0,
	0.0,12.0,-12.0,
	-12.0,0.0,12.0,
	12.0,0.0,12.0,
	-12.0,0.0,-12.0,
	12.0,0.0,-12.0,
	0.0,-12.0,12.0,
	-12.0,-12.0,0.0,
	12.0,-12.0,0.0,
	0.0,-12.0,-12.0,
	//6�����Ŀ�
	0.0,12.0,0.0,
	0.0,0.0,12.0,
	-12.0,0.0,0.0,
	12.0,0.0,0.0,
	0.0,0.0,-12.0,
	0.0,-12.0,0.0};

struct Action		//��ת��������
{
	int m;	//��ת��
	int d;	//��ת����
}action[48]={{3,-1},{4,1},{1,1},{3,-1},{4,-1},{4,1},{5,1},{4,-1},{3,-1},{4,1},{3,1},{4,-1},{3,-1},{4,1},{3,1},{4,-1},{3,-1},{4,1},{3,1},{4,-1},{3,-1},{4,1},{3,1},{4,-1},
{2,1},{5,1},{2,-1},{5,-1},{2,1},{5,1},{2,-1},{5,-1},{2,1},{5,1},{0,-1},{5,-1},{2,1},{5,1},{2,-1},{5,-1},{2,1},{5,1},{2,-1},{5,-1},{2,1},{5,1},{2,-1},{5,-1}};

//��ת���� m��ת�� d��ת���� d>0��ʱ�� d<0˳ʱ�� d=0����ת
void Rotation(int m,int d)
{
	static int i;
	if(d>0)	
		for(i=0;i<25;i++)
			*rotation[m][i]=*rotation[m][i+1];
	else if(d<0)
		for(i=25;i>0;i--)
			*rotation[m][i]=*rotation[m][i-1];

}

//��÷���n ��m ����ɫ���
int Getcolor(int n,int m)
{
	if(cube[n][m]==NULL)
		return 6;
	else 
		return cube[n][m]->color;
}
//��÷���n ��m ����ɫrgb����
float* Getrgb(int n,int m)
{
	return *(color+Getcolor(n,m));
}
	
void init() 
	{ 
	glEnable(GL_DEPTH_TEST);	//  ������Ȳ���
	glEnable(GL_CULL_FACE);		//���û����޳�
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f ); //����Ϊ��ɫ
	} 
void reshape(int w, int h) 
	{ 
	if(h == 0) h = 1; 
	//  ���������ߴ�
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	//  �����޼��ռ�
	if (w <= h)   
		glOrtho (-35.0f, 35.0f, -35.0f*h/w, 35.0f*h/w, -40.0f, 40.0f); 
	else   
		glOrtho (-35.0f*w/h, 35.0f*w/h, -35.0f, 35.0f, -40.0f, 40.0f);
	glMatrixMode(GL_MODELVIEW);
	}
 
void display(void) 
	{
	static int i;	//��ǰ���Ʒ�����
	static int act=0;		//��ǰ�������
	static float r,g,b;
	static float angle1 = 0.0;	//����ת�Ƕ�
	static float angle2 = 0.0;	//������ת�Ƕ�

/*	static LARGE_INTEGER l1,l2;
	static double s,f;
	static int j=1;
	QueryPerformanceFrequency(&l1);	//��ü�������ʱ��Ƶ��
	f=(double)l1.QuadPart;
	QueryPerformanceCounter(&l1); //��ó�ʼֵ	*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	//  ����ģ����ͼ����
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glRotatef(40, 1.0f, 0.0f, 0.0f);
	glRotatef(angle2, 0.0f, 1.0f, 0.0f);
	for(i=0;i<26;i++)
		{
		if(Getcolor(i,action[act].m)!=6)
			{
			glPushMatrix();
			glRotatef(angle1, axis[action[act].m][0], axis[action[act].m][1], axis[action[act].m][2]);
			}
		glBegin(GL_QUADS);
		//ǰ
		glColor3fv(Getrgb(i,0));		
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);
		// ��
		glColor3fv(Getrgb(i,1));
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);
		// ��
		glColor3fv(Getrgb(i,2));	
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);
		// ��
		glColor3fv(Getrgb(i,3));
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);
		// ��
		glColor3fv(Getrgb(i,4));
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0],  5.0+offset[i][1], -5.0+offset[i][2]);	
		// ��
		glColor3fv(Getrgb(i,5));
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1], -5.0+offset[i][2]);	
		glVertex3f( 5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);	
		glVertex3f(-5.0+offset[i][0], -5.0+offset[i][1],  5.0+offset[i][2]);	
		glEnd();
		if(Getcolor(i,action[act].m)!=6)
			glPopMatrix(); 
		}	
		angle1+=action[act].d*INC; //  ��������ת�Ƕ�
		angle2-=0.1;				//  ����������ת�Ƕ�
		if(angle1>=90.0||angle1<=-90.0)
		{
			angle1 = 0.0; 
			Rotation(action[act].m,action[act].d);
			act=++act%48;
		}
		if(angle2<-360.0) 
			angle2 = 0.0; 
		glutSwapBuffers(); 

/*		QueryPerformanceCounter(&l2);	//�����ֵֹ
		s+=(double)(l2.QuadPart-l1.QuadPart)/f;
		j++;
		if(j==1000)
		{
			printf("%f\n",s);
			j=1;
			s=0;
		}*/
	} 

void TimerFunc(int value) 
	{ 
	glutPostRedisplay(); 
	glutTimerFunc(16, TimerFunc,1); 
	}

int main(int argc, char* argv[]) 
	{ 
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600,600);
	glutCreateWindow("ħ��"); 
	init(); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutTimerFunc(1000, TimerFunc, 1); 
	glutMainLoop(); 
	return 0; 
	} 

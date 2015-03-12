#include<conio.h>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
int data[20][20];
int(*p)[20],x,y,z,r,retry;


void g(int i,int j)
{
p[i][j]=p[i][j]&47;
r++;
if((p[i][j]&15)==0)
{
if(i+1<20&&(p[i+1][j]&16)!=0)
g(i+1,j);
if(j-1>=0&&(p[i][j-1]&16)!=0)
g(i,j-1);
if(i-1>=0&&(p[i-1][j]&16)!=0)
g(i-1,j);
if(j+1<20&&(p[i][j+1]&16)!=0)
g(i,j+1);
}
}       


void f(void)      
{
register int i,j,a,b,c,d,t;
for(i=0;i<20;i++)
for(j=0;j<20;j++)
p[i][j]=0;
t=time(NULL);
for(a=0,b=2;a<50;b+=5)
{srand(t+b);
i=rand()%20;
srand(t-100*b);
j=rand()%20;
if(p[i][j]==0)
{p[i][j]=9;a++;}
}

for(i=0;i<20;i++)
for(j=0;j<20;j++)
{
if((p[i][j]&15)==0)
{
if(i==0)
{a=0;b=1;}
else if(i==19)
{a=18;b=19;}
else {a=i-1;b=i+1;}
if(j==0)
{t=0;d=1;}
else if(j==19)
{t=18;d=19;}
else {t=j-1;d=j+1;}
for(;a<=b;a++)
for(c=t;c<=d;c++)
{
if((p[a][c]&15)==9)
p[i][j]++;
}
}
p[i][j]+=16;
}
p[0][0]=p[0][0]^32;
}

void print(void)   
{
register int i,j;
for(i=0;i<20;i++)
for(j=0;j<20;j++)
{
switch (p[i][j]&112)
{
case 16:printf("□");break;
case 32:{switch (p[i][j]&15)
{
case 0:printf("●");break;
case 1:printf("①");break;
case 2:printf("②");break;
case 3:printf("③");break;
case 4:printf("④");break;
case 5:printf("⑤");break;
case 6:printf("⑥");break;
case 7:printf("⑦");break;
case 8:printf("⑧");break;
case 9:printf("※");break;
default : break;
		}}break;
case 48: printf("■");break;
case 80: printf("☆");break;
case 112:printf("★");break;
case 0:{switch (p[i][j]&15)
{
case 0:printf("○");break;
case 1:printf("１");break;
case 2:printf("２");break;
case 3:printf("３");break;
case 4:printf("４");break;
case 5:printf("５");break;
case 6:printf("６");break;
case 7:printf("７");break;
case 8:printf("８");break;
case 9:printf("※");break;
default :break;
	   }}
default :break;}if((j+1)%20==0)
printf("\n");}}



void move(int i,int j)   
{
p[x][y]=p[x][y]^32;
if(i<0)
x=19;
else if(i>19)
x=0;
else x=i;
	
if(j<0)
y=19;
else if(j>19)
y=0;
else y=j;
p[x][y]=p[x][y]^32;
}
void q(void)
{
register int i,j;
for(i=0;i<20;i++)
for(j=0;j<20;j++)
p[i][j]=p[i][j]&47;
}    


void k(void)  
{
register char a;
rk:
switch(getch())
{
case 'w':move(x-1,y);break;
case 'a':move(x,y-1);break;
case 's':move(x+1,y);break;
case 'd':move(x,y+1);break;
case 'j':
if((p[x][y]&16)==0)
goto rk;
{
g(x,y);
if((p[x][y]&15)==9)
{
system("cls");
printf("你输了!是否再玩一次?(y/n)\n");
q();
print();
B:
a=getch();
if(a=='y')
retry=1;
else if(a=='n')
z=0;
else goto B;
} 
else if(r==350)
{
system("cls");
printf("你赢了!是否再玩一次?(y/n)\n");
q();
print();
A:
a=getch();

if(a=='y')
retry=1;
else if(a=='n')
z=0;
else goto A;
}
}
break;
case 'k':{if((p[x][y]&16)!=0) p[x][y]=p[x][y]^64;
	else goto rk;}
	break;
case 'q':
{
system("cls");
printf("确定退出?(y/n)\n");
print();
C:
a=getch();
if(a=='y')
z=0;
else if(a!='n')
goto C;
} 
break;
default : goto rk ; break;
}
}


int main()
{
p=data;
printf("扫雷\n作者：dark_wander\n操作方法:\n方向:w a s d\n挖雷:j,标记:k,退出游戏:q\n");
retry:
r=0;
x=y=0;
z=1;
system("pause");
f();
retry=0;
while(z)
{
system("cls");
printf("扫雷\n");
print();
k();
if(retry==1)
goto retry;
}
system("cls");
printf("游戏结束\n");
system("pause");
return 0;
}
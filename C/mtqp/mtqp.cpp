#include<iostream>
#include<iomanip>
#include<stack>
using namespace std;

struct NODE							//定义坐标结构体
{
	int x,y;
	bool a[8];						//标记某方向是否走过
};


class abc
{
public:
	bool judge(int x,int y);			//判断是否越界
	int lushu(int x,int y);				//计算某格接下来可走的路数
	bool next();						//是否有下一步
	void walk(int x,int y);
	void print();
private:
	int step;
	int cb[8][8];
	stack<NODE> st;
	NODE cur;
	static int dir[8][2];
};

int abc::dir[8][2]={-1,-2,1,-2,2,-1,2,1,1,2,-1,2,-2,1,-2,-1};

void abc::print()
{
	int i,j;
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
		{
			cout<<setw(3)<<cb[j][i];
		}
		cout<<endl;
	}
}


bool abc::judge(int x,int y)
{
	return x>=0&&x<=7&&y>=0&&y<=7;
}

int abc::lushu(int x,int y)
{
	int a,b,s=0;
	for(int i=0;i<8;i++)
	{
		a=x+dir[i][0];
		b=y+dir[i][1];
		if(judge(a,b)&&cb[b][a]==0)
			s++;		
	} 
	return s;
}


bool abc::next()
{
	int j,x,y,s=9,min=9;
	for(int i=0;i<8;i++)
	{
		if(cur.a[i])
			continue;
		x=dir[i][0]+cur.x;
		y=dir[i][1]+cur.y;
		if(judge(x,y)&&cb[y][x]==0)		
			s=lushu(x,y);
		if(min>s)
		{
			min=s;
			j=i;
		}
	}
	if(min==9)
		return true;
	cur.a[j]=true;
	st.push(cur);
	cb[cur.y][cur.x]=step++;
	cur.x+=dir[j][0];
	cur.y+=dir[j][1];
	for(int k=0;k<8;k++)
	{
		cur.a[k]=false;
	}
	return false;
}


void abc::walk(int x,int y)
{
	for(int j=0;j<8;j++)
	{
		for(int i=0;i<8;i++)
			cb[j][i]=0;
	}
	step=1;	
	cur.x=x;
	cur.y=y;
	for(int i=0;i<8;i++)
	{
		cur.a[i]=false;
	}
	while(step<64)
	{	
		while(next())
		{
			cur=st.top();
			cb[cur.y][cur.x]=0;
			step--;
			st.pop();					
		}		
	}
	cb[cur.y][cur.x]=step;
}


int main()
{	
	int x,y;
	abc ab;
	while(cin>>x>>y)
	{
		if(ab.judge(x,y))
		{
			ab.walk(x,y);	
			ab.print();
		}
	}
	return 0;
}
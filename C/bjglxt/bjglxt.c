#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>

struct student{
	int n;			//学号
	char name[11];
	char zhuanye[21];		//专业
	char sex;
	float *score;	//指向多个成绩的指针
	float sum;
	float custom;	//自定义成绩统计信息
};

struct LinkNode{
	struct student data;
	struct LinkNode *next;
};

int len;	
struct LinkNode *head;
struct LinkNode *tail;

int m;			//学科数
char (*xkm)[21];	//学科名字指针
char custom[51];	//自定义成绩统计信息名
int save;		//判断最后一次修改后有无保存


void LinkList(void)		//初始化单链表
{
	len=0;
	head=tail=(struct LinkNode*)malloc(sizeof(struct LinkNode));
	head->next=NULL;
}

void clear()		//置单链表为空
{
	struct LinkNode *p=head->next,*q;
	while(p)
	{
		q=p->next;
		free(p->data.score);
		free(p);
		p=q;
	}
	tail=head;
	head->next=NULL;
	len=0;
}

/*******************************打印*************************************/
void print(struct student *data)		//打印传入节点信息
{
	int i;
	printf("%08d | %6s | %6s | %4c",data->n,data->name,data->zhuanye,data->sex)	;
	for(i=0;i<m;i++)
		printf(" | %5.1f",data->score[i]);
	printf(" | %5.1f",data->sum);
	if(strlen(custom))
		printf(" | %5.1f",data->custom);
	printf("\n");
	return;
}

void PrintTitle(void)		//打印信息标题
{
	int i;
	printf("  学号   |  名字  |  专业  | 性别");
		for(i=0;i<m;i++)
			printf(" | %5s",xkm[i]);	
	printf(" |  总分");
	if(strlen(custom))
		printf(" | %5s",custom);
	printf("\n");
}

void AllPrint(void)		//打印所有学生信息
{
	struct LinkNode *p;
	PrintTitle();
	for(p=head->next;p!=NULL;p=p->next)
		print(&p->data);
	system("pause");
}

/*******************************信息录入*****************************************/
void sum(struct student *p)			//求学生总成绩
{
	int i;
	p->sum=0;
	for(i=0;i<m;i++)
		p->sum+=p->score[i];
}

void AddInfo(void)		//添加信息
{
	int i;
	struct LinkNode *p;
	save=1;
	if(m==0)
	{
		printf("输入科目数:");
		scanf("%d",&m);
		xkm=malloc(m*21);
		fflush(stdin);
		for(i=0;i<m;i++)	
		{
			printf("输入第%d个学科名(最多20个字符):",i+1);
			gets(xkm[i]);
		}
	}
	while(1)
	{
		p=(struct LinkNode*)malloc(sizeof(struct LinkNode));
		p->data.score=(float*)malloc(m*sizeof(float));
		printf("添加第%d个学生信息\n输入学号(最多10位):",len+1);
		scanf("%d",&p->data.n);
		fflush(stdin);
		printf("输入名字(最多10字符):");
		gets(p->data.name);
		printf("输入专业(最多20字符):");
		gets(p->data.zhuanye);
		printf("输入性别(用一个字符表示):");
		scanf("%c",&p->data.sex);
		for(i=0;i<m;i++)
		{
			printf("输入%s学科成绩:",xkm[i]);
			scanf("%f",p->data.score+i);
		}
		sum(&p->data);
		tail->next=p;
		tail=p;
		tail->next=NULL;
		len++;
		printf("是否继续输入?(按回车继续,其他取消)\n");
			if(getch()!=13)
				break;
	}
}

/*********************************查询修改和删除****************************************/
struct LinkNode *LocateElem(int n)		//按学号查找单链表返回找到结点前一个结点指针
{
	struct LinkNode *p=head;
	for(;p->next!=NULL;p=p->next)
	{
		if(p->next->data.n==n)	
			return p;
	}
	return NULL;
}

struct LinkNode *LocateElemName(char *name)		//按名字查找单链表返回找到结点前一个结点指针
{
	struct LinkNode *p=head;
	for(;p->next!=NULL;p=p->next)
	{
		if(!strcmp(p->next->data.name,name))	
			return p;
	}
	return NULL;
}

void ModifyScore(struct LinkNode *p)		//修改传入结点指定科目的成绩
{
	int i;
	printf("学科编号: ");
	for(i=0;i<m;i++)
		printf("%d.%s  ",i+1,xkm[i]);
	while(1)
	{
		printf("\n输入要修改的学科编号(输入0取消):");	
		scanf("%d",&i);
		if(i==0)
			return;
		printf("输入成绩:");	
		scanf("%f",p->data.score+i-1);
		save=1;
	}
	sum(&p->data);
	return;
}

void Modify(struct LinkNode *p)		//修改信息
{
	int a;
	while(1)
	{
		printf("请选择要修改的信息\n1.学号 2.名字 3.专业 4.性别 5.修改成绩信息 6.取消\n");
		a=getch();
		fflush(stdin);
		switch(a)
		{
		case '1':printf("输入学号:");scanf("%d",&p->data.n);save=1;break;
		case '2':printf("输入名字(最多10字符):");gets(p->data.name);save=1;break;
		case '3':printf("输入专业(最多20字符):");gets(p->data.zhuanye);save=1;break;
		case '4':printf("输入性别:");scanf("%c",&p->data.sex);save=1;break;
		case '5':ModifyScore(p);break;
		default:return;
		}
	}
}


void del(struct LinkNode *p)	//删除结点
{
	struct LinkNode *t;
	if((t=p->next)==tail)
		tail=p;
	p->next=t->next;
	free(t->data.score);
	free(t);
	len--;
	save=1;
}
	

void Find(void)		//查询修改和删除
{
	int a;
	int n;
	char name[21];
	struct LinkNode *p;
	printf("查找要修改或删除的学生\n");
	while(1)
	{
		while(1)
		{
			printf("请选择查找方式\n1.按学号查找  2.按名字查找  3.返回主界面\n");
			switch(getch())
			{
			case '1':printf("输入学号:");
				scanf("%d",&n);
				p=LocateElem(n);break;
			case '2':fflush(stdin);printf("输入名字(最多10个字符):");
				gets(name);
				p=LocateElemName(name);break;
			default:return;
			}
			if(p)
				break;
			printf("未找到该学生信息\n");
		}
		printf("\n查找到学生信息如下\n");
		PrintTitle();
		print(&p->next->data);
		printf("\n请选择操作\n1.修改信息 2.删除 3.取消\n");
		a=getch();
		if(a=='1')
			Modify(p->next);
		else if(a=='2')
		{
			del(p);
			printf("删除成功!\n");
		}
	}
}

/******************************排序********************************/

int NKeyword;		//关键字个数
int Keyword[10];		//关键字序列 1表示学号关键字，2表示总成绩关键字，3 4....分别为各门学科关键字


//关键字比较函数
int Compare(struct LinkNode *old,struct LinkNode *pivot)
{
	int i;
	int key;
	for(i=0;i<NKeyword;i++)
	{
		key=Keyword[i];
		switch(key)
		{
		case 1:if(old->data.n<pivot->data.n) return 1;break;
		case 2:if(old->data.sum<pivot->data.sum) return 1;break;
		default:if(old->data.score[key-3]<pivot->data.score[key-3]) return 1;break;		
		}	
	}
	return 0;
}


//链表快排序
void QListSort(struct LinkNode **head, struct LinkNode *end) {		  
struct LinkNode *right;   
struct LinkNode **left_walk, **right_walk;   
struct LinkNode *pivot, *old;   
int count, left_count, right_count;   
if (*head == end)   
return;   
do {   
pivot = *head;   
left_walk = head;   
right_walk = &right;   
left_count = right_count = 0;   
//取第一个节点作为比较的基准，小于基准的在左面的子链表中，  
//大于基准的在右边的子链表中    
for (old = (*head)->next; old != end; old = old->next) {   
if (Compare(old,pivot)) {   /*小于基准,加入到左面的子链表,继续比较 */   
    ++left_count;   
	    *left_walk = old;            /*把该节点加入到左边的链表中，  */  
	    left_walk = &(old->next);   
	} else {                         /*大于基准,加入到右边的子链表，继续比较 */   
	    ++right_count;   
	    *right_walk = old;              
	    right_walk = &(old->next);   
	}   
}   
/*合并链表  */ 
*right_walk = end;       /*结束右链表  */ 
*left_walk = pivot;      /*把基准置于正确的位置上  */ 
pivot->next = right;     /*把链表合并   */ 
/*对较小的子链表进行快排序，较大的子链表进行迭代排序。*/   
if(left_walk > right_walk) {   
	QListSort(&(pivot->next), end);   
	end = pivot;   
	count = left_count;   
} else {   
	QListSort(head, pivot);   
	head = &(pivot->next);   
	count = right_count;   
}   
} while (count > 1); 
while(tail->next!=NULL)
	tail=tail->next;  
}


void Sort(void)		//排序
{
	int i;
	printf("关键字编号1.学号 2.总分");
		for(i=0;i<m;i++)
			printf(" %d.%s",i+3,xkm[i]);
	printf("\n");	
	for(i=0;i<10;i++)
	{
		printf("请输入第%d个关键字(输入0结束):",i+1);
		scanf("%d",&Keyword[i]);
		if(Keyword[i]==0)
			break;
	}
	if(i==0)
		return;
	NKeyword=i;
	QListSort(&head->next,NULL);
	printf("排序结束\n");
	save=1;
	system("pause");
}


/***************************统计功能********************************/

void HighScore(void)		//统计最高分
{
	struct LinkNode *p;
	int i;
	struct LinkNode* ptr[10];
	int n;
	while(1)
	{
		printf("请选择要统计的学科(输入0取消)\n1.总分");
		for(i=0;i<m;i++)
			printf(" %d.%s",i+2,xkm[i]);
		printf("\n");
		scanf("%d",&i);
		p=head->next;
		if(i<1||i>m+1)
			break;
		else if(i==1)
		{
			ptr[0]=p;
			n=1;
			for(p=p->next;p!=NULL;p=p->next)
			{
				if(p->data.sum>ptr[0]->data.sum)
				{
					ptr[0]=p;
					n=1;
				}
				else if(p->data.sum==ptr[0]->data.sum)
				{
					ptr[n]=p;
					n++;					
				}
			}
			printf("\n总分最高为: %4.1f\n最高分学生信息如下\n",ptr[0]->data.sum);
			PrintTitle();
			for(i=0;i<n;i++)
				print(&ptr[i]->data);
			printf("\n");
		}
		else
		{
			ptr[0]=p;
			n=1;
			for(p=p->next;p!=NULL;p=p->next)
			{
				if(p->data.score[i-2]>ptr[0]->data.score[i-2])
				{
					ptr[0]=p;
					n=1;
				}
				else if(p->data.score[i-2]==ptr[0]->data.score[i-2])
				{
					ptr[n]=p;
					n++;					
				}
			}
			printf("\n%s学科最高为: %4.1f\n最高分学生信息如下\n",xkm[i-2],ptr[0]->data.score[i-2]);
			PrintTitle();
			for(i=0;i<n;i++)
				print(&ptr[i]->data);
			printf("\n");		
		}
	}
}

void AverageScore(void)		//计算各学科平均分
{
	struct LinkNode *p;
	double *sum=(double *)malloc((m+1)*sizeof(double));	
	int i;
	for(i=0;i<=m;i++)
			sum[i]=0;
	for(p=head->next;p!=NULL;p=p->next)
	{
		for(i=0;i<m;i++)
			sum[i]+=p->data.score[i];
		sum[m]+=p->data.sum;
	}
	printf("\n总分平均分为: %4.1f\n",sum[m]/len);
	for(i=0;i<m;i++)
		printf("\n%s学科平均分为: %4.1f\n",xkm[i],sum[i]/len);
	system("pause");
}

void Interval(void)		//输出某分数区间人数
{
	int i;
	int a;
	int n;
	float min;
	float max;
	struct LinkNode *p;
	while(1)
	{
		n=0;
		printf("\n项目代号: 1.总分");
		for(i=0;i<m;i++)
			printf(" %d.%s",i+2,xkm[i]);
		printf("\n");
		printf("请选择要统计项目(输入0取消):");
		scanf("%d",&a);
		if(a==0)
			break;
		fflush(stdin);
		printf("请输入分数区间(格式a b):");
		scanf("%f %f",&min,&max);
		printf("\n满足条件的学生信息如下\n");
		PrintTitle();
		if(a==1)
			for(p=head->next;p!=NULL;p=p->next)
			{
				if(p->data.sum>=min&&p->data.sum<=max)
				{
					n++;
					print(&p->data);
				}
			}			
		else
			for(p=head->next;p!=NULL;p=p->next)
			{
				if(p->data.score[a-2]>=min&&p->data.score[a-2]<=max)
				{
					n++;
					print(&p->data);
				}
			}	
		printf("总人数为: %d\n",n);
	}
}


void Statistics(void)		//统计功能
{
	while(1)
	{
		printf("\n请选择功能\n1.最高分 2.平均分 3.分数区间人数 4.自定义统计量 5.返回主界面\n");
		switch(getch())
		{
		case '1':HighScore();break;
		case '2':AverageScore();break;
		case '3':Interval();break;
		case '4':break;
		default:return;		
		}
	}
}



/**********************************保存和载入*******************************************/
char *filename(void)			//获取文件名
{
	char *filename=(char *)malloc(50);
	char *txt=".txt";
	fflush(stdin);
	printf("输入文件名:");
	gets(filename);
	strcat(filename,txt);
	return filename;
}

void Save(void)				//保存文件
{
	FILE *fp;
	struct LinkNode *p;
	char n[11];				//暂时保存学号和成绩转成的字符串
	int i;
	char *fn=filename();
	if(!access(fn,0))
	{
		printf("文件已存在 是否覆盖(回车确定,其它取消)?\n");
		if(getch()!=13)
			return;
	}
	if(!(fp=fopen(fn,"w")))
	{
		printf("打开文件失败\n");
		system("pause");
		return;
	}
	fprintf(fp,"      学号        名字                  专业  性别");
	for(i=0;i<m;i++)
		{
			fprintf(fp,"  %8s",xkm[i]);
		}
	fprintf(fp,"      总分\n");
	for(p=head->next;p!=NULL;p=p->next)
	{
		sprintf(n,"%10d",p->data.n);
		fputs(n,fp);
		fprintf(fp,"  %10s  %20s     %c",p->data.name,p->data.zhuanye,p->data.sex);
		for(i=0;i<m;i++)
		{
			sprintf(n,"%8.1f",p->data.score[i]);
			fprintf(fp,"  %s",n);
		}
		sprintf(n,"%8.1f",p->data.sum);
		fprintf(fp,"  %s\n",n);
	}
	fclose(fp);
	printf("保存成功\n");
	save=0;
	system("pause");
}


void fileTitle(FILE *fp)		//获取文件标题信息
{
	int i=0,n=0,j;
	char a[200];
	fgets(a,200,fp);
	while(1)
	{
		while(a[i]==32)
		{i++;}
		if(a[i]==10)
			break;
		if(n==4)
			j=i;
		while(a[i]!=32&&a[i]!=10)
		{i++;}				
		n++;
	}
	if(xkm)
		free(xkm);
	m=n-5;
	xkm=malloc(m*21);
	for(i=0;i<m;i++)
	{
		n=0;
		while(1)
		{
			if(a[j]==32)
			{
				xkm[i][n]=0;
				break;
			}
			xkm[i][n]=a[j];
			n++;
			j++;
		}
		while(a[j]==32)
		{j++;}	
	}
}


char *Word(FILE *fp)		//获取一条记录中的一个字
{
	char *word=(char *)malloc(21);
	char c;
	int i=0;
	while((c=getc(fp))==32)
	{;}
	word[0]=c;
	i++;
	c=getc(fp);
	while(1)
	{
		if(c==32||c==10||c==EOF)
		{
			word[i]=0;
			break;
		}
		word[i]=c;
		c=getc(fp);		
		i++;	
	}
	return word;	
}


void load(void)			//读取文件
{
	FILE *fp;
	struct LinkNode *p;
	int i;
	if(save)
	{
		printf("已有信息存在,是否先保存原有信息?\n1.保存 2.不保存 3.取消\n");
		i=getch();
		if(i=='1')
			Save();
		else if(i!=2)
			return;
	}
	if(!(fp=fopen(filename(),"r")))
	{
		printf("打开文件失败\n");
		system("pause");
		return;
	}
	clear();
	fileTitle(fp);
	while(1)
	{
	p=(struct LinkNode*)malloc(sizeof(struct LinkNode));
	p->data.score=(float*)malloc(m*sizeof(float));
	sscanf(Word(fp),"%d",&p->data.n);
	strcpy(p->data.name,Word(fp));
	strcpy(p->data.zhuanye,Word(fp));
	p->data.sex=Word(fp)[0];
	for(i=0;i<m;i++)
		sscanf(Word(fp),"%f",&p->data.score[i]);
	sscanf(Word(fp),"%f",&p->data.sum);
	tail->next=p;
	tail=p;
	tail->next=NULL;
	len++;
	if(getc(fp)==EOF)
		break;
	fseek(fp,-1,1);
	}
	fclose(fp);
	printf("读取成功\n");
	save=0;
	system("pause");

}

/******************************************************************************/

void Quit(void)		//退出函数
{
	int a;
	if(save)
	{
		printf("信息已修改是否保存?\n1.保存 2.不保存 3.取消\n");
		a=getch();
		if(a=='1')
			Save();
		else if(a=='3')
			return;
	}
	else
	{
		printf("是否退出?\n1.退出 2.取消\n");
		if(getch()!='1')
			return;
	}	
	clear();
	free(head);
	if(xkm)
		free(xkm);
	exit(0);
}


int f(void)		//判断是否有数据
{
	if(len)
		return 1;
	printf("当前没有数据,请添加或载入!\n");
	system("pause");
	return 0;
}


int main()
{
	int a;
	LinkList();
	while(1)
	{
		system("cls");
		printf("班级管理系统\n1.信息录入\n2.查询修改和删除\n3.统计\n4.排序\n5.输出\n6.保存\n7.载入\n8.退出");
		a=getch();
		system("cls");
		switch(a)
		{
		case '1':AddInfo(); break;
		case '2':if(f()) Find();break;
		case '3':if(f()) Statistics();break;
		case '4':if(f()) Sort(); break;
		case '5':if(f()) AllPrint();break;
		case '6':if(f()) Save();break;
		case '7':load();break;
		case '8':Quit();break;
		}	
	}
	return 0;
}
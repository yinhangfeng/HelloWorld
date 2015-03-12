#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>

struct student{
	int n;			//ѧ��
	char name[11];
	char zhuanye[21];		//רҵ
	char sex;
	float *score;	//ָ�����ɼ���ָ��
	float sum;
	float custom;	//�Զ���ɼ�ͳ����Ϣ
};

struct LinkNode{
	struct student data;
	struct LinkNode *next;
};

int len;	
struct LinkNode *head;
struct LinkNode *tail;

int m;			//ѧ����
char (*xkm)[21];	//ѧ������ָ��
char custom[51];	//�Զ���ɼ�ͳ����Ϣ��
int save;		//�ж����һ���޸ĺ����ޱ���


void LinkList(void)		//��ʼ��������
{
	len=0;
	head=tail=(struct LinkNode*)malloc(sizeof(struct LinkNode));
	head->next=NULL;
}

void clear()		//�õ�����Ϊ��
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

/*******************************��ӡ*************************************/
void print(struct student *data)		//��ӡ����ڵ���Ϣ
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

void PrintTitle(void)		//��ӡ��Ϣ����
{
	int i;
	printf("  ѧ��   |  ����  |  רҵ  | �Ա�");
		for(i=0;i<m;i++)
			printf(" | %5s",xkm[i]);	
	printf(" |  �ܷ�");
	if(strlen(custom))
		printf(" | %5s",custom);
	printf("\n");
}

void AllPrint(void)		//��ӡ����ѧ����Ϣ
{
	struct LinkNode *p;
	PrintTitle();
	for(p=head->next;p!=NULL;p=p->next)
		print(&p->data);
	system("pause");
}

/*******************************��Ϣ¼��*****************************************/
void sum(struct student *p)			//��ѧ���ܳɼ�
{
	int i;
	p->sum=0;
	for(i=0;i<m;i++)
		p->sum+=p->score[i];
}

void AddInfo(void)		//�����Ϣ
{
	int i;
	struct LinkNode *p;
	save=1;
	if(m==0)
	{
		printf("�����Ŀ��:");
		scanf("%d",&m);
		xkm=malloc(m*21);
		fflush(stdin);
		for(i=0;i<m;i++)	
		{
			printf("�����%d��ѧ����(���20���ַ�):",i+1);
			gets(xkm[i]);
		}
	}
	while(1)
	{
		p=(struct LinkNode*)malloc(sizeof(struct LinkNode));
		p->data.score=(float*)malloc(m*sizeof(float));
		printf("��ӵ�%d��ѧ����Ϣ\n����ѧ��(���10λ):",len+1);
		scanf("%d",&p->data.n);
		fflush(stdin);
		printf("��������(���10�ַ�):");
		gets(p->data.name);
		printf("����רҵ(���20�ַ�):");
		gets(p->data.zhuanye);
		printf("�����Ա�(��һ���ַ���ʾ):");
		scanf("%c",&p->data.sex);
		for(i=0;i<m;i++)
		{
			printf("����%sѧ�Ƴɼ�:",xkm[i]);
			scanf("%f",p->data.score+i);
		}
		sum(&p->data);
		tail->next=p;
		tail=p;
		tail->next=NULL;
		len++;
		printf("�Ƿ��������?(���س�����,����ȡ��)\n");
			if(getch()!=13)
				break;
	}
}

/*********************************��ѯ�޸ĺ�ɾ��****************************************/
struct LinkNode *LocateElem(int n)		//��ѧ�Ų��ҵ��������ҵ����ǰһ�����ָ��
{
	struct LinkNode *p=head;
	for(;p->next!=NULL;p=p->next)
	{
		if(p->next->data.n==n)	
			return p;
	}
	return NULL;
}

struct LinkNode *LocateElemName(char *name)		//�����ֲ��ҵ��������ҵ����ǰһ�����ָ��
{
	struct LinkNode *p=head;
	for(;p->next!=NULL;p=p->next)
	{
		if(!strcmp(p->next->data.name,name))	
			return p;
	}
	return NULL;
}

void ModifyScore(struct LinkNode *p)		//�޸Ĵ�����ָ����Ŀ�ĳɼ�
{
	int i;
	printf("ѧ�Ʊ��: ");
	for(i=0;i<m;i++)
		printf("%d.%s  ",i+1,xkm[i]);
	while(1)
	{
		printf("\n����Ҫ�޸ĵ�ѧ�Ʊ��(����0ȡ��):");	
		scanf("%d",&i);
		if(i==0)
			return;
		printf("����ɼ�:");	
		scanf("%f",p->data.score+i-1);
		save=1;
	}
	sum(&p->data);
	return;
}

void Modify(struct LinkNode *p)		//�޸���Ϣ
{
	int a;
	while(1)
	{
		printf("��ѡ��Ҫ�޸ĵ���Ϣ\n1.ѧ�� 2.���� 3.רҵ 4.�Ա� 5.�޸ĳɼ���Ϣ 6.ȡ��\n");
		a=getch();
		fflush(stdin);
		switch(a)
		{
		case '1':printf("����ѧ��:");scanf("%d",&p->data.n);save=1;break;
		case '2':printf("��������(���10�ַ�):");gets(p->data.name);save=1;break;
		case '3':printf("����רҵ(���20�ַ�):");gets(p->data.zhuanye);save=1;break;
		case '4':printf("�����Ա�:");scanf("%c",&p->data.sex);save=1;break;
		case '5':ModifyScore(p);break;
		default:return;
		}
	}
}


void del(struct LinkNode *p)	//ɾ�����
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
	

void Find(void)		//��ѯ�޸ĺ�ɾ��
{
	int a;
	int n;
	char name[21];
	struct LinkNode *p;
	printf("����Ҫ�޸Ļ�ɾ����ѧ��\n");
	while(1)
	{
		while(1)
		{
			printf("��ѡ����ҷ�ʽ\n1.��ѧ�Ų���  2.�����ֲ���  3.����������\n");
			switch(getch())
			{
			case '1':printf("����ѧ��:");
				scanf("%d",&n);
				p=LocateElem(n);break;
			case '2':fflush(stdin);printf("��������(���10���ַ�):");
				gets(name);
				p=LocateElemName(name);break;
			default:return;
			}
			if(p)
				break;
			printf("δ�ҵ���ѧ����Ϣ\n");
		}
		printf("\n���ҵ�ѧ����Ϣ����\n");
		PrintTitle();
		print(&p->next->data);
		printf("\n��ѡ�����\n1.�޸���Ϣ 2.ɾ�� 3.ȡ��\n");
		a=getch();
		if(a=='1')
			Modify(p->next);
		else if(a=='2')
		{
			del(p);
			printf("ɾ���ɹ�!\n");
		}
	}
}

/******************************����********************************/

int NKeyword;		//�ؼ��ָ���
int Keyword[10];		//�ؼ������� 1��ʾѧ�Źؼ��֣�2��ʾ�ܳɼ��ؼ��֣�3 4....�ֱ�Ϊ����ѧ�ƹؼ���


//�ؼ��ֱȽϺ���
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


//���������
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
//ȡ��һ���ڵ���Ϊ�ȽϵĻ�׼��С�ڻ�׼����������������У�  
//���ڻ�׼�����ұߵ���������    
for (old = (*head)->next; old != end; old = old->next) {   
if (Compare(old,pivot)) {   /*С�ڻ�׼,���뵽�����������,�����Ƚ� */   
    ++left_count;   
	    *left_walk = old;            /*�Ѹýڵ���뵽��ߵ������У�  */  
	    left_walk = &(old->next);   
	} else {                         /*���ڻ�׼,���뵽�ұߵ������������Ƚ� */   
	    ++right_count;   
	    *right_walk = old;              
	    right_walk = &(old->next);   
	}   
}   
/*�ϲ�����  */ 
*right_walk = end;       /*����������  */ 
*left_walk = pivot;      /*�ѻ�׼������ȷ��λ����  */ 
pivot->next = right;     /*������ϲ�   */ 
/*�Խ�С����������п����򣬽ϴ����������е�������*/   
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


void Sort(void)		//����
{
	int i;
	printf("�ؼ��ֱ��1.ѧ�� 2.�ܷ�");
		for(i=0;i<m;i++)
			printf(" %d.%s",i+3,xkm[i]);
	printf("\n");	
	for(i=0;i<10;i++)
	{
		printf("�������%d���ؼ���(����0����):",i+1);
		scanf("%d",&Keyword[i]);
		if(Keyword[i]==0)
			break;
	}
	if(i==0)
		return;
	NKeyword=i;
	QListSort(&head->next,NULL);
	printf("�������\n");
	save=1;
	system("pause");
}


/***************************ͳ�ƹ���********************************/

void HighScore(void)		//ͳ����߷�
{
	struct LinkNode *p;
	int i;
	struct LinkNode* ptr[10];
	int n;
	while(1)
	{
		printf("��ѡ��Ҫͳ�Ƶ�ѧ��(����0ȡ��)\n1.�ܷ�");
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
			printf("\n�ܷ����Ϊ: %4.1f\n��߷�ѧ����Ϣ����\n",ptr[0]->data.sum);
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
			printf("\n%sѧ�����Ϊ: %4.1f\n��߷�ѧ����Ϣ����\n",xkm[i-2],ptr[0]->data.score[i-2]);
			PrintTitle();
			for(i=0;i<n;i++)
				print(&ptr[i]->data);
			printf("\n");		
		}
	}
}

void AverageScore(void)		//�����ѧ��ƽ����
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
	printf("\n�ܷ�ƽ����Ϊ: %4.1f\n",sum[m]/len);
	for(i=0;i<m;i++)
		printf("\n%sѧ��ƽ����Ϊ: %4.1f\n",xkm[i],sum[i]/len);
	system("pause");
}

void Interval(void)		//���ĳ������������
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
		printf("\n��Ŀ����: 1.�ܷ�");
		for(i=0;i<m;i++)
			printf(" %d.%s",i+2,xkm[i]);
		printf("\n");
		printf("��ѡ��Ҫͳ����Ŀ(����0ȡ��):");
		scanf("%d",&a);
		if(a==0)
			break;
		fflush(stdin);
		printf("�������������(��ʽa b):");
		scanf("%f %f",&min,&max);
		printf("\n����������ѧ����Ϣ����\n");
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
		printf("������Ϊ: %d\n",n);
	}
}


void Statistics(void)		//ͳ�ƹ���
{
	while(1)
	{
		printf("\n��ѡ����\n1.��߷� 2.ƽ���� 3.������������ 4.�Զ���ͳ���� 5.����������\n");
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



/**********************************���������*******************************************/
char *filename(void)			//��ȡ�ļ���
{
	char *filename=(char *)malloc(50);
	char *txt=".txt";
	fflush(stdin);
	printf("�����ļ���:");
	gets(filename);
	strcat(filename,txt);
	return filename;
}

void Save(void)				//�����ļ�
{
	FILE *fp;
	struct LinkNode *p;
	char n[11];				//��ʱ����ѧ�źͳɼ�ת�ɵ��ַ���
	int i;
	char *fn=filename();
	if(!access(fn,0))
	{
		printf("�ļ��Ѵ��� �Ƿ񸲸�(�س�ȷ��,����ȡ��)?\n");
		if(getch()!=13)
			return;
	}
	if(!(fp=fopen(fn,"w")))
	{
		printf("���ļ�ʧ��\n");
		system("pause");
		return;
	}
	fprintf(fp,"      ѧ��        ����                  רҵ  �Ա�");
	for(i=0;i<m;i++)
		{
			fprintf(fp,"  %8s",xkm[i]);
		}
	fprintf(fp,"      �ܷ�\n");
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
	printf("����ɹ�\n");
	save=0;
	system("pause");
}


void fileTitle(FILE *fp)		//��ȡ�ļ�������Ϣ
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


char *Word(FILE *fp)		//��ȡһ����¼�е�һ����
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


void load(void)			//��ȡ�ļ�
{
	FILE *fp;
	struct LinkNode *p;
	int i;
	if(save)
	{
		printf("������Ϣ����,�Ƿ��ȱ���ԭ����Ϣ?\n1.���� 2.������ 3.ȡ��\n");
		i=getch();
		if(i=='1')
			Save();
		else if(i!=2)
			return;
	}
	if(!(fp=fopen(filename(),"r")))
	{
		printf("���ļ�ʧ��\n");
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
	printf("��ȡ�ɹ�\n");
	save=0;
	system("pause");

}

/******************************************************************************/

void Quit(void)		//�˳�����
{
	int a;
	if(save)
	{
		printf("��Ϣ���޸��Ƿ񱣴�?\n1.���� 2.������ 3.ȡ��\n");
		a=getch();
		if(a=='1')
			Save();
		else if(a=='3')
			return;
	}
	else
	{
		printf("�Ƿ��˳�?\n1.�˳� 2.ȡ��\n");
		if(getch()!='1')
			return;
	}	
	clear();
	free(head);
	if(xkm)
		free(xkm);
	exit(0);
}


int f(void)		//�ж��Ƿ�������
{
	if(len)
		return 1;
	printf("��ǰû������,����ӻ�����!\n");
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
		printf("�༶����ϵͳ\n1.��Ϣ¼��\n2.��ѯ�޸ĺ�ɾ��\n3.ͳ��\n4.����\n5.���\n6.����\n7.����\n8.�˳�");
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
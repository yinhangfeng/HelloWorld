#include "InitSock.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	BUFFER_SIZE 1024
#define COMMAND_SIZE 512

CInitSock initSock;

char * getParameter(char *s)
{
	for(;!(*s!=' '&&*(s-1)==' ');s++);
	return s;
}

long filesize(FILE *fp)
{
    long savepos;
    long filesize;
    savepos=ftell(fp);
    fseek(fp,0,SEEK_END);
    filesize=ftell(fp);
    fseek(fp,savepos,SEEK_SET);
    return(filesize);
}

int main()
{
	char buffer[BUFFER_SIZE];
	char Command[COMMAND_SIZE];
	char ip[20];
	char *Parameter;
	char *fileName;
	FILE *file;
	int nRecv;
	long size;
	printf("���������IP:");
	gets(ip);	
	SOCKET s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(s==INVALID_SOCKET)
	{
		printf("Failed socket()\n");
		return 0;
	}
	sockaddr_in servAddr;
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(4567);
	servAddr.sin_addr.S_un.S_addr=inet_addr(ip);
	if(::connect(s,(sockaddr*)&servAddr,sizeof(servAddr))==-1)
	{
		printf("Failed socket()\n");
		return 0;
	}
	while(TRUE)
	{
		printf("ftp>");
		fflush(stdin);
		gets(Command);
		if(strnicmp(Command,"cd",2)==0&&Command[2]==' ')
		{
			Parameter=getParameter(Command);
			::send(s,"cd",2,0);
			::recv(s,buffer,BUFFER_SIZE,0);
			::send(s,Parameter,strlen(Parameter),0);
			nRecv=::recv(s,buffer,BUFFER_SIZE,0);
			buffer[nRecv]='\0';
			if(strcmp(buffer,"fail")==0)
				printf("�ı�Ŀ¼ʧ��\n");
			else
				printf("��ǰĿ¼: %s\n",buffer);
		}
		else if(stricmp(Command,"ls")==0)
		{
			::send(s,Command,strlen(Command),0);
			nRecv=::recv(s,buffer,BUFFER_SIZE,0);
			buffer[nRecv]='\0';
			printf("%s",buffer);
		}
		else if(strnicmp(Command,"get",3)==0&&Command[3]==' ')
		{
			Parameter=getParameter(Command);
			::send(s,"get",3,0);
			::recv(s,buffer,BUFFER_SIZE,0);
			::send(s,Parameter,strlen(Parameter),0);
			nRecv=::recv(s,buffer,BUFFER_SIZE,0);
			if(nRecv>0)
			{
				buffer[nRecv]='\0';
				if(strcmp(buffer,"File not found")!=0)
				{
					fileName=strrchr(Parameter,'\\');			
					if(fileName==NULL)
						fileName=Parameter;	
					else
						fileName++;	
					file=fopen(fileName,"wb");								
					::send(s,"1",1,0);	
					printf("�ļ�������...\n");
					size=*(long *)buffer;
					for(;size>0;size-=nRecv)
					{
						nRecv=::recv(s,buffer,BUFFER_SIZE,0);
						if(nRecv>0)
							fwrite(buffer,1,nRecv,file);
					}
					printf("�ļ��������\n");
					fclose(file);
				}
				else
					printf("�ļ�������\n");
			}
		}
		else if(strnicmp(Command,"send",4)==0&&Command[4]==' ')
		{
			Parameter=getParameter(Command);
			file=fopen(Parameter,"rb");
			if(file==NULL)
				printf("�ļ�������\n");
			else
			{
				fileName=strrchr(Parameter,'\\');
				if(fileName==NULL)
					fileName=Parameter;	
				else
					fileName++;	
				::send(s,"send",4,0);
				::recv(s,buffer,BUFFER_SIZE,0);
				::send(s,fileName,strlen(fileName),0);
				::recv(s,buffer,BUFFER_SIZE,0);
				size=filesize(file);
				::send(s,(char *)&size,4,0);
				::recv(s,buffer,BUFFER_SIZE,0);
				printf("�ļ��ϴ���...\n");
				int length=0;
				while ( (length=fread(buffer,1,BUFFER_SIZE,file))>0 )
					::send(s,buffer,length,0);
				printf("�ļ��������\n");
				fclose(file);
			}
		}
		else if(stricmp(Command,"quit")==0)
			break;
		else if(stricmp(Command,"help")==0)
			printf("cd Ŀ¼�� :����Ŀ¼\nls :�г���ǰĿ¼\nget �ļ�·�� :�����ļ�\nsend �ļ�·�� :�ϴ��ļ�\nquit :�˳�\n");
		else
			printf("��Ч����,����help��ð���\n");
	}
	::closesocket(s);
	return 0;	
}

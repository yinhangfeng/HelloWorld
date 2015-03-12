#include "InitSock.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define	BUFFER_SIZE 1024
#define PATH_SIZE 512

CInitSock initSock;

int main(int argc, char ** argv)
{
	char buffer[BUFFER_SIZE];
	char filePath[PATH_SIZE];
	char ip[20];
	char a[20];
	char *fileName;
	FILE *file;

	if(argc==1)
	{
		printf("输入服务器IP:");
		gets(ip);
		printf("输入文件路径:");
		gets(filePath);			
	}
	else if(argc!=3)
    {
        printf("命令行格式:%s <ServerIP> <Path>\n如:%s 127.0.0.1 C:\test.jpg\n",argv[0],argv[0]);
		getchar();
		return 1;
    }
	else
	{
		strcpy(ip,argv[1]);
		strcpy(filePath,argv[2]);
	}
	
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
		::send(s,filePath,strlen(filePath),0);
		int nRecv=::recv(s,a,20,0);		
		if(nRecv>0)
		{
			a[nRecv]='\0';
			if(strcmp(a,"File not found")!=0)
			{
				for(fileName=filePath+strlen(filePath);*(fileName-1)!='\\'&&fileName!=filePath;fileName--);	
				file=fopen(fileName,"wb");
				long *size=(long *)a;
				long i=*size/BUFFER_SIZE;
				if(*size%BUFFER_SIZE!=0)
					i++;
				printf("文件传输中...\n");
				for(;i>0;i--)
				{
					nRecv=::recv(s,buffer,BUFFER_SIZE,0);
					if(nRecv>0)
						fwrite(buffer,1,nRecv,file);
				}
				printf("文件传输完毕\n");
				fclose(file);
			}
			else
				printf("文件不存在\n");
		}
		printf("继续获取文件?(Y/N)\n");
		fflush(stdin);
		char b=getchar();
		if(b=='n'||b=='N')
		{
			::send(s,"finish",7,0);
			break;
		}
		::send(s,"continue",9,0);
		fflush(stdin);
		printf("输入文件路径:");
		gets(filePath);
	}
	::closesocket(s);
	return 0;	
}	


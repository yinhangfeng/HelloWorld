#include "InitSock.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define	BUFFER_SIZE 1024
#define PATH_SIZE 512

CInitSock initSock;

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
	char filePath[PATH_SIZE];
	char a[20];
	FILE *file;

	SOCKET sListen=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sListen==INVALID_SOCKET)
	{
		printf("Failed socket()\n");
		return 0;
	}
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(4567);
	sin.sin_addr.S_un.S_addr=INADDR_ANY;
	if(::bind(sListen,(LPSOCKADDR)&sin,sizeof(sin))==SOCKET_ERROR)
	{
		printf("Failed socket()\n");
		return 0;
	}
	if(::listen(sListen,2)==SOCKET_ERROR)
	{
		printf("Failed socket()\n");
		return 0;	
	}
	sockaddr_in remoteAddr;
	int nAddrLen=sizeof(remoteAddr);
	SOCKET sClient;

	while(TRUE)
	{
		sClient=::accept(sListen,(SOCKADDR*)&remoteAddr,&nAddrLen);
		if(sClient==INVALID_SOCKET)
		{
			printf("Failed socket()");
			continue;
		}
		while(TRUE)
		{
			int nRecv=::recv(sClient,filePath,PATH_SIZE,0);		
			if(nRecv>0)
			{
				filePath[nRecv]='\0';
				file=fopen(filePath,"rb");
				if(file==NULL)
					::send(sClient,"File not found",15,0);
				else
				{
					long size=filesize(file);
					::send(sClient,(char *)&size,4,0);
					Sleep(1000);
					int length=0;
					while ( (length=fread(buffer,1,BUFFER_SIZE,file))>0 )
						::send(sClient,buffer,length,0);
					fclose(file);
				}
			}
			nRecv=::recv(sClient,a,20,0);
			if(nRecv>0)
			{
				a[nRecv]='\0';
				if(strcmp(a,"finish")==0)
					break;
			}
		}		
		::closesocket(sClient);
	}
	::closesocket(sListen);
	return 0;
}	

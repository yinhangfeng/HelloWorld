#include "InitSock.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>

#define	BUFFER_SIZE 1024
#define COMMAND_SIZE 512
#define PATH_SIZE 512

CInitSock initSock;

char current[FD_SETSIZE][PATH_SIZE];
char initPath[PATH_SIZE];

void ls(char *buffer)
{
	long Handle;
	struct _finddata_t FileInfo;
	Handle=_findfirst("*",&FileInfo);
	buffer+=sprintf(buffer,"%s\n",FileInfo.name);
	while(_findnext(Handle,&FileInfo)==0)
		buffer+=sprintf(buffer,"%s\n",FileInfo.name);
	_findclose(Handle);
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
	long size;
	FILE *file;
	_getcwd(initPath,PATH_SIZE);
	
	USHORT nPort = 4567;
	SOCKET sListen = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(sListen , (sockaddr*)&sin , sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind()\n");
		return -1;
	}
	::listen(sListen , 5);
	fd_set fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(sListen , &fdSocket);
	while(true)
	{
		fd_set fdRead = fdSocket;
		int nRet = ::select(0 , &fdRead , NULL , NULL , NULL );
		if(nRet > 0)
		{
			for(int i = 0 ; i < (int)fdSocket.fd_count ; i++)
			{
				if(FD_ISSET(fdSocket.fd_array[i] , &fdRead))
				{
					if(fdSocket.fd_array[i] == sListen)
					{
						if(fdSocket.fd_count < FD_SETSIZE)
						{
							sockaddr_in addrRemote;
							int nAddrLen = sizeof(addrRemote);
							SOCKET sNew =
								::accept(sListen , (SOCKADDR*)&addrRemote, &nAddrLen);
							FD_SET(sNew , &fdSocket);
							printf("接受到的连接(%s) \n连接数:%d\n" , ::inet_ntoa(addrRemote.sin_addr),fdSocket.fd_count-1);
						}
						else
						{
							printf("Too much connection!\n");
							continue;
						}
					}
					else
					{
						if(current[i][0]=='\0')
							strcpy(current[i],initPath);
						_chdir(current[i]);
						int nRecv=::recv(fdSocket.fd_array[i],Command,COMMAND_SIZE,0);
						if(nRecv > 0)
						{
							Command[nRecv]='\0';
							if(stricmp(Command,"cd")==0)
							{
								::send(fdSocket.fd_array[i],"1",1,0);
								nRecv=::recv(fdSocket.fd_array[i],Command,COMMAND_SIZE,0);
								Command[nRecv]='\0';
								if(_chdir(Command)!=0)
									::send(fdSocket.fd_array[i],"fail",4,0);
								else
								{
									_getcwd(current[i],PATH_SIZE);
									::send(fdSocket.fd_array[i],current[i],sizeof(current[i]),0);
								}
							}
							else if(stricmp(Command,"ls")==0)
							{
								ls(buffer);
								::send(fdSocket.fd_array[i],buffer,sizeof(buffer),0);
							}
							else if(stricmp(Command,"get")==0)
							{
								::send(fdSocket.fd_array[i],"1",1,0);
								nRecv=::recv(fdSocket.fd_array[i],Command,COMMAND_SIZE,0);
								Command[nRecv]='\0';
								file=fopen(Command,"rb");
								if(file==NULL)
									::send(fdSocket.fd_array[i],"File not found",15,0);
								else
								{
									size=filesize(file);
									::send(fdSocket.fd_array[i],(char *)&size,4,0);
									::recv(fdSocket.fd_array[i],buffer,BUFFER_SIZE,0);
									int length=0;
									while ((length=fread(buffer,1,BUFFER_SIZE,file))>0)
										::send(fdSocket.fd_array[i],buffer,length,0);
									fclose(file);
								}
							}
							else if(stricmp(Command,"send")==0)
							{
								::send(fdSocket.fd_array[i],"1",1,0);
								nRecv=::recv(fdSocket.fd_array[i],Command,COMMAND_SIZE,0);
								Command[nRecv]='\0';
								file=fopen(Command,"wb");
								::send(fdSocket.fd_array[i],"1",1,0);
								::recv(fdSocket.fd_array[i],buffer,BUFFER_SIZE,0);
								size=*(long *)buffer;
								::send(fdSocket.fd_array[i],"1",1,0);
								for(;size>0;size-=nRecv)
								{
									nRecv=::recv(fdSocket.fd_array[i],buffer,BUFFER_SIZE,0);
									if(nRecv>0)
										fwrite(buffer,1,nRecv,file);
								}
								fclose(file);
							}
						}				
						else
						{
							::closesocket(fdSocket.fd_array[i]);
							FD_CLR(fdSocket.fd_array[i] , &fdSocket);
							printf("连接数:%d\n",fdSocket.fd_count-1);
							current[i][0]='\0';
						}
					}
				}
			}
		}
		else
		{
			printf("Failed select()\n");
			break;
		}
	}
	return 0;
}

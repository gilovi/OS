/*
 * srftp.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#include "srftp.h"
#include "general.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

#define USAGE_ERR "Usage: srftp server-port max-file-size"

using namespace std;

static int maxFileSize ;
int portno , socketFd ;
struct sockaddr_in serv_addr;

//void error(string msg)
//{
//	cout<<msg<<endl;
//	exit(ERROR_CODE);
//
//}


int main( int argc, char* argv[] )
{
	//	invalid number of parameters
	if (argc != ARGS)
	{
		cout<< USAGE_ERR << endl;
		exit(ERROR_CODE);
	}


	maxFileSize = atoi(argv[MAX_FILE_SIZE_IDX]);
	//	invalid max-file-size
	if (maxFileSize < 0)
	{
		cout << USAGE_ERR << endl;
		exit(ERROR_CODE);
	}


	//	char buff[BUFF_SIZE];
	//memset(buff,'0',sizeof(buff));


	portno = atoi(argv[1]);
	//	invalid port number
	if (portno < MIN_PORT_NUM || portno > MAX_PORT_NUM)
	{
		cout<< USAGE_ERR << endl;
		exit(ERROR_CODE);
	}
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
	{
		int err = errno;
		cerr << sysErr("socket", err) << endl;
		pthread_exit(0);
	}
	memset(&serv_addr,'0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(socketFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		int err = errno;
		cerr << sysErr("bind", err) << endl;
		pthread_exit(0);
	}
	listen(socketFd , 5);

	while(true)
	{
		int* connectionFd = new int;
		*connectionFd = accept(socketFd,NULL,NULL);
		if (*connectionFd < 0)
		{
			int err = errno;
			cerr << sysErr("accept", err) << endl;
			pthread_exit(0);
		}

		pthread_t tid;
		pthread_create(&tid, NULL, fetchData, connectionFd);
		pthread_detach (tid);
	}
	return SUCCESS;

}
void rcvBuff(char* buffer , int bufferSize , int connectionFd)
{
	int bytesGot = 0;
	int got = 1;

	while (bytesGot < bufferSize){
		got = read(connectionFd, buffer + bytesGot, bufferSize - bytesGot);
		if (got < 0 )
		{
			int err = errno;
			cerr << sysErr("read", err) << endl;
			pthread_exit(0);
		}
		bytesGot += got;
	}
	//buffer[bufferSize+1] = '\0';
}

void writeFile(ofstream* writeTo, int sizeOfFile , int connectionFd)
{
	char* buff = new char[BUFF_SIZE];
	int i = sizeOfFile/BUFF_SIZE;
	int remainder =  sizeOfFile % BUFF_SIZE;

	for(; i > 0 ; i--)
	{
		rcvBuff(buff, BUFF_SIZE, connectionFd);
		writeTo->write(buff, BUFF_SIZE) ;
	}
	if (remainder)
	{
		rcvBuff(buff, remainder, connectionFd);
		writeTo->write(buff, remainder) ;
	}

	delete[] buff;
}


void* fetchData(void* fd)
{
	int connectionFd = *(int*) fd;
	delete (int*)fd;
	int response = 0;

	char* sizeBuff = new char[sizeof(int)];
	int intSize;

	//fetch file name size & then the name
	rcvBuff(sizeBuff, sizeof(int), connectionFd);
	intSize = *(int*)sizeBuff;
	char* fileNameBuff = new char[intSize + 1];
	rcvBuff(fileNameBuff, intSize, connectionFd);
	fileNameBuff[intSize + 1] = '\0';
	//get size of file & verify its valid.
	rcvBuff(sizeBuff, sizeof(int), connectionFd);
	intSize = *(int*)sizeBuff;
	if (intSize > maxFileSize)
	{
		response = -1;
	}
	write(connectionFd,(char*)&response,sizeof(int) );

	ofstream outFile(fileNameBuff, ios::app);
	if (!outFile){
		delete[] sizeBuff;
		delete[] fileNameBuff;
	//	error("");
	}
	//fetch the file.
	writeFile(&outFile, intSize, connectionFd);

	outFile.close();
	delete[] sizeBuff;
	delete[] fileNameBuff;

	pthread_exit(0);
}

/*
 * srftp.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#include "srftp.h"
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

using namespace std;
/*srftp::srftp() {
	// TODO Auto-generated constructor stub

}

srftp::~srftp() {
	// TODO Auto-generated destructor stub
}*/


//memset(&my_addr, 0, sizeof(struct sockaddr_un));



int main( int argc, char* argv[] )

  {


	int portno , socketFd  ;
	int livingClients = 0;
	struct sockaddr_in serv_addr;

	char buff[BUFF_SIZE];
	memset(buff,'0',sizeof(buff));

	portno = atoi(argv[1]);
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	     if (socketFd < 0)
	     {
	    	 //TODO: error handle;
	     }
	memset(&serv_addr,'0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(socketFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		//TODO:error handle
	}
	listen(socketFd , 5);

	while(true)
	{
	connectionFd = accept(socketFd,NULL,NULL);
	if (connectionFd < 0)
	{
		//TODO: error hand

	}
	pthread_t tid;
	pthread_create(&tid, NULL, fetchData, NULL);
	pthread_detach (tid);
	}

  }
void rcvBuff(char* buffer , int bufferSize)
{
	int bytesGot = 0;
	int got = 1;

		while (bytesGot<bufferSize){
			 got = read(connectionFd, buffer + bytesGot, bufferSize - bytesGot);
			if (got < 0 )
			{
			//TODO:error ha
			}
			bytesGot += got;
		}
}

 void rcvFile(ofstream* writeTo, int sizeOfFile)
 {
	 char buff[BUFF_SIZE] =
	 int i = sizeOfFile/BUFF_SIZE;
	 int reminder =  sizeOfFile % BUFF_SIZE;

	 for(i; i>0; i--)
	 {
		 rcvBuff()

	 }

 }


void* fetchData(void* p)
{

}



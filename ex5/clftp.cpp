/*
 * clftp.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#include "clftp.h"
#include "general.h"


using namespace std;


Clftp::Clftp(int port, struct hostent* host):
		_portno(port), server(host)
{
		if (server == NULL)
		{
			//TODO: error
		}
		//create a socket:
		_socketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (_socketFd < 0)
		{
		 //TODO:  error("ERROR opening socket");
		}
		memset(&serv_addr,'0', sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(_portno);
		//connecting to server
		//if (connect(_SocketFd,((struct sockaddr*)& serv_addr), sizeof(serv_addr)) < 0)

		{
				//TODO: error("ERROR connecting");
		}

}

Clftp::~Clftp(){}

void Clftp::sendBuff(const char* buffer , int bufferSize )
{
	int bytesSent = 0;
	int sent = 1;

	while (bytesSent<bufferSize){
		sent = send(_socketFd, buffer + bytesSent, bufferSize - bytesSent,0);
		if (sent == ERROR)
		{
		//TODO:	error ("ERROR: reciving data");
		}
		bytesSent += sent;
	}

}
void Clftp::sendFile(string localName , string remoteName )
{
	int fileSize, nameSize;
	//connect to server.
	if (connect(_socketFd,(struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
	{
		//TODO: error
	}

	ifstream ifs (localName.c_str());
	if (!ifs)
		{
		//TODO: error("ERROR: open file");
		}
	fileSize = int(getFileSize(ifs));
	nameSize = remoteName.length();

	sendBuff((char*)&nameSize,sizeof(int)); //sending the size of the name size
	sendBuff(remoteName.c_str() , nameSize); //sent the name.
	sendBuff((char*)&fileSize , sizeof(int)); //sent the file size.
	sendFile(ifs, fileSize);//send the file.
	ifs.close();
}
int Clftp::getFileSize(ifstream &ifs) {
	long begin,end;
	begin = ifs.tellg();
	ifs.seekg (0, ios::end);
	end = ifs.tellg();
	ifs.seekg(ios::beg);
	return end-begin;
}

void Clftp::sendFile(ifstream& toSend , int fileSize)
{

    int i = fileSize/BUFF_SIZE;
    int rem = fileSize%BUFF_SIZE;

    char* buff = new char[BUFF_SIZE];
    for (; i > 0 ; i--)
    {
		toSend.read(buff, BUFF_SIZE);
		sendBuff(buff , BUFF_SIZE);

	}
	if (rem)
	{
		toSend.read(buff, rem);
		sendBuff(buff, rem);
	}
	delete buff;

}

int main( int argc, char* argv[] )
{
	if (argc != ARGS)
	{
		//TODO: error
	}

	Clftp client = Clftp(atoi(argv[PORT_IDX]), gethostbyname(argv[HOST_IDX]) );
	client.sendFile(argv[FILE_NAME_IDX] , argv[OUT_FILE_NAME_IDX] );

}


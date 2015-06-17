/*
 * clftp.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#include "clftp.h"
#include "general.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define USAGE_ERR "Usage: clftp server-port server-hostname file-to-transfer filename-in-server"
#define TRANS_ERR "Transmission failed: too big file"

using namespace std;


Clftp::Clftp(int port, struct hostent* host):
				_portno(port), server(host)
{
//	if (server == NULL)
//	{
//		//TODO: error
//	}
	//create a socket:
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
	{
		int err = errno;
		cerr << sysErr("socket", err) << endl;
	}
	memset(&serv_addr,'0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(_portno);
	//connecting to server
	//if (connect(_SocketFd,((struct sockaddr*)& serv_addr), sizeof(serv_addr)) < 0)

//	{
		//TODO: error("ERROR connecting");
//	}

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
			int err = errno;
			cerr << sysErr("send", err) << endl;
		}
		bytesSent += sent;
	}

}
void Clftp::sendFile(string localName , string remoteName )
{
	int fileSize, nameSize;
	char* response = new char[sizeof(int)];
	//connect to server.
	if (connect(_socketFd,(struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
	{
		int err = errno;
		cerr << sysErr("connect", err) << endl;
	}

	ifstream ifs (localName.c_str());
	if (!ifs)
	{
		int err = errno;
		cerr << sysErr("open", err) << endl;
	}
	fileSize = int(getFileSize(ifs));
	nameSize = remoteName.length();

	sendBuff((char*)&nameSize,sizeof(int)); //sending the size of the name size
	sendBuff(remoteName.c_str() , nameSize); //sent the name.
	sendBuff((char*)&fileSize , sizeof(int)); //sent the file size.

	read(_socketFd,response,sizeof(int));
	if ((*(int*)response) < 0)
	{
		cout << TRANS_ERR << endl;
	}
	delete[] response;
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
	delete[] buff;

}

int main( int argc, char* argv[] )
{
	if (argc != ARGS)
	{
		cout<< USAGE_ERR << endl;
		exit(ERROR_CODE);
	}
	int portno = atoi(argv[PORT_IDX]);
	if (portno < MIN_PORT_NUM || portno > MAX_PORT_NUM)
	{
		cout<< USAGE_ERR << endl;
		exit(ERROR_CODE);
	}
	string fileToTransPath = argv[FILE_NAME_IDX];
	struct stat sb;

	if ( ( stat(fileToTransPath.c_str(),&sb ) == 0 && S_ISDIR(sb.st_mode) )
			|| ( access( fileToTransPath.c_str(), F_OK ) == -1 )  )
	{
		cout << USAGE_ERR <<endl;
		exit(ERROR_CODE);
	}
	Clftp client = Clftp(portno, gethostbyname(argv[HOST_IDX]) );
	client.sendFile(fileToTransPath, argv[OUT_FILE_NAME_IDX] );
	return 0;
}


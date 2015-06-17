/*
 * clftp.h
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#ifndef CLFTP_H_
#define CLFTP_H_


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


#define PORT_IDX 1
#define HOST_IDX 2
#define FILE_NAME_IDX 3
#define OUT_FILE_NAME_IDX 4



class Clftp {
public:
	Clftp::Clftp(int port, struct hostent* host, std::string sourceFileName, std::string saveFileName);
	virtual ~Clftp();

private:
	void sendBuff(char* buffer , int bufferSize );
	void sendFile(std::ifstream& toSend);
	int getFileSize(std::ifstream &ifs) ;

	int connect();

	int _portno;//
	char* _fileNameToSend;//
	char* _nameToSave;//
	int _nameSize;

	int _SocketFd;//
	int _fileSize;

	struct sockaddr_in serv_addr;
	struct hostent *server ;//

};
#endif /* CLFTP_H_ */

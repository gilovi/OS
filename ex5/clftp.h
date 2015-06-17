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
#include <string.h>
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
#define ARGS 5


class Clftp {
public:
	Clftp(int port, struct hostent* host);
    ~Clftp();
	void sendFile(std::string localName , std::string remoteName );

private:
	void sendBuff(const char* buffer , int bufferSize );
	void sendFile(std::ifstream& toSend, int fileSize);
	int getFileSize(std::ifstream &ifs) ;

	int _portno;//
	int _socketFd;//

	struct sockaddr_in serv_addr;
	struct hostent *server ;//

};
#endif /* CLFTP_H_ */

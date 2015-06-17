/*
 * clftp.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#include "clftp.h"
#include "general.h"


using namespace std;


Clftp::Clftp(int port, struct hostent* host, string sourceFileName, string saveFileName):
		_portno(port), _fileNameToSend(sourceFileName),_nameToSave(saveFileName),_nameSize(saveFileName.length()),server(host)
{
		//create a socket:
		_SocketFd = socket(AF_INET, SOCK_STREAM, 0);
		memset(&serv_addr,'0', sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(_sPort);

		if (connect(_SocketFd,((struct sockaddr*)&serv_addr),sizeof(serv_addr)) < 0)//connecting to server
				error("ERROR connecting");

}

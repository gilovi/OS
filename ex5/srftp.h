/*
 * srftp.h
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#ifndef SRFTP_H_
#define SRFTP_H_

/*
class srftp {
public:
	srftp();
	virtual ~srftp();

private:
int portNum, maxFileSize;
char buffer[];
};
*/
#define BUFF_SIZE 1024
static int connectionFd;

void*  fetchData(void*);

#endif /* SRFTP_H_ */

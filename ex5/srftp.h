/*
 * srftp.h
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#ifndef SRFTP_H_
#define SRFTP_H_

#define PORT_IDX 1
#define MAX_FILE_SIZE_IDX 2
#define ARGS 3

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

static int MAX_FILE_SIZE ;
void*  fetchData(void*);

#endif /* SRFTP_H_ */

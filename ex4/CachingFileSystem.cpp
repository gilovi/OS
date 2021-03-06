/*
 * CachingFileSystem.cpp
 *
 * Created on: 15 April 2015
 * Author: Netanel Zakay, HUJI, 67808 (Operating Systems 2014-2015).
 */
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <list>
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "cache.h"
#include <unistd.h>
#include <fstream>

#define ISDIR(path) (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
#define INPUT_ERR "usage: CachingFileSystem rootdir mountdir numberOfBlocks blockSize\n"
#define MAIN_ERR "System Error"

using namespace std;
#define LOG_CALL_CHECK(path) if(!strcmp(path, "/.filesystem.log")) return -ENOENT



static ofstream logFile;

void log_msg(const string funcName)
{
    logFile << time(NULL) << " " << funcName << endl;
}

struct CacheData
{
	char* rootdir;
	Cache cache;
	~CacheData()
	{
		free(rootdir);
	}
};
CacheData* gData;

struct fuse_operations caching_oper;
static void fullpath(char fpath[PATH_MAX],const char *path)
{
	strcpy(fpath,gData->rootdir);
	strncat(fpath,path,PATH_MAX);
}
/** Get file attributes.
 *
 * Similar to stat(). The 'st_dev' and 'st_blksize' fields are
 * ignored. The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int caching_getattr(const char *path, struct stat *statbuf){

	LOG_CALL_CHECK(path);


	log_msg("getattr");
	int ret = 0;

	char fpath[PATH_MAX];

	fullpath(fpath,path);
	ret = lstat(fpath, statbuf);
	if(0!= ret)
	{

		ret = -errno;

	}
	return ret;
}
/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above). Later it may be called for
 * invocations of fstat() too.
 *
 * Introduced in version 2.5
 */
int caching_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi){
	log_msg("fgetattr");
	int ret = 0;
	char fpath[PATH_MAX];

	fullpath(fpath,path);
	ret = fstat(fi->fh, statbuf);
	if (0!=ret)
	{
		ret = -errno;
	}

	return ret;
}
/**
 * Check file access permissions
 *
 * This will be called for the access() system call. If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 *
 * Introduced in version 2.5
 */
int caching_access(const char *path, int mask)
{

    

	log_msg("access");
	LOG_CALL_CHECK(path);

	int ret = 0;
	char fpath[PATH_MAX];

	fullpath(fpath, path);
	ret = access(fpath,mask);
	if (ret < 0)
	{
		ret = -errno;
	}
	return ret;
}
/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open(). Open should check if the operation
 * is permitted for the given flags. Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 * pay attention that the max allowed path is PATH_MAX (in limits.h).
 * if the path is longer, return error.
 * Changed in version 2.2
 */
int caching_open(const char *path, struct fuse_file_info *fi){

	log_msg("open");
	LOG_CALL_CHECK(path);

	int retstat = 0;
	int fd;
	char fpath[PATH_MAX];
	fullpath(fpath, path);
	fd = open(fpath, fi->flags);
	if (fd < 0)
	{
		retstat = -errno;
	}
	fi->fh = fd;
	return retstat;
}
/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.
 *
 * Changed in version 2.2
 */
int caching_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi){
	log_msg("read");

	int ret = 0;
	ret = gData->cache.read(path,buf,size,offset,fi->fh);
	if (ret<0)
	{
		ret = -errno;
	}
	return ret;
}
/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync(). It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor. So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors. Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open(). This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls. It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally. Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */
int caching_flush(const char *path, struct fuse_file_info *fi)
{
	log_msg("flush");
	int ret = 0;
	return ret;
}
/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor. It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file. The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int caching_release(const char *path, struct fuse_file_info *fi){

    log_msg("release");

	int ret = 0;
	ret = close(fi->fh);

	return ret;
}
/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this directory
 *
 * Introduced in version 2.3
 */
int caching_opendir(const char *path, struct fuse_file_info *fi){

	log_msg("opendir");
	LOG_CALL_CHECK(path);

	DIR *dp;
	int retstat = 0;
	char fpath[PATH_MAX];
	fullpath(fpath, path);
	dp = opendir(fpath);
	if (dp == NULL)
	{
		retstat = -errno;
	}
	fi->fh = (intptr_t) dp;
	return retstat;
}
/** Read directory
 *
 * This supersedes the old getdir() interface. New applications
 * should use this.
 *
 * The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset. The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation. This
 * works just like the old getdir() method.
 *
 * Introduced in version 2.3
 */
int caching_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
		struct fuse_file_info *fi){

	log_msg("readdir");

	int retstat = 0;
	DIR *dp;
	struct dirent *de;
	dp = (DIR *) (uintptr_t) fi->fh;
	de = readdir(dp);
	if (de == 0)
	{
		retstat = -errno;
		return retstat;
	}
	do {
		// log_msg("calling filler with name %s\n", de->d_name);
		if (filler(buf, de->d_name, NULL, 0) != 0) {
			// log_msg(" ERROR bb_readdir filler: buffer full");
			return -ENOMEM;
		}
	} while ((de = readdir(dp)) != NULL);
	return retstat;
}
/** Release directory
 *
 * Introduced in version 2.3
 */
int caching_releasedir(const char *path, struct fuse_file_info *fi){

	log_msg("releasedir");

	int ret = 0;
	closedir((DIR*) (uintptr_t)fi->fh);
	return ret;
}
/** Rename a file */
int caching_rename(const char *path, const char *newpath){

	log_msg("rename");
	LOG_CALL_CHECK(path);

	int ret = 0;

	char fpath[PATH_MAX];
	char fnewpath[PATH_MAX];
	fullpath(fpath,path);
	fullpath(fnewpath,newpath);
	// update cache:
	gData->cache.rename(path, newpath);
	ret = rename(fpath, fnewpath);
	if (ret<0)
	{
		return -errno;
	}
	return ret;
}
/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
void *caching_init(struct fuse_conn_info *conn){

	log_msg("init");

	return gData;
}
/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
void caching_destroy(void *userdata){

	log_msg("destroy");
	logFile.close();

	delete gData;
}
/**
 * Ioctl from the FUSE sepc:
 * flags will have FUSE_IOCTL_COMPAT set for 32bit ioctls in
 * 64bit environment. The size and direction of data is
 * determined by _IOC_*() decoding of cmd. For _IOC_NONE,
 * data will be NULL, for _IOC_WRITE data is out area, for
 * _IOC_READ in area and if both are set in/out area. In all
 * non-NULL cases, the area is of _IOC_SIZE(cmd) bytes.
 *
 * However, in our case, this function only needs to print cache table to the log file .
 *
 * Introduced in version 2.8
 */
int caching_ioctl (const char *, int cmd, void *arg,
		struct fuse_file_info *, unsigned int flags, void *data){

	log_msg("ioctl");
	gData->cache.PrintToLog(logFile);

	return 0;
}
// Initialise the operations.
// You are not supposed to change this function.
void init_caching_oper()
{
	caching_oper.getattr = caching_getattr;
	caching_oper.access = caching_access;
	caching_oper.open = caching_open;
	caching_oper.read = caching_read;
	caching_oper.flush = caching_flush;
	caching_oper.release = caching_release;
	caching_oper.opendir = caching_opendir;
	caching_oper.readdir = caching_readdir;
	caching_oper.releasedir = caching_releasedir;
	caching_oper.rename = caching_rename;
	caching_oper.init = caching_init;
	caching_oper.destroy = caching_destroy;
	caching_oper.ioctl = caching_ioctl;
	caching_oper.fgetattr = caching_fgetattr;

	caching_oper.readlink = NULL;
	caching_oper.getdir = NULL;
	caching_oper.mknod = NULL;
	caching_oper.mkdir = NULL;
	caching_oper.unlink = NULL;
	caching_oper.rmdir = NULL;
	caching_oper.symlink = NULL;
	caching_oper.link = NULL;
	caching_oper.chmod = NULL;
	caching_oper.chown = NULL;
	caching_oper.truncate = NULL;
	caching_oper.utime = NULL;
	caching_oper.write = NULL;
	caching_oper.statfs = NULL;
	caching_oper.fsync = NULL;
	caching_oper.setxattr = NULL;
	caching_oper.getxattr = NULL;
	caching_oper.listxattr = NULL;
	caching_oper.removexattr = NULL;
	caching_oper.fsyncdir = NULL;
	caching_oper.create = NULL;
	caching_oper.ftruncate = NULL;
}
//basic main. You need to complete it.
int main(int argc, char* argv[]){

	if (argc!=5)
	{
		cout << INPUT_ERR << endl;
	}
	char tmpRoot[PATH_MAX], tmpMount[PATH_MAX];
    if(realpath(argv[1], tmpRoot) == NULL)
	{
		return -errno;
	}
    if(realpath(argv[2], tmpMount) == NULL)
    {
    	return -errno;
    }
	int numOfBlocks = atoi(argv[3]);
	size_t blockSize = atoi(argv[4]);

    struct stat sb;
    if (ISDIR(tmpRoot) == 0 || ISDIR(tmpMount) == 0 ||numOfBlocks <=0 || blockSize <= 0)
    {
    	cout << INPUT_ERR << endl;
    }

	gData = new CacheData();
	gData->rootdir = tmpRoot;
	gData->cache = Cache(blockSize, numOfBlocks);

	init_caching_oper();
	argv[1] = argv[2];
	for (int i = 2; i< (argc - 1); i++){
		argv[i] = NULL;
	}

	argv[2] = (char*) "-s";

	argc = 3;

	char logPath[PATH_MAX];

	strcpy(logPath,gData->rootdir);
	strncat(logPath,"/.filesystem.log",PATH_MAX);
	logFile.open(logPath, ios::app);

	int fuse_stat = fuse_main(argc, argv, &caching_oper, gData);
	return fuse_stat;
}

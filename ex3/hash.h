#ifndef _HASH_H
#define _HASH_H

/*
 *       A multi-threaded blockchain database manager.
 *       -- Supplementary hash library.
 * 
 *       Author: OS, huji.os.2015@gmail.com
 */

#include <stdlib.h>

/**
 * Initializes resources required by the hash library.
 *
 * You must call this function inside your init_blockchain() function.
 */
#if defined(__GNUG__)
extern "C"
#endif
void init_hash_generator();

/**
 * Receives the block's intended number and the block number of its intended father.
 * Generates and returns a random nonce ("number used only once").
 * 
 * You must use this nonce when generating a hash for the data.
 * Notice that if the intended father is substituted, the nonce and hash must be re-calculated.
 */
#if defined(__GNUG__)
extern "C"
#endif
int generate_nonce(int block_num, int father_block_num);

/**
 * Generates a cryptographic hash signature of the binary data,
 *  based on the the provided nonce.
 * This procedure might take some time to return.
 *
 * Returns the hash signature of the data as a null-terminated char array (C-string).
 * * Don't forget to deallocate it when done.
 *
 * You must use this function to create a hash for each piece of data you attach to the block chain.
 * Notice that if the intended father is substituted, the nonce and hash must be re-calculated.
 */
#if defined(__GNUG__)
extern "C"
#endif
char * generate_hash(const char data[], size_t length, int nonce);

/**
 * Frees resources used by the library.
 *
 * You must call this function when closing your library.
 */
#if defined(__GNUG__)
extern "C"
#endif
void close_hash_generator();

#endif
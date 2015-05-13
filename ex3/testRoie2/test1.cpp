/*
 * test1.cpp
 *
 *  Created on: May 10, 2015
 *      Author: roigreenberg
 */

#include <unistd.h>
#include <iostream>
#include "../blockchain.h"
#include "../hash.h"
#include <stack>

using namespace std;

#define IN_ITER 10
#define OUT_ITER 5
#define PHASE_2 9
#define PHASE_3 5
#define PHASE_4 10

bool a1 = true;
bool a2 = true;

void* f(void*)
{
	long i =0;
	while(a1)
	{
	i++;
	if (i%20000000 == 0)
		cout << "this should print while wainting to be closed" << endl;
	}
	while(a2);

	for (int i = PHASE_4; i > 0 ; i--)
	{
		cout << "attach " << i << ": ";
		cout << attach_now(i) << endl;
	}


}

int main() {

	cout << "********* start test **********" << endl;
	init_hash_generator();
	init_blockchain();

	cout << "finish initialing " << endl;

	pthread_t fT;
	char l[] = "lll";
	char* pl = l;

	cout << "\nPhase 1:\n" << endl;

	for (int j = 1; j <= OUT_ITER; j++)
	{
		cout << "\nAdding level " << j << ":\n" << endl;

		for (int i = 1; i <= IN_ITER; i++)
		{
			cout << "add " << i << ": ";
			cout << add_block(pl, 4) << endl;
		}
		cout << "Block num " <<  j*IN_ITER << " should not be added yet:  " << was_added( j*IN_ITER) << endl;
		while(chain_size() < j*IN_ITER);
		cout << "Now it does:  " << was_added( j*IN_ITER) << endl;
	}

	cout << "Expected chain size: " << OUT_ITER*IN_ITER << ". actual: " << chain_size() << endl;
	prune_chain();

	cout << "Phase 2:\n" << endl;

	int id = 0;

	for (int i = 1; i <= PHASE_2; i++)
	{
		cout << "add " << i << ": ";
		id = add_block(pl, 4);
		cout << id << endl;
		to_longest(id);
	}
	while(chain_size() < PHASE_2 + OUT_ITER*IN_ITER);
	cout << "Expected chain size: " << PHASE_2 + OUT_ITER*IN_ITER << ". actual: " << chain_size() << endl;
	prune_chain();

	cout << "\nPhase 3:\n" << endl;

	for (int i = 1; i <= PHASE_3; i++)
	{
		cout << "add " << i << ": ";
		cout << add_block(pl, 4) << endl;
	}
	cout << "closing" << endl;
	close_chain();
	pthread_create(&fT, NULL, f, NULL);
	return_on_close();
	a1 = false;
	cout << "closed" << endl;

	cout << "initate new chain:" << endl;
	init_blockchain();
	cout << "new chain size sholud be 0: " << chain_size() << endl;

	for (int i = 1; i <= PHASE_4; i++)
	{
		cout << "add " << i << ": ";
		id = add_block(pl, 4);
		cout << id << endl;
		to_longest(id);
	}
	a2 = false;

	pthread_join(fT, NULL);
	prune_chain();
	cout << "closing" << endl;
	close_chain();


	return_on_close();

	cout << "********* test end **********" << endl;
//	free(pl);
	return 0;
}


//void printChain(void)
//{
//	cout << "print :" << endl;
//
//
//	Block* cur // this should get the longest chain
//
//	cout << (*cur).num_block << endl;
//	while(cur->num_block != 0)
//	{
//		cout << cur->num_block << "  ->  " ;
//		cur = cur->father; //move to the father
//	}
//	cout << cur->num_block << endl;
//
//}

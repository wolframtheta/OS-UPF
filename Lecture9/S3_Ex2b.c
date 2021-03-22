
#include <malloc.h>
#include <pthread.h>
#include "myutils.h"

#define NACCOUNTS 1000 // multiple of 4

typedef struct {
	int id;
	double balance;
	int is_free;
	pthread_mutex_t lock;
	pthread_cond_t cond_free;
} Account;

Account bank[NACCOUNTS];

void account_lock(Account * ac) {
	pthread_mutex_lock(&ac->lock);
	while (!ac->is_free)
		pthread_cond_wait(&ac->cond_free, &ac->lock);
	ac->is_free = 0;
	pthread_mutex_unlock(&ac->lock);
}

void account_unlock(Account * ac) {
	pthread_mutex_lock(&ac->lock);
	ac->is_free = 1;
	pthread_cond_signal(&ac->cond_free);
	pthread_mutex_unlock(&ac->lock);
}

void transfer(Account * ac1, Account * ac2, double amount) {
	Account * lo = ac1->id < ac2->id ? ac1 : ac2;
	Account * hi = ac1->id < ac2->id ? ac2 : ac1;

	account_lock(lo);
	account_lock(hi);

	if (amount <= ac1->balance) {
		ac1->balance -= amount;
		ac2->balance += amount;
	}

	account_unlock(hi);
	account_unlock(lo);
}

void * totalBalance() {
	long sum = 0;
	for (int i = 0; i < NACCOUNTS; ++i) {
		account_lock(&bank[i]);
		sum += (long)bank[i].balance;
	}
	for (int i = 0; i < NACCOUNTS; ++i)
		account_unlock(&bank[i]);
	return (void *)sum;
}

typedef struct {
	int op;
	int accId;
	int accId2;
	double amount;
} ThreadData;

void * ProcessRequest(void * param) {
	ThreadData * data = (ThreadData *)param;
	if (data->op == 0) transfer(&bank[data->accId], &bank[data->accId2], data->amount);
	else if (data->op == 1) return totalBalance();
	free(param);
}

int main(void) {
	ThreadData * data;
	pthread_t tids[2 * NACCOUNTS + 1];

	for (int i = 0; i < NACCOUNTS; ++i) {
		bank[i].id = 1000 + i;
		bank[i].balance = 1000;
		bank[i].is_free = 1;
		pthread_mutex_init(&bank[i].lock, NULL);
		pthread_cond_init(&bank[i].cond_free, NULL);
	}

	for (int i = 0; i < NACCOUNTS; ++i) {
		data = malloc(sizeof(ThreadData));
		data->op = 0; // transfer
		data->accId = i % ( NACCOUNTS / 2 );
		data->accId2 = ( i + NACCOUNTS / 2 + 1 ) % NACCOUNTS;
		data->amount = 100;
		printf("Transfer 100 from %d to %d\n", data->accId, data->accId2);
		pthread_create(&tids[i], NULL, ProcessRequest, data);
	}

	data = malloc(sizeof(ThreadData));
	data->op = 1; // totalBalance
	pthread_create(&tids[NACCOUNTS], NULL, ProcessRequest, data);

	for (int i = 0; i < NACCOUNTS; ++i) {
		data = malloc(sizeof(ThreadData));
		data->op = 0; // transfer
		data->accId = NACCOUNTS / 4 + ( i + NACCOUNTS / 4 ) % ( NACCOUNTS / 2 );
		data->accId2 = i % ( NACCOUNTS / 4 );
		data->amount = 50;
		printf("Transfer 50 from %d to %d\n", data->accId, data->accId2);
		pthread_create(&tids[NACCOUNTS + 1 + i], NULL, ProcessRequest, data);
	}

	for (int i = 0; i < NACCOUNTS; ++i) {
		pthread_join(tids[i], NULL);
		pthread_join(tids[NACCOUNTS + 1 + i], NULL);
	}

	void * status;
	pthread_join(tids[NACCOUNTS], &status);

	for (int i = 0; i < NACCOUNTS; ++i)
		printf("%d: %f\n", bank[i].id, bank[i].balance);
	printf("Total balance: %ld\n", (long)status);

	return 0;
}


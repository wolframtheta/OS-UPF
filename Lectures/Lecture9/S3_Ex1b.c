
#include <malloc.h>
#include <pthread.h>
#include "myutils.h"

#define NACCOUNTS 1000

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

void withdraw(Account * ac, double amount) {
	account_lock(ac);

	if (amount <= ac->balance)
		ac->balance -= amount;

	account_unlock(ac);
}

void maintenance() {
	for (int i = 0; i < NACCOUNTS; ++i) {
		account_lock(&bank[i]);
		bank[i].balance -= 0.1;
	}
	for (int i = 0; i < NACCOUNTS; ++i)
		account_unlock(&bank[i]);
}

typedef struct {
	int op;
	int accId;
	double amount;
} ThreadData;

void * ProcessRequest(void * param) {
	ThreadData * data = (ThreadData *)param;
	if (data->op == 0) withdraw(&bank[data->accId], data->amount);
	else if (data->op == 1) maintenance();
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
		data->op = 0; // withdraw
		data->accId = i;
		data->amount = 100;
		pthread_create(&tids[i], NULL, ProcessRequest, data);
	}

	data = malloc(sizeof(ThreadData));
	data->op = 1; // maintenance
	pthread_create(&tids[NACCOUNTS], NULL, ProcessRequest, data);

	for (int i = 0; i < NACCOUNTS; ++i) {
		data = malloc(sizeof(ThreadData));
		data->op = 0; // withdraw
		data->accId = i;
		data->amount = 100;
		pthread_create(&tids[NACCOUNTS + 1 + i], NULL, ProcessRequest, data);
	}

	for (int i = 0; i < 2 * NACCOUNTS + 1; ++i)
		pthread_join(tids[i], NULL);

	for (int i = 0; i < NACCOUNTS; ++i)
		printf("%d: %f\n", bank[i].id, bank[i].balance);

	return 0;
}


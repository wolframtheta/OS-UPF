#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct str_acc{
	int id;
	double balance;
	bool busy;
	pthread_mutex_t lock;
	pthread_cond_t cond_free;
	
}Account;

//we can w8 condition to be true inside each account
void mutex_lock_monitor(Account* ac){
	pthread_mutex_lock(&ac->lock));
	while(ac->busy) pthread_cond_wait(&ac->cond_free,&ac->lock));
	ac->busy = true;
	pthread_thread_unlock(&ac->lock));
}

void mutex_unlock_monitor(Account* ac){
	pthread_mutex_lock(&ac->lock));
	ac->busy = false;
	pthread_cond_signal(&ac->cond_free);
	pthread_thread_unlock(&ac->lock));
}

bool withdraw (Account* ac, float amount){
	mutex_lock_monitor(ac);
	bool ok = ac->balance >= amount;
	if(ok) ac->balance -= amount;
	mutex_unlock_monitor(ac);
	return ok;
}

void deposite(Account* ac, float amount){
	mutex_lock_monitor(ac);
	bool ok = ac->balance >= amount;
	if(ok) ac->balance += amount;
	mutex_unlock_monitor(ac);
	return ok;
}
void transfer(Account* from , Account* to, float amount){
	bool ok from->balance >= amount;
	if(ok){
		withdraw(from, amount);
		deposite(to,amount);
}


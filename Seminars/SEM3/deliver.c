#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#define NBANKS 1000

typedef struct str_acc{
	int id;
	double balance;
	bool busy;
	pthread_mutex_t lock;
	pthread_cond_t cond_free;
	
}Account;

Account bank[NBANKS];

//we can w8 condition to be true inside each account
void mutex_lock_monitor(Account* ac){
	pthread_mutex_lock(&ac->lock);
	while(ac->busy) pthread_cond_wait(&ac->cond_free,&ac->lock);
	ac->busy = true;
	pthread_mutex_unlock(&ac->lock);
}

void mutex_unlock_monitor(Account* ac){
	pthread_mutex_lock(&ac->lock);
	ac->busy = false;
	pthread_cond_signal(&ac->cond_free);
	pthread_mutex_unlock(&ac->lock);
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
}
void transfer(Account* from , Account* to, float amount){
	bool ok = from->balance >= amount;
	if(ok){
		withdraw(from, amount);
		deposite(to,amount);
	}
}

/* 
The idea is to put as a false the ac->busy, this way when the monitor tries to acces
the critical part, it must wait until busy becames false. Then we can extract
the 0.1 for each account and finally set the busy back to false and signal the thread
*/
void bank_maintence(Account account[NBANKS]){
	int taxes = 0.1;
	for(int i = 0; i<NBANKS , i++;){account[i].busy = true;} //busy flag as true
	for(int i = 0; i<NBANKS , i++;){account[i].balance  -= taxes;}// apply the taxes
	for(int i = 0; i<NBANKS , i++;){// wake up the threads
		account[i].busy  = false;
		pthread_cond_signal(&account[i].cond_free);
	}
}
int main (int argc, char *argv[]){}





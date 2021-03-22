#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>


pthread_mutex_t global_lock;
bool global_is_busy = false;
pthread_cond_t global_cond_free;

typedef struct str_acc{
	int id;
	double balance;
	bool busy;
	pthread_mutex_t lock;
	pthread_cond_t cond_free;
	
}Account;

//Using un lock, the monitor performs like a single mutex_lock
void mutex_lock_monitor(){
	pthread_mutex_lock(&global_lock);
	while(global_is_busy) pthread_cond_wait(&global_cond_free,&global_lock);
	global_is_busy = true;
	pthread_thread_unlock(&global_lock);
}

void mutex_unlock_monitor(){
	pthread_mutex_lock(&global_lock);
	global_is_busy = false;
	pthread_cond_signal(&global_cond_free);
	pthread_thread_unlock(&global_lock);
}
//with this implementation, we get busy waiting and it's like sequential
bool withdraw(Account *acc , float amount){
	pthread_mutex_lock(&global_lock);
	bool ok = ac->balance >= amount;
	if(ok) ac->balance -= amount;
	pthread_mutex_unlock(&global_lock);
	return ok;
}

// MONITOR: same as lock but more higher level, we have 1 more variable to check if is busy
bool withdrawMonitor (Account *acc, float amount){
	mutex_lock_monitor();
	bool ok = ac->balance >= amount;
	if(ok) ac->balance -= amount;
	mutex_unlock_monitor();
	return ok;
}






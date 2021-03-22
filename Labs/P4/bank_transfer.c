#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "myutils.h"

#define Nprocess 200
#define N 10

typedef struct struct_account{
	int id;
	int account_type;
	double balance;
} account;

int SOLUTION = 0; 
sem_t* named_mutex;

void do_something() {
	int n = 20;
	int sum = 0;
	for(int i=0;i<n;i++) 
	for(int j=0;j<n;j++) 
	for(int k=0;k<n;k++) sum++;
}
bool check(account* acc, double amount) {
	return acc->balance - amount >= 0;
}
bool withdraw(account* acc, double amount) {
	if(check(acc,amount))  { 
		acc->balance -= amount; 
		do_something();
		return true;
	}
	return false;
}

void deposit(account* acc, double amount) {
	acc->balance += amount;
	do_something() ;
}

bool trans_file_lock(int i_from,int i_to,double amount){
	int fd = open("bank.dat",O_RDWR);
	int len = sizeof(account);
	account from,to;
		if(fd < 0){
			printf("File does not exist\n");
			exit(1);
		}
		
	//Avoid dead locks
	if(i_from < i_to){
		while(file_lock(fd,i_from*len,len)<0 || file_lock(fd,i_to*len,len)<0);
	}else{
		while(file_lock(fd,i_to*len,len)<0 || file_lock(fd,i_from*len,len)<0);
	}
	
	//reading accounts info
	lseek(fd,i_from*len,SEEK_SET);
	read(fd,&from,len);
	lseek(fd,i_to*len,SEEK_SET);
	read(fd,&to,len);
	
	
	//apply the operations 
	bool bDone = withdraw(&from,amount);
	if(bDone) deposit(&to,amount);
	
	//set back the info to the file
	lseek(fd,i_from*len,SEEK_SET);
	write(fd,&from,len);
	lseek(fd,i_to*len,SEEK_SET);
	write(fd,&to,len);
	
	// unlock file locks
	file_unlock(fd,i_from,len);
	file_unlock(fd,i_to,len);
	
	//printf("transfer from: %d to: %d amount: %lf\n",i_from,i_to,amount);
	close(fd);
	
}

bool trans_sem(int i_from,int i_to,double amount){
	int fd = open("bank.dat",O_RDWR);
	int len = sizeof(account);
	if(fd < 0){
		printf("File does not exist\n");
		_exit(1);
	}
	account from,to;
	sem_wait(named_mutex);
	
	//reading accounts info
	lseek(fd,i_from*len,SEEK_SET);
	read(fd,&from,len);
	lseek(fd,i_to*len,SEEK_SET);
	read(fd,&to,len);
	
	//apply the operations 
	bool bDone = withdraw(&from,amount);
	if(bDone) deposit(&to,amount);
	
	//set back the info to the file
	lseek(fd,i_from*len,SEEK_SET);
	write(fd,&from,len);
	lseek(fd,i_to*len,SEEK_SET);
	write(fd,&to,len);
	
	//signal the semaphore
	sem_post(named_mutex);
	close(fd);
	return bDone;
}
void child_transfers(){
	int from = -1;
	int to = -1;
	int iter = 1000 / Nprocess;
	while(iter--) {
		from = -1;
		to = -1;
		while(from == to) {
			from = rand()%N;
			to = rand()%N;
		}
		if(SOLUTION == 0){
			trans_file_lock(from,to,rand()%50+1);
		}else if (SOLUTION == 1){
			trans_sem(from,to,rand()%50+1);	
		}
	}
}

int main(int argc, char *argv[]){
  	if(argc > 1) SOLUTION = atoi(argv[1]);
  	sem_unlink("named_mutex");
  	sem_open("named_mutex",O_CREAT,0600,1);
  	startTimer(0);
    	for(int i=0;i<Nprocess;i++) {
    		int pid = fork();
        	if(pid == 0){
        		child_transfers();
        		_exit(1);
        	}
    	}
  	while (wait(NULL) > 0); //wait process to finish
  	sem_close(named_mutex);
  	
  	// print the info
  	int fd = open("bank.dat",O_RDONLY);
	double bank_sum = 0;
	account ac;
	for (int i = 0; i<N;i++){
		lseek(fd,i*sizeof(account),SEEK_SET);
		read(fd,&ac,sizeof(account));
		bank_sum += ac.balance;
		printf("account %d, balance: %lf \n",i+1,ac.balance);
	}
	close(fd);
	printf("Total balance of all accounts: %lf\n", bank_sum);
	printf("Done: %ld millis\n",endTimer(0));

}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define N 10

typedef struct {
	int id;
	int account_type;
	double balance;
}account;


int main (){
	int i= 0;
	while(i!=4){
		int fd = open("bank.dat",O_RDONLY);
		if(fd < 0){
			printf("File does not exist\n");
			break;
		}
		account ac;
		double bank_sum = 0;
		for (int i = 0; i<N;i++){
			lseek(fd,i*sizeof(account),SEEK_SET);
			read(fd,&ac,sizeof(account));
			bank_sum += ac.balance;
			printf("account %d, balance: %lf \n",i+1,ac.balance);
		}
		close(fd);
		printf("Total balance of all accounts: %lf\n", bank_sum);
		i++;
		sleep(0.5);
	}
	return 0;
}

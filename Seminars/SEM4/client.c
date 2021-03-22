#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


struct sockaddr_in serv_addr;

int main(int argc, char * argv[]){
  if (argc < 3) {fprintf(stderr,"usage: %s hostname port\n",argv[0]);exit(1);}
  
  socketfd = socket(AF_INET,SOCK_STREAM,0);
  printf("Socket created\n");
  fill_addr (argv[1],argv[2],&serv_addr);
  printf("about to connect\n");
  int retcon = connect(socketfd,(struct socketaddr *)&serv_addr , sizeof(serv_addr));
  int num = 0;
  printf("Enter a number: \n");
  scanf("%d",&num);
  for(int i = 1; i<num;i++){
    printf("Client sending %d\n",num);
    write(sockfd,&i,sizeof(int));
  }
  return 0;
}

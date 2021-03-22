
#include "myutils.c"


struct sockaddr_in serv_addr,cli_addr;

int main(int argc, char * argv[]){
  if (argc < 2) {fprintf(stderr,"usage: ./server port\n");exit(1);}
  
  int socketfd = socket(AF_INET,SOCK_STREAM,0);
  printf("Socket created\n");
  fill_addr(NULL,argv[1],&serv_addr);//this initialize all params in serv_addres
  printf("about to connect\n");
  
  //this is to reuse the same port everytime we run it
  int reuse = 1;
  setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,(const char*) &reuse, sizeof(reuse));
  
  
  bind(socketfd,(struct socketaddr *)&serv_addr,sizeof(serv_addr));
  listen(socketfd,100);
  printf("Listen done\n");
  
  socklen_t clilen = sizeof(cli_addr); //create this variable because need to passs size by reference
  
  while(1){
      int newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
      printf("Client accepted\n");
      //read the numbers from clients
      int sum = 0;
      int num;
      while(read(newsockfd,&num,sizeof(num))){
        printf("Received %d\n",num);
        sum += sum;
      }
      printf("Total sum: %d\n",sum);
      
  }
  

    
  return 0;
}

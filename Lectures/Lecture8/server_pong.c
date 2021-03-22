
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include "myutils.h"

struct sockaddr_in serv_addr, cli_addr;

int main(int argc, char * argv[]) {
  if (argc < 2) {
    fprintf(stderr,"usage: ./server port\n");
    exit(1);
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  printf("socket created\n");
     
  fill_addr(NULL, argv[1], &serv_addr);

  printf("about to bind\n");
     
  bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  // socket file descriptor and number of connections that can be waiting
  listen(sockfd, 10);

  printf("listen done\n");

  // blocks until client connects
  socklen_t clilen = sizeof(cli_addr);
  int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  if (newsockfd > 2) printf("accepted\n");
  else {
    printf("Error creating socket");
    exit(1);
  }

  int num;
  read(newsockfd, &num, sizeof(int));
  printf("Server received %d\n", num);
  while (num >= 0) {
    num++;
    printf("Server sent %d\n", num);
    write(newsockfd, &num, sizeof(int));
    read(newsockfd, &num, sizeof(int));
    printf("Server received %d\n", num);
  }

  close(newsockfd);
  close(sockfd);

  return 0; 
}


#include "myutils.h"

#define N 256

struct sockaddr_in serv_addr;

int main(int argc, char * argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(1);
  }

  // Client call to Socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  printf("socket created\n");

  fill_addr(argv[1], argv[2], &serv_addr);

  printf("about to connect\n");

  // Client connect
  int retcon = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  printf("Please enter a starting number: \n");

  char buffer[N];
  read(0, buffer, N);

  // when sending to a web this second '\n' is important
  sprintf(buffer, "%s\n", buffer);

  int num = atoi(buffer);
  printf("Client sending converted text to int: %d\n", num);
  write(sockfd, &num, sizeof(int));

  // Deal with server response here

  close(sockfd);

  return 0;
}

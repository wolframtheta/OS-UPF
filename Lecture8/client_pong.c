
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

  int num = 0;
  printf("Client sent %d\n", num);
  write(sockfd, &num, sizeof(int));
  while (num >= 0) {
    read(sockfd, &num, sizeof(int));
    printf("Client received %d\n", num);
    num = num < 10 ? num + 1 : -1;
    printf("Client sent %d\n", num);
    write(sockfd, &num, sizeof(int));
  }

  close(sockfd);

  return 0;
}

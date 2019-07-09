#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define DELAY 200


void usage(void)
{}

int main(int argc, char *argv[])
{
  if(argc != 2){
    exit(1);
  }
  printf("%s",argv[1]);
  return 0;
  FILE* f = fopen(argv[1], "r");
  sleep(200);
  return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

const int WPM = 140; //desired speed

#define AVG_CHAR_PER_WORD 6 /* https://diuna.biz/length-of-words-average-number-of-characters-in-a-word/ */

const int DELAY = 10e5 / ((WPM * AVG_CHAR_PER_WORD) / 60.0f);

void usage(void)
{
  printf("\n");
}

void print_delay(FILE *file)
{
  char c;
  while((c = fgetc(file)) != EOF){
    putchar(c);
    usleep(DELAY);
    fflush(stdout);
  }
}

int main(int argc, char *argv[])
{
  if(argc != 2){
    usage();
    exit(-1);
  }

  FILE* file = fopen(argv[1], "r");
  if(!file){
    printf("Error: %s\n", strerror(errno));
    exit(-1);
  }

  print_delay(file);

  fclose(file);

  return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
 * Program to output files character by character to simulate typing.
 * F Fitzgerald 2019
 */

#define AVG_CHAR_PER_WORD 6

void usage(char *argv[])
{
  printf("Outputs a file character by character using a simulated typing speed.\n");
  printf("Usage: %s [-w words_per_min] file\n", argv[0]);
}

void print_delay(FILE *file, int delay)
{
  char c;
  while((c = fgetc(file)) != EOF){
    putchar(c);
    usleep(delay);
    fflush(stdout);
  }
}

int main(int argc, char *argv[])
{
  int wpm = 140; //default words per minute
  int opt;
  while((opt = getopt(argc, argv, "w:")) != -1){
    switch(opt){
      case 'w':
        wpm = atoi(optarg);
        break;
      default:
        usage(argv);
        exit(-1);
    }
  }

  if((argc - optind) != 1){
    usage(argv);
    exit(-1);
  }

  FILE* file = fopen(argv[optind], "r");
  if(!file){
    printf("Error: %s\n", strerror(errno));
    exit(-1);
  }

  //first clear the console for full fake effect
  printf("\033c");

  //typing delay is based on characters per second
  //since we are using usleep seconds are microseconds
  int delay = 10e5 / ((wpm * AVG_CHAR_PER_WORD) / 60.0f);
  print_delay(file, delay);

  fclose(file);

  return 0;
}

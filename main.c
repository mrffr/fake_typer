#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <signal.h>

/*
 * Program to output files character by character to simulate typing.
 */

#define AVG_CHAR_PER_WORD 6 //guesstimate

void cleanup()
{
  printf("\033c");
  exit(0);
}

// print errno and exit program
void die()
{
  fprintf(stderr, "Error: %s\n", strerror(errno));
  exit(-1);
}

void usage(char *argv[])
{
  fprintf(stderr, "Outputs a file character by character using a simulated typing speed.\n");
  fprintf(stderr, "Usage: %s [-w words_per_min] file\n", argv[0]);
}

void print_delay(FILE *file, int delay)
{
  char c;
  while((c = fgetc(file)) != EOF){
    putchar(c);
    usleep(delay);
    fflush(stdout); //need to flush after each char to see the output
  }
}

int main(int argc, char *argv[])
{
  signal(SIGINT, cleanup); //reset terminal when we Ctrl-C to exit

  int wpm = 140; //default words per minute

  /* handle args */
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

  /* Open the input file */
  FILE* file = fopen(argv[optind], "r");
  if(!file) die();

  /* set terminal to not echo any key inputs */
  struct termios term;

  if(tcgetattr(STDIN_FILENO, &term)) die();

  term.c_lflag &= ~((tcflag_t) ECHO); //turn off echo

  if(tcsetattr(STDIN_FILENO, TCSANOW, &term)) die();

  /* Clear the console for a better effect */
  printf("\033c"); //clear console
  printf("\033[?;25;l"); //hide cursor

  /* Typing delay is based on characters per second
   * since we are using usleep seconds are microseconds */
  int delay = 10e5 / ((wpm * AVG_CHAR_PER_WORD) / 60.0f);
  print_delay(file, delay);

  fclose(file);

  cleanup();

  return 0;
}

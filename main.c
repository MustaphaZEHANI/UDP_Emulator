#include <stdio.h>
#include <stdlib.h>

#define debug 1

int main (int argc, char** argv)
{
  puts("UDP Emulator 1.0 \n");

  /* The Received message */
  if (argc == 2)
  {
    if (debug) 
      printf(" The message received is = %s\n",argv[1]);
  }
  else if (argc > 2)
  {
    if (debug) 
      printf(" The message format is incorrect ! \n there's %d supplied arguments\n",--argc);
    return 0;
  }
  else
  {
    if (debug)
      printf(" The message format is incorrect !! pls put an argument\n");

    return 0;
  }
  

  /*if (debug) 
    printf(" The number of Supplied argument = %d \n The Argument Supplied = %s \n"
            , --argc, argv[1]);*/


  /*Developing the */

  return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

void my_handler(int dada)
{
  printf("Nu mai tasta CTRL+%s caci nu are efect.\n",
         (dada == SIGINT ? "C" : "\\"));
}

int main()
{
  int i;
  signal(SIGINT, my_handler);
  signal(SIGQUIT, my_handler);

  for (i = 0; i < 10; i++)
  {
    printf("Portiune corupta...\n");
    sleep(1);
  }
  printf("Sfarsit portiune corupta.\n");

  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
  for (i = 0; i < 10; i++)
  {
    printf("Portiune necorupta...\n");
    sleep(1);
  }
  printf("Sfarsit program.\n");

  return 0;
}

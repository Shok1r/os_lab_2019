#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {

  pid_t child_process = fork();

  if (child_process > 0) {
    sleep(40);
  } else {
    exit(0);
  }

  sleep(40);

  return 0;
}

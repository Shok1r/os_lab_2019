#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

volatile pid_t *child_pid_array;
volatile int child_number;

static void alarmHandler(int signal)
{
  for (int i = 0; i < child_number; i++)
  {
    kill(child_pid_array[i], SIGKILL);
  }
}

int main(int argc, char **argv)
{
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  bool with_files = false;

  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1)
      break; //Если закончились параметры командной строки

    switch (c)
    {
    case 0:
      switch (option_index)
      {
      case 0:
        seed = atoi(optarg);
        break;
      case 1:
        array_size = atoi(optarg);
        break;
      case 2:
        pnum = atoi(optarg);
        break;
      case 3:
        timeout = atoi(optarg);
        break;
      case 4:
        with_files = true;
        break;

      defalut:
        printf("Index %d is out of options\n", option_index);
      }
      break;
    case 'f':
      with_files = true;
      break;

    case '?':
      printf("Found error");
      break;

    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc)
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1)
  {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
  int array_step = pnum < array_size ? (array_size / pnum) : 1;

  /*Выставление таймаута*/
  if (timeout != -1)
  {
    printf("Установка таймера\n");
    alarm(timeout);
    signal(SIGALRM, alarmHandler);
  }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int fd[2];
  if (with_files)
  {
    FILE *file;
    file = fopen("lab3.txt", "w+");
    fprintf(file, "");
    fclose(file);
  }
  else
  {
    if (pipe(fd) < 0)
    {
      perror("Can\'t create pipe\n");
      exit(EXIT_FAILURE);
    }
  }

  child_pid_array = (pid_t *)malloc(sizeof(pid_t) * pnum);
  child_number = 0;

  for (int i = 0; i < pnum; i++)
  {
    pid_t child_pid = fork();

    if (child_pid >= 0)
    {
      // successful fork
      active_child_processes += 1;

      if (child_pid == 0)
      {
        unsigned int start = array_step * (active_child_processes - 1);
        unsigned int end = start + array_step;

        start = start > array_size ? array_size : start;
        end = end > array_size ? array_size : end;

        if (active_child_processes == pnum) //Для нечетного массива
          end = array_size;

        struct MinMax min_max = GetMinMax(array, start, end);

        if (with_files)
        {
          FILE *file;
          file = fopen("lab3.txt", "a+");
          flock(file, LOCK_EX);
          fwrite(&min_max, sizeof(struct MinMax), 1, file);
          flock(file, LOCK_UN);
          fclose(file);
        }
        else
        {
          close(fd[0]);
          write(fd[1], &min_max, sizeof(struct MinMax));
          close(fd[1]);
        }
        return 0;
      }
    }
    else
    {
      printf("Fork failed!\n");
      return 1;
    }

    /*Запоминаем PID процесса*/
    if (child_pid != 0)
    {
      child_pid_array[i] = child_pid;
      child_number += 1;
    }
  }

  while (active_child_processes > 0)
  {
    waitpid(-1, NULL, WNOHANG);
    active_child_processes -= 1;
  }

  FILE *file;
  file = fopen("lab3.txt", "r");

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++)
  {
    int min = INT_MAX;
    int max = INT_MIN;
    struct MinMax tmp_min_max;

    if (with_files)
    {
      fread(&tmp_min_max, sizeof(struct MinMax), 1, file);
      if (i == pnum - 1)
        fclose(file);
    }
    else
    {
      close(fd[1]);
      read(fd[0], &tmp_min_max, sizeof(struct MinMax));
      close(fd[0]);
    }

    min = tmp_min_max.min;
    max = tmp_min_max.max;

    if (min < min_max.min)
      min_max.min = min;
    if (max > min_max.max)
      min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

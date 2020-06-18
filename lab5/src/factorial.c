#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/time.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static int fac_value = 1;

typedef struct
{
    int *array_numbers;
    int module;
    int begin;
    int end;
} fac_part;

void *start_factorial(void *args)
{
    fac_part *local_part = (fac_part *)args;
    int local_ans = 1;
    for (int i = local_part->begin; i < local_part->end; i++)
        local_ans *= local_part->array_numbers[i];
    local_ans = (local_ans % local_part->module);
    pthread_mutex_lock(&mut);
    fac_value *= local_ans;
    pthread_mutex_unlock(&mut);
}

int main(int argc, char **argv)
{

    uint32_t k = -1;
    uint32_t threads_mum = -1;
    uint32_t mod_fac = -1;

    while (1)
    {
        static struct option options[] = {{"k", required_argument, 0, 0},
                                          {"pnum", required_argument, 0, 0},
                                          {"mod", required_argument, 0, 0},
                                          {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "?", options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            switch (option_index)
            {
            case 0:
                k = atoi(optarg);
                break;
            case 1:
                threads_mum = atoi(optarg);
                break;
            case 2:
                mod_fac = atoi(optarg);
                break;
            }
            break;

        case '?':
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

    if (k == -1 || threads_mum == -1 || mod_fac == -1)
    {
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
               argv[0]);
        return 1;
    }

    int *array_multiplier = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        array_multiplier[i] = i + 1;
    }

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int step = k > threads_mum ? (k/threads_mum) : 1;
    pthread_t threads[threads_mum];
    fac_part fac_parts[threads_mum];
    for(int i = 0; i < threads_mum; i++){
        fac_parts[i].module = mod_fac;
        fac_parts[i].array_numbers = array_multiplier;
        fac_parts[i].begin = step * i;
        fac_parts[i].end = (i+1) * step;
        if (i == threads_mum - 1)
            fac_parts[i].end = k;
    }

    for(uint32_t i =0; i < threads_mum; i++) {
        if(pthread_create(&threads[i], NULL, start_factorial, (void*)&fac_parts[i])) {
            perror("\nError create thread\n");
            return 1;
        }
    }

    for (uint32_t i = 0; i < threads_mum; i++) {
        pthread_join(threads[i], NULL);
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    pthread_mutex_destroy(&mut);
    printf("Значение факториала %d по модулю %d равно %d\n", k, mod_fac, fac_value % mod_fac);
    printf("Elapsed time: %fms\n", elapsed_time);

    return 0;
}
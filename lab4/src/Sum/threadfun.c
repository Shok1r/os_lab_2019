#include <stdlib.h>

#include "threadfun.h"
#include "utils.h"

int Sum(const struct SumArgs *args) {
  int sum = 0;
  for (size_t i = args->begin; i < args->end; i++){
      sum += args->array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    return (void *)(size_t)Sum(sum_args);
}
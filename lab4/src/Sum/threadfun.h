#ifndef THREAD_FUN_H_
#define THREAD_FUN_H_

struct  SumArgs;

int Sum(const struct SumArgs *args);

void *ThreadSum(void *args);

#endif


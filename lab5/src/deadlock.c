#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mut_1;
pthread_mutex_t mut_2;


void *deadlock_first(void *args) {
  pthread_mutex_lock(&mut_1);
  sleep(1);
  pthread_mutex_lock(&mut_2);
  printf("Критическая секция первого потока");
  pthread_mutex_unlock(&mut_1);
  pthread_mutex_unlock(&mut_2);
}

void *deadlock_second(void *args) {
  pthread_mutex_lock(&mut_2);
  sleep(1);
  pthread_mutex_lock(&mut_1);
  printf("Критическая секция второго потока");
  pthread_mutex_unlock(&mut_2);
  pthread_mutex_unlock(&mut_1);
}

int main() {

  pthread_t threads[2];
  pthread_mutex_init(&mut_1, NULL);
  pthread_mutex_init(&mut_2, NULL);
  pthread_create(&threads[0], NULL, deadlock_first, NULL);
  pthread_create(&threads[1], NULL, deadlock_second, NULL);
  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
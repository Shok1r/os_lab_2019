#ifndef UTILS_H
#define UTILS_H

/*Структура с диапазонами суммирования*/
struct SumArgs {
  int *array; /*Указатель на массив суммирования*/
  int begin; /*индекс начала суммирования*/
  int end; /*индекс конца суммирования*/
};

void GenerateArray(int *array, unsigned int array_size, unsigned int seed);

#endif
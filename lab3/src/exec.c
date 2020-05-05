#include <unistd.h>
#include <stdio.h>

int main(int argc, int *argv[]){
	printf("Выполнение %ls\n", argv[0]);
	execv("sequential_min_max", argv);
	return 0;
}

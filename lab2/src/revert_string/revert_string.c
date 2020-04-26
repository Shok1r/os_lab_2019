#include "revert_string.h"

void RevertString(char *str){
	
	unsigned int len = strlen(str);
	char *newstr = malloc(sizeof(char) * (len + 1));
	for(int i = 0; i < len; i++){
		*(newstr + i) = *(str + len - 1 - i);
		}
	for(int i = 0; i < len; i++){
		*(str + i) = *(newstr + i);
		}
	free(newstr);
}


#include <stddef.h>
#include <stdio.h>

size_t my_strlen(const char *s){
	size_t i = 0;
	while (*s != '\0'){
		s = s + 1;
		i++;
	}
	return i;
}

int main(void){
	printf("%zu\n",my_strlen("cat"));	
	printf("%zu\n",my_strlen(""));
	return 0;
}


#include <stdio.h>
int main(int argc, char *argv[]){
	printf("Let's start1!");
	for(int i=0;i<argc; i++){
		printf("receive > %s \n", argv[i]);
	}
	printf("don't receive > %s \n", argv[100]);
	return 0;
}

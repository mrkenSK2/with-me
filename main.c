#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	system("clear");
	//printf("\e[2J\e[1;1H");                   // clear
	printf("\e[33m\e[46m");                   // color
	printf("change\n");
	printf("\e[39m\e[49m");                   // reset
	for(int i=0;i<argc; i++){
		printf("receive > %s \n", argv[i]);
	}
	return 0;
}

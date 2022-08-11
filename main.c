#include <stdio.h>
#include <stdlib.h>

void file_print(char *filename){
	FILE *fp;
	char str[10];
	if((fp=fopen(filename, "r")) == NULL){
		fprintf(stderr, "file cannot open\n");
		exit(1);
	}
	while(fgets(str, sizeof(str), fp)){
		printf("%s", str);
	}
	fclose(fp);
	return;
}
		
int main(int argc, char *argv[]){
	system("clear");
	//printf("\e[2J\e[1;1H");                   // clear
	if(argc != 2){
		printf("please input filename\n");
	}else{
		file_print(argv[1]);
	}
	return 0;
}

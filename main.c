#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

struct termios cookedMode, rawMode; 

void file_print(char *filename){
	FILE *fp;
	char str[BUFFER_SIZE];
	if((fp=fopen(filename, "r")) == NULL){
		fprintf(stderr, "file cannot open\n");
		exit(1);
	}
	while(fgets(str, sizeof(str), fp)!=NULL){
		printf("%s", str);
	}
	fclose(fp);
	return;
}

struct string{
	char str[BUFFER_SIZE];
	struct string* prev;
	struct string* next;
};

struct string* insert(struct string* insert_to_nextpoint){
	struct string* new_str = (struct string*)malloc(sizeof(struct string));
	if(insert_to_nextpoint == NULL){
		new_str->prev = NULL;
		new_str->next = NULL;
	}else{
		new_str->prev = insert_to_nextpoint;
		if(insert_to_nextpoint->next != NULL){
			insert_to_nextpoint->next->prev = new_str;
			new_str->next = insert_to_nextpoint->next;
		}else{
			new_str->next = NULL;
		}
		insert_to_nextpoint->next = new_str;
	}
	return new_str;
}

void file_read(char* filename, struct string* head){
	FILE* fp;
	char buf[BUFFER_SIZE];
	
	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "file open error\n");
		exit(1);
	}
	struct string* current = head;
	while(fgets(buf, sizeof(buf), fp) != NULL){
		strcpy(current->str, buf);
		insert(current);
		current = current->next;
	}
	fclose(fp);
	return;
}

		
int main(int argc, char **argv){
	struct string* head = (struct string*)malloc(sizeof(struct string));
	head->prev = NULL;
	head->next = NULL;
	if(argc != 2){
		printf("usage: ./a.out filename\n");
	}else{
		file_read(argv[1], head);
		struct string* current = head;
		char input_key;
		if(tcgetattr(STDIN_FILENO, &cookedMode) != 0){
			perror("tcgetattr() error");
		}else{
			cfmakeraw(&rawMode);
			tcsetattr(STDIN_FILENO, 0, &rawMode);	
			while(current != NULL){
				system("clear");
				printf("%s", current->str);
				
				input_key = getchar();
				if(input_key == 'n'){
					current = current->next;
				}
				if(input_key == 'p'){
					current = current->prev;
				}
				if(input_key == 'e'){
					system("clear");
					break;
				}
			}
			tcsetattr(STDIN_FILENO, 0, &cookedMode);
		}
		free(head);
	}
	return 0;
}

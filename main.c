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
	// only first time
	if(fgets(buf, sizeof(buf), fp) != NULL){
		strcpy(current->str, buf);
	}
	while(fgets(buf, sizeof(buf), fp) != NULL){
		insert(current);
		current = current->next;
		strcpy(current->str, buf);
	}
	fclose(fp);
	return;
}

void flag_reset(int *array, int length){
	for(int i=0;i<length;i++){
		array[i] = 0;
	}
	return;
}
		
int main(int argc, char **argv){
	struct string* head = (struct string*)malloc(sizeof(struct string));
	int line_num=1;
	head->prev = NULL;
	head->next = NULL;
	if(argc != 2){
		printf("usage: ./a.out filename\n");
	}else{
		file_read(argv[1], head);
		struct string* tmp = head;
		// check line
		while(tmp->next != NULL){
			++line_num;
			tmp = tmp->next;
		}
		
		struct string* current = head;
		char input_key;
		int line = 1;
		if(tcgetattr(STDIN_FILENO, &cookedMode) != 0){
			perror("tcgetattr() error");
		}else{
			cfmakeraw(&rawMode);
			tcsetattr(STDIN_FILENO, 0, &rawMode);	
			while(1){
				system("clear");
				printf("%s", current->str);
				printf("\n\rline%d\n", line);
				if(line == 1) printf("\rhead of line\n");
				else if(line == line_num) printf("\rend of line\n");

				input_key = getchar();
				if(input_key == 'p'){
					if(current->prev == NULL){
						continue;
					}
					--line;
					current = current->prev;
				}
				if(input_key == 'n'){
					if(current->next == NULL){
						continue;
					}
					++line;
					current = current->next;
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


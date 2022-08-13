#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "str.h"

#define BUFFER_SIZE 100

struct termios cookedMode, rawMode; 
		
int main(int argc, char **argv){
	struct str* head = (struct str*)malloc(sizeof(struct str));
	int line_num=1;
	head->prev = NULL;
	head->next = NULL;
	if(argc != 2){
		printf("usage: ./a.out filename\n");
	}else{
		file_read(argv[1], head);
		struct str* tmp = head;
		// check line
		while(tmp->next != NULL){
			++line_num;
			tmp = tmp->next;
		}
		
		struct str* current = head;
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


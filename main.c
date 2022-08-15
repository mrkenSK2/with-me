#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "str.h"

#define BUFFER_SIZE 100

struct termios cookedMode, rawMode; 
		
int main(int argc, char **argv){
	struct str* head = (struct str*)malloc(sizeof(struct str));
	int line_num=1;
	int cursor[2] = {1, 1};
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
		int line_limit = 40;
		if(tcgetattr(STDIN_FILENO, &cookedMode) != 0){
			perror("tcgetattr() error");
		}else{
			cfmakeraw(&rawMode);
			tcsetattr(STDIN_FILENO, 0, &rawMode);
			system("clear");
			if(line_num<40) line_limit = line_num;
			for(int i=0;i<line_limit;i++){
				//system("clear");
				printf("%s\r", current->str);
				current = current->next;
			}
			// back 1 line
			printf("\033[1;1H");
			current = head;
			cursor[0]= 1;
			cursor[1]= 1;
			//printf("\033[%02d;%02dH", 1, 1);
			int offset;
			while(1){
				system("clear");
				for(int i=0;i<line_limit;i++){
					printf("%s\r", current->str);
					current = current->next;
				}
				printf("\033[41;1H");
				printf("\033[0J");
				printf("line%d", line);
				printf("\033[%d;1H", cursor[0]);
				input_key = getchar();
				if(input_key == 'k'){
					if(current->prev == NULL){
						continue;
					}
					--line;
					cursor[0] = line;
					current = current->prev;
					printf("\033[1A");
				}
				if(input_key == 'j'){
					if(current == NULL){
						current = head;
						for(int i=0;i<offset;i++){
							current=current->next;
						}
						continue;
					}
					++line;
					current = current->next;
					if(line - line_limit <= 0){
						offset = 0;
					}else{
						offset = line - line_limit;
					}
					current = head;
					for(int i=0;i<offset;i++){
						current=current->next;
					}
					if(!(line<=line_num && offset > 0)){
						// cursor is more than 40 line
						//current = head;
						cursor[0] += 1;
					}
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


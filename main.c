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
		// 出力行番号の始まり終わり
		int out_start_index, out_end_index;
		struct str *out_start, *out_end;
		if(tcgetattr(STDIN_FILENO, &cookedMode) != 0){
			perror("tcgetattr() error");
		}else{
			cfmakeraw(&rawMode);
			tcsetattr(STDIN_FILENO, 0, &rawMode);
			system("clear");
			if(line_num<40) line_limit = line_num;
			for(int i=0;i<line_limit;i++){
				printf("%s\r", current->str);
				out_end = current;
				current = current->next;
			}
			out_start = head;
			//out_end = current->prev;
			// back 1 line
			printf("\033[1;1H");
			current = head;
			cursor[0]= 1;
			cursor[1]= 1;
			out_start_index = 1;
			out_end_index = line_limit;
			while(1){
				system("clear");
				// 	ここが始まるときは書き始め
				tmp = out_start;
				for(int i=0;i<line_limit;i++){
					printf("%s\r", tmp->str);
					tmp = tmp->next;
				}
				printf("\033[41;1H");
				printf("\033[0J");
				printf("line%d", line);
				printf("\033[%d;%dH", cursor[0], cursor[1]);
				input_key = getchar();
				if(input_key == 'k'){
					if(out_start->prev == NULL){
						// 表示は変わらないカーソルのみずらす
						if(line!=1){
							--line;
							cursor[0] -= 1;
							current = current->prev;
							int str_len = strlen(current->str);
							if(cursor[1] > str_len
							|| current->str[cursor[1]-1] == '\n'){
								if(current->str[str_len-1] == '\n') cursor[1] = str_len-1;
								else cursor[1] = str_len;
							}
						}
						out_start_index = 1;
						out_end_index = line_limit;

						continue;
					}
					// 表示も変わるか、カーソルのみずれるか
					--line;
					current = current->prev;
					int str_len = strlen(current->str);
					if(cursor[1] > str_len
					|| current->str[cursor[1]-1] == '\n'){
						if(current->str[str_len-1] == '\n') cursor[1] = str_len-1;
						else cursor[1] = str_len;
					}
					if(line < out_start_index){
						// 出力行が変わる
						out_start = out_start->prev;
						out_end = out_end->prev;
						out_start_index -= 1;
						out_end_index -= 1;
					}else{
						// カーソルのみ
						cursor[0] -= 1;
					}
				}
				if(input_key == 'j'){
					if(out_end->next == NULL){
						if(line!=line_num){
							++line;
							cursor[0] += 1;
							current = current->next;
							int str_len = strlen(current->str);
							if(cursor[1] > str_len
							|| current->str[cursor[1]-1] == '\n'){
								if(current->str[str_len-1] == '\n') cursor[1] = str_len-1;
								else cursor[1] = str_len;
							}
						}
						out_end_index = line_num;
						continue;
					}
					++line;
					current = current->next;
					int str_len = strlen(current->str);
					if(cursor[1] > str_len
					|| current->str[cursor[1]-1] == '\n'){
						if(current->str[str_len-1] == '\n') cursor[1] = str_len-1;
						else cursor[1] = str_len;
					}
					if(line > out_end_index){
						// 出力行が変わる
						out_start = out_start->next;
						out_end = out_end->next;
						out_start_index += 1;
						out_end_index += 1;
					}else{
						// カーソルのみ
						cursor[0] += 1;
					}
				}
				if(input_key == 'l'){
					if(cursor[1] != strlen(current->str) && current->str[cursor[1]] != '\n'){
						cursor[1] += 1;
					}
				}
				if(input_key == 'h'){
					if(cursor[1] != 1){
						cursor[1] -= 1;
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


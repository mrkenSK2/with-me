#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

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
}

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
	while(fgets(buf, sizeof(buf), fp)){
		strcpy(current->str, buf);
		insert(current);
		current = current->next;
	}
	fclose(fp);
	return;
}

		
int main(int argc, char **argv){
	system("clear");
	struct string* head = (struct string*)malloc(sizeof(struct string));
	head->prev = NULL;
	head->next = NULL;
	if(argc != 2){
		printf("usage: ./a.out filename\n");
	}else{
		file_read(argv[1], head);
		struct string* current  head;

	while(current->next != NULL){
		printf("%s", current->str):
		current = current->next;
	}
	printf("%s", current->str);

	// reverse
	while(current->prev != NULL){
		printf("%c", current->str[1]);
	 	printf("%c", current->str[0]);
	current = current->prev;
	}

	printf("%c", current->str[1]);
	printf("%c", current->str[0]);
	}
	return 0;
}

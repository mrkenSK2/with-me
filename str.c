#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

struct str* insert(struct str* insert_to_nextpoint){
	struct str* new_str = (struct str*)malloc(sizeof(struct str));
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

void file_read(char* filename, struct str* head){
	FILE* fp;
	char buf[BUFFER_SIZE];
	
	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "file open error\n");
		exit(1);
	}
	struct str* current = head;
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

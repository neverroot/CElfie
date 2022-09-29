#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "elf.h"
#include "debug.h"

int main(){
	FILE *fp = fopen("/usr/lib/firefox/firefox", "rb");

	if(fp == NULL){
		printf("File error!\n");
		return -1;
	}

	fseek(fp,0,SEEK_END);
	uint64_t size = ftell(fp);
	fseek(fp,0,SEEK_SET);

	uint8_t* buffer = malloc(size+1);
	uint64_t bytes_read = fread(buffer,size,1,fp);

	memdump(NULL,buffer,sizeof(elf64_t) + sizeof(program_header_t));
	elf64_t* header = parse_header(buffer);


	program_header_t* prog_hdr = parse_program_header(buffer,header);
	
	fclose(fp);
	return 0;
}




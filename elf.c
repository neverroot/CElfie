#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "elf.h"
#include "debug.h"

bool is_power_of_2(uint64_t x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}

// only give arr of uint64_t elements
// return 0 if found and -1 if not found
int in_valid_range(uint64_t val, void* arr, uint32_t size){
    printf("val 0x%lx\n",val);
    for(int i=0;i<(size * sizeof(uint64_t));i+=sizeof(uint64_t)){
        if(val == *(uint64_t*)(arr+i)){
            return 0;
        }
    }
    return -1;
}

void* parse_program_header(uint8_t* buf, elf64_t* elf_header){
    uint16_t num_of_headers = elf_header->prog_hdr_entries_num;
    uint16_t header_size = elf_header->prog_hdr_entry_size; 
    program_header_t program_headers[num_of_headers];

    // for(uint16_t i=0;i<num_of_headers*header_size;i+=header_size){

    // }
    // printf("Size of program headers: 0x%lx\n", sizeof(program_headers));
    if(check_program_header(buf+elf_header->prog_hdr_table) < 0){
        printf("Invalid program header\n");
        return NULL;
    }
    return program_headers;
}

int check_program_header(uint8_t* buf){
    program_header_t prog_hdr = *(program_header_t*)buf;

    uint64_t valid_types[] = {0x00000000,0x00000001,0x00000002,0x00000003,0x00000004,0x00000005,0x00000006,0x00000007,0x60000000,0x6FFFFFFF,0x70000000,0x7FFFFFFF};
    if((in_valid_range(prog_hdr.type,valid_types,sizeof(valid_types)/sizeof(uint64_t)))){
        printf("Invalid type\n");
        return -1;
    }

    if(prog_hdr.flags){
        printf("Program header flags: 0x%x\n",prog_hdr.flags);
    }

    if(prog_hdr.seg_offset){
        printf("Offset of the segment in the file image: 0x%lx\n",prog_hdr.seg_offset);
    }

    if(prog_hdr.seg_virt_addr){
        printf("Virtual address of the segment in memory: 0x%lx\n",prog_hdr.seg_virt_addr);
    }

    if(prog_hdr.seg_phys_addr){
        printf("Physical address of the segment in memory: 0x%lx\n",prog_hdr.seg_phys_addr);
    }

    if(prog_hdr.seg_file_size){
        printf("Size in bytes of the segment in the file image: 0x%lx\n",prog_hdr.seg_file_size);
    }

    if(prog_hdr.seg_mem_size){
        printf("Size in bytes of the segment in memory: 0x%lx\n",prog_hdr.seg_mem_size);
    }

    if((prog_hdr.alignment == NO_ALIGNMENT_0) && (prog_hdr.alignment == NO_ALIGNMENT_1)){
        printf("No alignment\n");
    } else if (prog_hdr.seg_virt_addr == (prog_hdr.seg_offset % prog_hdr.alignment)){
        printf("Alignment: 0x%lx\n",prog_hdr.alignment);
    } else if(is_power_of_2(prog_hdr.alignment)){
        printf("Alignment: 0x%lx\n",prog_hdr.alignment);
    } else{
        printf("Invalid lignment: 0x%lx\n",prog_hdr.alignment);
    }

    return 0;
}

elf64_t* parse_header(uint8_t* buf){
    elf64_t* res = NULL;
    if(check_header(buf) < 0){
        return NULL;
    }
    res = (elf64_t*)malloc(sizeof(elf64_t));
    memcpy(res, buf, sizeof(elf64_t));
    return res;
}

int check_header(uint8_t* buf){
    elf64_t hdr = *(elf64_t*)buf;
    memdump(NULL,&hdr,sizeof(elf64_t));
    if(hdr.magic != ELF_MAGIC_BYTES){
        printf("Couldn't find magic bytes\n");
        return -1;
    }

    if(hdr.class != ELF_64bit){
        printf("This is not a 64-bit ELF file\n");
        return -1;
    }

    if(hdr.endianness != ELF_LITTLE_ENDIAN){
        printf("This is not a LE ELF\n");
        return -1;
    }

    if(hdr.version != 1){
        printf("Version: %x", hdr.version);
    }

    if(hdr.ABI_ver){
        printf("Target operating system ABI not found\n");
    }
    // skip padding here
    if(hdr.file_type){
        printf("Object file type not found\n");
    }

    if(hdr.machine){
        printf("Machine type not found\n");
    }

    if(hdr.orig_version != 1){
        printf("e_version: %x",hdr.orig_version);
    }

    if(hdr.entry_point){
        printf("Entry point: %ld\n",hdr.entry_point);
    }

    if(hdr.prog_hdr_table){
        printf("Program header table starts at: 0x%lx\n",hdr.prog_hdr_table);
    }

    if(hdr.sect_hdr_table){
        printf("Section header table starts at: 0x%lx\n",hdr.sect_hdr_table);
    }

    if(hdr.flags){
        printf("Flags: 0x%x\n", hdr.flags);
    }

    if(hdr.elf_header_size != 0x40){
        printf("This is not a 64-bit ELF file\n");
        return -1;
    }

    if(hdr.prog_hdr_entry_size){
        printf("Size of program header table entry: 0x%x\n",hdr.prog_hdr_entry_size);
    }

    if(hdr.prog_hdr_entry_size){
        printf("Number of entries in program header table: 0x%x\n",hdr.prog_hdr_entries_num);
    }

    if(hdr.sect_hdr_entry_size){
        printf("Size of program header table entry: 0x%x\n",hdr.sect_hdr_entry_size);
    }

    if(hdr.sect_hdr_entry_size){
        printf("Number of entries in program header table: 0x%x\n",hdr.sect_hdr_entry_size);
    }

    if(hdr.section_name_idx){
        printf("Index of the section header table entry that contains the section names: 0x%x\n",hdr.section_name_idx);
    }

    return 0;
}


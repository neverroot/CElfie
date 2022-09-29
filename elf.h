#include <stdint.h>

#define ELF_MAGIC_BYTES 0x464c457f

#define ELF_LITTLE_ENDIAN 1
#define ELF_BIG_ENDIAN 2

#define ELF_32bit 1
#define ELF_64bit 2

#define NO_ALIGNMENT_0  0
#define NO_ALIGNMENT_1  1

typedef struct {
    uint32_t magic;
    uint8_t class;
    uint8_t endianness;
    uint8_t version;
    uint8_t OS_ABI;
    uint8_t ABI_ver;
    uint8_t padding[7];
    uint16_t file_type;
    uint16_t machine;
    uint32_t orig_version;
    uint64_t entry_point;
    uint64_t prog_hdr_table;
    uint64_t sect_hdr_table;
    uint32_t flags;
    uint16_t elf_header_size;
    uint16_t prog_hdr_entry_size;
    uint16_t prog_hdr_entries_num;
    uint16_t sect_hdr_entry_size;
    uint16_t sect_hdr_entries_num;
    uint16_t section_name_idx;
} elf64_t;

typedef struct {
    uint32_t type;
    uint32_t flags;
    uint64_t seg_offset;
    uint64_t seg_virt_addr;
    uint64_t seg_phys_addr;
    uint64_t seg_file_size;
    uint64_t seg_mem_size;
    uint64_t alignment;
} program_header_t;

int in_valid_range(uint64_t val, void* arr, uint32_t size);
// Parse program header given pointer to start of header
void* parse_program_header(uint8_t* buf, elf64_t* elf_header);
int check_program_header(uint8_t* buf);
// Parse header given the start of the binary
elf64_t* parse_header(uint8_t* buf);
int check_header(uint8_t* buf);
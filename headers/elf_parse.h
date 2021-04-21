#ifndef ELFPARSE
#define ELFPARSE

#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <errno.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
enum class STATUS {
    SUCCESS,
    OPEN_ERROR,
    FSTAT_ERROR,
    MMAP_ERROR,
    IS_NOT_ELF_ERROR
};

enum class ELF_TYPE {
    ELF_NONE,
    ELF_REL,
    ELF_EXEC,
    ELF_DYN,
    ELF_CORE,
    OTHER
};

template<typename Key,typename Value>
std::vector<Value> get_dict_value(std::map<Key,std::vector<Value>> m, Key key);
class ELFParser32 {
public:
    ELFParser32(std::string filename) {
        this->filename = filename;
    }

    STATUS parse();

    ELF_TYPE get_filetype();

    uint32_t get_entry();

    ~ELFParser32();

private:
    Elf32_Ehdr* get_ehdr_address() {
        return (Elf32_Ehdr*)file_mem;
    }
    Elf32_Phdr* get_phdr_address() {
        return (Elf32_Phdr*)&file_mem[ehdr->e_phoff];
    }
    Elf32_Shdr* get_shdr_address() {
        return (Elf32_Shdr*)&file_mem[ehdr->e_shoff];
    }
    Elf32_Ehdr* ehdr;
    Elf32_Phdr* phdr;
    Elf32_Shdr* shdr;
    uint8_t* file_mem;
    uint32_t entry;
    std::string filename;
    struct stat st;
    std::map<std::string,std::vector<Elf32_Shdr>> section_headers;
    std::map<std::string,std::vector<Elf32_Phdr>> program_headers;
};
#endif
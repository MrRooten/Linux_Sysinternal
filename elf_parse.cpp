#include "headers/elf_parse.h"

template<typename Key,typename Value>
std::vector<Value>* get_dict_value(std::map<Key,std::vector<Value>>* m, Key key) {
    typename std::map<Key,std::vector<Value>>::const_iterator it = m->find( key );
   if ( it == m->end() ) {
      std::vector<Value> v;
      (*m)[key] = v;
      return &(*m)[key];
   }
   else {
      return (std::vector<Value>*)&it->second;
   }
}


STATUS ELFParser32::parse() {
    int fd;
    struct stat st;
    STATUS status;
    do {
        if ((fd = open(this->filename.c_str(),O_RDONLY)) < 0) {
            status = STATUS::OPEN_ERROR;
            break;
        }

        if (fstat(fd,&st) < 0) {
            status = STATUS::FSTAT_ERROR;
            break;
        }

        this->st = st;
        this->file_mem = (uint8_t*)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE,fd, 0);
        if (this->file_mem == MAP_FAILED) {
            status = STATUS::MMAP_ERROR;
            break;
        }
        this->ehdr = get_ehdr_address();
        this->phdr = get_phdr_address();
        this->shdr = get_shdr_address();

        if (this->file_mem[0] != 0x7f && strcmp((char*)&file_mem[1],"ELF")) {
            status = STATUS::IS_NOT_ELF_ERROR;
            break;
        }
        status = STATUS::SUCCESS;
   
        char* StringTable = (char*)&file_mem[shdr[ehdr->e_shstrndx].sh_offset];
        for(int i=1;i < ehdr->e_shnum;i++) {
            get_dict_value<std::string,Elf32_Shdr>(&section_headers,&StringTable[shdr[i].sh_name])->push_back(shdr[i]);
        }

        for(int i=0;i < ehdr->e_phnum;i++) {
            switch(phdr[i].p_type) {
                case PT_NULL:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_NULL")->push_back(phdr[i]);
                    break;
                case PT_LOAD:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_LOAD")->push_back(phdr[i]);
                    break;
                case PT_DYNAMIC:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_DYNAMIC")->push_back(phdr[i]);
                    break;
                case PT_INTERP:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_INTERP")->push_back(phdr[i]);
                    break;
                case PT_NOTE:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_NOTE")->push_back(phdr[i]);
                    break;
                case PT_SHLIB:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_SHLIB")->push_back(phdr[i]);
                    break;
                case PT_PHDR:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_PHDR")->push_back(phdr[i]);
                    break;
                case PT_LOPROC:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_LOPROC")->push_back(phdr[i]);
                    break;
                case PT_HIPROC:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_HIPROC")->push_back(phdr[i]);
                    break;
                case PT_GNU_STACK:
                    get_dict_value<std::string,Elf32_Phdr>(&program_headers,"PT_GNU_STACK")->push_back(phdr[i]);
                    break;
            }
        }
    }while(0);
    
    close(fd);
    return status;
}

ELF_TYPE ELFParser32::get_filetype() {
    ELF_TYPE e_filetype;
    switch(ehdr->e_type) {
        case ET_EXEC:
            e_filetype = ELF_TYPE::ELF_EXEC;
            break;
        case ET_DYN:
            e_filetype = ELF_TYPE::ELF_DYN;
            break;
        case ET_REL:
            e_filetype = ELF_TYPE::ELF_REL;
            break;
        case ET_CORE:
            e_filetype = ELF_TYPE::ELF_CORE;
            break;
        case ET_NONE:
            e_filetype = ELF_TYPE::ELF_NONE;
            break;
        default:
            e_filetype = ELF_TYPE::OTHER;
            break;
    }

    return e_filetype;
}
ELFParser32::~ELFParser32() {
    munmap(this->file_mem,this->st.st_size);
}

int main(int argc,char** argv) {
    if (argc < 1) {
        printf("Usage:%s <filename>\n",argv[0]);
        return 0;
    }
    ELFParser32 elfp(argv[1]);
    elfp.parse();
    return 0;
}
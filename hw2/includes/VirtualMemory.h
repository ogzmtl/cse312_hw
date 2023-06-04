#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H

#include "types.h"
#include "PhysicalMemory.h"
#include <cstdio>

typedef struct p_table{
    unsigned int reference_bit; 
    unsigned int modified_bit; 
    unsigned int pres_absent_bit; //If this bit is 1, entry is valid. If 0, entry is not currently in memory.
    long int pageFrame_number;
    unsigned int protection_bit; //three bits read, write, execute
    int counter;
    int mapping_address;
    
}pt;

class VirtualMemory {

    private:
        pt* page_table;
        int data[64*1024];
        int page_replacement_algorithm;
        long int page_table_size;
        long int cursor_position = 0;
        char* filename;
        int num_of_reads;
        int num_of_writes;
        int num_of_page_misses;
        int num_of_page_replacements;
        int num_of_disk_page_writes;
        int num_of_disk_page_reads;
        FILE* file;

    public:
        VirtualMemory();
        VirtualMemory(int pagetable_size);
        ~VirtualMemory();
        int get_data(int index);
        void set_data(int index, int value);
        void set_page_table_size(long int page_table_size);
        int get_value(int page_replacement_algorithm, int address,PhysicalMemory& pm);
        int get_from_physical_memory(int address, int map_value,PhysicalMemory& pm);
        int go_to_disk();
        int LRU(int value,PhysicalMemory& pm, int address);
        void set_disk_filename(char* filen);
        void set_file(char* filen);
};



#endif
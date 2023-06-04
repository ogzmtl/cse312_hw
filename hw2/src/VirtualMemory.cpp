#include "VirtualMemory.h"
#include <iostream>

void VirtualMemory::set_data(int index, int value)
{
    if(index > 1024*64 && index < 0){
        std::cout <<"Invalid index";
        return;
    }

    data[index] = value;
}

int VirtualMemory::get_data(int index){
    if(index > page_table_size && index < 0){
        std::cout <<"Invalid index";
        return -1;
    }

    return page_table[index].mapping_address;
}

void VirtualMemory::set_page_table_size(long int page_table_size){
    if(page_table_size<0){
        exit(1);
    }

    page_table = new pt[page_table_size];
    this->page_table_size = page_table_size;

    for(int i=0; i<page_table_size;i++){
        page_table[i].mapping_address = -1;
    }
}

void VirtualMemory::set_file(char* filen){
    this->file = fopen(filen,"r");
    this->filename = filen;
}

int VirtualMemory :: get_value(int page_replacement_algorithm, int address, PhysicalMemory& pm)
{

    if(page_replacement_algorithm == 1 ){//do LRU

        if(page_table[address].mapping_address == -1){
            int data_from_disk = go_to_disk();
            std::cout<<"Data received from the disk : "<< data_from_disk << std::endl;
            LRU(data_from_disk, pm, address);
            page_table[address].counter++;
        }
        else if(page_table[address].mapping_address >= 0)
        {
            num_of_reads++;
            num_of_writes++;
            return pm.get_memory(page_table[address].mapping_address);
        }
    }
    else if (page_replacement_algorithm == 2){
        std::cout << "TO DO... WSCLOCK Algorithm"<< std::endl;
    }
    else{
        return -1;
    }    

    return -1;
}

int VirtualMemory :: go_to_disk(){
    num_of_disk_page_reads++;
    num_of_disk_page_writes++;
    num_of_page_replacements++;
    num_of_page_misses++;

    std::cout<<"Reading from disk file..."<< std::endl;
    
    int num;
    
    while (fscanf(file, "%d", &num) == 1) {
        std::cout<<"Read number: "<< num;
        break;
    }
    
    return num;
}

void VirtualMemory::set_disk_filename(char* filen){
    this->filename = filen;
}

int VirtualMemory ::LRU(int value, PhysicalMemory& pm, int address)
{
    
    int temp; 
    int temp_address; 
    int temp_vm_address;
    for(int i = 0; i < page_table_size; i++){
        
        if(i == 0){
            temp = page_table[i].counter;
            temp_address = page_table[i].mapping_address;
            temp_vm_address = i;
        }

        if(temp < page_table[i].counter){
            temp = page_table[i].counter;
            temp_address = page_table[i].mapping_address;
            temp_vm_address = i;
        }
    }

    pm.set_memory(page_table[temp_vm_address].mapping_address, value);
    page_table[temp_vm_address].counter = 0;
    page_table[address].mapping_address = page_table[temp_address].mapping_address;
    page_table[temp_vm_address].mapping_address = -1;

    for(int i = 0; i < page_table_size; i++){
        page_table[i].counter = 0;
    }

    std::cout<<"LRU Page replacement algorithm works writing " << value << " to the " <<page_table[address].mapping_address <<std::endl;
    return 1;
}



VirtualMemory::VirtualMemory()
{
    //intentionally empty 
    
}

VirtualMemory::VirtualMemory(int pagetable_size)
{
    page_table = new pt[pagetable_size];
}

VirtualMemory::~VirtualMemory()
{
    delete page_table;
}


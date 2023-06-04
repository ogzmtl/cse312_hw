#include "PhysicalMemory.h"
#include <iostream>

PhysicalMemory::PhysicalMemory()
{

}

PhysicalMemory::PhysicalMemory(int size)
{
    if (size < 0)
    {
        std::cout << "Invalid index " <<std::endl;
        return;
    }

    memory_data = new int[size];
    this->size = size;
}

void PhysicalMemory::set_memory(int index, int value){
    if(index > size && index < 0){
        std::cout << "Invalid index " <<std::endl;
        return;
    }

    memory_data[index] = value;
}
void PhysicalMemory:: set_memory_size(int size ){
    if(size < 0){
        std::cout << "Invalid index " <<std::endl;
        exit(1);
    }

    memory_data = new int[size];
    this->size = size;

    for(int i = 0; i < size; i++){
        memory_data[i] = -1;
    }
}

int PhysicalMemory ::get_memory(int index)
{
    if (index > size && index < 0)
    {
        std::cout << "Invalid index " << std::endl;
        return 0;
    }

    // if(memory_data[index] == -1){
    //     //read_from file();
    // }

    return memory_data[index];
}

int PhysicalMemory::get_memory_size()
{
    return size;
}

PhysicalMemory::~PhysicalMemory()
{
    free(memory_data);
};
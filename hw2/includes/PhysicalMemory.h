#ifndef __PHYSICAL_MEM_H
#define __PHYSICAL_MEM_H


class PhysicalMemory 
{
    friend class VirtualMemory;
    public: 
        PhysicalMemory();
        PhysicalMemory(int size);
        void set_memory(int index, int value);
        int get_memory(int index);
        void set_memory_size(int size);
        int get_memory_size();
        ~PhysicalMemory();

    protected:
        int* memory_data; 
        int size; 
};

#endif
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <random>
#include <mutex>
#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <fcntl.h>     // for file control options
#include <sys/stat.h>  // for file modes
#include <sys/syscall.h>  // for syscall
#include <unistd.h> 
#include <pthread.h>
#include <time.h>

#define VM_SIZE 1024*64
#define PM_SIZE 1024*16
#define MIN_RAND_VALUE 0
#define MAX_RAND_VALUE 10000
#define FRAME_SIZE 1024*4


using namespace std;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
vector<int> buffer;
VirtualMemory vm;
PhysicalMemory pm; 

int physical_frames; 
int virtual_frames; 
int page_size;
int window_size;
int counter= 0;

class physical_memory{

    public: 
        physical_memory(int size){
            if(size < 0){
                cout<<"invalid size"<< endl;
                return; 
            }
        
            memory_data = new int[size];
            this->size = size;
        }

        void set_memory(int index, int value){
            if(index > size && index < 0){
                cout<<"invalid index"<< endl;
                exit(1);
            }

            memory_data[index] = value;
        }
        
        int get_memory(int index){

            if(index > size && index < 0){
                cout<<"invalid index"<< endl;
                exit(1);
            }

            return memory_data[index];
        }

        ~physical_memory(){
            free(memory_data);
        };

    private:
        int* memory_data; 
        int size; 

};

class vm_page_table{

    public: 
        vm_page_table(int size){
            if(size < 0){
                cout<<"invalid size"<< endl;
                return; 
            }
        
            pt_address = new int[size];
            this->size = size;
        }

        void set_pt_address(int index, int value){
            if(index > size && index < 0){
                cout<<"invalid index"<< endl;
                exit(1);
            }

            pt_address[index] = value;
        }
        
        int get_pt_address(int index){

            if(index > size && index < 0){
                cout<<"invalid index"<< endl;
                exit(1);
            }

            return pt_address[index];
        }

        ~vm_page_table(){
            free(pt_address);
        };

    private:
        int* pt_address; 
        int size; 


};



vector<int> matrix_vector(vector<vector<int>> matrix, vector<int> v)
{

    std::vector<int> result(matrix.size(), 0.0);

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            result[i] += matrix[i][j] * v[j];
        }
    }

    return result;
    
}

vector<int> transpose(const vector<vector<int>>& matrix) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    vector<int> result(cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j] = matrix[i][j];
        }
    }

    return result;
}

int matrixMultiplyWithTranspose(vector<int>& matrix) {
    
    int sum = 0; 
    for(long unsigned int i = 0; i < matrix.size(); i++)
    {
        sum += matrix[i]*matrix[i];
    }

    return sum;
}


void* matrix_vector_multiplication_thread(void* arg){
    vector<vector<int>> rand_numbers(page_size/window_size, vector<int>(window_size));

    vector<int> random_vector(page_size/window_size);
 
    srand(time(0));
    
    for(int i = 0; i < page_size/window_size; i++){

        for(int j = 0; j < window_size; j++){
            rand_numbers[i][j] = (rand()%1000);
        }
        
        random_vector[i] = (rand() % 1000);
    }



    vector<int> result_vector = matrix_vector(rand_numbers, random_vector);
    int transpose_vector_result =abs(matrixMultiplyWithTranspose(result_vector));


    pthread_mutex_lock(&mtx);
    counter++;
    // cout<<"Virtual Frames: "<<virtual_frames<<endl;
    buffer.push_back(transpose_vector_result%virtual_frames);
    pthread_mutex_unlock(&mtx);    
    
    return NULL;
}

void* consumer_function(void* arg){
    arg = NULL;
    int get_data = buffer[0];

    // for(int i = 0; i < pm.get_memory_size(); i++){
        cout<<"Reading "<<get_data<<" index from the virtual memory" <<endl;
        cout<<"Virtual Address Space: "<< vm.get_data(get_data)<<endl;
        
    // }


    pthread_mutex_lock(&mtx);
    
    buffer.erase(buffer.begin());
    
    // sort_buffer_linear_search(buffer);

    //choose algorithm 
    // int page_replacement_algorithm = 1; //LRU for 1
    
    
    cout<<"Physical Memory Data :" <<vm.get_value(1, get_data, pm)<<endl;

    pthread_mutex_unlock(&mtx);
    return NULL;

}


int main(int argc, char** argv)
{

    if(argc != 8){
        cout<<"Invalid command line arguments";
        return 0; 
    }
    
    physical_memory py(physical_frames * page_size);
    pthread_t *producer_threads;

    page_size = atoi(argv[1]);
    physical_frames = stoi(argv[2]);
    virtual_frames = stoi(argv[3]);

    cout<<" Frame size : "<<page_size<<endl;
    cout<<"virtual frames : " << virtual_frames<< endl;
    cout<<"physical frames: " << physical_frames<<endl;

    

    page_size = (int)(pow(2, page_size) + 0.5);
    physical_frames = (int)(pow(2,physical_frames) + 0.5);
    virtual_frames = (int)(pow(2,virtual_frames)+0.5); 
    window_size = page_size/4;
 
    vm.set_page_table_size(virtual_frames); 
    pm.set_memory_size(physical_frames*page_size);
    cout<<" Frame size : "<<page_size<<endl;
    cout<<"virtual frames : " << virtual_frames<< endl;
    cout<<"physical frames: " << physical_frames<<endl;


    // for(int i = 0; i < virtual_frames*page_size; i++ )
    // {
    //     vm.set_data(i, rand()%MAX_RAND_VALUE);
    // } 

    // for (int i = 0; i < physical_frames; i++) {
    //     for (int j = 0; j < page_size; j++) {
    //         py.set_memory(i*j, rand()%MAX_RAND_VALUE);
    //     }
    // }
    
    int file_descriptor = open(argv[7], O_WRONLY|O_CREAT|O_APPEND , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (file_descriptor == -1) {
        perror("Error creating file");
        return 1;
    }

    for(int i = 0; i < physical_frames*page_size; i++) {
        string valueString = to_string(i);
        valueString.append("\n");
        ssize_t bytes_written = syscall(SYS_write, file_descriptor, valueString.c_str(), valueString.size());
        if(bytes_written == 0) {
            perror("Error writing file");
            exit(1);
        }    
    }
    
    if (close(file_descriptor) == -1) {
        // Failed to close the file
        perror("Error closing file");
        return 1;
    }
    vm.set_file(argv[7]);
   
    pthread_t consumer_thread;
    producer_threads =(pthread_t*) malloc(sizeof(pthread_t)*virtual_frames);

    
    for(int i = 0; i < virtual_frames; i++ ){
        //VIRTUAL_FRAME ve PYHSICAL FRAME GONDER ????? 
        pthread_create(&producer_threads[i], NULL, matrix_vector_multiplication_thread, NULL);
    }

    pthread_create(&consumer_thread, NULL, consumer_function, NULL);
 
    for(int i = 0; i < virtual_frames; i++ ){
        //VIRTUAL_FRAME ve PYHSICAL FRAME GONDER ????? 
        pthread_join(producer_threads[i], NULL);
    }    


return 0;
}
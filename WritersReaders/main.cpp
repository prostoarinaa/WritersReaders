#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <unistd.h>

#include <semaphore>
using namespace std;

pthread_mutex_t sBlockWrite;
pthread_cond_t sBlockRead;
  
void write_file()
{
    ofstream fileW("/Users/pk/Desktop/OI/TASKS/WritersReaders/WritersReaders/Output.txt");
    fileW.is_open();
    if(fileW.is_open())
    {
        while(1){
        pthread_mutex_lock(&sBlockWrite);
            fileW<<this_thread::get_id()<<" Input"<<endl;
        pthread_cond_signal(&sBlockRead);
        pthread_mutex_unlock(&sBlockWrite);
        this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }
    else
    {
        cout<<"Unable to open file for write"<<endl;
    }
    fileW.close();
    
}

void read_file()
{
    ifstream fileR("/Users/pk/Desktop/OI/TASKS/WritersReaders/WritersReaders/Output.txt");
        fileR.is_open();
    if(fileR.is_open())
    {
        while(1){
            this_thread::sleep_for(chrono::milliseconds(2000));
            pthread_mutex_lock(&sBlockWrite);
            pthread_cond_wait(&sBlockRead,&sBlockWrite);
            cout << this_thread::get_id()<< " output"<<endl;
            pthread_mutex_unlock(&sBlockWrite);
        }
    }
    else
    {
        cout<<"Unable to open file for read"<<endl;
    }
    fileR.close();
}

int main()
{
   
    thread thread_array[4];
    thread_array[0] = thread([&](){
        read_file();
    });
    thread_array[1] = thread([&](){
        read_file();
    });
    thread_array[2] = thread([&](){
        write_file();
    });
    thread_array[3] = thread([&](){
        write_file();
    });
    thread_array[0].join();
    thread_array[1].join();
    thread_array[2].join();
    thread_array[3].join();
    return 0;
}

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
using namespace std;
// структура данных для вывода
struct parameters_fuct
{
    string print_str;
};
pthread_mutex_t semaf_mutex;     // семафор блокировки потоков записи
sem_t count_write_sem;           // семафор ,счетчик записанных данных
pthread_cond_t stop_read_thread; // семафор блокиратор потоков чтения
  
// потоковая функция записи в фаил
void *write_file(void *arg)
{
    struct parameters_fuct *p=(struct parameters_fuct*) arg;
    ofstream line_write_file("buf_file.txt");   // получаем дескриптор фаил для записи
    if(line_write_file)
    {
        for(;;){
  
        pthread_mutex_lock(&semaf_mutex); // блокируем один из потоков записи
  
        for(int i=0; i<100; i++)
            {
                // получаем текущее время
                struct timeval tv;
                struct timezone tz;
                struct tm *tm;
                gettimeofday(&tv, &tz);
                tm=localtime(&tv.tv_sec);
                // записуем его в фаил
                line_write_file<<tm->tm_hour<<":"<<tm->tm_min<<":"<<tm->tm_sec<<":"<<tv.tv_usec<<"\t"<<p->print_str<<endl;
                sem_post(&count_write_sem);// сообщаем что в файле появились данные
            }
        pthread_cond_signal(&stop_read_thread);// посылаем сигнал разблокировки потока чтения
        pthread_mutex_unlock(&semaf_mutex); // разблокируем один из потоков записи
        usleep(1500); // ожидаем , даем потоку считать данные
        }
    }
    else
    {
        cout<<"Unable to open file for write"<<endl;
    }
  return NULL;
}
// потоковая функция чтения файла
void *read_file(void *arg)
{
    ifstream line_read_file("/Users/pk/Desktop/OI/TASKS/WritersReaders/WritersReaders/Output.txt"); // получаем дескриптор файла для чтения
  
    if(line_read_file.is_open())
    {
        pthread_mutex_lock(&semaf_mutex);
        pthread_cond_wait(&stop_read_thread,&semaf_mutex); // ожидаем если выполняется поток записи
        for(;;)
        {
  
     sem_wait(&count_write_sem); // ожидаем поступления данных в фаил
  
                string temp_string;
                getline(line_read_file,temp_string); // получаем строку из файла
                cout<<temp_string<<endl;
                usleep(10000);
  
  
        }
         pthread_mutex_unlock(&semaf_mutex);
    }
  
    else
    {
        cout<<"Unable to open file for read"<<endl;
    }
  return NULL;
}
int main()
{
    // установка счетчика записей в файле
    sem_init(&count_write_sem,0,0);
    // 4 потока
    pthread_t thread_write_1;
    pthread_t thread_write_2;
    pthread_t thread_read_1;
    pthread_t thread_read_2;
  
  
    // структуры данных
    struct parameters_fuct thread_write1_arg;
    struct parameters_fuct thread_write2_arg;
  
    thread_write1_arg.print_str="Write thread first";
    //создание потоков записи
    int result=pthread_create(&thread_write_1,NULL,&write_file,&thread_write1_arg);
    if(result!=0)
    {
        cout<<"Tread write 1 not create"<<endl;
    }
  
    thread_write2_arg.print_str="Write second thread";
    result=pthread_create(&thread_write_2,NULL,&write_file,&thread_write2_arg);
    if(result!=0)
    {
        cout<<"Tread write 2 not create"<<endl;
    }
    //создание потоков чтения
    result=pthread_create(&thread_read_1,NULL,&read_file,NULL);
    if(result!=0)
    {
        cout<<"Tread read 1 create"<<endl;
    }
    result=pthread_create(&thread_read_2,NULL,&read_file,NULL);
    if(result!=0)
    {
        cout<<"Tread read 2 create"<<endl;
    }
    // ожидание выполнения потоков
    result = pthread_join(thread_read_1, NULL);
    if (result != 0)
     {
        cout<<"Joining the first read thread"<<endl;
        return 0;
     }
     result = pthread_join(thread_read_2, NULL);
    if (result != 0)
     {
        cout<<"Joining the second read thread"<<endl;
        return 0;
     }
     result=pthread_join(thread_write_1,NULL);
     if(result!=0)
     {
         cout<<"Joining the first write thread"<<endl;
         return 0;
     }
     result=pthread_join(thread_write_2,NULL);
     if(result!=0)
     {
         cout<<"Joining the second write thread"<<endl;
         return 0;
     }
    return 0;
}

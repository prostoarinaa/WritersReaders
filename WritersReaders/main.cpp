//
//  main.cpp
//  WritersReaders
//
//  Created by ПК on 12.10.2021.
//
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <semaphore>
#include <chrono>
#include <thread>
using namespace std;
int readcount=0, writecount=0;
mutex mutex1, mutex2, w, r;

class PROCESS {
public:
    void Reader() {
        
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(2000));
        r.lock();
        mutex1.lock();
        readcount = readcount + 1;
        if (readcount==1) w.lock();
        mutex1.unlock();
        r.unlock();

        cout << "reader #" << readcount << "...читаем..." << endl;

        mutex1.lock();
        readcount = readcount - 1;
        if (readcount == 0) w.unlock();
        mutex1.unlock();
        }
    };
    void Writter() {
        
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(8000));
        mutex2.lock();
        writecount = writecount + 1;
        if (writecount==1) r.unlock();
        mutex2.unlock();

        w.lock();
        cout << "...пишем..." << endl;
        w.unlock();

        mutex2.lock();
        writecount = writecount - 1;
        if (writecount == 0) r.unlock();
        mutex2.unlock();
        }
    };
};
 
  
int main() {
    PROCESS process;
    thread read([&]() {
        process.Reader();
    });
    thread write([&]() {
        process.Writter();
    });

    read.join();
    write.join();
    return 0;
}

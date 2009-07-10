#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void *ping(){
    while(1){
        cout << "Ping running\n";
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pingthrd;
    int retval;
    if(argc != 1){
        cout << "Usage: " << argv[0] << endl;
        exit(1);
    }
    retval = pthread_create((pthread_t*)&pingthrd,NULL,(void* (*)(void*))ping,NULL);
    while(1){
        cout << "Main running\n";
        sleep(2);
    }
    return 0;
}

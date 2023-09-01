#include "Condition.h"
#include "MutexLock.h"
#include <stdio.h>
#include <pthread.h>

Condition::Condition(MutexLock & mutex)
:_mutex(mutex)
{
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret){
        perror("pthread_cond_init");
        return;
    }
}

Condition::~Condition(){
    int ret = pthread_cond_destroy(&_cond);
    if(ret){
        perror("pthread_cond_destroy");
        return;
    }
}

void Condition::wait(){
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
    if(ret){
        perror("pthread_cond_wait");
        return;
    }
}

void Condition::signal(){
    int ret = pthread_cond_signal(&_cond);
    if(ret){
        perror("pthread_cond_signal");
        return;
    }
}

void Condition::broadcast(){
    int ret = pthread_cond_broadcast(&_cond);
    if(ret){
        perror("pthread_cond_broadcast");
        return;
    }
}

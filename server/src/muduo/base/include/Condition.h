//
// Created by lsill on 2024/2/4.
//

#ifndef SERVER_CONDITION_H
#define SERVER_CONDITION_H

#include "Mutex.h"

#include <pthread.h>

namespace muduo
{

    class Condition : noncopyable
    {
    public:
        explicit Condition(MutexLock& mutex)
                : mutex_(mutex)
        {
            MCHECK(pthread_cond_init(&pcond_, nullptr));
        }

        ~Condition()
        {
            MCHECK(pthread_cond_destroy(&pcond_));
        }

        void wait()
        {
            MutexLock::UnassignGuard ug(mutex_);
            MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
        }

        // returns true if time out, false otherwise.
        bool waitForSeconds(double seconds);

        void notify()
        {
            MCHECK(pthread_cond_signal(&pcond_));
        }

        void notifyAll()
        {
            MCHECK(pthread_cond_broadcast(&pcond_));
        }

    private:
        MutexLock& mutex_;
        pthread_cond_t pcond_;
    };

}  // namespace muduo

#endif //SERVER_CONDITION_H

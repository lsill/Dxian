//
// Created by lsill on 2024/2/4.
//

#ifndef SERVER_COUNTDOWNLATCH_H
#define SERVER_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace muduo
{

    class CountDownLatch : noncopyable
    {
    public:

        explicit CountDownLatch(int count);

        void wait();

        void countDown();

        int getCount() const;

    private:
        mutable MutexLock mutex_;
        Condition condition_ GUARDED_BY(mutex_);
        int count_ GUARDED_BY(mutex_);
    };

}  // namespace muduo

#endif //SERVER_COUNTDOWNLATCH_H

//
// Created by lsill on 2024/2/5.
//

#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H

#include "Atomic.h"
#include "Timestamp.h"
#include "Callbacks.h"

namespace muduo {
namespace net {

class Timer : noncopyable {
public:
    Timer(TimerCallback cb, Timestamp when, double interval):callback_(cb), expiration_(when), 
        interval_(interval),repeat_(interval > 0.0), sequence_(s_numCreated_.incrementAndGet()) {}

    void run() const {
        callback_();
    }

    Timestamp expiration() const  { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_; }

    void restart(Timestamp now);

    static int64_t numCreated() {return s_numCreated_.get();}

private:
    const TimerCallback callback_;
    Timestamp expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;

    static AtomicInt64 s_numCreated_;
};

} // namespace muduo
} // namespace net

#endif //SERVER_TIMER_H

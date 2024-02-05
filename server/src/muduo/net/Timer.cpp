/*
 * @Author: lsill 
 * @Date: 2024-02-05 23:15:14 
 * @Last Modified by: lsill
 * @Last Modified time: 2024-02-05 23:15:35
 */
#include "include/Timer.h"

using namespace muduo;
using namespace muduo::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now) {
    if (repeat_) {
        expiration_ = addTime(now, interval_);
    } else {
        expiration_ = Timestamp::invalid();
    }
}
//
// Created by lsill on 2024/2/4.
//

#ifndef DXIAN_TIMERID_H
#define DXIAN_TIMERID_H

#include "copyable.h"
#include <cstdint>

namespace muduo
{
    namespace net
    {

        class Timer;

///
/// An opaque identifier, for canceling Timer.
///
        class TimerId : public copyable
        {
        public:
            TimerId()
                    : timer_(nullptr),
                      sequence_(0)
            {
            }

            TimerId(Timer* timer, int64_t seq)
                    : timer_(timer),
                      sequence_(seq)
            {
            }

            // default copy-ctor, dtor and assignment are okay

            friend class TimerQueue;

        private:
            Timer* timer_;
            int64_t sequence_;
        };

    }  // namespace net
}  // namespace muduo

#endif //DXIAN_TIMERID_H

//
// Created by lsill on 2024/2/5.
//

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "include/TimerQueue.h"
#include "include/EventLoop.h"
#include "include/Timer.h"
#include "include/TimerId.h"
#include "Logging.h"



#include <sys/timerfd.h>
#include <unistd.h>
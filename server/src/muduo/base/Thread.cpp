// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "include//Thread.h"
#include "include//CurrentThread.h"
#include "include//Exception.h"
#include "include//Logging.h"

#include <type_traits>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#ifdef __linux__
#include <linux/unistd.h>
#include <sys/prctl.h>
#elif defined(__APPLE__) && defined(__MACH__)
#include <pthread.h>
#include <mach/mach.h>
#endif


namespace muduo
{
    namespace detail
    {

        pid_t gettid()
        {
#if __linux__
            return static_cast<pid_t>(::syscall(SYS_gettid));
#elif __APPLE__
            // TODO 这个做法没有任何意思 只是为了编译好看，pthread_self()才是mac进程唯一标识符
            mach_port_t machTID = pthread_mach_thread_np(pthread_self());
            return static_cast<pid_t>(machTID);
#endif
        }

        void afterFork()
        {
            muduo::CurrentThread::t_cachedTid = 0;
            muduo::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
            // no need to call pthread_atfork(NULL, NULL, &afterFork);
        }

        class ThreadNameInitializer
        {
        public:
            ThreadNameInitializer()
            {
                muduo::CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(NULL, NULL, &afterFork);
            }
        };

        ThreadNameInitializer init;

        struct ThreadData
        {
            typedef muduo::Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            string name_;
            pid_t* tid_;
            CountDownLatch* latch_;

            ThreadData(ThreadFunc func,
                       const string& name,
                       pid_t* tid,
                       CountDownLatch* latch)
                    : func_(std::move(func)),
                      name_(name),
                      tid_(tid),
                      latch_(latch)
            { }

            void setThreadName(const char* threadName) {
#if defined(__linux__)
                ::prctl(PR_SET_NAME, threadName);
#elif defined(__APPLE__) && defined(__MACH__)
                pthread_setname_np(threadName);
#endif
            }
            void runInThread()
            {
                *tid_ = muduo::CurrentThread::tid();
                tid_ = nullptr;
                latch_->countDown();
                latch_ = nullptr;

                muduo::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
                setThreadName(muduo::CurrentThread::t_threadName);
                try
                {
                    func_();
                    muduo::CurrentThread::t_threadName = "finished";
                }
                catch (const Exception& ex)
                {
                    muduo::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                    fprintf(stderr, "reason: %s\n", ex.what());
                    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                    abort();
                }
                catch (const std::exception& ex)
                {
                    muduo::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                    fprintf(stderr, "reason: %s\n", ex.what());
                    abort();
                }
                catch (...)
                {
                    muduo::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                    throw; // rethrow
                }
            }
        };

        void* startThread(void* obj)
        {
            ThreadData* data = static_cast<ThreadData*>(obj);
            data->runInThread();
            delete data;
            return nullptr;
        }

    }  // namespace detail

    void CurrentThread::cacheTid()
    {
        if (t_cachedTid == 0)
        {
            t_cachedTid = detail::gettid();
            t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        }
    }

    bool CurrentThread::isMainThread()
    {
        return tid() == ::getpid();
    }

    void CurrentThread::sleepUsec(int64_t usec)
    {
        struct timespec ts = { 0, 0 };
        ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
        ::nanosleep(&ts, NULL);
    }

    AtomicInt32 Thread::numCreated_;

    Thread::Thread(ThreadFunc func, const string& n)
            : started_(false),
              joined_(false),
              pthreadId_(0),
              tid_(0),
              func_(std::move(func)),
              name_(n),
              latch_(1)
    {
        setDefaultName();
    }

    Thread::~Thread()
    {
        if (started_ && !joined_)
        {
            pthread_detach(pthreadId_);
        }
    }

    void Thread::setDefaultName()
    {
        int num = numCreated_.incrementAndGet();
        if (name_.empty())
        {
            char buf[32];
            snprintf(buf, sizeof buf, "Thread%d", num);
            name_ = buf;
        }
    }

    void Thread::start()
    {
        assert(!started_);
        started_ = true;
        // FIXME: move(func_)
        detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
        if (pthread_create(&pthreadId_, nullptr, &detail::startThread, data))
        {
            started_ = false;
            delete data; // or no delete?
            LOG_SYSFATAL << "Failed in pthread_create";
        }
        else
        {
            latch_.wait();
            assert(tid_ > 0);
        }
    }

    int Thread::join()
    {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, NULL);
    }

}  // namespace muduo

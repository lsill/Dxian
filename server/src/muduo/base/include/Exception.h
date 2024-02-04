//
// Created by lsill on 2024/2/4.
//

#ifndef SERVER_EXCEPTION_H
#define SERVER_EXCEPTION_H

#include "Types.h"
#include <exception>

namespace muduo
{

    class Exception : public std::exception
    {
    public:
        Exception(string what);
        ~Exception() noexcept override = default;

        // default copy-ctor and operator= are okay.

        const char* what() const noexcept override
                {
                        return message_.c_str();
                }

        const char* stackTrace() const noexcept
        {
            return stack_.c_str();
        }

    private:
        string message_;
        string stack_;
    };

}  // namespace muduo

#endif //SERVER_EXCEPTION_H

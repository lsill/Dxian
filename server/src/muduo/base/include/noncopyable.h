//
// Created by lsill on 2024/2/4.
//

#ifndef DXIAN_NONCOPYABLE_H
#define DXIAN_NONCOPYABLE_H

namespace muduo
{

    class noncopyable
    {
    public:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;

    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };

}  // namespace muduo

#endif //DXIAN_NONCOPYABLE_H

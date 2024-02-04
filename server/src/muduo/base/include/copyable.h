//
// Created by lsill on 2024/2/4.
//

#ifndef DXIAN_COPYABLE_H
#define DXIAN_COPYABLE_H

namespace muduo
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
    class copyable
    {
    protected:
        copyable() = default;
        ~copyable() = default;
    };

}  // namespace muduo

#endif //DXIAN_COPYABLE_H

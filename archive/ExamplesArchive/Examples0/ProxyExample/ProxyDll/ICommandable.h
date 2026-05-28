#pragma once

namespace chrisg
{
    class ICommandable
    {
    public:
        virtual void Invoke() = 0;
    };
}
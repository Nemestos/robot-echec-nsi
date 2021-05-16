#pragma once
#include "Pool.h"
#include "Logger.h"

class Utils
{
public:
    template <class T>
    static T *getPoolRess(const char *name) noexcept
    {
        return Pool::getInst().get_ressource<T>(name);
    }
    
    template <typename T>
    static T clamp(const T &n, const T &lower, const T &upper) noexcept
    {
        return max(lower, min(n, upper));
    }
};

#include "lock.h"

void spinInit(lock* lock)
{
    *lock = 1;
}

void spinLock(lock* lock)
{
    while (!__sync_bool_compare_and_swap(lock, 1, 0)) {};
}

void spinUnlock(lock* lock)
{
    *lock = 1;
}
#ifndef LOCK_H
#define LOCK_H

typedef int lock;

extern int attemptLock(lock* attemptedLock);

#endif
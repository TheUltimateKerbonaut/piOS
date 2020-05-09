#ifndef LOCK_H
#define LOCK_H

typedef int lock;

void spinInit(lock* lock);
void spinLock(lock* lock);
void spinUnlock(lock* lock);
#endif
.section ".text"

.global attemptLock

// Takes a pointer to a lock variable and uses atomic operations to acquire the lock.
// Returns 0 if it wasn't aquired, 1 if it was

attemptLock:
    mov     x1, #0
    swp     x2, x1, [x0]    // stores the value in r1 into the address in r0, and stores the value at the address in r0 into r2
    mov     x0, x2
    //blx     lr 
    ret
    
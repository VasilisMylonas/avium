#ifndef AVIUM_PRIVATE_THREADS_H
#define AVIUM_PRIVATE_THREADS_H

#include "avium/core.h"
#include "avium/threads.h"

typedef struct
{
    object arg;
    AvmThreadCallback callback;
} ThreadState;

static ThreadState* ThreadStateNew(object arg, AvmThreadCallback callback)
{
    ThreadState* t = AvmAlloc(sizeof(ThreadState));
    t->arg = arg;
    t->callback = callback;
    return t;
}

#endif // AVIUM_PRIVATE_THREADS_H

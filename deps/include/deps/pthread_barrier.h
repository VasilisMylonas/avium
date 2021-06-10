#ifndef AVIUM_DEPS_PTHREAD_BARRIER_H
#define AVIUM_DEPS_PTHREAD_BARRIER_H

#ifdef AVM_GNU
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#ifdef __unused
#undef __unused
#endif

#include <../DarwinPthreadBarrier/src/pthread_barrier.c>
#include <../DarwinPthreadBarrier/src/pthread_barrier.h>

#ifdef AVM_GNU
#pragma GCC diagnostic pop
#endif

#endif // AVIUM_DEPS_PTHREAD_BARRIER_H

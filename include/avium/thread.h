#ifndef AVIUM_THREAD_H
#define AVIUM_THREAD_H

#include "avium/core.h"

AVM_CLASS(AvmThread, object, {
    void* _state;
    AvmThrowContext* _context;
});

#ifndef AVM_WIN32
AVM_CLASS(AvmMutex, object, { void* _mutex; });

AVMAPI AvmMutex AvmMutexNew();
AVMAPI void AvmMutexLock(const AvmMutex* self);
AVMAPI void AvmMutexUnlock(const AvmMutex* self);

#define lock(mutex)                                                            \
    AvmMutexLock(&mutex);                                                      \
    for (uint i = 0; i < 1; i++, AvmMutexUnlock(&mutex))
#endif

typedef void (*AvmThreadCallback)(object);

AVMAPI AvmThread AvmThreadNew(AvmThreadCallback callback, object value);

/**
 * @brief Exits the current thread with an exit code.
 *
 * @param code The exit code.
 * @return This function never returns.
 */
AVMAPI never AvmThreadExit(AvmExitCode code);

/**
 * @brief Returns a handle to the current thread.
 *
 * @return The handle to the current thread.
 */
AVMAPI const AvmThread* AvmThreadGetCurrent(void);

/**
 * @brief Waits for another thread to finish executing.
 *
 * @param thread The thread to wait for.
 * @return The thread exit code.
 */
AVMAPI AvmExitCode AvmThreadJoin(const AvmThread* thread);

/**
 * @brief Pauses execution of the current thread.
 *
 * @param ms The time in milliseconds to pause execution.
 */
AVMAPI void AvmThreadSleep(uint ms);

#endif // AVIUM_THREAD_H
